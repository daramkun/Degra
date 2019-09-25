using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32;
using JsonSerializer = System.Runtime.Serialization.Json.DataContractJsonSerializer;
using JsonSerializerSettings = System.Runtime.Serialization.Json.DataContractJsonSerializerSettings;

namespace Daramee.Degra
{
	public sealed class Optionizer<T> where T : class
	{
		public static Optionizer<T> SharedOptionizer { get; private set; }
		public static T SharedOptions { get { return SharedOptionizer.Options; } }

		JsonSerializer serializer = new JsonSerializer ( typeof ( T ), new JsonSerializerSettings () { UseSimpleDictionaryFormat = true } );

		string saveDirectory;

		public T Options { get; set; }

		public Optionizer ()
		{
			SharedOptionizer = this;

			saveDirectory = $"{AppDomain.CurrentDomain.BaseDirectory}\\Degra.config.json";
			if ( File.Exists ( saveDirectory ) )
			{
				using ( Stream stream = File.Open ( saveDirectory, FileMode.Open ) )
				{
					if ( stream.Length != 0 )
						Options = serializer.ReadObject ( stream ) as T;
				}
			}
			else
				Options = Activator.CreateInstance<T> ();
		}

		public void Save ()
		{
			using ( Stream stream = File.Open ( saveDirectory, FileMode.Create ) )
				serializer.WriteObject ( stream, Options );
		}
	}
}
