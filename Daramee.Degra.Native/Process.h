#ifndef __DARAMEE_DEGRA_PROCESS_H__
#define __DARAMEE_DEGRA_PROCESS_H__

#include <wincodec.h>
#include <functional>

namespace Daramee_Degra
{
	ref class Argument;
}

#define DEFINE_PROCESS(name)								IWICBitmapSource* ##name ( IWICImagingFactory* wicFactory, IWICBitmapSource* source, Daramee_Degra::Argument^ args )

DEFINE_PROCESS ( Process_Resize );
DEFINE_PROCESS ( Process_DeepCheckAlpha );
DEFINE_PROCESS ( Process_ReformatForWebP );
DEFINE_PROCESS ( Process_ReformatForJpeg );
DEFINE_PROCESS ( Process_ReformatForPng );

typedef IWICBitmapSource* ProcessFunc ( IWICImagingFactory*, IWICBitmapSource*, Daramee_Degra::Argument^ );
static std::function<ProcessFunc> g_processFunctions [] =
{
	Process_Resize,
	Process_DeepCheckAlpha,
	Process_ReformatForWebP,
	Process_ReformatForJpeg,
	Process_ReformatForPng,
};

#endif