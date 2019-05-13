#include "../DegraCore.h"

DEFINE_PROCESS ( Process_ReformatForPng )
{
	if ( dynamic_cast< Daramee_Degra::PngSettings^ > ( args->Settings ) != nullptr
		&& dynamic_cast< Daramee_Degra::PngSettings^ > ( args->Settings )->Indexed )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( source, GUID_WICPixelFormat8bppIndexed,
			args->Dither ? WICBitmapDitherTypeErrorDiffusion : WICBitmapDitherTypeNone,
			nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		return formatConverter.Detach ();
	}

	return source;
}