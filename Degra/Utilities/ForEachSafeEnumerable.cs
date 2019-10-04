using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Daramee.Degra.Utilities
{
	public struct ForEachSafeEnumerable<T> : IEnumerable<T>
	{
		readonly ObservableCollection<T> original;
		readonly Func<T, bool> condition;

		public ForEachSafeEnumerable ( ObservableCollection<T> o, Func<T, bool> c = null )
		{
			original = o;
			condition = c;
		}

		public IEnumerator<T> GetEnumerator () => new ForEachSafeEnumerator ( original, condition );
		IEnumerator IEnumerable.GetEnumerator () => new ForEachSafeEnumerator ( original, condition );

		struct ForEachSafeEnumerator : IEnumerator<T>
		{
			ObservableCollection<T> original;
			T next;
			int index;
			readonly Func<T, bool> condition;

			public T Current => next;

			object IEnumerator.Current => next;

			public ForEachSafeEnumerator ( ObservableCollection<T> o, Func<T, bool> c )
			{
				original = o;
				next = default ( T );
				index = -1;
				condition = c;
				original.CollectionChanged += CollectionChangedEvent;
			}

			public void Dispose ()
			{
				original.CollectionChanged -= CollectionChangedEvent;
				original = null;
			}

			private void CollectionChangedEvent ( object sender, NotifyCollectionChangedEventArgs e )
			{
				if ( e.Action == NotifyCollectionChangedAction.Remove )
				{
					if ( e.OldStartingIndex <= index )
						index -= e.OldStartingIndex;
				}
				else if ( e.Action == NotifyCollectionChangedAction.Add )
				{
					if ( e.NewStartingIndex <= index )
						index += e.NewStartingIndex;
				}
				else if ( e.Action == NotifyCollectionChangedAction.Reset )
					index = -1;
			}

			public bool MoveNext ()
			{
				if ( original.Count <= index + 1 )
					return false;
				next = original [ Interlocked.Increment ( ref index ) ];
				if ( condition != null && !condition ( next ) )
					return MoveNext ();
				return true;
			}

			public void Reset ()
			{
				index = -1;
			}
		}
	}
}
