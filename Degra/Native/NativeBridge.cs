using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra.Native
{
	public static class NativeBridge
	{
		public delegate ulong DegraStreamRead ( IntPtr userData, IntPtr buffer, ulong length );
		public delegate ulong DegraStreamWrite ( IntPtr userData, IntPtr buffer, ulong length );
		public delegate bool DegraStreamSeek ( IntPtr userData, SeekOrigin origin, ulong offset );
		public delegate void DegraStreamFlush ( IntPtr userData );
		public delegate ulong DegraStreamPosition ( IntPtr userData );
		public delegate ulong DegraStreamLength ( IntPtr userData );

		[StructLayout ( LayoutKind.Sequential )]
		public struct DegraStreamInitializer
		{
			public IntPtr user_data;
			public DegraStreamRead read;
			public DegraStreamWrite write;
			public DegraStreamSeek seek;
			public DegraStreamFlush flush;
			public DegraStreamPosition position;
			public DegraStreamLength length;
		}

		public enum DegraImageResizeFilter
		{
			Nearest,
			Linear,
			Bicubic,
			Ranczos,
		};

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		[return: MarshalAs ( UnmanagedType.Bool )]
		public static extern bool Degra_Initialize ();
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		[return: MarshalAs ( UnmanagedType.Bool )]
		public static extern bool Degra_Uninitialize ();

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern IntPtr Degra_CreateStream ( ref DegraStreamInitializer initializer );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern void Degra_DestroyStream ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr stream );

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern IntPtr Degra_LoadImageFromStream ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr stream );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern void Degra_DestroyImage ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image );

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern void Degra_GetImageSize ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image, out uint width, out uint height );

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern IntPtr Degra_ImagePixelFormatToPalette8Bit ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern IntPtr Degra_ImageResize ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image, DegraImageResizeFilter filter, int height );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		public static extern IntPtr Degra_ImageHistogramEqualization ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image );

		[StructLayout ( LayoutKind.Sequential )]
		public struct JPEGOptions
		{
			public uint quality;
		}
		[StructLayout ( LayoutKind.Sequential )]
		public struct WebPOptions
		{
			public uint quality;
			[MarshalAs ( UnmanagedType.Bool )]
			public bool lossless;
		}
		[StructLayout ( LayoutKind.Sequential )]
		public struct PNGOptions
		{
			[MarshalAs ( UnmanagedType.Bool )]
			public bool zopfli;
		}

		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		[return: MarshalAs ( UnmanagedType.Bool )]
		public static extern bool Degra_SaveImageToStreamJPEG ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image, ref JPEGOptions options, /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr stream );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		[return: MarshalAs ( UnmanagedType.Bool )]
		public static extern bool Degra_SaveImageToStreamWebP ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image, ref WebPOptions options, /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr stream );
		[DllImport ( "DegraCore", CallingConvention = CallingConvention.StdCall )]
		[return: MarshalAs ( UnmanagedType.Bool )]
		public static extern bool Degra_SaveImageToStreamPNG ( /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr image, ref PNGOptions options, /*[MarshalAs ( UnmanagedType.Interface )]*/ IntPtr stream );
	}
}
