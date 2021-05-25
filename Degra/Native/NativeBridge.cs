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
		public delegate ulong DegraStreamRead(IntPtr userData, IntPtr buffer, ulong length);
		public delegate ulong DegraStreamWrite(IntPtr userData, IntPtr buffer, ulong length);
		public delegate bool DegraStreamSeek(IntPtr userData, SeekOrigin origin, ulong offset);
		public delegate void DegraStreamFlush(IntPtr userData);
		public delegate ulong DegraStreamPosition(IntPtr userData);
		public delegate ulong DegraStreamLength(IntPtr userData);

		[StructLayout(LayoutKind.Sequential)]
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

		public enum DegraResizeFilter
		{
			Nearest,
			Linear,
			Bicubic,
			Lanczos,
			LanczosX5,
		};

		public enum DegraSaveFormat
		{
			SameFormat,
			Png,
			Jpeg,
			WebP,
		}

		public enum DegraResult
		{
			Failed,
			Passed,
			Succeeded,
		}

		[StructLayout(LayoutKind.Sequential, Pack = 0)]
		public struct DegraOptions
		{
			[MarshalAs(UnmanagedType.U4)]
			public DegraSaveFormat save_format;
			[MarshalAs(UnmanagedType.U4)]
			public uint quality;
			[MarshalAs(UnmanagedType.U4)]
			public uint max_height;
			[MarshalAs(UnmanagedType.U4)]
			public DegraResizeFilter resize_filter;
			[MarshalAs(UnmanagedType.Bool)]
			public bool use_lossless;
			[MarshalAs(UnmanagedType.Bool)]
			public bool use_8bit_palette;
			[MarshalAs(UnmanagedType.Bool)]
			public bool use_8bit_palette_but_no_use_over_256_color;
			[MarshalAs(UnmanagedType.Bool)]
			public bool use_grayscale;
			[MarshalAs(UnmanagedType.Bool)]
			public bool use_grayscale_but_no_use_to_grayscale_image;
			[MarshalAs(UnmanagedType.Bool)]
			public bool no_convert_to_png_when_detected_transparent_color;
		}

		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool Degra_Initialize();
		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool Degra_Uninitialize();

		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		public static extern IntPtr Degra_CreateStream(ref DegraStreamInitializer initializer);
		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		public static extern void Degra_DestroyStream(IntPtr stream);

		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		public static extern IntPtr Degra_LoadImageFromStream(IntPtr stream);
		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		public static extern void Degra_DestroyImage(IntPtr image);

		[DllImport("DegraCore", CallingConvention = CallingConvention.StdCall)]
		public static extern DegraResult Degra_DoProcess(IntPtr inputStream, IntPtr outputStream,
			in DegraOptions options, out DegraSaveFormat savedFormat);
	}
}
