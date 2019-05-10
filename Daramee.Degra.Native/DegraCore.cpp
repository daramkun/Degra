#include "DegraCore.h"

#include <webp/encode.h>
#pragma comment ( lib, "libwebp.lib" )

#pragma comment ( lib, "WindowsCodecs.lib" )

#pragma region Settings Objects
Daramee_Degra::WebPSettings::WebPSettings ( int quality )
{
	if ( quality < 1 || quality > 100 )
		throw ref new Platform::InvalidArgumentException ( L"Quality value must 1-100." );

	this->quality = quality;
}

Daramee_Degra::JpegSettings::JpegSettings ( int quality )
{
	if ( quality < 1 || quality > 100 )
		throw ref new Platform::InvalidArgumentException ( L"Quality value must 1-100." );

	this->quality = quality;
}

Daramee_Degra::PngSettings::PngSettings ( bool indexed )
{
	this->indexed = indexed;
}

Daramee_Degra::Argument::Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, unsigned int maximumHeight )
{
	this->dither = dither;
	this->resizeBicubic = resizeBicubic;
	this->settings = settings;
	this->maximumHeight = maximumHeight;
}
#pragma endregion

class ImplementedIStream : public IStream
{
public:
	ImplementedIStream ( Daramee_Degra::IDegraStream^ stream ) : refCount ( 1 ), stream ( stream ) { }
	~ImplementedIStream () { stream = nullptr; }

public:
	virtual HRESULT __stdcall QueryInterface ( REFIID riid, void** ppvObject ) override
	{
		if ( riid == __uuidof ( IStream ) || riid == __uuidof ( IUnknown )
			|| riid == __uuidof( ISequentialStream ) )
		{
			*ppvObject = this;
			AddRef ();
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	virtual ULONG __stdcall AddRef ( void ) override { return InterlockedIncrement ( &refCount ); }
	virtual ULONG __stdcall Release ( void ) override { auto ret = InterlockedDecrement ( &refCount ); if ( ret == 0 ) delete this; return ret; }

public:
	virtual HRESULT __stdcall Read ( void* pv, ULONG cb, ULONG* pcbRead ) override
	{
		auto arr = ref new Platform::Array<byte> ( ( byte* ) pv, cb );
		auto read = stream->Read ( arr, cb );
		memcpy ( pv, arr->Data, read );
		if ( pcbRead )
			* pcbRead = read;
		return S_OK;
	}
	virtual HRESULT __stdcall Write ( const void* pv, ULONG cb, ULONG* pcbWritten ) override
	{
		auto arr = ref new Platform::Array<byte> ( ( byte* ) pv, cb );
		auto written = stream->Write ( arr, cb );
		if ( pcbWritten )
			* pcbWritten = written;
		return S_OK;
	}
	virtual HRESULT __stdcall Seek ( LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition ) override
	{
		int pos = stream->Seek ( ( Daramee_Degra::SeekOrigin ) dwOrigin, ( int ) dlibMove.QuadPart );
		if ( plibNewPosition )
			plibNewPosition->QuadPart = ( ULONGLONG ) pos;
		return S_OK;
	}

	virtual HRESULT __stdcall Stat ( STATSTG* pstatstg, DWORD grfStatFlag ) override
	{
		memset ( pstatstg, 0, sizeof ( STATSTG ) );
		pstatstg->type = STGTY_STREAM;
		pstatstg->cbSize.QuadPart = stream->Length;
		pstatstg->grfMode = STGM_READ | STGM_WRITE;
		return S_OK;
	}

	virtual HRESULT __stdcall SetSize ( ULARGE_INTEGER libNewSize ) override { return E_NOTIMPL; }
	virtual HRESULT __stdcall CopyTo ( IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten ) override { return E_NOTIMPL; }
	virtual HRESULT __stdcall Commit ( DWORD grfCommitFlags ) override { stream->Flush (); return S_OK; }
	virtual HRESULT __stdcall Revert ( void ) override { return E_NOTIMPL; }
	virtual HRESULT __stdcall LockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) override { return E_NOTIMPL; }
	virtual HRESULT __stdcall UnlockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) override { return E_NOTIMPL; }
	virtual HRESULT __stdcall Clone ( IStream** ppstm ) override { return E_NOTIMPL; }

private:
	ULONG refCount;
	Daramee_Degra::IDegraStream^ stream;
};

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
	else if ( args->Dither )
	{
		WICPixelFormatGUID pixelFormat;
		ret->GetPixelFormat ( &pixelFormat );

		CComPtr<IWICFormatConverter> formatConverter;
		if ( FAILED ( wicFactory->CreateFormatConverter ( &formatConverter ) ) )
			throw ref new Platform::FailureException ( L"Initializing Dither Image is failed." );

		if ( FAILED ( formatConverter->Initialize ( ret, pixelFormat,
			args->Dither ? WICBitmapDitherTypeOrdered16x16 : WICBitmapDitherTypeNone,
			nullptr, 1, WICBitmapPaletteTypeMedianCut ) ) )
			throw ref new Platform::FailureException ( L"Dithering Image is failed." );

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

	if ( *arranged != ret )
		* arranged = ret.Detach ();
}

