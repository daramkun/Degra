using Daramee.Degra.Native;
using Daramee.FileTypeDetector;
using Daramee.Winston.Dialogs;
using Daramee.Winston.File;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace Daramee.Degra
{
	/// <summary>
	/// MainWindow.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class MainWindow : Window, INotifyPropertyChanged
	{
		public static MainWindow SharedWindow { get; private set; }

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }

		ObservableCollection<FileInfo> files = new ObservableCollection<FileInfo> ();
		
		Settings settings = new Settings ();
		Optionizer<Settings> optionizer = new Optionizer<Settings> ();

		ProgressStatus status = new ProgressStatus ();

		public string ConversionPath { get { return settings.ConversionPath; } set { settings.ConversionPath = value; PC ( nameof ( ConversionPath ) ); } }
		public bool FileOverwrite { get { return settings.FileOverwrite; } set { settings.FileOverwrite = value; } }
		public DegrationFormat ImageFormat { get { return settings.ImageFormat; } set { settings.ImageFormat = value; } }
		public uint MaximumImageHeight { get { return settings.MaximumImageHeight; } set { settings.MaximumImageHeight = value; } }
		public ResizeFilter ResizeFilter { get { return settings.ResizeFilter; } set { settings.ResizeFilter = value; } }
		public ushort ImageQuality { get { return settings.ImageQuality; } set { settings.ImageQuality = value; } }
		public bool Lossless { get { return settings.Lossless; } set { settings.Lossless = value; } }
		public bool IndexedPixelFormat { get { return settings.IndexedPixelFormat; } set { settings.IndexedPixelFormat = value; } }
		public bool GrayscalePixelFormat { get { return settings.GrayscalePixelFormat; } set { settings.GrayscalePixelFormat = value; } }
		public bool ZopfliPNGOptimization { get { return settings.ZopfliPNGOptimization; } set { settings.ZopfliPNGOptimization = value; } }
		public bool HistogramEqualization { get { return settings.HistogramEqualization; } set { settings.HistogramEqualization = value; } }
		public bool NoConvertTransparentDetected { get { return settings.NoConvertTransparentDetected; } set { settings.NoConvertTransparentDetected = value; } }
		public int ThreadCount { get { return settings.ThreadCount; } set { settings.ThreadCount = value; } }

		public MainWindow ()
		{
			SharedWindow = this;

			settings = optionizer.Options;

			InitializeComponent ();

			ListViewFiles.ItemsSource = files;

			ProgressBarLog.DataContext = status;
			TextBlockLog.DataContext = status;
		}

		private void Window_Closing ( object sender, CancelEventArgs e )
		{
			optionizer.Options = settings;
			optionizer.Save ();
		}

		private void ButtonBrowseConversionPath_Click ( object sender, RoutedEventArgs e )
		{
			Winston.Dialogs.OpenFolderDialog ofd = new Winston.Dialogs.OpenFolderDialog
			{
				InitialDirectory = ConversionPath
			};
			if ( ofd.ShowDialog ( this ) == false )
				return;
			ConversionPath = ofd.FileName;
		}

		private void AddItem ( string filename )
		{
			if ( System.IO.File.Exists ( filename ) )
			{
				var fileInfo = new FileInfo ( filename );
				if ( !files.Contains ( fileInfo ) )
				{
					using ( Stream stream = new FileStream ( filename, FileMode.Open, FileAccess.Read, FileShare.Read ) )
					{
						if ( stream.Length == 0 )
							return;
						var detector = DetectorService.DetectDetector ( stream );
						if ( detector == null )
							return;
						if ( !( detector.Extension == "jpg" || detector.Extension == "png"
							|| detector.Extension == "jp2" || detector.Extension == "bmp"
							|| detector.Extension == "webp" || detector.Extension == "zip" ) )
							return;
					}
					files.Add ( fileInfo );
				}
			}
			else
			{
				foreach ( string ss in FilesEnumerator.EnumerateFiles ( filename, "*.*", false ) )
					AddItem ( ss );
			}
		}

		private void MenuItem_Open_Click ( object sender, RoutedEventArgs e )
		{
			OpenFileDialog ofd = new OpenFileDialog ();
			ofd.AllowMultiSelection = true;
			if ( ofd.ShowDialog ( this ) == false )
				return;

			foreach ( var filename in ofd.FileNames )
				AddItem ( filename );
		}

		private void MenuItem_Clear_Click ( object sender, RoutedEventArgs e )
		{
			files.Clear ();
		}

		CancellationTokenSource cancelToken;

		private async void MenuItem_Apply_Click ( object sender, RoutedEventArgs e )
		{
			ButtonApply.IsEnabled = ButtonClear.IsEnabled = ScrollViewerSettings.IsEnabled = false;
			ButtonCancel.IsEnabled = true;

			foreach ( var fileInfo in files )
				fileInfo.Status = DegraStatus.Waiting;

			cancelToken = new CancellationTokenSource ();

			if ( IndexedPixelFormat && GrayscalePixelFormat )
			{
				cancelToken.Cancel ();
				TaskDialog.Show ( "Settings Error.", "8-bit Indexed Pixel Format and Grayscale Pixel Format is checked both. You can turn on only one.", "Please check those settings.", TaskDialogCommonButtonFlags.OK, TaskDialogIcon.Error );
			}

			try
			{
				await Task.Run ( () =>
				{
					foreach ( var fileInfo in files )
					{
						if ( !fileInfo.Queued )
							continue;

						status.ProceedFile = fileInfo.OriginalFilename;
						status.Progress = 0;

						Daramee.Winston.File.Operation.Begin ();
						Degrator.Degration ( fileInfo, status, settings, cancelToken.Token );
						Daramee.Winston.File.Operation.End ();
					}

					Degrator.CleanupMemory ();
				}, cancelToken.Token );
			}
			catch { }

			ButtonCancel.IsEnabled = false;
			ButtonApply.IsEnabled = ButtonClear.IsEnabled = ScrollViewerSettings.IsEnabled = true;
		}

		private void MenuItem_Cancel_Click ( object sender, RoutedEventArgs e )
		{
			cancelToken.Cancel ();
			ButtonCancel.IsEnabled = false;
		}

		private void ListViewFiles_Drop ( object sender, DragEventArgs e )
		{
			if ( e.Data.GetDataPresent ( DataFormats.FileDrop ) )
			{
				var temp = e.Data.GetData ( DataFormats.FileDrop ) as string [];
				foreach ( string s in from b in temp orderby b select b )
				{
					AddItem ( s );
				}
			}
		}

		private void ListViewFiles_DragEnter ( object sender, DragEventArgs e )
		{
			if ( e.Data.GetDataPresent ( DataFormats.FileDrop ) )
				e.Effects = DragDropEffects.None;
		}
	}
}
