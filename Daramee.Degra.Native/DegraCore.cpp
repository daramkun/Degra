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

	CComPtr<IWICBitmap> tempBitmap;
	if ( FAILED ( wicFactory->CreateBitmapFromSource ( decodeFrame, WICBitmapCacheOnDemand, &tempBitmap ) ) )
		throw ref new Platform::FailureException ( L"Failed Create Copied Bitmap." );

	*source = tempBitmap.Detach ();
}

void STDMETHODCALLTYPE Degra_Inner_ArrangeImage ( IWICImagingFactory* wicFactory, IWICBitmapSource * source, Daramee_Degra::Argument^ args, IWICBitmapSource * *arranged )
{
	CComQIPtr<IWICBitmapSource> ret;
	ret = source;

	for ( auto process : g_processFunctions )
		ret = process ( wicFactory, ret, args );

	//ret = Process_Resize ( wicFactory, ret, args );
	//ret = Process_DeepCheckAlpha ( wicFactory, ret, args );
	//ret = Process_ReformatForWebP ( wicFactory, ret, args );
	//ret = Process_ReformatForJpeg ( wicFactory, ret, args );
	//ret = Process_ReformatForPng ( wicFactory, ret, args );

	*arranged = ret.Detach ();
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
