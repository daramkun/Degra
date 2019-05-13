#include "../DegraCore.h"

DEFINE_PROCESS ( Process_ReformatForWebP )
{
	if ( dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings ) )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		WICPixelFormatGUID pixelFormat;
		source->GetPixelFormat ( &pixelFormat );

		if ( pixelFormat == GUID_WICPixelFormat128bppPRGBAFloat || pixelFormat == GUID_WICPixelFormat128bppRGBAFixedPoint || pixelFormat == GUID_WICPixelFormat128bppRGBAFloat
			|| pixelFormat == GUID_WICPixelFormat16bppBGRA5551 || pixelFormat == GUID_WICPixelFormat32bppBGRA || pixelFormat == GUID_WICPixelFormat32bppPBGRA
			|| pixelFormat == GUID_WICPixelFormat32bppPRGBA || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2 || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2HDR10
			|| pixelFormat == GUID_WICPixelFormat32bppRGBA || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102 || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102XR )
			pixelFormat = GUID_WICPixelFormat32bppBGRA;
		else
			pixelFormat = GUID_WICPixelFormat24bppBGR;

		if ( FAILED ( formatConverter->Initialize ( source, pixelFormat, WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		return formatConverter.Detach ();
	}

	return source;
}