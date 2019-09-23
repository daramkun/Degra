using Daramee.Degra.Native;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Daramee.Degra.Converters
{
	class ResizeFilterConverter : IValueConverter
	{
		public object Convert ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			return ( int ) ( ResizeFilter ) value;
		}

		public object ConvertBack ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			return ( ResizeFilter ) ( int ) value;
		}
	}
}
