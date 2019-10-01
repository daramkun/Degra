using Daramee.Degra.Native;
using Daramee.FileTypeDetector;
using SharpCompress.Archives;
using SharpCompress.Archives.Zip;
using SharpCompress.Readers;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
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
			get { return progress; }
			set
			{
				progress = value;
				PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( nameof ( Progress ) ) );
			}
		}
		public string ProceedFile
		{
			get { return proceedFile; }
			set
			{
				proceedFile = value;
				PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( nameof ( ProceedFile ) ) );
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
		public static bool Degration ( FileInfo fileInfo, ProgressStatus status, in Settings args, CancellationToken cancellationToken )
		{
			fileInfo.Status = DegraStatus.Processing;

			status.ProceedFile = fileInfo.OriginalFilename;
			status.Progress = 0;

			if ( cancellationToken.IsCancellationRequested )
			{
				status.Progress = 1;
				fileInfo.Status = DegraStatus.Cancelled;
				return false;
			}
			if ( !File.Exists ( fileInfo.OriginalFilename ) )
			{
				status.Progress = 1;
				fileInfo.Status = DegraStatus.Failed;
				return false;
			}

			using ( Stream sourceStream = new FileStream ( fileInfo.OriginalFilename, FileMode.Open, FileAccess.Read ) )
			{
				string tempFileName = Path.Combine ( args.ConversionPath, Path.GetFileName ( Path.GetTempFileName () ) );
				string newFileName = null;
				bool ret = false;
				try
				{
					using ( Stream destinationStream = new FileStream ( tempFileName, FileMode.Create, FileAccess.ReadWrite ) )
					{
						var detector = DetectorService.DetectDetector ( sourceStream );
						sourceStream.Position = 0;
						if ( detector.Extension == "zip" || detector.Extension == "rar" || detector.Extension == "7z" || detector.Extension == "tar" )
						{
							ret = Degration_Zip ( destinationStream, sourceStream, status, args, cancellationToken );
							if ( ret )
								newFileName = Path.Combine ( args.ConversionPath, Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ) + ".zip" );
						}
						else
						{
							DegrationFormat format;
							ret = Degration_SingleFile ( destinationStream, sourceStream, out format, args, cancellationToken );

							if ( ret )
								newFileName = Path.Combine ( args.ConversionPath, GetFileName ( Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ), format ) );
						}
					}
					if ( newFileName != null )
						Daramee.Winston.File.Operation.Move ( newFileName, tempFileName, args.FileOverwrite );
				}
				catch
				{
					Daramee.Winston.File.Operation.Delete ( tempFileName );
					ret = false;
				}
				finally
				{
					if ( ret )
						fileInfo.Status = DegraStatus.Done;
					else if ( cancellationToken.IsCancellationRequested )
						fileInfo.Status = DegraStatus.Cancelled;
					else
						fileInfo.Status = DegraStatus.Failed;

					status.Progress = 1;
				}

				return ret;
			}
		}

		private static void StreamCopy (Stream dest, Stream src)
		{
			byte [] buffer = new byte [ 4096 ];
			int totalRead = 0;
			while ( true )
			{
				int read = src.Read ( buffer, 0, buffer.Length );
				if ( read == 0 ) break;
				dest.Write ( buffer, 0, read );
				totalRead += read;
			}
		}

		private static string GetExtension ( DegrationFormat format )
		{
			switch(format)
			{
				case DegrationFormat.WebP: return "webp";
				case DegrationFormat.JPEG: return "jpg";
				case DegrationFormat.PNG: return "png";
				default: return "";
			}
		}

		private static string GetFileName ( string filename, DegrationFormat format )
		{
			int extensionPosition = filename.LastIndexOf ( '.' );
			if ( extensionPosition < filename.LastIndexOf ( '\\' ) || extensionPosition < filename.LastIndexOf ( '/' ) )
				extensionPosition = -1;

			if ( extensionPosition == -1 )
				return $"{filename}.{GetExtension ( format )}";
			else
				return $"{filename.Substring ( 0, extensionPosition )}.{GetExtension ( format )}";
		}

		private static DegrationFormat GetFormat (IDetector detector, DegrationFormat format)
		{
			if ( format == DegrationFormat.OriginalFormat )
			{
				if ( detector.Extension == "png" )
					return DegrationFormat.PNG;
				else if ( detector.Extension == "webp" )
					return DegrationFormat.WebP;
				else if ( detector.Extension == "jpg" )
					return DegrationFormat.JPEG;
				else if ( detector.Extension == "jp2"
					|| detector.Extension == "tif"
					|| detector.Extension == "tga"
					|| detector.Extension == "bmp" )
					return DegrationFormat.WebP;
				else
					throw new ArgumentException ();
			}
			return format;
		}

		static ConcurrentQueue<MemoryStream> StreamBag = new ConcurrentQueue<MemoryStream> ();
		static MemoryStream GetMemoryStream()
		{
			if ( StreamBag.TryDequeue ( out MemoryStream ret ) )
				return ret;
			return new MemoryStream ();
		}
		static void ReturnMemoryStream (MemoryStream stream)
		{
			StreamBag.Enqueue ( stream );
		}
		public static void CleanupMemory()
		{
			while ( StreamBag.TryDequeue ( out MemoryStream stream ) )
				stream.Dispose ();
			GC.Collect ();
		}

		private static bool Degration_Zip ( Stream dest, Stream src, ProgressStatus status, in Settings args, CancellationToken cancellationToken )
		{
			using ( IArchive srcArchive = ArchiveFactory.Open ( src, new ReaderOptions () { LeaveStreamOpen = true } ) )
			{
				int entryCount = srcArchive.Entries.Count ();
				using ( System.IO.Compression.ZipArchive destArchive = new System.IO.Compression.ZipArchive ( dest, System.IO.Compression.ZipArchiveMode.Create, true ) )
				{
					int proceed = 0;
					ConcurrentQueue<KeyValuePair<string, MemoryStream>> cache = new ConcurrentQueue<KeyValuePair<string, MemoryStream>> ();

					Settings dupArgs = args.Clone () as Settings;

					Task.Run (
						() =>
						{
							try
							{
								Parallel.ForEach ( srcArchive.Entries,
									new ParallelOptions () { CancellationToken = cancellationToken, MaxDegreeOfParallelism = dupArgs.ThreadCount == 0 ? Math.Max ( Environment.ProcessorCount - 1, 1 ) : dupArgs.ThreadCount },
									( entry ) =>
									{
										if ( entry.IsDirectory || cancellationToken.IsCancellationRequested )
										{
											Interlocked.Increment ( ref proceed );
											return;
										}

										MemoryStream readStream = GetMemoryStream ();
										MemoryStream convStream = GetMemoryStream ();

										lock ( srcArchive )
										{
											using ( Stream entryStream = entry.OpenEntryStream () )
												StreamCopy ( readStream, entryStream );
										}

										status.ProceedFile = entry.Key;
										var detector = DetectorService.DetectDetector ( readStream );
										if ( detector != null && ( detector.Extension == "bmp" || detector.Extension == "jpg" || detector.Extension == "jp2" || detector.Extension == "webp" || detector.Extension == "png" ) )
										{
											readStream.Position = 0;
											DegrationFormat format2;
											bool ret = Degration_SingleFile ( convStream, readStream, out format2, dupArgs, cancellationToken );
											convStream.Position = 0;

											if ( !ret || ( ( GetExtension ( format2 ) == detector.Extension ) && ( readStream.Length <= convStream.Length ) && !dupArgs.HistogramEqualization ) )
												cache.Enqueue ( new KeyValuePair<string, MemoryStream> ( entry.Key, readStream ) );
											else
												cache.Enqueue ( new KeyValuePair<string, MemoryStream> ( GetFileName ( entry.Key, format2 ), convStream ) );
										}
										else
											cache.Enqueue ( new KeyValuePair<string, MemoryStream> ( entry.Key, readStream ) );
									}
								);
							}
							catch ( OperationCanceledException ex )
							{
								Debug.WriteLine ( ex );
							}
						}
					);

					Task.Run ( () =>
					{
						while ( status.Progress < 1 )
						{
							if ( cancellationToken.IsCancellationRequested )
								break;

							if ( !cache.TryDequeue ( out KeyValuePair<string, MemoryStream> kv ) )
								continue;

							status.ProceedFile = kv.Key;

							var destEntry = destArchive.CreateEntry ( kv.Key );
							using ( Stream destEntryStream = destEntry.Open () )
							{
								kv.Value.Position = 0;
								StreamCopy ( destEntryStream, kv.Value );
							}

							status.Progress = Interlocked.Increment ( ref proceed ) / ( double ) entryCount;

							kv.Value.SetLength ( 0 );
							ReturnMemoryStream ( kv.Value );
						}
					} );

					while ( status.Progress < 1 )
					{
						if ( cancellationToken.IsCancellationRequested )
							break;
						Thread.Sleep ( 0 );
					}
				}
			}
			return cancellationToken.IsCancellationRequested ? false : true;
		}

		private static bool Degration_SingleFile ( Stream dest, Stream src, out DegrationFormat format, in Settings args, CancellationToken cancellationToken )
		{
			if ( cancellationToken.IsCancellationRequested )
			{
				format = DegrationFormat.OriginalFormat;
				return false;
			}

			var detector = DetectorService.DetectDetector ( src );
			src.Position = 0;

			if ( args.ImageFormat == DegrationFormat.OriginalFormat )
			{
				if ( detector.Extension == "png" )
					format = DegrationFormat.PNG;
				else if ( detector.Extension == "webp" )
					format = DegrationFormat.WebP;
				else if ( detector.Extension == "jpg" )
					format = DegrationFormat.JPEG;
				else if ( detector.Extension == "jp2"
					|| detector.Extension == "tif"
					|| detector.Extension == "tga"
					|| detector.Extension == "bmp" )
					format = DegrationFormat.WebP;
				else
				{
					format = DegrationFormat.OriginalFormat;
					return false;
				}
			}
			else
				format = args.ImageFormat;

			DegraBitmap bitmap = new DegraBitmap ( src );
			dest.Position = 0;

			if ( format == DegrationFormat.JPEG && args.NoConvertTransparentDetected )
				if ( bitmap.DetectTransparent () )
					return false;

			if ( bitmap.Size.Height > args.MaximumImageHeight )
				bitmap.Resize ( ( int ) args.MaximumImageHeight, args.ResizeFilter );

			if ( args.HistogramEqualization )
				bitmap.HistogramEqualization ();

			if ( args.GrayscalePixelFormat && format != DegrationFormat.WebP )
				bitmap.To8BitGrayscaleColorFormat ();
			else if ( args.IndexedPixelFormat && format == DegrationFormat.PNG )
				bitmap.To8BitIndexedColorFormat ();

			switch ( format )
			{
				case DegrationFormat.WebP:
					{
						bitmap.SaveToWebP ( dest, args.ImageQuality, args.Lossless );
					}
					break;

				case DegrationFormat.JPEG:
					{
						bitmap.SaveToJPEG ( dest, args.ImageQuality );
					}
					break;

				case DegrationFormat.PNG:
					{
						bitmap.SaveToPNG ( dest, args.ZopfliPNGOptimization );
					}
					break;

				default: return false;
			}

			return true;
		}
	}
}
