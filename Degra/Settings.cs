using Daramee.Degra.Native;
using System;
using System.ComponentModel;

namespace Daramee.Degra
{
	public class Settings : INotifyPropertyChanged
	{
		public static Settings SharedSettings { get; private set; }

		string convPathText = Environment.GetFolderPath ( Environment.SpecialFolder.MyPictures );
		bool fileOverwrite = false;
		DegrationFormat imageFormat = DegrationFormat.OriginalFormat;
		uint maxHeight = 8192;
		NativeBridge.DegraResizeFilter resizeFilter = NativeBridge.DegraResizeFilter.Lanczos;
		ushort quality = 80;
		bool losslessCompression = false;
		bool onlyConvertNoTransparentDetected = true;
		bool indexedPixelFormat = false;
		bool onlyIndexedPixelFormat = false;
		bool grayscalePixelFormat = false;
		bool onlyGrayscalePixelFormat = true;
		int threadCount = 0;

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }

		public Settings ()
		{
			SharedSettings ??= this;
		}

		public string ConversionPath
		{
			get => convPathText;
			set
			{
				convPathText = value;
				PC ( nameof ( ConversionPath ) );
			}
		}

		public bool FileOverwrite
		{
			get => fileOverwrite;
			set
			{
				fileOverwrite = value;
				PC ( nameof ( FileOverwrite ) );
			}
		}

		public DegrationFormat ImageFormat
		{
			get => imageFormat;
			set
			{
				imageFormat = value;
				PC ( nameof ( ImageFormat ) );
			}
		}

		public ushort ImageQuality
		{
			get => quality;
			set
			{
				quality = value;
				PC ( nameof ( ImageQuality ) );
			}
		}

		public int ThreadCount
		{
			get => threadCount;
			set
			{
				threadCount = value;
				PC ( nameof ( ThreadCount ) );
			}
		}
		public int LogicalThreadCount => ThreadCount != 0 ? ThreadCount : Environment.ProcessorCount;

		public uint MaximumImageHeight
		{
			get => maxHeight;
			set
			{
				maxHeight = value;
				PC ( nameof ( MaximumImageHeight ) );
			}
		}

		public NativeBridge.DegraResizeFilter ResizeFilter
		{
			get => resizeFilter;
			set
			{
				resizeFilter = value;
				PC ( nameof ( ResizeFilter ) );
			}
		}

		public bool LosslessCompression
		{
			get => losslessCompression;
			set
			{
				losslessCompression = value;
				PC ( nameof ( LosslessCompression ) );
			}
		}

		public bool OnlyConvertNoTransparentDetected
		{
			get => onlyConvertNoTransparentDetected;
			set
			{
				onlyConvertNoTransparentDetected = value;
				PC ( nameof ( OnlyConvertNoTransparentDetected ) );
			}
		}

		public bool IndexedPixelFormat
		{
			get => indexedPixelFormat;
			set
			{
				indexedPixelFormat = value;
				PC ( nameof ( IndexedPixelFormat ) );
			}
		}

		public bool OnlyIndexedPixelFormat
		{
			get => onlyIndexedPixelFormat;
			set
			{
				onlyIndexedPixelFormat = value;
				PC ( nameof ( OnlyIndexedPixelFormat ) );
			}
		}

		public bool GrayscalePixelFormat
		{
			get => grayscalePixelFormat;
			set
			{
				grayscalePixelFormat = value;
				PC ( nameof ( GrayscalePixelFormat ) );
			}
		}

		public bool OnlyGrayscalePixelFormat
		{
			get => onlyGrayscalePixelFormat;
			set
			{
				onlyGrayscalePixelFormat = value;
				PC ( nameof ( OnlyGrayscalePixelFormat ) );
			}
		}
	}
}
