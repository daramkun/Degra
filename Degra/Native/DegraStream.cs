using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Daramee.Degra.Native
{
	sealed class DegraStream : IDisposable
	{
		GCHandle gcHandle;
		IntPtr stream;

		public static implicit operator IntPtr ( DegraStream stream ) { return stream.stream; }

		public DegraStream ( Stream stream )
		{
			gcHandle = GCHandle.Alloc ( stream, GCHandleType.Normal );
			var initializer = new NativeBridge.DegraStreamInitializer ();
			initializer.user_data = GCHandle.ToIntPtr ( gcHandle );
			if ( stream.CanRead )
			{
				initializer.read = ( IntPtr userData, IntPtr buffer, ulong length ) =>
			  {
				  GCHandle handle = GCHandle.FromIntPtr ( userData );
				  var originalStream = handle.Target as Stream;

				  byte [] arr = new byte [ length ];
				  int read = originalStream.Read ( arr, 0, ( int ) length );
				  Marshal.Copy ( arr, 0, buffer, read );

				  return ( ulong ) read;
			  };
			}
			if ( stream.CanWrite )
			{
				initializer.write = ( IntPtr userData, IntPtr data, ulong length ) =>
				{
					GCHandle handle = GCHandle.FromIntPtr ( userData );
					var originalStream = handle.Target as Stream;

					byte [] arr = new byte [ length ];
					Marshal.Copy ( data, arr, 0, ( int ) length );

					originalStream.Write ( arr, 0, ( int ) length );

					return length;
				};
			}
			if ( stream.CanSeek )
			{
				initializer.seek = ( IntPtr userData, System.IO.SeekOrigin origin, ulong offset ) =>
				{
					GCHandle handle = GCHandle.FromIntPtr ( userData );
					var originalStream = handle.Target as Stream;

					originalStream.Seek ( ( long ) offset, origin );

					return true;
				};
			}
			initializer.flush = ( IntPtr userData ) =>
			{
				GCHandle handle = GCHandle.FromIntPtr ( userData );
				var originalStream = handle.Target as Stream;
				originalStream.Flush ();
			};
			initializer.position = ( IntPtr userData ) =>
			{
				GCHandle handle = GCHandle.FromIntPtr ( userData );
				var originalStream = handle.Target as Stream;
				return ( ulong ) originalStream.Position;
			};
			initializer.length = ( IntPtr userData ) =>
			{
				GCHandle handle = GCHandle.FromIntPtr ( userData );
				var originalStream = handle.Target as Stream;
				return ( ulong ) originalStream.Length;
			};

			this.stream = NativeBridge.Degra_CreateStream ( ref initializer );
			if ( this.stream == IntPtr.Zero )
				throw new IOException ();
		}

		~DegraStream ()
		{
			Dispose ( false );
		}

		public void Dispose ()
		{
			Dispose ( true );
			GC.SuppressFinalize ( this );
		}

		void Dispose ( bool disposing )
		{
			if ( stream != IntPtr.Zero )
				NativeBridge.Degra_DestroyStream ( stream );
			gcHandle.Free ();
		}
	}
}
