using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
	/// UpDownTextBox.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class UpDownTextBox : UserControl
	{
		public static readonly DependencyProperty MaximumProperty = DependencyProperty.Register ( "Maximum", typeof ( int ), typeof ( UpDownTextBox ), new PropertyMetadata ( OnMinMaxChanged ) );
		public static readonly DependencyProperty MinimumProperty = DependencyProperty.Register ( "Minimum", typeof ( int ), typeof ( UpDownTextBox ), new PropertyMetadata ( OnMinMaxChanged ) );
		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register ( "Value", typeof ( int ), typeof ( UpDownTextBox ), new PropertyMetadata ( OnValueChanged ) );

		public int Minimum
		{
			get { return ( int ) GetValue ( MinimumProperty ); }
			set
			{
				if ( value > Maximum )
					throw new ArgumentOutOfRangeException ();
				SetValue ( MinimumProperty, value );
			}
		}

		public int Maximum
		{
			get { return ( int ) GetValue ( MaximumProperty ); }
			set
			{
				if ( value < Minimum )
					throw new ArgumentOutOfRangeException ();
				SetValue ( MaximumProperty, value );
			}
		}

		public int Value
		{
			get { return ( int ) GetValue ( ValueProperty ); }
			set
			{
				if ( value > Maximum )
					value = Maximum;
				else if ( value < Minimum )
					value = Minimum;
				SetValue ( ValueProperty, value );
			}
		}

		public UpDownTextBox ()
		{
			InitializeComponent ();
			SetValue ( MinimumProperty, 0 );
			SetValue ( MaximumProperty, 100 );
			Value = 0;
		}

		private static void OnMinMaxChanged ( DependencyObject sender, DependencyPropertyChangedEventArgs e )
		{
			if ( ( int ) sender.GetValue ( MaximumProperty ) < ( int ) sender.GetValue ( ValueProperty ) )
				sender.SetValue ( ValueProperty, sender.GetValue ( MaximumProperty ) );
			else if ( ( int ) sender.GetValue ( MinimumProperty ) > ( int ) sender.GetValue ( ValueProperty ) )
				sender.SetValue ( ValueProperty, sender.GetValue ( MinimumProperty ) );
		}

		private static void OnValueChanged ( DependencyObject sender, DependencyPropertyChangedEventArgs e )
		{
			UpDownTextBox self = sender as UpDownTextBox;
			self.TextBoxNumeric.Text = self.Value.ToString ();
		}

		private void UpButton_Click ( object sender, RoutedEventArgs e )
		{
			++Value;
		}

		private void DownButton_Click ( object sender, RoutedEventArgs e )
		{
			--Value;
		}

		static readonly Regex NumericRegex = new Regex ( "^-?[0-9]+$" );
		private void TextBox_PreviewTextInput ( object sender, TextCompositionEventArgs e )
		{
			e.Handled = !NumericRegex.IsMatch ( e.Text );
		}

		private void TextBoxNumeric_TextChanged ( object sender, TextChangedEventArgs e )
		{
			if ( !NumericRegex.IsMatch ( ( sender as TextBox ).Text ) )
			{
				( sender as TextBox ).Text = new Regex ( "[^0-9\\-]+" ).Replace ( ( sender as TextBox ).Text, "" );
				if ( string.IsNullOrEmpty ( ( sender as TextBox ).Text ) )
					( sender as TextBox ).Text = "0";
				if ( ( sender as TextBox ).Text.IndexOf ( '-', 1 ) > 0 )
				{
					bool signed = ( sender as TextBox ).Text.IndexOf ( '-' ) == 0;
					( sender as TextBox ).Text = ( signed ? "-" : "" )
						+ ( sender as TextBox ).Text.Replace ( "-", "" );
				}
				return;
			}
			if ( e.Changes.Count != 0 )
			{
				int value = int.Parse ( ( sender as TextBox ).Text );
				Value = value;
				( sender as TextBox ).Text = Value.ToString ();
			}
		}
	}
}
