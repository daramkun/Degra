using Daramee.Degra.Native;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra
{
	public class SaveData : INotifyPropertyChanged
	{
		string convPathText = System.Environment.GetFolderPath ( Environment.SpecialFolder.MyPictures );
		bool fileOverwrite = false;
		DegrationFormat imageFormat = DegrationFormat.OriginalFormat;
		uint maxHeight = 8192;
		ResizeFilter resizeFilter = ResizeFilter.Lanczos;
		ushort quality = 90;
		bool lossless = false;
		bool indexedPixelFormat = true;
		bool zopfliOpt = true;
		bool histogramEqualization = false;
		bool noConvTransparentDetect = true;
		int threadCount = 0;

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }

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

		public ushort ImageQuality
		{
			get { return quality; }
			set
			{
				quality = value;
				PC ( nameof ( ImageQuality ) );
			}
		}

		public bool Lossless
		{
			get { return lossless; }
			set
			{
				lossless = value;
				PC ( nameof ( Lossless ) );
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

		public bool NoConvertTransparentDetected
		{
			get { return noConvTransparentDetect; }
			set
			{
				noConvTransparentDetect = value;
				PC ( nameof ( NoConvertTransparentDetected ) );
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
	}
}
