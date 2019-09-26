using Daramee.Degra.Native;
using Daramee.FileTypeDetector;
using SharpCompress.Archives;
using SharpCompress.Archives.Zip;
using SharpCompress.Readers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
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

	public enum DegrationFormat
	{
		Zip = -1,
		Auto = 0,
		WebP,
		JPEG,
		PNG,
	}

	public struct DegrationArguments
	{
		public DegrationFormat Format;
		public ResizeFilter ResizeFilter;
		public int MaximumImageHeight;
		public int ImageQuality;
		public bool LosslessCompression;
		public bool ZopfliPNGOptimization;
		public bool PNGPixelFormatTo8BitQuantization;
	}

	public static class Degrator
	{
		public static bool Degration ( FileInfo fileInfo, string destination, bool overwrite, ProgressStatus status, DegrationArguments args )
		{
			fileInfo.Status = DegraStatus.Processing;

			status.ProceedFile = fileInfo.OriginalFilename;
			status.Progress = 0;

			using ( Stream sourceStream = new FileStream ( fileInfo.OriginalFilename, FileMode.Open, FileAccess.Read ) )
			{
				string tempFileName = Path.Combine ( destination, Path.GetFileName ( Path.GetTempFileName () ) );
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
							ret = Degration_Zip ( destinationStream, sourceStream, status, args );
							if ( ret )
								newFileName = Path.Combine ( destination, Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ) + ".zip" );
						}
						else
						{
							DegrationFormat format;
							ret = Degration_SingleFile ( destinationStream, sourceStream, out format, args );

							if ( ret )
								newFileName = Path.Combine ( destination, GetFileName ( Path.GetFileNameWithoutExtension ( fileInfo.OriginalFilename ), format ) );
						}
					}
					if ( newFileName != null )
						Daramee.Winston.File.Operation.Move ( newFileName, tempFileName, overwrite );
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
			if ( format == DegrationFormat.Auto )
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

		private static bool Degration_Zip ( Stream dest, Stream src, ProgressStatus status, DegrationArguments args )
		{
			using ( IArchive srcArchive = ArchiveFactory.Open ( src, new ReaderOptions () { LeaveStreamOpen = true } ) )
			{
				int entryCount = srcArchive.Entries.Count ();
				using ( System.IO.Compression.ZipArchive destArchive = new System.IO.Compression.ZipArchive (dest, System.IO.Compression.ZipArchiveMode.Create, true ) )
				{
					using ( MemoryStream readStream = new MemoryStream (), convStream = new MemoryStream () )
					{
						int proceed = 0;
						foreach ( var entry in srcArchive.Entries )
						{
							if ( entry.IsDirectory )
								continue;
							using ( Stream entryStream = entry.OpenEntryStream () )
							{
								readStream.SetLength ( 0 );
								convStream.SetLength ( 0 );

								StreamCopy ( readStream, entryStream );

								status.ProceedFile = entry.Key;
								var detector = DetectorService.DetectDetector ( readStream );
								if ( detector.Extension == "bmp" || detector.Extension == "jpg" || detector.Extension == "jp2" || detector.Extension == "webp" || detector.Extension == "png" )
								{
									readStream.Position = 0;
									DegrationFormat format2;
									bool ret = Degration_SingleFile ( convStream, readStream, out format2, args );
									convStream.Position = 0;
									status.Progress = ( ++proceed ) / ( double ) entryCount;

									if ( !ret )
									{
										var destEntry = destArchive.CreateEntry ( entry.Key );
										using ( Stream destEntryStream = destEntry.Open () )
										{
											StreamCopy ( destEntryStream, entryStream );
										}
										continue;
									}
									else
									{
										var destEntry = destArchive.CreateEntry ( GetFileName ( entry.Key, format2 ) );
										using ( Stream destEntryStream = destEntry.Open () )
										{
											StreamCopy ( destEntryStream, convStream );
										}
									}
								}
								else
								{
									var destEntry = destArchive.CreateEntry ( entry.Key );
									using ( Stream destEntryStream = destEntry.Open () )
									{
										StreamCopy ( destEntryStream, entryStream );
									}
									status.Progress = ( ++proceed ) / ( double ) entryCount;
								}
							}
						}
					}
				}
			}
			return true;
		}

		private static bool Degration_SingleFile ( Stream dest, Stream src, out DegrationFormat format, DegrationArguments args )
		{
			var detector = DetectorService.DetectDetector ( src );
			src.Position = 0;

			if ( args.Format == DegrationFormat.Auto )
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
					format = DegrationFormat.Auto;
					return false;
				}
			}
			else
				format = args.Format;

			DegraBitmap bitmap = new DegraBitmap ( src );
			dest.Position = 0;
			if ( bitmap.Size.Height > args.MaximumImageHeight )
				bitmap.Resize ( args.MaximumImageHeight, args.ResizeFilter );

			switch ( format )
			{
				case DegrationFormat.WebP:
					{
						bitmap.SaveToWebP ( dest, args.ImageQuality, args.LosslessCompression );
					}
					break;

				case DegrationFormat.JPEG:
					{
						bitmap.SaveToJPEG ( dest, args.ImageQuality );
					}
					break;

				case DegrationFormat.PNG:
					{
						if ( args.PNGPixelFormatTo8BitQuantization )
							bitmap.To8BitIndexedColorFormat ();
						bitmap.SaveToPNG ( dest, args.ZopfliPNGOptimization );
					}
					break;

				default: return false;
			}

			return true;
		}
	}
}
