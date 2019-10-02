using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Daramee.Degra.Converters
{
	class StatusTextConverter : IValueConverter
	{
		public object Convert ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			switch ( ( DegraStatus ) value )
			{
				case DegraStatus.Waiting: return "대기 중";
				case DegraStatus.Processing: return "변환 중";
				case DegraStatus.Done: return "완료";
				case DegraStatus.Failed: return "실패";
				case DegraStatus.Cancelled: return "취소";
				default: throw new ArgumentOutOfRangeException ();
			}
		}

		public object ConvertBack ( object value, Type targetType, object parameter, CultureInfo culture )
		{
			throw new NotImplementedException ();
		}
	}
}
