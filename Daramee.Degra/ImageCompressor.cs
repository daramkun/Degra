using Daramee.FileTypeDetector;
using Daramee_Degra;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.Storage;

namespace Daramee.Degra
{
	public class ProgressState
	{
		public double Progress;
		public string ProceedFile;
	}

	public enum ProceedFormat
	{
		Unknown,

		WebP,
		Jpeg,
		Png,

		Zip,
	}

	public static class ImageCompressor
	{
		static readonly DegraCore core = new DegraCore ();

		public static async Task<ProceedFormat> DoCompression ( string dest, string src, Argument args, ProgressState state = null )
		{
			var srcStorageFile = await Windows.Storage.StorageFile.GetFileFromPathAsync ( src );
			var srcStorageFileStream = await srcStorageFile.OpenAsync ( Windows.Storage.FileAccessMode.Read, Windows.Storage.StorageOpenOptions.AllowOnlyReaders );
			using var sourceStream = srcStorageFileStream.AsStream ();

			var destStorageFolder = await Windows.Storage.StorageFolder.GetFolderFromPathAsync ( Path.GetDirectoryName ( dest ) );
			var destStorageFile = await destStorageFolder.CreateFileAsync ( Path.GetFileName ( dest ) );
			//var destStorageFile = await Windows.Storage.StorageFile.GetFileFromPathAsync ( dest );
			var destStorageFileStream = await destStorageFile.OpenAsync ( Windows.Storage.FileAccessMode.ReadWrite, Windows.Storage.StorageOpenOptions.AllowOnlyReaders );
			using var destinationStream = destStorageFileStream.AsStream ();
			
			var detector = DetectorService.DetectDetector ( sourceStream );
			sourceStream.Position = 0;
			if ( detector.Extension == "zip" )
			{
				using ZipArchive sourceArchive = new ZipArchive ( sourceStream, ZipArchiveMode.Read );
				using ZipArchive destinationArchive = new ZipArchive ( destinationStream, ZipArchiveMode.Create );

				string extension;
				if ( args.Settings is WebPSettings ) extension = ".webp";
				else if ( args.Settings is JpegSettings ) extension = ".jpg";
				else if ( args.Settings is PngSettings ) extension = ".png";
				else throw new ArgumentException ();

				int entryCount = sourceArchive.Entries.Count, proceedCount = 0;
				using Stream memoryStream = new MemoryStream ();
				foreach ( var sourceEntry in sourceArchive.Entries )
				{
					memoryStream.SetLength ( 0 );
					memoryStream.Position = 0;

					using Stream sourceEntryStream = sourceEntry.Open ();
					sourceEntryStream.CopyTo ( memoryStream );

					memoryStream.Position = 0;
					var imgDetect = DetectorService.DetectDetector ( memoryStream );
					memoryStream.Position = 0;
					if ( imgDetect != null && ( imgDetect.Extension == "jpg" || imgDetect.Extension == "bmp"
						|| imgDetect.Extension == "png" || imgDetect.Extension == "gif" || imgDetect.Extension == "tif"
						|| imgDetect.Extension == "hdp" ) )
					{
						var destinationEntry = destinationArchive.CreateEntry (
							Path.Combine ( Path.GetDirectoryName ( sourceEntry.FullName ), Path.GetFileNameWithoutExtension ( sourceEntry.FullName ) + extension )
						);
						using Stream destinationEntryStream = destinationEntry.Open ();

						Compress ( destinationEntryStream, memoryStream, args );
					}
					else
					{
						var destinationEntry = destinationArchive.CreateEntry ( sourceEntry.FullName );
						using Stream destinationEntryStream = destinationEntry.Open ();
						memoryStream.CopyTo ( destinationEntryStream );
					}

					if ( state != null )
					{
						state.Progress = Interlocked.Increment ( ref proceedCount ) / ( double ) entryCount;
						state.ProceedFile = $"{src} - {sourceEntry.FullName}";
					}
				}

				return ProceedFormat.Zip;
			}
			else
			{
				Compress ( destinationStream, sourceStream, args );
				destinationStream.Flush ();

				if ( state != null )
				{
					state.Progress = 1;
					state.ProceedFile = src;
				}

				if ( args.Settings is WebPSettings ) return ProceedFormat.WebP;
				else if ( args.Settings is JpegSettings ) return ProceedFormat.Jpeg;
				else if ( args.Settings is PngSettings ) return ProceedFormat.Png;
			}

			return ProceedFormat.Unknown;
		}

		private static void Compress ( Stream dest, Stream src, Argument args )
		{
			IDegraStream destStream, srcStream;
			destStream = new StreamBridge ( dest );
			srcStream = new StreamBridge ( src );

			core.ConvertImage ( destStream, srcStream, args );
		}

		private sealed class StreamBridge : IDegraStream
		{
			public Stream BaseStream { get; private set; }

			public int Length => ( int ) BaseStream.Length;

			public StreamBridge ( Stream stream )
			{
				BaseStream = stream;
			}

			public int Read ( byte [] buffer, int length )
			{
				return BaseStream.Read ( buffer, 0, length );
			}

			public int Write ( byte [] data, int length )
			{
				BaseStream.Write ( data, 0, length );
				return length;
			}

			public int Seek ( Daramee_Degra.SeekOrigin origin, int offset )
			{
				return ( int ) BaseStream.Seek ( offset, ( System.IO.SeekOrigin ) origin );
			}

			public void Flush ()
			{
				BaseStream.Flush ();
			}
		}
	}
}
