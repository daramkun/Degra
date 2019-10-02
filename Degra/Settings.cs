using Daramee.Degra.Native;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra
{
	public class Settings : INotifyPropertyChanged
	{
		public static Settings SharedSettings { get; private set; }

		string convPathText = System.Environment.GetFolderPath ( Environment.SpecialFolder.MyPictures );
		bool fileOverwrite = false;
		DegrationFormat imageFormat = DegrationFormat.OriginalFormat;
		uint maxHeight = 8192;
		ResizeFilter resizeFilter = ResizeFilter.Lanczos;
		ushort quality = 90;
		bool losslessCompression = false;
		bool onlyConvertNoTransparentDetected = true;
		bool indexedPixelFormat = true;
		bool onlyIndexedPixelFormat = true;
		bool grayscalePixelFormat = false;
		bool onlyGrayscalePixelFormat = true;
		bool zopfliOpt = true;
		bool histogramEqualization = false;
		int threadCount = 0;

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }

		public Settings ()
		{
			if ( SharedSettings == null )
				SharedSettings = this;
		}

		public string ConversionPath
		{
			get { return convPathText; }
			set
			{
				convPathText = value;
				PC ( nameof ( ConversionPath ) );
			}
		}

		public bool FileOverwrite
		{
			get { return fileOverwrite; }
			set
			{
				fileOverwrite = value;
				PC ( nameof ( FileOverwrite ) );
			}
		}

		public DegrationFormat ImageFormat
		{
			get { return imageFormat; }
			set
			{
				imageFormat = value;
				PC ( nameof ( ImageFormat ) );
			}
		}

		public ushort ImageQuality
		{
			get { return quality; }
			set
			{
				quality = value;
				PC ( nameof ( ImageQuality ) );
			}
		}

		public int ThreadCount
		{
			get { return threadCount; }
			set
			{
				threadCount = value;
				PC ( nameof ( ThreadCount ) );
			}
		}
		public int LogicalThreadCount => ThreadCount != 0 ? ThreadCount : Environment.ProcessorCount;

		public uint MaximumImageHeight
		{
			get { return maxHeight; }
			set
			{
				maxHeight = value;
				PC ( nameof ( MaximumImageHeight ) );
			}
		}

		public ResizeFilter ResizeFilter
		{
			get { return resizeFilter; }
			set
			{
				resizeFilter = value;
				PC ( nameof ( ResizeFilter ) );
			}
		}

		public bool LosslessCompression
		{
			get { return losslessCompression; }
			set
			{
				losslessCompression = value;
				PC ( nameof ( LosslessCompression ) );
			}
		}

		public bool OnlyConvertNoTransparentDetected
		{
			get { return onlyConvertNoTransparentDetected; }
			set
			{
				onlyConvertNoTransparentDetected = value;
				PC ( nameof ( OnlyConvertNoTransparentDetected ) );
			}
		}

		public bool IndexedPixelFormat
		{
			get { return indexedPixelFormat; }
			set
			{
				indexedPixelFormat = value;
				PC ( nameof ( IndexedPixelFormat ) );
			}
		}

		public bool OnlyIndexedPixelFormat
		{
			get { return onlyIndexedPixelFormat; }
			set
			{
				onlyIndexedPixelFormat = value;
				PC ( nameof ( OnlyIndexedPixelFormat ) );
			}
		}
		public bool LogicalOnlyIndexedPixelFormat => IndexedPixelFormat && OnlyIndexedPixelFormat;

		public bool GrayscalePixelFormat
		{
			get { return grayscalePixelFormat; }
			set
			{
				grayscalePixelFormat = value;
				PC ( nameof ( GrayscalePixelFormat ) );
			}
		}

		public bool OnlyGrayscalePixelFormat
		{
			get { return onlyGrayscalePixelFormat; }
			set
			{
				onlyGrayscalePixelFormat = value;
				PC ( nameof ( OnlyGrayscalePixelFormat ) );
			}
		}
		public bool LogicalOnlyGrayscalePixelFormat => GrayscalePixelFormat && OnlyGrayscalePixelFormat;

		public bool ZopfliPNGOptimization
		{
			get { return zopfliOpt; }
			set
			{
				zopfliOpt = value;
				PC ( nameof ( ZopfliPNGOptimization ) );
			}
		}

		public bool HistogramEqualization
		{
			get { return histogramEqualization; }
			set
			{
				histogramEqualization = value;
				PC ( nameof ( HistogramEqualization ) );
			}
		}
	}
}
