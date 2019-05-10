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

		static readonly string [] SupportDecodingImageFormats = new [] { "bmp", "png", "jpg", "hdp", "tif", "gif" };

		private static ProceedFormat CompressionWIC ( Stream dest, Stream src, Argument args )
		{
			Compress ( dest, src, args );
			dest.Flush ();

			if ( args.Settings is WebPSettings ) return ProceedFormat.WebP;
			else if ( args.Settings is JpegSettings ) return ProceedFormat.Jpeg;
			else if ( args.Settings is PngSettings ) return ProceedFormat.Png;

			return ProceedFormat.Unknown;
		}

		private static ProceedFormat CompressionZIPDifferent ( Stream dest, Stream src, Argument args, ProgressState state, string srcPath )
		{
			using ZipArchive sourceArchive = new ZipArchive ( src, ZipArchiveMode.Read );
			using ZipArchive destinationArchive = new ZipArchive ( dest, ZipArchiveMode.Create );

			var extension = args.Settings.Extension;

			List<ZipArchiveEntry> entries = new List<ZipArchiveEntry> ( sourceArchive.Entries );
			int proceedCount = 0;
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
				if ( imgDetect != null && SupportDecodingImageFormats.Contains ( imgDetect.Extension ) )
				{
					var destinationEntry = destinationArchive.CreateEntry (
						Path.Combine ( Path.GetDirectoryName ( sourceEntry.FullName ), Path.GetFileNameWithoutExtension ( sourceEntry.FullName ) + extension )
					);
					Stream destinationEntryStream = destinationEntry.Open ();

					try
					{
						Compress ( destinationEntryStream, memoryStream, args );
					}
					catch
					{
						destinationEntryStream.Dispose ();
						destinationEntry.Delete ();

						destinationEntry = destinationArchive.CreateEntry ( sourceEntry.FullName, CompressionLevel.Optimal );
						destinationEntryStream = destinationEntry.Open ();
						memoryStream.CopyTo ( destinationEntryStream );
					}
					finally
					{
						destinationEntryStream.Flush ();
						destinationEntryStream.Dispose ();
					}
				}
				else
				{
					var destinationEntry = destinationArchive.CreateEntry ( sourceEntry.FullName, CompressionLevel.Optimal );
					using Stream destinationEntryStream = destinationEntry.Open ();
					memoryStream.CopyTo ( destinationEntryStream );
					destinationEntryStream.Flush ();
					destinationEntryStream.Dispose ();
				}

				if ( state != null )
				{
					state.Progress = Interlocked.Increment ( ref proceedCount ) / ( double ) entries.Count;
					state.ProceedFile = $"{srcPath} - {sourceEntry.FullName}";
				}
			}

			return ProceedFormat.Zip;
		}

		private static ProceedFormat CompressionZIPSame ( Stream dest, Argument args, ProgressState state, string srcPath )
		{
			using ZipArchive destinationArchive = new ZipArchive ( dest, ZipArchiveMode.Update );

			string extension;
			if ( args.Settings is WebPSettings ) extension = ".webp";
			else if ( args.Settings is JpegSettings ) extension = ".jpg";
			else if ( args.Settings is PngSettings ) extension = ".png";
			else throw new ArgumentException ();

			List<ZipArchiveEntry> entries = new List<ZipArchiveEntry> ( destinationArchive.Entries );
			int proceedCount = 0;
			using Stream readStream = new MemoryStream (), writeStream = new MemoryStream ();
			foreach ( var sourceEntry in entries )
			{
				readStream.SetLength ( 0 );
				writeStream.SetLength ( 0 );

				using Stream sourceEntryStream = sourceEntry.Open ();
				sourceEntryStream.CopyTo ( readStream );

				readStream.Position = 0;
				var imgDetect = DetectorService.DetectDetector ( readStream );
				readStream.Position = 0;
				if ( imgDetect != null && ( imgDetect.Extension == "jpg" || imgDetect.Extension == "bmp"
					|| imgDetect.Extension == "png" || imgDetect.Extension == "gif" || imgDetect.Extension == "tif"
					|| imgDetect.Extension == "hdp" ) )
				{
					var sourceEntryName = sourceEntry.FullName;
					sourceEntry.Delete ();

					var destinationEntry = destinationArchive.CreateEntry (
						Path.Combine ( Path.GetDirectoryName ( sourceEntryName ), Path.GetFileNameWithoutExtension ( sourceEntryName ) + extension )
						, CompressionLevel.Optimal
					);
					Stream destinationEntryStream = destinationEntry.Open ();

					try
					{
						Compress ( destinationEntryStream, readStream, args );
					}
					catch
					{
						destinationEntry.Delete ();
						destinationEntry = destinationArchive.CreateEntry ( sourceEntryName, CompressionLevel.Optimal );
						destinationEntryStream = destinationEntry.Open ();
						readStream.Position = 0;
						readStream.CopyTo ( destinationEntryStream );
					}
					finally
					{
						destinationEntryStream.Flush ();
						destinationEntryStream.Dispose ();
					}
				}

				if ( state != null )
				{
					state.Progress = Interlocked.Increment ( ref proceedCount ) / ( double ) entries.Count;
					state.ProceedFile = $"{srcPath} - {sourceEntry.FullName}";
				}
			}

			return ProceedFormat.Zip;
		}

		public static async Task<ProceedFormat> DoCompression ( IStorageFile dest, IStorageFile src, Argument args, ProgressState state = null )
		{
			using var srcStorageFileStream = await src.OpenAsync ( Windows.Storage.FileAccessMode.Read );
			using var sourceStream = srcStorageFileStream.AsStream ();

			using var destStorageFileStream = await dest.OpenAsync ( Windows.Storage.FileAccessMode.ReadWrite );
			using var destinationStream = destStorageFileStream.AsStream ();
			
			var detector = DetectorService.DetectDetector ( sourceStream );
			sourceStream.Position = 0;
			if ( detector.Extension == "zip" )
			{
				return ( dest == src ) ? CompressionZIPDifferent ( destinationStream, sourceStream, args, state, src.Path ) :
					CompressionZIPSame ( destinationStream, args, state, src.Path );
			}
			else
			{
				var format = CompressionWIC ( destinationStream, sourceStream, args );

				if ( format != ProceedFormat.Unknown && state != null )
				{
					state.Progress = 1;
					state.ProceedFile = src.Path;
				}

				return format;
			}
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
			public StreamBridge ( Stream stream ) { BaseStream = stream; }
			public int Read ( byte [] buffer, int length ) { return BaseStream.Read ( buffer, 0, length ); }
			public int Write ( byte [] data, int length ) { BaseStream.Write ( data, 0, length ); return length; }
			public int Seek ( Daramee_Degra.SeekOrigin origin, int offset ) { return ( int ) BaseStream.Seek ( offset, ( System.IO.SeekOrigin ) origin ); }
			public void Flush () { BaseStream.Flush (); }
		}
	}
}
