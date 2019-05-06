using Daramee_Degra;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage.Pickers;
using Windows.System.Threading;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// 빈 페이지 항목 템플릿에 대한 설명은 https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x412에 나와 있습니다.

namespace Daramee.Degra
{
    /// <summary>
    /// 자체적으로 사용하거나 프레임 내에서 탐색할 수 있는 빈 페이지입니다.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

			var coreTitleBar = CoreApplication.GetCurrentView ().TitleBar;
			coreTitleBar.ExtendViewIntoTitleBar = true;

			var titleBar = ApplicationView.GetForCurrentView ().TitleBar;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
		}

		private void HamburgerButton_Click ( object sender, RoutedEventArgs e )
		{
			splitView.IsPaneOpen = !splitView.IsPaneOpen;
		}

		private void TextBoxNumbersOnly_BeforeTextChanging ( TextBox sender, TextBoxBeforeTextChangingEventArgs args )
		{
			args.Cancel = args.NewText.Any ( c => !char.IsDigit ( c ) );
		}

		private void TextBoxNumbersOnly_TextChanging ( TextBox sender, TextBoxTextChangingEventArgs args )
		{
			sender.Text = new string ( sender.Text.Where ( char.IsDigit ).ToArray () );
		}

		private async void SelectFiles_Click ( object sender, RoutedEventArgs e )
		{
			FileOpenPicker picker = new FileOpenPicker
			{
				ViewMode = PickerViewMode.Thumbnail,
				SuggestedStartLocation = PickerLocationId.PicturesLibrary
			};
			picker.FileTypeFilter.Add ( ".zip" );
			picker.FileTypeFilter.Add ( ".bmp" );
			picker.FileTypeFilter.Add ( ".jpg" );
			picker.FileTypeFilter.Add ( ".jpeg" );
			picker.FileTypeFilter.Add ( ".gif" );
			picker.FileTypeFilter.Add ( ".png" );
			picker.FileTypeFilter.Add ( ".tif" );
			picker.FileTypeFilter.Add ( ".tiff" );
			picker.FileTypeFilter.Add ( ".hdp" );

			var pathes = new List<string> ();
			var files = await picker.PickMultipleFilesAsync ();
			foreach ( var storageFile in files )
				pathes.Add ( storageFile.Path );

			await DoCompression ( pathes );
		}

		private void SplitView_DragOver ( object sender, DragEventArgs e )
		{
			if ( e.DataView.Contains ( StandardDataFormats.StorageItems ) )
				e.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Move;
		}

		private void SplitView_DragEnter ( object sender, DragEventArgs e )
		{

		}

		private async void SplitView_Drop ( object sender, DragEventArgs e )
		{
			if ( !e.DataView.Contains ( StandardDataFormats.StorageItems ) )
				return;
			var items = await e.DataView.GetStorageItemsAsync ();

			List<string> pathes = new List<string> ();
			foreach ( var path in items )
			{
				pathes.Add ( path.Path );
			}

			await DoCompression ( pathes );
		}

		private async Task DoCompression ( IReadOnlyList<string> pathes )
		{
			ButtonSelectFiles.IsEnabled = ToggleFileOverwrite.IsEnabled = ComboBoxImageFormat.IsEnabled
				= TextBoxMaximumHeight.IsEnabled = ToggleDither.IsEnabled = ToggleResizeBicubic.IsEnabled
				= TextBoxQuality.IsEnabled = ToggleIndexedPixelFormat.IsEnabled = false;
			Progress.IsIndeterminate = false;
			Progress.Value = 0;
			Progress.Maximum = 1;

			if ( pathes.Count > 0 )
			{
				IEncodingSettings settings;
				switch ( ComboBoxImageFormat.SelectedIndex )
				{
					case 0: settings = new WebPSettings ( int.Parse ( TextBoxQuality.Text ) ); break;
					case 1: settings = new JpegSettings ( int.Parse ( TextBoxQuality.Text ) ); break;
					case 2: settings = new PngSettings ( ToggleIndexedPixelFormat.IsOn ); break;
					default: throw new ArgumentException ();
				}

				Argument args = new Argument ( settings, ToggleDither.IsOn, ToggleResizeBicubic.IsOn, uint.Parse ( TextBoxMaximumHeight.Text ) );
				bool fileOverwrite = ToggleFileOverwrite.IsOn;

				int proceedCount = 0;
				foreach ( var path in pathes )
				{
					ProgressState state = new ProgressState ();
					string tempPath = Path.Combine ( Path.GetDirectoryName ( path ), Guid.NewGuid ().ToString () );
					try
					{
						await Windows.System.Threading.ThreadPool.RunAsync ( async ( IAsyncAction operation ) =>
						{
							var format = await ImageCompressor.DoCompression ( tempPath, path, args, state );
							string formatExtension = format switch
							{
								ProceedFormat.Zip => ".zip",
								ProceedFormat.WebP => ".webp",
								ProceedFormat.Jpeg => ".jpg",
								ProceedFormat.Png => ".png",
								_ => throw new Exception (),
							};
							string newPath = Path.Combine ( Path.GetDirectoryName ( path ), Path.GetFileNameWithoutExtension ( path ) );
							if ( path == newPath + formatExtension && !fileOverwrite )
								newPath += " - New";
							newPath += formatExtension;

							var destStorageFolder = await Windows.Storage.StorageFolder.GetFolderFromPathAsync ( Path.GetDirectoryName ( tempPath ) );
							var destStorageFile = await destStorageFolder.GetFileAsync ( Path.GetFileName ( tempPath ) );

							await destStorageFile.MoveAsync ( destStorageFolder, Path.GetFileName ( newPath ),
								fileOverwrite ? Windows.Storage.NameCollisionOption.ReplaceExisting : Windows.Storage.NameCollisionOption.GenerateUniqueName );
						}, WorkItemPriority.Normal, WorkItemOptions.TimeSliced );

						string lastFilename = null;
						while ( state.Progress < 1 )
						{
							if ( state.ProceedFile != lastFilename )
							{
								await Dispatcher.RunIdleAsync ( ( IdleDispatchedHandlerArgs e ) =>
								{
									TextBlockProceedLog.Text = $"Proceed: {state.ProceedFile}";
									Progress.Value = ( proceedCount / ( double ) pathes.Count ) + ( ( 1 / ( double ) pathes.Count ) * state.Progress );
								} );
								lastFilename = state.ProceedFile;
							}
							await Task.Delay ( 1 );
						}
					}
					catch ( Exception ex )
					{
						Debug.WriteLine ( ex );
						File.Delete ( tempPath );

						var destStorageFolder = await Windows.Storage.StorageFolder.GetFolderFromPathAsync ( Path.GetDirectoryName ( tempPath ) );
						var destStorageFile = await destStorageFolder.GetFileAsync ( Path.GetFileName ( tempPath ) );
						await destStorageFile.DeleteAsync ();

						TextBlockProceedLog.Text = $"Error: {state.ProceedFile}";
						Progress.Value = ( ( proceedCount + 1 ) / ( double ) pathes.Count );
					}

					System.Threading.Interlocked.Increment ( ref proceedCount );
				}
			}

			TextBlockProceedLog.Text = "Done.";
			Progress.IsIndeterminate = true;
			ButtonSelectFiles.IsEnabled = ToggleFileOverwrite.IsEnabled = ComboBoxImageFormat.IsEnabled
				= TextBoxMaximumHeight.IsEnabled = ToggleDither.IsEnabled = ToggleResizeBicubic.IsEnabled
				= TextBoxQuality.IsEnabled = ToggleIndexedPixelFormat.IsEnabled = true;

			MessageDialog messageDialog = new MessageDialog ( "Compression is done.", "Degra" );
			await messageDialog.ShowAsync ();
		}
	}
}
