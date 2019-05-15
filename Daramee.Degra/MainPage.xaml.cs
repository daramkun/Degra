using Daramee_Degra;
using System;
using System.Collections.Concurrent;
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
using Windows.Services.Store;
using Windows.Storage;
using Windows.Storage.AccessCache;
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
		public MainPage ()
		{
			this.InitializeComponent ();

			var coreTitleBar = CoreApplication.GetCurrentView ().TitleBar;
			coreTitleBar.ExtendViewIntoTitleBar = true;

			var titleBar = ApplicationView.GetForCurrentView ().TitleBar;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;

			LoadSettings ();
			Windows.UI.Core.Preview.SystemNavigationManagerPreview.GetForCurrentView ().CloseRequested += MainPage_CloseRequested;
		}

		private async void MainPage_CloseRequested ( object sender, Windows.UI.Core.Preview.SystemNavigationCloseRequestedPreviewEventArgs e )
		{
			if ( !ButtonSelectFiles.IsEnabled )
			{
				var resourceLoader = Windows.ApplicationModel.Resources.ResourceLoader.GetForCurrentView ();

				ContentDialog ask = new ContentDialog ();
				ask.Title = resourceLoader.GetString ( "AskClosingTitle" );
				ask.Content = resourceLoader.GetString ( "AskClosingContent" );
				ask.CloseButtonText = resourceLoader.GetString ( "ButtonYes" );
				ask.PrimaryButtonText = resourceLoader.GetString ( "ButtonNo" );

				var msgRet = await ask.ShowAsync ();
				if ( msgRet == ContentDialogResult.Primary )
					e.Handled = true;
			}

			SaveSettings ();
		}

		private void LoadSettings ()
		{
			try
			{
				var settings = ApplicationData.Current.LocalSettings;
				var container = settings.CreateContainer ( "Degra_Settings", ApplicationDataCreateDisposition.Existing );
				if ( container == null ) return;

				ToggleFileOverwrite.IsOn = ( bool ) container.Values [ "FileOverwrite" ];
				//ToggleDeleteSourceFile.IsOn = ( bool ) container.Values [ "DeleteSourceFile" ];
				ComboBoxImageFormat.SelectedIndex = ( int ) container.Values [ "ImageFormat" ];
				TextBoxMaximumHeight.Text = ( ( int ) container.Values [ "MaximumHeight" ] ).ToString ();
				ToggleDither.IsOn = ( bool ) container.Values [ "Dither" ];
				ToggleResizeBicubic.IsOn = ( bool ) container.Values [ "ResizeBicubic" ];
				ToggleDeepCheckAlpha.IsOn = ( bool ) container.Values [ "DeepCheckAlpha" ];
				TextBoxQuality.Text = ( ( int ) container.Values [ "Quality" ] ).ToString ();
				ToggleLossless.IsOn = ( bool ) container.Values [ "LosslessCompression" ];
				ToggleIndexedPixelFormat.IsOn = ( bool ) container.Values [ "IndexedPixelFormat" ];
				ToggleUseZopfli.IsOn = ( bool ) container.Values [ "UseZopfli" ];
			}
			catch { }
		}

		private void SaveSettings ()
		{
			var settings = ApplicationData.Current.LocalSettings;
			var container = settings.CreateContainer ( "Degra_Settings", ApplicationDataCreateDisposition.Always );

			container.Values [ "FileOverwrite" ] = ToggleFileOverwrite.IsOn;
			//container.Values [ "DeleteSourceFile" ] = ToggleDeleteSourceFile.IsOn;
			container.Values [ "ImageFormat" ] = ComboBoxImageFormat.SelectedIndex;
			container.Values [ "MaximumHeight" ] = int.Parse ( TextBoxMaximumHeight.Text );
			container.Values [ "Dither" ] = ToggleDither.IsOn;
			container.Values [ "ResizeBicubic" ] = ToggleResizeBicubic.IsOn;
			container.Values [ "DeepCheckAlpha" ] = ToggleDeepCheckAlpha.IsOn;
			container.Values [ "Quality" ] = int.Parse ( TextBoxQuality.Text );
			container.Values [ "LosslessCompression" ] = ToggleLossless.IsOn;
			container.Values [ "IndexedPixelFormat" ] = ToggleIndexedPixelFormat.IsOn;
			container.Values [ "UseZopfli" ] = ToggleUseZopfli.IsOn;
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
			picker.FileTypeFilter.Add ( ".rar" );
			picker.FileTypeFilter.Add ( ".7z" );

			picker.FileTypeFilter.Add ( ".bmp" );
			picker.FileTypeFilter.Add ( ".jpg" );
			picker.FileTypeFilter.Add ( ".jpeg" );
			picker.FileTypeFilter.Add ( ".gif" );
			picker.FileTypeFilter.Add ( ".png" );
			picker.FileTypeFilter.Add ( ".tif" );
			picker.FileTypeFilter.Add ( ".tiff" );
			picker.FileTypeFilter.Add ( ".hdp" );

			var files = await picker.PickMultipleFilesAsync ();

			await DoCompression ( files );
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
			var files = await e.DataView.GetStorageItemsAsync ();

			await DoCompression ( files );
		}

		private void EnableControls ( bool enable )
		{
			ButtonSelectFiles.IsEnabled
				= ToggleFileOverwrite.IsEnabled
				= ComboBoxImageFormat.IsEnabled = TextBoxMaximumHeight.IsEnabled
				= ToggleDither.IsEnabled = ToggleResizeBicubic.IsEnabled = ToggleDeepCheckAlpha.IsEnabled
				= TextBoxQuality.IsEnabled = ToggleLossless.IsEnabled
				= ToggleIndexedPixelFormat.IsEnabled = ToggleUseZopfli.IsEnabled
				= enable;
		}

		private async Task DoCompression ( IReadOnlyList<IStorageItem> storageItems )
		{
			if ( storageItems.Count > 0 )
			{
				var resourceLoader = Windows.ApplicationModel.Resources.ResourceLoader.GetForCurrentView ();

				EnableControls ( false );
				Progress.IsIndeterminate = false;
				Progress.Value = 0;
				Progress.Maximum = 1;
				TextBlockProceedLog.Text = resourceLoader.GetString ( "Working" );

				int imageFormat = ComboBoxImageFormat.SelectedIndex;
				IEncodingSettings webPSettings = webPSettings = new WebPSettings ( int.Parse ( TextBoxQuality.Text ), ToggleLossless.IsOn ),
					jpegSettings = jpegSettings = new JpegSettings ( int.Parse ( TextBoxQuality.Text ) ),
					pngSettings = pngSettings = new PngSettings ( ToggleIndexedPixelFormat.IsOn, ToggleUseZopfli.IsOn );

				Argument args = new Argument ( null, ToggleDither.IsOn, ToggleResizeBicubic.IsOn, ToggleDeepCheckAlpha.IsOn, uint.Parse ( TextBoxMaximumHeight.Text ) );
				bool fileOverwrite = ToggleFileOverwrite.IsOn;

				var failed = new ConcurrentQueue<string> ();
				int proceedCount = 0;
				await Task.Run ( async () =>
				{
					foreach ( var sourceStorageItem in storageItems )
					{
						int innerImageFormat = imageFormat;

						StorageApplicationPermissions.FutureAccessList.Add ( sourceStorageItem );

						if ( !( sourceStorageItem is IStorageFile ) )
						{
							failed.Enqueue ( $"{resourceLoader.GetString ( "Error_IO" )} - {sourceStorageItem.Path}" );
							continue;
						}

						ProgressState state = new ProgressState ();
						IStorageFolder sourceFolder = null;
						try
						{
							sourceFolder = await StorageFolder.GetFolderFromPathAsync ( Path.GetDirectoryName ( sourceStorageItem.Path ) );
						}
						catch
						{
							sourceFolder = null;
							innerImageFormat = 0;
						}

						var sourceFile = sourceStorageItem;
						IStorageFile newFile;
						try
						{
							if ( !fileOverwrite )
								newFile = await sourceFolder.CreateFileAsync ( Guid.NewGuid ().ToString ()
									, CreationCollisionOption.GenerateUniqueName );
							else newFile = sourceFile as IStorageFile;
						}
						catch { newFile = sourceFile as IStorageFile; }

						try
						{
							var compressionTask = ImageCompressor.DoCompression (
								newFile,
								sourceFile as IStorageFile, args,
								( innerImageFormat == 0 || innerImageFormat == 1 ) ? webPSettings : null,
								( innerImageFormat == 0 || innerImageFormat == 2 ) ? jpegSettings : null,
								( innerImageFormat == 0 || innerImageFormat == 3 ) ? pngSettings : null,
								state );

							string lastFilename = null;
							while ( !( compressionTask.Status == TaskStatus.RanToCompletion || compressionTask.Status == TaskStatus.Faulted || compressionTask.Status == TaskStatus.Canceled ) )
							{
								if ( state.ProceedFile != lastFilename )
								{
									await Dispatcher.RunIdleAsync ( ( IdleDispatchedHandlerArgs e ) =>
									{
										TextBlockProceedLog.Text = $"{resourceLoader.GetString ( "Proceed" )}: {state.ProceedFile}";
										Progress.Value = ( proceedCount / ( double ) storageItems.Count ) + ( ( 1 / ( double ) storageItems.Count ) * state.Progress );
									} );
									lastFilename = state.ProceedFile;
								}
								await Task.Delay ( 1 );
							}
							var format = compressionTask.Result;

							string formatExtension = format switch
							{
								ProceedFormat.WebP => ".webp",
								ProceedFormat.Jpeg => ".jpg",
								ProceedFormat.Png => ".png",

								ProceedFormat.Zip => ".zip",

								_ => throw new Exception (),
							};
							string newPath = Path.Combine ( Path.GetDirectoryName ( sourceFile.Path ), Path.GetFileNameWithoutExtension ( sourceFile.Path ) );
							if ( sourceFile.Path == newPath + formatExtension && !fileOverwrite )
								newPath += " (1)";
							newPath += formatExtension;

							if ( sourceStorageItem.Path != newPath )
								await newFile.MoveAsync ( sourceFolder, Path.GetFileName ( newPath ),
									fileOverwrite
										? NameCollisionOption.ReplaceExisting
										: NameCollisionOption.GenerateUniqueName );
						}
						catch ( Exception ex )
						{
							Debug.WriteLine ( ex );

							var failedMessage = sourceStorageItem.Path;
							if ( ex is Exception && ex.InnerException != null )
								ex = ex.InnerException;

							if ( ex is UnauthorizedAccessException )
								failedMessage = $"{resourceLoader.GetString ( "Error_Unauthorized" )} - {failedMessage}";
							else if ( ex is System.IO.IOException )
								failedMessage = $"{resourceLoader.GetString ( "Error_IO" )} - {failedMessage}";
							else if ( ex.Message == "Decoder from Stream is failed."
								|| ex.Message == "Getting Image Frame is failed." )
								failedMessage = $"{resourceLoader.GetString ( "Error_IsNotImage" )} - {failedMessage}";
							else
								failedMessage = $"{resourceLoader.GetString ( "Error_Unknown" )} - {failedMessage}";

							failed.Enqueue ( failedMessage );

							if ( !( ex is UnauthorizedAccessException ) )
							{
								await newFile.DeleteAsync ();
							}

							await Dispatcher.RunIdleAsync ( ( IdleDispatchedHandlerArgs e ) =>
							{
								TextBlockProceedLog.Text = $"{resourceLoader.GetString ( "Error" )}: {sourceStorageItem}";
								Progress.Value = ( ( proceedCount + 1 ) / ( double ) storageItems.Count );
							} );
						}

						System.Threading.Interlocked.Increment ( ref proceedCount );
					}
				} );
				StorageApplicationPermissions.FutureAccessList.Clear ();

				var flyoutDone = Resources [ "FlyoutDone" ] as Flyout;
				if ( failed.Count > 0 )
				{
					ListBoxFlyoutDoneFailed.Items.Clear ();
					foreach ( var path in failed )
						ListBoxFlyoutDoneFailed.Items.Add ( path );
					StackPanelErroredList.Visibility = Visibility.Visible;
				}
				else
					StackPanelErroredList.Visibility = Visibility.Collapsed;
				flyoutDone.ShowAt ( ButtonSelectFiles );

				TextBlockProceedLog.Text = resourceLoader.GetString ( "Done" );
				Progress.IsIndeterminate = true;
				EnableControls ( true );
			}
		}

		private void ButtonDoneOK_Click ( object sender, RoutedEventArgs e )
		{
			var flyoutDone = Resources [ "FlyoutDone" ] as Flyout;
			flyoutDone.Hide ();
		}

		private void ButtonDoneHelp_Click ( object sender, RoutedEventArgs e )
		{

		}

		private void TextBoxMaximumHeight_LostFocus ( object sender, RoutedEventArgs e )
		{
			if ( string.IsNullOrEmpty ( TextBoxMaximumHeight.Text ) )
				TextBoxMaximumHeight.Text = "1";
			else if ( int.TryParse ( TextBoxMaximumHeight.Text, out int result ) )
			{
				if ( result < 1 )
					TextBoxMaximumHeight.Text = "1";
				else if ( result > int.MaxValue )
					TextBoxMaximumHeight.Text = int.MaxValue.ToString ();
			}
			else
				TextBoxMaximumHeight.Text = "4096";
		}

		private void TextBoxQuality_LostFocus ( object sender, RoutedEventArgs e )
		{
			if ( string.IsNullOrEmpty ( TextBoxQuality.Text ) )
				TextBoxQuality.Text = "1";
			else if ( int.TryParse ( TextBoxQuality.Text, out int result ) )
			{
				if ( result < 1 )
					TextBoxQuality.Text = "1";
				else if ( result > 100 )
					TextBoxQuality.Text = "100";
			}
			else
				TextBoxQuality.Text = "90";
		}
	}
}
