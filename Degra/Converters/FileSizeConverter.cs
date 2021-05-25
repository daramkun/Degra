using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Daramee.Degra.Converters
{
	class FileSizeConverter : IValueConverter
	{
		private static readonly string[] Units = new[]
		{
			"B", "KB", "MB", "GB", "TB", "PB"
		};

		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			double fileSize = (double)(long) value;
			int unitIndex = 0;

			while (fileSize > 1024)
			{
				fileSize = Math.Round(fileSize / 1024);
				++unitIndex;
			}

			return $"{fileSize}{Units[unitIndex]}";
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
