#ifndef __DARAMEE_DEGRA_ARGUMENT_H__
#define __DARAMEE_DEGRA_ARGUMENT_H__

#include "Settings.h"

namespace Daramee_Degra
{
	public ref class Argument sealed
	{
	public:
		Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, bool deepCheckAlpha, unsigned int maximumHeight );

	public:
		property unsigned int MaximumHeight { unsigned int get () { return maximumHeight; } };
		property bool Dither { bool get () { return dither; } }
		property bool ResizeBicubic { bool get () { return resizeBicubic; } }
		property bool DeepCheckAlpha { bool get () { return deepCheckAlpha; } }
		property IEncodingSettings^ Settings
		{
			IEncodingSettings^ get () { return settings; }
			void set ( IEncodingSettings^ value ) { settings = value; }
		}

	private:
		unsigned int maximumHeight;
		bool dither, resizeBicubic, deepCheckAlpha;
		IEncodingSettings^ settings;
	};
}

#endif