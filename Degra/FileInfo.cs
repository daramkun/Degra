using Daramee.Degra.Native;
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

	public class FileInfo : INotifyPropertyChanged
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

		public FileInfo(string filename)
		{
			OriginalFilename = filename;
		}

		public event PropertyChangedEventHandler PropertyChanged;
		private void PC ( string name ) { PropertyChanged?.Invoke ( this, new PropertyChangedEventArgs ( name ) ); }
	}
}