int Degra_Inner_SaveTo_WebP_Writer ( const uint8_t * data, size_t data_size, const WebPPicture * picture )
{
	ULONG written;
	IStream* stream = reinterpret_cast< IStream* >( picture->user_data );
	if ( FAILED ( stream->Write ( data, ( ULONG ) data_size, &written ) ) )
		return -1;
	return written;
}

void STDMETHODCALLTYPE Degra_Inner_SaveTo_WebP ( IWICBitmapSource * source, IStream * dest, Daramee_Degra::Argument^ args )
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

	config.quality = ( float ) dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings )->Quality;

	picture.width = width;
	picture.height = height;
	picture.colorspace = WEBP_YUV420;
	picture.user_data = dest;
	picture.writer = Degra_Inner_SaveTo_WebP_Writer;

	if ( !WebPConfigInit ( &config ) )
		throw ref new Platform::FailureException ( L"WebP Config initializing is failed." );
	if ( !WebPPictureAlloc ( &picture ) )
		throw ref new Platform::FailureException ( L"WebP Picture initializing is failed." );

	config.thread_level = 1;

	if ( !WebPValidateConfig ( &config ) )
		throw ref new Platform::FailureException ( L"WebP Config check validation is failed." );

	BYTE * bytes;
	if ( pixelFormat == GUID_WICPixelFormat32bppBGRA )
	{
		int stride = width * 4;
		bytes = new BYTE [ stride * height ];
		source->CopyPixels ( nullptr, stride, stride * height, bytes );
		WebPPictureImportBGRA ( &picture, bytes, stride );
		delete [] bytes;
	}
	else
	{
		int bytesPerPixel = ( 24 + 7 ) / 8;
		int stride = 4 * ( ( width * bytesPerPixel + 3 ) / 4 );
		bytes = new BYTE [ stride * height ];
		source->CopyPixels ( nullptr, stride, stride * height, bytes );
		WebPPictureImportBGR ( &picture, bytes, stride );
		delete [] bytes;
	}

	if ( !WebPEncode ( &config, &picture ) )
		throw ref new Platform::FailureException ( L"WebP Encoding is failed." );

	WebPPictureFree ( &picture );
}

