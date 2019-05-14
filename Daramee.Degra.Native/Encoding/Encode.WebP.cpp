#include "../DegraCore.h"

#include <webp/encode.h>
#pragma comment ( lib, "libwebp.lib" )

void Encode_WebP ( IStream* stream, IWICBitmapSource* source, int quality, bool lossless )
{
	UINT width, height;
	if ( FAILED ( source->GetSize ( &width, &height ) ) ) throw ref new Platform::FailureException ( L"Getting Image Size is failed." );
	WICPixelFormatGUID pixelFormat;
	if ( FAILED ( source->GetPixelFormat ( &pixelFormat ) ) )
		throw ref new Platform::FailureException ( L"Getting Image Pixel Format is failed." );

	WebPConfig config = {};
	WebPPicture picture = {};

	memset ( &config, 0, sizeof ( config ) );
	memset ( &picture, 0, sizeof ( picture ) );

	config.quality = ( float ) quality;
	config.lossless = lossless;

	picture.width = width;
	picture.height = height;
	picture.colorspace = WEBP_YUV420;
	picture.user_data = stream;
	picture.writer = [] ( const uint8_t * data, size_t data_size, const WebPPicture * picture ) -> int
	{
		ULONG written;
		IStream* stream = reinterpret_cast< IStream* >( picture->user_data );
		if ( FAILED ( stream->Write ( data, ( ULONG ) data_size, &written ) ) )
			return -1;
		return written;
	};

	if ( !WebPConfigInit ( &config ) || !WebPPictureAlloc ( &picture ) )
		throw ref new Platform::FailureException ( L"WebP initializing is failed." );

	config.thread_level = 1;
	if ( !WebPValidateConfig ( &config ) )
		throw ref new Platform::FailureException ( L"WebP Config check validation is failed." );

	int stride;
	std::function<int ( WebPPicture*, const uint8_t*, int )> importPixels;
	if ( pixelFormat == GUID_WICPixelFormat32bppBGRA )
	{
		stride = width * 4;
		importPixels = WebPPictureImportBGRA;
		picture.use_argb = true;
	}
	else
	{
		stride = 4 * ( ( width * ( ( 24 + 7 ) / 8 ) + 3 ) / 4 );
		importPixels = WebPPictureImportBGR;
	}

	BYTE * bytes = new BYTE [ stride * height ];
	source->CopyPixels ( nullptr, stride, stride * height, bytes );
	importPixels ( &picture, bytes, stride );
	delete [] bytes;

	if ( !WebPEncode ( &config, &picture ) )
		throw ref new Platform::FailureException ( L"WebP Encoding is failed." );

	WebPPictureFree ( &picture );
}