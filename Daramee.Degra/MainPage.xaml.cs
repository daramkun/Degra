﻿using Daramee_Degra;
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
				TextBoxQuality.Text = ( ( int ) container.Values [ "Quality" ] ).ToString ();
				ToggleIndexedPixelFormat.IsOn = ( bool ) container.Values [ "IndexedPixelFormat" ];
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
			container.Values [ "Quality" ] = int.Parse ( TextBoxQuality.Text );
			container.Values [ "IndexedPixelFormat" ] = ToggleIndexedPixelFormat.IsOn;
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

			var pathes = new List<string> ();
			var files = await picker.PickMultipleFilesAsync ();
			foreach ( var storageFile in files )
			{
				StorageApplicationPermissions.FutureAccessList.Add ( storageFile );
				pathes.Add ( storageFile.Path );
			}

			await DoCompression ( files );

			StorageApplicationPermissions.FutureAccessList.Clear ();
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
			foreach ( var storageFile in items )
			{
				StorageApplicationPermissions.FutureAccessList.Add ( storageFile );
				pathes.Add ( storageFile.Path );
			}

			await DoCompression ( items );

			StorageApplicationPermissions.FutureAccessList.Clear ();
		}

		private async Task DoCompression ( IReadOnlyList<IStorageItem> storageItems )
		{
			if ( storageItems.Count > 0 )
			{
				var resourceLoader = Windows.ApplicationModel.Resources.ResourceLoader.GetForCurrentView ();

				ButtonSelectFiles.IsEnabled
					= ToggleFileOverwrite.IsEnabled
					//= ToggleDeleteSourceFile.IsEnabled
					= ComboBoxImageFormat.IsEnabled = TextBoxMaximumHeight.IsEnabled = ToggleDither.IsEnabled
					= ToggleResizeBicubic.IsEnabled = TextBoxQuality.IsEnabled = ToggleIndexedPixelFormat.IsEnabled
					= false;
				Progress.IsIndeterminate = false;
				Progress.Value = 0;
				Progress.Maximum = 1;

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

				var failed = new ConcurrentQueue<string> ();
				int proceedCount = 0;
				foreach ( var sourceStorageItem in storageItems )
				{
					if ( !( sourceStorageItem is IStorageFile ) )
					{
						failed.Enqueue ( $"{resourceLoader.GetString ( "Error_IO" )} - {sourceStorageItem.Path}" );
						continue;
					}

					ProgressState state = new ProgressState ();
					var sourceFolder = await StorageFolder.GetFolderFromPathAsync ( sourceStorageItem.Path );
					var sourceFile = sourceStorageItem;
					IStorageFile newFile;
					try
					{
						if ( !fileOverwrite )
							newFile = await sourceFolder.CreateFileAsync ( Path.Combine ( sourceFolder.Path, Guid.NewGuid ().ToString () )
								, CreationCollisionOption.GenerateUniqueName );
						else newFile = sourceFile as IStorageFile;
					}
					catch
					{
						newFile = sourceFile as IStorageFile;
					}

					try
					{
						var compressionTask = ImageCompressor.DoCompression ( newFile, sourceFile as IStorageFile, args, state );

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
						
						if ( sourceStorageItem != newFile )
							await newFile.MoveAsync ( sourceFolder, Path.GetFileName ( newPath ),
								fileOverwrite ? Windows.Storage.NameCollisionOption.ReplaceExisting : Windows.Storage.NameCollisionOption.GenerateUniqueName );
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
				ButtonSelectFiles.IsEnabled
					= ToggleFileOverwrite.IsEnabled
					//= ToggleDeleteSourceFile.IsEnabled
					= ComboBoxImageFormat.IsEnabled = TextBoxMaximumHeight.IsEnabled = ToggleDither.IsEnabled
					= ToggleResizeBicubic.IsEnabled = TextBoxQuality.IsEnabled = ToggleIndexedPixelFormat.IsEnabled
					= true;
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
