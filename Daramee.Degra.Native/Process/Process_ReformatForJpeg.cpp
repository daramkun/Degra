#include "../DegraCore.h"

DEFINE_PROCESS ( Process_ReformatForJpeg )
{
	if ( dynamic_cast< Daramee_Degra::JpegSettings^ >( args->Settings ) )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( source, GUID_WICPixelFormat24bppBGR,
			WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		return formatConverter.Detach ();
	}

	return source;
}