void STDMETHODCALLTYPE Degra_Inner_SaveTo_WIC ( IWICImagingFactory* wicFactory, IWICBitmapSource * source, IStream * dest, Daramee_Degra::Argument^ args )
{
	GUID containerFormat;
	if ( dynamic_cast< Daramee_Degra::JpegSettings^ > ( args->Settings ) != nullptr ) containerFormat = GUID_ContainerFormatJpeg;
	else if ( dynamic_cast< Daramee_Degra::PngSettings^ > ( args->Settings ) != nullptr ) containerFormat = GUID_ContainerFormatPng;
	else throw ref new Platform::InvalidArgumentException ();

	CComPtr<IWICBitmapEncoder> encoder;
	if ( FAILED ( wicFactory->CreateEncoder ( containerFormat, &GUID_VendorMicrosoft, &encoder ) ) )
		throw ref new Platform::FailureException ( L"Encoder Create is failed." );

	if ( FAILED ( encoder->Initialize ( dest, WICBitmapEncoderNoCache ) ) )
		throw ref new Platform::FailureException ( L"Encoder Initialize is failed." );

	CComPtr<IWICBitmapFrameEncode> encodeFrame;
	CComPtr<IPropertyBag2> options;
	if ( FAILED ( encoder->CreateNewFrame ( &encodeFrame, &options ) ) )
		throw ref new Platform::FailureException ( L"New Frame creation is failed." );

	if ( containerFormat == GUID_ContainerFormatJpeg )
	{
		PROPBAG2 imageQuality = {};
		imageQuality.pstrName = ( LPOLESTR ) L"ImageQuality";
		imageQuality.vt = VT_R4;

		VARIANT imageQualityVar = {};
		imageQualityVar.vt = VT_R4;
		imageQualityVar.fltVal = dynamic_cast< Daramee_Degra::JpegSettings^ > ( args->Settings )->Quality / 100.0f;

		if ( FAILED ( options->Write ( 0, &imageQuality, &imageQualityVar ) ) )
			throw ref new Platform::FailureException ( L"JPEG Option setting is failed." );
	}
	else if ( containerFormat == GUID_ContainerFormatPng )
	{
		PROPBAG2 filter = {};
		filter.pstrName = ( LPOLESTR ) L"FilterOption";
		filter.vt = VT_R4;

		VARIANT filterVar = {};
		filterVar.vt = VT_UI1;
		filterVar.bVal = WICPngFilterAdaptive;

		if ( FAILED ( options->Write ( 0, &filter, &filterVar ) ) )
			throw ref new Platform::FailureException ( L"PNG Option setting is failed." );
	}

	if ( FAILED ( encodeFrame->Initialize ( options ) ) )
		throw ref new Platform::FailureException ( L"Image Frame initializing is failed." );

	UINT width, height;
	if ( FAILED ( source->GetSize ( &width, &height ) ) )
		throw ref new Platform::FailureException ( L"Getting image size is failed." );
	if ( FAILED ( encodeFrame->SetSize ( width, height ) ) )
		throw ref new Platform::FailureException ( L"Setting image size is failed." );

	WICPixelFormatGUID pixelFormat;
	if ( FAILED ( source->GetPixelFormat ( &pixelFormat ) ) )
		throw ref new Platform::FailureException ( L"Getting Pixel format is failed." );
	if ( FAILED ( encodeFrame->SetPixelFormat ( &pixelFormat ) ) )
		throw ref new Platform::FailureException ( L"Setting Pixel format is failed." );

	double dpiX, dpiY;
	if ( FAILED ( source->GetResolution ( &dpiX, &dpiY ) ) )
		throw ref new Platform::FailureException ( L"Getting DPI is failed." );
	if ( FAILED ( encodeFrame->SetResolution ( dpiX, dpiY ) ) )
		throw ref new Platform::FailureException ( L"Setting DPI is failed." );

	if ( pixelFormat == GUID_WICPixelFormat8bppIndexed || pixelFormat == GUID_WICPixelFormat4bppIndexed
		|| pixelFormat == GUID_WICPixelFormat2bppIndexed || pixelFormat == GUID_WICPixelFormat1bppIndexed )
	{
		CComPtr<IWICPalette> palette;
		if ( FAILED ( wicFactory->CreatePalette ( &palette ) ) )
			throw ref new Platform::FailureException ( L"Palette Creation is failed." );
		if ( FAILED ( source->CopyPalette ( palette ) ) )
			throw ref new Platform::FailureException ( L"Palette Copying is failed." );
		if ( FAILED ( encodeFrame->SetPalette ( palette ) ) )
			throw ref new Platform::FailureException ( L"Setting Palette is failed." );
	}

	if ( FAILED ( encodeFrame->WriteSource ( source, nullptr ) ) )
		throw ref new Platform::FailureException ( L"WIC Encoding is failed." );

	if ( FAILED ( encodeFrame->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Frame Committing is failed." );
	if ( FAILED ( encoder->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Committing is failed." );
}

void STDMETHODCALLTYPE Degra_Inner_SaveTo ( IWICImagingFactory* wicFactory, IWICBitmapSource * source, IStream * dest, Daramee_Degra::Argument^ args )
{
	if ( dynamic_cast< Daramee_Degra::WebPSettings^ > ( args->Settings ) )
		Degra_Inner_SaveTo_WebP ( source, dest, args );
	else
		Degra_Inner_SaveTo_WIC ( wicFactory, source, dest, args );
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

void Daramee_Degra::DegraCore::ConvertImage ( IDegraStream^ destStream, IDegraStream^ srcStream, Argument^ argument )
{
	CComPtr<IStream> destStreamNative, srcStreamNative;
	*&destStreamNative = new ImplementedIStream ( destStream );
	*&srcStreamNative = new ImplementedIStream ( srcStream );

	CComPtr<IWICBitmapSource> sourceBitmap;
	Degra_Inner_LoadBitmap ( wicFactory, srcStreamNative, &sourceBitmap );

	CComPtr<IWICBitmapSource> arranged;
	Degra_Inner_ArrangeImage ( wicFactory, sourceBitmap, argument, &arranged );

	destStreamNative->Seek ( { 0 }, STREAM_SEEK_SET, nullptr );
	Degra_Inner_SaveTo ( wicFactory, arranged, destStreamNative, argument );
}
