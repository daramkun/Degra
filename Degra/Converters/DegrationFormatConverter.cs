using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Daramee.Degra.Converters
{
	class DegrationFormatConverter : IValueConverter
	{
		public object Convert ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			return ( int ) ( DegrationFormat ) value;
		}

		public object ConvertBack ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			return ( DegrationFormat ) ( int ) value;
		}
	}
}
