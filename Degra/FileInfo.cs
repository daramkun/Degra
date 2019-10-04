using Daramee.Degra.Native;
using Daramee.FileTypeDetector;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra
{
	public enum DegraStatus
	{
		Waiting,
		Processing,
		Done,
		Failed,
		Cancelled,
	}

	public class FileInfo : IEquatable<FileInfo>, INotifyPropertyChanged
	{
		bool queued = true;
		DegraStatus status = DegraStatus.Waiting;

		public bool Queued
		{
			get { return queued; }
			set
			{
				queued = value;
				PC ( nameof ( Queued ) );
			}
		}
		public string OriginalFilename { get; private set; }

		public DegraStatus Status
		{
			get { return status; }
			set
			{
				status = value;
				PC ( nameof ( Status ) );
			}
		}

		public string Extension { get; private set; }

		public FileInfo(string filename)
		{
			OriginalFilename = filename;
		}

		public void CheckExtension ()
		{
			if ( !File.Exists ( OriginalFilename ) )
			{
				Extension = null;
				return;
			}

			try
			{
				using ( Stream stream = new FileStream ( OriginalFilename, FileMode.Open, FileAccess.Read, FileShare.Read ) )
				{
					if ( stream.Length == 0 )
					{
						Extension = null;
						return;
					}

					var detector = DetectorService.DetectDetector ( stream );
					if ( detector == null || !( detector.Extension == "jpg" || detector.Extension == "png"
						|| detector.Extension == "jp2" || detector.Extension == "bmp"
						|| detector.Extension == "webp"
						|| detector.Extension == "zip" || detector.Extension == "rar" ) )
						Extension = null;
					else
						Extension = detector.Extension;
				}
			}
			catch
			{
				Extension = null;
			}
		}

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }

		public bool Equals ( FileInfo other ) => Path.GetFullPath ( OriginalFilename ) == Path.GetFullPath ( other.OriginalFilename );
	}
}
