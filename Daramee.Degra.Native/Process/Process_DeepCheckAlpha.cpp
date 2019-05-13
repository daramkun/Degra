#include "../DegraCore.h"

#include <ppl.h>

DEFINE_PROCESS ( Process_DeepCheckAlpha )
{
	if ( !args->DeepCheckAlpha )
		return source;

	WICPixelFormatGUID pixelFormat;
	if ( FAILED ( source->GetPixelFormat ( &pixelFormat ) ) )
		throw ref new Platform::FailureException ( L"Failed getting Pixel format." );

	if ( pixelFormat == GUID_WICPixelFormat128bppPRGBAFloat || pixelFormat == GUID_WICPixelFormat128bppRGBAFixedPoint || pixelFormat == GUID_WICPixelFormat128bppRGBAFloat
		|| pixelFormat == GUID_WICPixelFormat16bppBGRA5551 || pixelFormat == GUID_WICPixelFormat32bppBGRA || pixelFormat == GUID_WICPixelFormat32bppPBGRA
		|| pixelFormat == GUID_WICPixelFormat32bppPRGBA || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2 || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2HDR10
		|| pixelFormat == GUID_WICPixelFormat32bppRGBA || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102 || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102XR
		|| pixelFormat == GUID_WICPixelFormat8bppIndexed || pixelFormat == GUID_WICPixelFormat1bppIndexed || pixelFormat == GUID_WICPixelFormat2bppIndexed
		|| pixelFormat == GUID_WICPixelFormat4bppIndexed )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( source, GUID_WICPixelFormat32bppBGRA,
			WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeCustom ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		UINT width, height;
		formatConverter->GetSize ( &width, &height );

		int stride = width * 4, totalLength = stride * height, length = width * height;

#pragma pack ( push, 1 )
		struct PIXEL { byte b, g, r, a; };
#pragma pack ( pop )
		std::shared_ptr<PIXEL []> buffer ( new PIXEL [ totalLength ] );
		if ( FAILED ( formatConverter->CopyPixels ( nullptr, stride, totalLength, ( BYTE* ) &buffer [ 0 ] ) ) )
			throw ref new Platform::FailureException ( L"Failed get pixels from source." );

		if ( std::thread::hardware_concurrency () == 2 )
		{
			for ( int i = 0; i < length / 2; ++i )
			{
				if ( buffer [ i ].a != 255 && buffer [ length - i - 1 ].a != 255 )
					return formatConverter.Detach ();
			}
		}
		else
		{
			bool alphaFound = false;

			Concurrency::cancellation_token_source cts;
			Concurrency::run_with_cancellation_token ( [ & ] () {
				Concurrency::parallel_for ( 0, length / 2, [ & ] ( int i )
				{
					if ( buffer [ i ].a != 255 && buffer [ length - i - 1 ].a != 255 )
					{
						alphaFound = true;
						cts.cancel ();
					}
				} );
			}, cts.get_token () );

			if ( alphaFound )
				return formatConverter.Detach ();
		}

		CComPtr<IWICFormatConverter> formatConverter2;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter2 ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter2->Initialize ( source, GUID_WICPixelFormat24bppBGR,
			WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeCustom ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		return formatConverter2.Detach ();
	}

	return source;
}