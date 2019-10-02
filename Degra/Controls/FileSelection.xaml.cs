using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Daramee.Degra.Controls
{
	/// <summary>
	/// FileSelection.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class FileSelection : UserControl
	{
		public static DependencyProperty PathProperty = DependencyProperty.Register ( "Path", typeof ( string ), typeof ( FileSelection ) );

		public string Path
		{
			get { return GetValue ( PathProperty ) as string; }
			set { SetValue ( PathProperty, value ); }
		}

		public FileSelection ()
		{
			InitializeComponent ();
		}

		private void Hyperlink_RequestNavigate ( object sender, RequestNavigateEventArgs e )
		{
			Process.Start ( Path );
		}

		private void ButtonBrowse_Click ( object sender, RoutedEventArgs e )
		{
			Winston.Dialogs.OpenFolderDialog ofd = new Winston.Dialogs.OpenFolderDialog
			{
				InitialDirectory = Path
			};
			if ( ofd.ShowDialog ( Window.GetWindow ( this ) ) == false )
				return;
			Path = ofd.FileName;
		}
	}
}
