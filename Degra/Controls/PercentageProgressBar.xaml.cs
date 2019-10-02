using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Daramee.Degra.Controls
{
	/// <summary>
	/// PercentageProgressBar.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class PercentageProgressBar : UserControl
	{
		public static DependencyProperty MaximumProperty = DependencyProperty.Register ( "Maximum", typeof ( double ), typeof ( PercentageProgressBar ), new PropertyMetadata ( OnMinMaxChanged ) );
		public static DependencyProperty MinimumProperty = DependencyProperty.Register ( "Minimum", typeof ( double ), typeof ( PercentageProgressBar ), new PropertyMetadata ( OnMinMaxChanged ) );
		public static DependencyProperty ValueProperty = DependencyProperty.Register ( "Value", typeof ( double ), typeof ( PercentageProgressBar ), new PropertyMetadata ( OnValueChanged ) );

		public double Minimum
		{
			get { return ( double ) GetValue ( MinimumProperty ); }
			set
			{
				if ( value > Maximum )
					throw new ArgumentOutOfRangeException ();
				SetValue ( MinimumProperty, value );
			}
		}
		public double Maximum
		{
			get { return ( double ) GetValue ( MaximumProperty ); }
			set
			{
				if ( value < Minimum )
					throw new ArgumentOutOfRangeException ();
				SetValue ( MaximumProperty, value );
			}
		}
		public double Value
		{
			get { return ( double ) GetValue ( ValueProperty ); }
			set
			{
				if ( value > Maximum )
					value = Maximum;
				else if ( value < Minimum )
					value = Minimum;
				SetValue ( ValueProperty, value );
			}
		}

		public PercentageProgressBar ()
		{
			InitializeComponent ();
			SetValue ( MinimumProperty, 0.0 );
			SetValue ( MaximumProperty, 1.0 );
			Value = 0;
		}

		private static void OnMinMaxChanged ( DependencyObject sender, DependencyPropertyChangedEventArgs e )
		{
			if ( ( double ) sender.GetValue ( MaximumProperty ) < ( double ) sender.GetValue ( ValueProperty ) )
				sender.SetValue ( ValueProperty, sender.GetValue ( MaximumProperty ) );
			else if ( ( double ) sender.GetValue ( MinimumProperty ) > ( double ) sender.GetValue ( ValueProperty ) )
				sender.SetValue ( ValueProperty, sender.GetValue ( MinimumProperty ) );
		}

		private static void OnValueChanged ( DependencyObject sender, DependencyPropertyChangedEventArgs e )
		{
			var ppb = sender as PercentageProgressBar;
			ppb.TextBlockPercentage.Text = string.Format ( "{0:0.00}%", ( ppb.Value - ppb.Minimum ) / ( ppb.Maximum - ppb.Minimum ) * 100 );
		}
	}
}
