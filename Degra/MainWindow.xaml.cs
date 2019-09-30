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
		
		SaveData saveData = new SaveData ();
		Optionizer<SaveData> optionizer = new Optionizer<SaveData> ();

		ProgressStatus status = new ProgressStatus ();

		public string ConversionPath { get { return saveData.ConversionPath; } set { saveData.ConversionPath = value; PC ( nameof ( ConversionPath ) ); } }
		public bool FileOverwrite { get { return saveData.FileOverwrite; } set { saveData.FileOverwrite = value; } }
		public DegrationFormat ImageFormat { get { return saveData.ImageFormat; } set { saveData.ImageFormat = value; } }
		public uint MaximumImageHeight { get { return saveData.MaximumImageHeight; } set { saveData.MaximumImageHeight = value; } }
		public ResizeFilter ResizeFilter { get { return saveData.ResizeFilter; } set { saveData.ResizeFilter = value; } }
		public ushort ImageQuality { get { return saveData.ImageQuality; } set { saveData.ImageQuality = value; } }
		public bool Lossless { get { return saveData.Lossless; } set { saveData.Lossless = value; } }
		public bool IndexedPixelFormat { get { return saveData.IndexedPixelFormat; } set { saveData.IndexedPixelFormat = value; } }
		public bool GrayscalePixelFormat { get { return saveData.GrayscalePixelFormat; } set { saveData.GrayscalePixelFormat = value; } }
		public bool ZopfliPNGOptimization { get { return saveData.ZopfliPNGOptimization; } set { saveData.ZopfliPNGOptimization = value; } }
		public bool HistogramEqualization { get { return saveData.HistogramEqualization; } set { saveData.HistogramEqualization = value; } }
		public bool NoConvertTransparentDetected { get { return saveData.NoConvertTransparentDetected; } set { saveData.NoConvertTransparentDetected = value; } }
		public int ThreadCount { get { return saveData.ThreadCount; } set { saveData.ThreadCount = value; } }

		public MainWindow ()
		{
			SharedWindow = this;

			saveData = optionizer.Options;

			InitializeComponent ();

			ListViewFiles.ItemsSource = files;

			ProgressBarLog.DataContext = status;
			TextBlockLog.DataContext = status;
		}

		private void Window_Closing ( object sender, CancelEventArgs e )
		{
			optionizer.Options = saveData;
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

			DegrationArguments args = new DegrationArguments
			{
				Format = ImageFormat,
				ImageQuality = ImageQuality,
				MaximumImageHeight = ( int ) MaximumImageHeight,
				ResizeFilter = ResizeFilter,
				LosslessCompression = Lossless,
				PNGPixelFormatTo8BitQuantization = IndexedPixelFormat,
				GrayscalePixelFormat = GrayscalePixelFormat,
				ZopfliPNGOptimization = ZopfliPNGOptimization,
				HistogramEqualization = HistogramEqualization,
				NoConvertTransparentDetected = NoConvertTransparentDetected,
				ThreadCount = ThreadCount
			};
			if ( ThreadCount == 0 )
				args.ThreadCount = Math.Max ( Environment.ProcessorCount - 1, 1 );

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
						Degrator.Degration ( fileInfo, ConversionPath, FileOverwrite, status, args, cancelToken.Token );
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
