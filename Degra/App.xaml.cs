using Daramee.Degra.Native;
using Daramee.FileTypeDetector;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace Daramee.Degra
{
	/// <summary>
	/// App.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class App : Application
	{
		public App ()
		{
			DetectorService.AddDetectors ( Assembly.GetEntryAssembly (), FormatCategories.Archive );
			DetectorService.AddDetectors ( Assembly.GetEntryAssembly (), FormatCategories.Image );
			NativeBridge.Degra_Initialize ();

			RenderOptions.ProcessRenderMode = System.Windows.Interop.RenderMode.SoftwareOnly;
		}
	}
}
