#include "DegraCore.h"

#include <functional>

#pragma comment ( lib, "WindowsCodecs.lib" )

#pragma region Native Functions
void STDMETHODCALLTYPE Degra_Inner_LoadBitmap ( IWICImagingFactory * wicFactory, IStream* src, IWICBitmapSource** source )
{
	CComPtr<IWICBitmapDecoder> decoder;
	if ( FAILED ( wicFactory->CreateDecoderFromStream ( src, nullptr, WICDecodeMetadataCacheOnDemand, &decoder ) ) )
		throw ref new Platform::FailureException ( L"Decoder from Stream is failed." );

	CComPtr<IWICBitmapFrameDecode> decodeFrame;
	if ( FAILED ( decoder->GetFrame ( 0, &decodeFrame ) ) )
		throw ref new Platform::FailureException ( L"Getting Image Frame is failed." );

	*source = decodeFrame.Detach ();
}

void STDMETHODCALLTYPE Degra_Inner_ArrangeImage ( IWICImagingFactory* wicFactory, IWICBitmapSource * source, Daramee_Degra::Argument^ args, IWICBitmapSource * *arranged )
{
	CComQIPtr<IWICBitmapSource> ret;
	ret = source;

	UINT width, height;
	if ( FAILED ( ret->GetSize ( &width, &height ) ) )
		throw ref new Platform::FailureException ( L"Getting Image Size is failed." );

	if ( height > args->MaximumHeight )
	{
		float scaleFactor = args->MaximumHeight / ( float ) height;
		CComPtr<IWICBitmapScaler> scaler;
		if ( FAILED ( wicFactory->CreateBitmapScaler ( &scaler ) ) )
			throw ref new Platform::FailureException ( L"Initializing Scale Image is failed." );

		if ( FAILED ( scaler->Initialize ( ret,
			( UINT ) ( width * scaleFactor ), ( UINT ) ( height * scaleFactor ),
			args->ResizeBicubic ? WICBitmapInterpolationModeHighQualityCubic : WICBitmapInterpolationModeNearestNeighbor ) ) )
			throw ref new Platform::FailureException ( L"Scaling Image is failed." );

		ret = scaler.Detach ();
	}

	if ( dynamic_cast< Daramee_Degra::PngSettings ^> ( args->Settings ) != nullptr && dynamic_cast< Daramee_Degra::PngSettings^ > ( args->Settings )->Indexed )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( ret, GUID_WICPixelFormat8bppIndexed,
			args->Dither ? WICBitmapDitherTypeOrdered16x16 : WICBitmapDitherTypeNone,
			nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		ret = formatConverter.Detach ();
	}

	if ( dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings ) )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		WICPixelFormatGUID pixelFormat;
		ret->GetPixelFormat ( &pixelFormat );

		if ( pixelFormat == GUID_WICPixelFormat128bppPRGBAFloat || pixelFormat == GUID_WICPixelFormat128bppRGBAFixedPoint || pixelFormat == GUID_WICPixelFormat128bppRGBAFloat
			|| pixelFormat == GUID_WICPixelFormat16bppBGRA5551 || pixelFormat == GUID_WICPixelFormat32bppBGRA || pixelFormat == GUID_WICPixelFormat32bppPBGRA
			|| pixelFormat == GUID_WICPixelFormat32bppPRGBA || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2 || pixelFormat == GUID_WICPixelFormat32bppR10G10B10A2HDR10
			|| pixelFormat == GUID_WICPixelFormat32bppRGBA || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102 || pixelFormat == GUID_WICPixelFormat32bppRGBA1010102XR )
			pixelFormat = GUID_WICPixelFormat32bppBGRA;
		else
			pixelFormat = GUID_WICPixelFormat24bppBGR;

		if ( FAILED ( formatConverter->Initialize ( ret, pixelFormat, WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		ret = formatConverter.Detach ();
	}
	else if ( dynamic_cast< Daramee_Degra::JpegSettings^ >( args->Settings ) )
	{
		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Reformat Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( ret, GUID_WICPixelFormat24bppBGR,
			WICBitmapDitherTypeNone, nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Reformatting Image is failed." );

		ret = formatConverter.Detach ();
	}

	CComPtr<IWICBitmap> tempBitmap;
	if ( FAILED ( wicFactory->CreateBitmapFromSource ( ret, WICBitmapCacheOnDemand, &tempBitmap ) ) )
		throw ref new Platform::FailureException ( L"Failed Create Copied Bitmap." );

	*arranged = tempBitmap.Detach ();
}

void STDMETHODCALLTYPE Degra_Inner_SaveTo ( IWICImagingFactory* wicFactory, IWICBitmapSource * source, IStream * dest, Daramee_Degra::Argument^ args )
{
	if ( dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings ) )
		Encode_WebP ( dest, source, dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings )->Quality );
	else if ( dynamic_cast< Daramee_Degra::JpegSettings^ >( args->Settings ) )
	{
#if defined( _M_AMD64 ) || defined ( _M_IA32 )
		Encode_MozJpeg_Jpeg ( dest, source, dynamic_cast< Daramee_Degra::JpegSettings^ >( args->Settings )->Quality );
#else
		Encode_WIC_Jpeg ( wicFactory, dest, source, dynamic_cast< Daramee_Degra::JpegSettings^ >( args->Settings )->Quality );
#endif
	}
	else if ( dynamic_cast< Daramee_Degra::PngSettings^ >( args->Settings ) )
	{
		auto pngSettings = dynamic_cast< Daramee_Degra::PngSettings^ >( args->Settings );
		if ( pngSettings->UseZopfli )
			Encode_Zopfli_PNG ( wicFactory, dest, source );
		else
			Encode_WIC_PNG ( wicFactory, dest, source );
	}
}
#pragma endregion

Daramee_Degra::DegraCore::DegraCore ()
	: wicFactory ( nullptr )
{
	if ( FAILED ( CoCreateInstance ( CLSID_WICImagingFactory2, nullptr, CLSCTX_ALL,
		__uuidof ( IWICImagingFactory2 ), ( void** ) & wicFactory ) ) )
		throw ref new Platform::NotImplementedException ( L"Windows Imaging Factory initialize is failed." );
}

Daramee_Degra::DegraCore::~DegraCore ()
{
	if ( wicFactory == nullptr )
		wicFactory->Release ();
	wicFactory = nullptr;
}

void Daramee_Degra::DegraCore::CompressImage ( IDegraStream^ destStream, IDegraStream^ srcStream, Argument^ argument )
{
	CComPtr<IWICBitmapSource> arranged;
	{
		CComPtr<IWICBitmapSource> sourceBitmap;
		{
			CComPtr<IStream> srcStreamNative;
			*&srcStreamNative = new ImplementedIStream ( srcStream );
			Degra_Inner_LoadBitmap ( wicFactory, srcStreamNative, &sourceBitmap );
		}

		Degra_Inner_ArrangeImage ( wicFactory, sourceBitmap, argument, &arranged );
	}

	{
		CComPtr<IStream> destStreamNative;
		*&destStreamNative = new ImplementedIStream ( destStream );
		destStreamNative->Seek ( { 0 }, STREAM_SEEK_SET, nullptr );
		Degra_Inner_SaveTo ( wicFactory, arranged, destStreamNative, argument );
	}
}
