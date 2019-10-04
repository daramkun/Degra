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
		public static bool Degration ( FileInfo fileInfo, ProgressStatus status, CancellationToken cancellationToken )
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
				string tempFileName = Path.Combine ( Settings.SharedSettings.ConversionPath, Path.GetFileName ( Path.GetTempFileName () ) );
				string newFileName = null;
				bool ret = false;
				try
				{
					using ( Stream destinationStream = new FileStream ( tempFileName, FileMode.Create, FileAccess.ReadWrite ) )
					{
						if ( fileInfo.Extension == "zip" || fileInfo.Extension == "rar" || fileInfo.Extension == "7z" || fileInfo.Extension == "tar" )
						{
							ret = Degration_Zip ( destinationStream, sourceStream, status, cancellationToken );
							if ( ret )
								newFileName = Path.Combine ( Settings.SharedSettings.ConversionPath, Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ) + ".zip" );
						}
						else
						{
							DegrationFormat format;
							ret = Degration_SingleFile ( destinationStream, sourceStream, fileInfo.Extension, out format, cancellationToken );

							if ( ret )
								newFileName = Path.Combine ( Settings.SharedSettings.ConversionPath, GetFileName ( Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ), format ) );
						}
					}
					if ( newFileName != null )
						Daramee.Winston.File.Operation.Move ( newFileName, tempFileName, Settings.SharedSettings.FileOverwrite );
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

		private static bool Degration_Zip ( Stream dest, Stream src, ProgressStatus status, CancellationToken cancellationToken )
		{
			using ( IArchive srcArchive = ArchiveFactory.Open ( src, new ReaderOptions () { LeaveStreamOpen = true } ) )
			{
				int entryCount = srcArchive.Entries.Count ();
				using ( System.IO.Compression.ZipArchive destArchive = new System.IO.Compression.ZipArchive ( dest, System.IO.Compression.ZipArchiveMode.Create, true ) )
				{
					int proceed = 0;
					ConcurrentQueue<KeyValuePair<string, MemoryStream>> cache = new ConcurrentQueue<KeyValuePair<string, MemoryStream>> ();

					Task.Run (
						() =>
						{
							try
							{
								Parallel.ForEach ( srcArchive.Entries,
									new ParallelOptions () { CancellationToken = cancellationToken, MaxDegreeOfParallelism = Settings.SharedSettings.LogicalThreadCount },
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
											bool ret = Degration_SingleFile ( convStream, readStream, detector.Extension, out format2, cancellationToken );
											convStream.Position = 0;

											if ( !ret || ( ( GetExtension ( format2 ) == detector.Extension ) && ( readStream.Length <= convStream.Length ) && !Settings.SharedSettings.HistogramEqualization ) )
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

		private static bool Degration_SingleFile ( Stream dest, Stream src, string ext, out DegrationFormat format, CancellationToken cancellationToken )
		{
			if ( cancellationToken.IsCancellationRequested )
			{
				format = DegrationFormat.OriginalFormat;
				return false;
			}

			if ( Settings.SharedSettings.ImageFormat == DegrationFormat.OriginalFormat )
			{
				if ( ext == "png" )
					format = DegrationFormat.PNG;
				else if ( ext == "webp" )
					format = DegrationFormat.WebP;
				else if ( ext == "jpg" )
					format = DegrationFormat.JPEG;
				else if ( ext == "jp2"
					|| ext == "tif"
					|| ext == "tga"
					|| ext == "bmp" )
					format = DegrationFormat.WebP;
				else
				{
					format = DegrationFormat.OriginalFormat;
					return false;
				}
			}
			else
				format = Settings.SharedSettings.ImageFormat;

			DegraBitmap bitmap = new DegraBitmap ( src );
			dest.Position = 0;

			bool containsTransparent = false, grayscaleOnly = false, palettable = false;
			if ( ( format == DegrationFormat.JPEG && Settings.SharedSettings.OnlyConvertNoTransparentDetected )
				|| Settings.SharedSettings.LogicalOnlyIndexedPixelFormat
				|| Settings.SharedSettings.LogicalOnlyGrayscalePixelFormat )
			{
				bitmap.DetectBitmapProperties ( out containsTransparent, out grayscaleOnly, out palettable );
				if ( format == DegrationFormat.JPEG && Settings.SharedSettings.OnlyConvertNoTransparentDetected && containsTransparent )
					return false;
			}

			if ( bitmap.Size.Height > Settings.SharedSettings.MaximumImageHeight )
				bitmap.Resize ( ( int ) Settings.SharedSettings.MaximumImageHeight, Settings.SharedSettings.ResizeFilter );

			if ( Settings.SharedSettings.HistogramEqualization )
				bitmap.HistogramEqualization ();

			if ( Settings.SharedSettings.GrayscalePixelFormat && format != DegrationFormat.WebP )
				if ( Settings.SharedSettings.LogicalOnlyGrayscalePixelFormat && grayscaleOnly )
					bitmap.To8BitGrayscaleColorFormat ();
			if ( Settings.SharedSettings.IndexedPixelFormat && format == DegrationFormat.PNG )
				if ( Settings.SharedSettings.LogicalOnlyIndexedPixelFormat && palettable )
					bitmap.To8BitIndexedColorFormat ();

			switch ( format )
			{
				case DegrationFormat.WebP:
					bitmap.SaveToWebP ( dest, Settings.SharedSettings.ImageQuality, Settings.SharedSettings.LosslessCompression );
					break;

				case DegrationFormat.JPEG:
					bitmap.SaveToJPEG ( dest, Settings.SharedSettings.ImageQuality );
					break;

				case DegrationFormat.PNG:
					bitmap.SaveToPNG ( dest, Settings.SharedSettings.ZopfliPNGOptimization );
					break;

				default: return false;
			}

			return true;
		}
	}
}
