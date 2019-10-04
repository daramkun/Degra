using Daramee.Degra.Native;
using Daramee.Degra.Utilities;
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
		
		Optionizer<Settings> optionizer = new Optionizer<Settings> ();

		ProgressStatus status = new ProgressStatus ();

		public MainWindow ()
		{
			SharedWindow = this;

			InitializeComponent ();

			ListViewFiles.ItemsSource = files;

			ProgressBarLog.DataContext = status;
			TextBlockLog.DataContext = status;
		}

		private void Window_Closing ( object sender, CancelEventArgs e )
		{
			optionizer.Save ();
		}

		public void AddItem ( string filename )
		{
			if ( System.IO.File.Exists ( filename ) )
			{
				var fileInfo = new FileInfo ( filename );

				if ( !files.Contains ( fileInfo ) )
				{
					fileInfo.CheckExtension ();
					if ( fileInfo.Extension == null )
						return;

					Dispatcher.Invoke ( () => files.Add ( fileInfo ) );
				}
			}
			else
			{
				foreach ( string ss in FilesEnumerator.EnumerateFiles ( filename, "*.*", false ) )
					AddItem ( ss );
			}
		}

		private async void MenuItem_Open_Click ( object sender, RoutedEventArgs e )
		{
			OpenFileDialog ofd = new OpenFileDialog ();
			ofd.AllowMultiSelection = true;
			if ( ofd.ShowDialog ( this ) == false )
				return;

			await Task.Run ( () =>
			{
				foreach ( var filename in ofd.FileNames )
					AddItem ( filename );
			} );
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

			if ( Settings.SharedSettings.IndexedPixelFormat && Settings.SharedSettings.GrayscalePixelFormat )
			{
				cancelToken.Cancel ();
				TaskDialog.Show ( "설정 오류.", "8비트 팔레트 픽셀 형식과 회색조 픽셀 형식을 동시에 켜둘 수 없습니다.", "위 설정을 다시 한번 확인해주세요.", TaskDialogCommonButtonFlags.OK, TaskDialogIcon.Error );
			}

			try
			{
				await Task.Run ( () =>
				{
					foreach ( var fileInfo in new ForEachSafeEnumerable<FileInfo> ( files ) )
					{
						if ( !fileInfo.Queued )
							continue;

						status.ProceedFile = fileInfo.OriginalFilename;
						status.Progress = 0;

						Daramee.Winston.File.Operation.Begin ();
						Degrator.Degration ( fileInfo, status, cancelToken.Token );
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

		private async void ListViewFiles_Drop ( object sender, DragEventArgs e )
		{
			if ( e.Data.GetDataPresent ( DataFormats.FileDrop ) )
			{
				var temp = e.Data.GetData ( DataFormats.FileDrop ) as string [];

				await Task.Run ( () =>
				{
					foreach ( string s in from b in temp orderby b select b )
						AddItem ( s );
				} );
			}
		}

		private void ListViewFiles_DragEnter ( object sender, DragEventArgs e )
		{
			if ( e.Data.GetDataPresent ( DataFormats.FileDrop ) )
				e.Effects = DragDropEffects.None;
		}

		private void ListViewFiles_KeyUp ( object sender, KeyEventArgs e )
		{
			if ( e.Key == Key.Delete )
			{
				bool foundProcessing = false;
				foreach ( var fileInfo in files )
					if ( fileInfo.Status == DegraStatus.Processing )
					{
						foundProcessing = true;
						break;
					}

				List<FileInfo> selected = new List<FileInfo> ();
				foreach ( FileInfo fi in ListViewFiles.SelectedItems )
					selected.Add ( fi );

				if ( !foundProcessing )
				{
					foreach ( var fileInfo in selected )
						files.Remove ( fileInfo );
				}
				else
				{
					foreach ( var fileInfo in selected )
					{
						if ( fileInfo.Status == DegraStatus.Processing || fileInfo.Status == DegraStatus.Waiting )
							continue;
						files.Remove ( fileInfo );
					}
				}
			}
		}

		private void ButtonLicense_Click ( object sender, RoutedEventArgs e )
		{
			var licenseWindow = new LicenseWindow ();
			licenseWindow.Owner = this;
			licenseWindow.ShowDialog ();
		}
	}
}
