using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace Daramee.Degra.Converters
{
	class ListViewItemColorConverter : IValueConverter
	{
		public object Convert ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			switch ((DegraStatus)value)
			{
				case DegraStatus.Waiting: return Colors.Transparent;
				case DegraStatus.Processing: return Colors.AliceBlue;
				case DegraStatus.Done: return Colors.ForestGreen;
				case DegraStatus.Failed: return Colors.PaleVioletRed;
				case DegraStatus.Cancelled: return Colors.LightYellow;
				default: throw new ArgumentException ();
			}
		}

		public object ConvertBack ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			throw new NotImplementedException ();
		}
	}
}
