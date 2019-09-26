using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra.Native
{
	public enum ResizeFilter
	{
		Nearest,
		Bilinear,
		Bicubic,
		Lanczos,
	}

	public sealed class DegraBitmap : IDisposable
	{
		IntPtr nativeBitmap;

		internal DegraBitmap (IntPtr bitmap)
		{
			if ( bitmap == IntPtr.Zero )
				throw new ArgumentNullException ();
			nativeBitmap = bitmap;
		}

		public DegraBitmap (Stream stream)
		{
			if ( stream == null )
				throw new ArgumentNullException ();

			using ( DegraStream degraStream = new DegraStream ( stream ) )
			{
				nativeBitmap = NativeBridge.Degra_LoadImageFromStream ( degraStream );
			}
		}

		~DegraBitmap ()
		{
			DisposeInternal ();
		}

		public void Dispose ()
		{
			DisposeInternal ();
			GC.SuppressFinalize ( this );
		}

		private void DisposeInternal ()
		{
			if ( nativeBitmap != IntPtr.Zero )
				NativeBridge.Degra_DestroyImage ( nativeBitmap );
			nativeBitmap = IntPtr.Zero;
		}

		public Size Size
		{
			get
			{
				NativeBridge.Degra_GetImageSize ( nativeBitmap, out uint width, out uint height );
				return new Size ( ( int ) width, ( int ) height );
			}
		}

		public void To8BitIndexedColorFormat ()
		{
			var newBitmap = NativeBridge.Degra_ImagePixelFormatToPalette8Bit ( nativeBitmap );
			if ( newBitmap != IntPtr.Zero )
			{
				NativeBridge.Degra_DestroyImage ( nativeBitmap );
				nativeBitmap = newBitmap;
			}
			else throw new Exception ();
		}

		public void Resize ( int height, ResizeFilter filter = ResizeFilter.Bilinear )
		{
			var newBitmap = NativeBridge.Degra_ImageResize ( nativeBitmap, ( NativeBridge.DegraImageResizeFilter ) ( int ) filter, height );
			if ( newBitmap != IntPtr.Zero )
			{
				NativeBridge.Degra_DestroyImage ( nativeBitmap );
				nativeBitmap = newBitmap;
			}
			else throw new Exception ();
		}

		public void HistogramEqualization ()
		{
			var newBitmap = NativeBridge.Degra_ImageHistogramEqualization ( nativeBitmap );
			if ( newBitmap != IntPtr.Zero )
			{
				NativeBridge.Degra_DestroyImage ( nativeBitmap );
				nativeBitmap = newBitmap;
			}
			else throw new Exception ();
		}
		public bool DetectTransparent ()
		{
			return NativeBridge.Degra_DetectTransparent ( nativeBitmap );
		}

		public void SaveToJPEG ( Stream stream, int quality )
		{
			if ( quality < 1 || quality > 100 )
				throw new ArgumentOutOfRangeException ();
			using ( DegraStream degraStream = new DegraStream ( stream ) )
			{
				var options = new NativeBridge.JPEGOptions () { quality = ( uint ) quality };
				NativeBridge.Degra_SaveImageToStreamJPEG ( nativeBitmap, ref options, degraStream );
			}
		}

		public void SaveToWebP ( Stream stream, int quality, bool lossless )
		{
			if ( quality < 1 || quality > 100 )
				throw new ArgumentOutOfRangeException ();
			using ( DegraStream degraStream = new DegraStream ( stream ) )
			{
				var options = new NativeBridge.WebPOptions ()
				{
					quality = ( uint ) quality,
					lossless = lossless
				};
				NativeBridge.Degra_SaveImageToStreamWebP ( nativeBitmap, ref options, degraStream );
			}
		}

		public void SaveToPNG ( Stream stream, bool zopfliRecompress )
		{
			using ( DegraStream degraStream = new DegraStream ( stream ) )
			{
				var options = new NativeBridge.PNGOptions () { zopfli = zopfliRecompress };
				NativeBridge.Degra_SaveImageToStreamPNG ( nativeBitmap, ref options, degraStream );
			}
		}
	}
}
