using Daramee.Degra.Native;
using Daramee.Degra.Utilities;
using Daramee.FileTypeDetector;
using SharpCompress.Archives;
using SharpCompress.Readers;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace Daramee.Degra
{
	public class ProgressStatus : INotifyPropertyChanged
	{
		double progress = 0;
		string proceedFile = "";

		public double Progress
		{
			get => progress;
			set
			{
				progress = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Progress)));
			}
		}
		public string ProceedFile
		{
			get => proceedFile;
			set
			{
				proceedFile = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProceedFile)));
			}
		}

		public event PropertyChangedEventHandler PropertyChanged;
	}

	public enum DegrationFormat : int
	{
		Zip = -1,
		OriginalFormat = 0,
		WebP,
		JPEG,
		PNG,
	}

	public static class Degrator
	{
		public static bool Degration(FileInfo fileInfo, ProgressStatus status, CancellationToken cancellationToken)
		{
			fileInfo.Status = DegraStatus.Processing;

			status.ProceedFile = fileInfo.OriginalFilename;
			status.Progress = 0;

			if (cancellationToken.IsCancellationRequested)
			{
				status.Progress = 1;
				fileInfo.Status = DegraStatus.Cancelled;
				return false;
			}
			if (!File.Exists(fileInfo.OriginalFilename))
			{
				status.Progress = 1;
				fileInfo.Status = DegraStatus.Failed;
				return false;
			}

			var options = new NativeBridge.DegraOptions()
			{
				max_height = Settings.SharedSettings.MaximumImageHeight,
				use_lossless = Settings.SharedSettings.LosslessCompression,
				quality = Settings.SharedSettings.ImageQuality,
				use_8bit_palette = Settings.SharedSettings.IndexedPixelFormat,
				use_8bit_palette_but_no_use_over_256_color =
					Settings.SharedSettings.OnlyIndexedPixelFormat,
				use_grayscale = Settings.SharedSettings.GrayscalePixelFormat,
				use_grayscale_but_no_use_to_grayscale_image =
					Settings.SharedSettings.OnlyGrayscalePixelFormat,
				no_convert_to_png_when_detected_transparent_color =
					Settings.SharedSettings.OnlyConvertNoTransparentDetected,
				resize_filter = Settings.SharedSettings.ResizeFilter,
			};

			using var sourceStream = new FileStream(fileInfo.OriginalFilename, FileMode.Open, FileAccess.Read);
			var tempFileName = Path.Combine(Settings.SharedSettings.ConversionPath, Path.GetFileName(Path.GetTempFileName()));
			string newFileName = null;
			var ret = false;
			try
			{
				using (Stream destinationStream = new FileStream(tempFileName, FileMode.Create, FileAccess.ReadWrite))
				{
					if (ProcessingFormat.IsSupportContainerFormat(fileInfo.Extension))
					{
						ret = Degration_Zip(destinationStream, sourceStream, Path.GetFileName(fileInfo.OriginalFilename), status, cancellationToken);
						if (ret)
							newFileName = Path.Combine(Settings.SharedSettings.ConversionPath, Path.GetFileNameWithoutExtension(fileInfo.OriginalFilename) + ".zip");
					}
					else
					{
						ret = Degration_SingleFile(destinationStream, sourceStream, options, out var format, cancellationToken);

						if (ret)
							newFileName = Path.Combine(Settings.SharedSettings.ConversionPath, GetFileName(Path.GetFileNameWithoutExtension(fileInfo.OriginalFilename), format));
					}
				}
				if (newFileName != null)
					Daramee.Winston.File.Operation.Move(newFileName, tempFileName, Settings.SharedSettings.FileOverwrite);
			}
			catch
			{
				Daramee.Winston.File.Operation.Delete(tempFileName);
				ret = false;
			}
			finally
			{
				if (ret)
					fileInfo.Status = DegraStatus.Done;
				else if (cancellationToken.IsCancellationRequested)
					fileInfo.Status = DegraStatus.Cancelled;
				else
					fileInfo.Status = DegraStatus.Failed;

				status.Progress = 1;
			}

			return ret;
		}

		private static void StreamCopy(Stream dest, Stream src)
		{
			var buffer = new byte[4096];
			while (true)
			{
				var read = src.Read(buffer, 0, buffer.Length);
				if (read == 0) break;
				dest.Write(buffer, 0, read);
			}
		}

		private static string GetExtension(DegrationFormat format)
		{
			return format switch
			{
				DegrationFormat.WebP => "webp",
				DegrationFormat.JPEG => "jpg",
				DegrationFormat.PNG => "png",
				_ => ""
			};
		}

		private static string GetFileName(string filename, DegrationFormat format)
		{
			var extensionPosition = filename.LastIndexOf('.');
			if (extensionPosition < filename.LastIndexOf('\\') || extensionPosition < filename.LastIndexOf('/'))
				extensionPosition = -1;

			return extensionPosition == -1 
				? $"{filename}.{GetExtension(format)}"
				: $"{filename[..extensionPosition]}.{GetExtension(format)}";
		}

		private static DegrationFormat GetFormat(string extension, DegrationFormat format)
		{
			if (format != DegrationFormat.OriginalFormat)
				return format;

			return extension switch
			{
				"png" => DegrationFormat.PNG,
				"webp" => DegrationFormat.WebP,
				"jpg" => DegrationFormat.JPEG,
				_ => ProcessingFormat.IsSupportImageFormat(extension)
					? DegrationFormat.WebP
					: DegrationFormat.OriginalFormat
			};
		}

		private static readonly ConcurrentQueue<MemoryStream> StreamBag = new();
		static MemoryStream GetMemoryStream()
		{
			return StreamBag.TryDequeue(out var ret)
				? ret
				: new MemoryStream();
		}
		static void ReturnMemoryStream(MemoryStream stream)
		{
			StreamBag.Enqueue(stream);
		}
		public static void CleanupMemory()
		{
			while (StreamBag.TryDequeue(out var stream))
				stream.Dispose();
			GC.Collect();
		}

		private static bool Degration_Zip(Stream dest, Stream src, string containerName, ProgressStatus status, CancellationToken cancellationToken)
		{
			using var srcArchive = ArchiveFactory.Open(src, new ReaderOptions() { LeaveStreamOpen = true });
			var entryCount = srcArchive.Entries.Count();
			using var destArchive = new System.IO.Compression.ZipArchive(dest, System.IO.Compression.ZipArchiveMode.Create, true);
			var proceed = 0;
			var cache = new ConcurrentQueue<KeyValuePair<string, MemoryStream>>();

			var options = new NativeBridge.DegraOptions()
			{
				save_format = (NativeBridge.DegraSaveFormat) Settings.SharedSettings.ImageFormat,
				max_height = Settings.SharedSettings.MaximumImageHeight,
				use_lossless = Settings.SharedSettings.LosslessCompression,
				quality = Settings.SharedSettings.ImageQuality,
				use_8bit_palette = Settings.SharedSettings.IndexedPixelFormat,
				use_8bit_palette_but_no_use_over_256_color =
					Settings.SharedSettings.OnlyIndexedPixelFormat,
				use_grayscale = Settings.SharedSettings.GrayscalePixelFormat,
				use_grayscale_but_no_use_to_grayscale_image =
					Settings.SharedSettings.OnlyGrayscalePixelFormat,
				no_convert_to_png_when_detected_transparent_color =
					Settings.SharedSettings.OnlyConvertNoTransparentDetected,
				resize_filter = Settings.SharedSettings.ResizeFilter,
			};

			Task.Run(
				() =>
				{
					try
					{
						Parallel.ForEach(srcArchive.Entries,
							new ParallelOptions() { CancellationToken = cancellationToken, MaxDegreeOfParallelism = Settings.SharedSettings.LogicalThreadCount },
							(entry) =>
							{
								if (entry.IsDirectory || cancellationToken.IsCancellationRequested)
								{
									Interlocked.Increment(ref proceed);
									return;
								}

								var readStream = GetMemoryStream();
								var convStream = GetMemoryStream();

								lock (srcArchive)
								{
									using var entryStream = entry.OpenEntryStream();
									StreamCopy(readStream, entryStream);
								}

								status.ProceedFile = $"{containerName} - {entry.Key}";
								var detector = DetectorService.DetectDetector(readStream);
								if (detector != null && ProcessingFormat.IsSupportImageFormat(detector.Extension))
								{
									readStream.Position = 0;
									var ret = Degration_SingleFile(convStream, readStream, options, out var format2, cancellationToken);
									convStream.Position = 0;

									if (!ret || ((GetExtension(format2) == detector.Extension) && (readStream.Length <= convStream.Length)))
										cache.Enqueue(new KeyValuePair<string, MemoryStream>(entry.Key, readStream));
									else
										cache.Enqueue(new KeyValuePair<string, MemoryStream>(GetFileName(entry.Key, format2), convStream));
								}
								else
									cache.Enqueue(new KeyValuePair<string, MemoryStream>(entry.Key, readStream));
							}
						);
					}
					catch (OperationCanceledException ex)
					{
						Debug.WriteLine(ex);
					}
				}, cancellationToken);

			Task.Run(() =>
			{
				while (status.Progress < 1)
				{
					if (cancellationToken.IsCancellationRequested)
						break;

					if (!cache.TryDequeue(out KeyValuePair<string, MemoryStream> kv))
						continue;

					status.ProceedFile = kv.Key;

					var destEntry = destArchive.CreateEntry(kv.Key);
					using (var destEntryStream = destEntry.Open())
					{
						kv.Value.Position = 0;
						StreamCopy(destEntryStream, kv.Value);
					}

					status.Progress = Interlocked.Increment(ref proceed) / (double)entryCount;

					kv.Value.SetLength(0);
					ReturnMemoryStream(kv.Value);
				}
			}, cancellationToken);

			while (status.Progress < 1)
			{
				if (cancellationToken.IsCancellationRequested)
					break;
				Thread.Sleep(0);
			}

			return !cancellationToken.IsCancellationRequested;
		}

		private static bool Degration_SingleFile(Stream dest, Stream src, in NativeBridge.DegraOptions options, out DegrationFormat format, CancellationToken cancellationToken)
		{
			if (cancellationToken.IsCancellationRequested)
			{
				format = DegrationFormat.OriginalFormat;
				return false;
			}

			using var inputStream = new DegraStream(src);
			using var outputStream = new DegraStream(dest);

			var result = NativeBridge.Degra_DoProcess(inputStream, outputStream, options, out var savedFormat);

			switch (savedFormat)
			{
				case NativeBridge.DegraSaveFormat.WebP:
					format = DegrationFormat.WebP;
					break;

				case NativeBridge.DegraSaveFormat.Jpeg:
					format = DegrationFormat.JPEG;
					break;

				case NativeBridge.DegraSaveFormat.Png:
					format = DegrationFormat.PNG;
					break;

				default:
					format = DegrationFormat.OriginalFormat;
					return false;
			}

			return result != NativeBridge.DegraResult.Failed;
		}
	}
}
