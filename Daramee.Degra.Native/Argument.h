#ifndef __DARAMEE_DEGRA_ARGUMENT_H__
#define __DARAMEE_DEGRA_ARGUMENT_H__

#include "Settings.h"

namespace Daramee_Degra
{
	public ref class Argument sealed
	{
	public:
		Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, unsigned int maximumHeight );

	public:
		property unsigned int MaximumHeight { unsigned int get () { return maximumHeight; } };
		property bool Dither { bool get () { return dither; } }
		property bool ResizeBicubic { bool get () { return resizeBicubic; } }
		property IEncodingSettings^ Settings { IEncodingSettings^ get () { return settings; } }

	private:
		unsigned int maximumHeight;
		bool dither, resizeBicubic;
		IEncodingSettings^ settings;
	};
}

#endif