using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra.Utilities
{
	public static class ProcessingFormat
	{
		static string [] ContainerFormats = new [] { "zip", "rar", "7z", "tar" };
		static string [] ImageFormats = new [] { "bmp", "png", "jpg", "png", "tif", "tga", "webp" };

		public static bool IsSupportContainerFormat ( string extension )
			=> ContainerFormats.Contains ( extension );
		public static bool IsSupportImageFormat ( string extension )
			=> ImageFormats.Contains ( extension );
	}
}
