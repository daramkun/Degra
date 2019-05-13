#ifndef __DARAMEE_DEGRA__CORE_H__
#define __DARAMEE_DEGRA__CORE_H__

#include <wrl.h>
#include <collection.h>
#include <ppltasks.h>

#include <atlbase.h>
#include <wincodec.h>

#include "IDegraStream.h"
#include "Argument.h"
#include "Encoding.h"
#include "Process.h"

namespace Daramee_Degra
{
	public ref class DegraCore sealed
	{
	public:
		DegraCore ();

	private:
		~DegraCore ();

	public:
		void CompressImage ( IDegraStream^ destStream, IDegraStream^ srcStream, Argument^ argument );

	private:
		IWICImagingFactory2* wicFactory;
	};
}

#endif