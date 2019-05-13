#include "../DegraCore.h"

void Encode_WIC_Jpeg ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source, int quality )
{
	CComPtr<IWICBitmapEncoder> encoder;
	if ( FAILED ( wicFactory->CreateEncoder ( GUID_ContainerFormatJpeg, &GUID_VendorMicrosoft, &encoder ) ) )
		throw ref new Platform::FailureException ( L"Encoder Create is failed." );

	if ( FAILED ( encoder->Initialize ( stream, WICBitmapEncoderNoCache ) ) )
		throw ref new Platform::FailureException ( L"Encoder Initialize is failed." );

	CComPtr<IWICBitmapFrameEncode> encodeFrame;
	CComPtr<IPropertyBag2> options;
	if ( FAILED ( encoder->CreateNewFrame ( &encodeFrame, &options ) ) )
		throw ref new Platform::FailureException ( L"New Frame creation is failed." );

	PROPBAG2 imageQuality = {};
	imageQuality.pstrName = ( LPOLESTR ) L"ImageQuality";
	imageQuality.vt = VT_R4;

	VARIANT imageQualityVar = {};
	imageQualityVar.vt = VT_R4;
	imageQualityVar.fltVal = quality / 100.0f;

	if ( FAILED ( options->Write ( 0, &imageQuality, &imageQualityVar ) ) )
		throw ref new Platform::FailureException ( L"JPEG Option setting is failed." );

	if ( FAILED ( encodeFrame->Initialize ( options ) ) )
		throw ref new Platform::FailureException ( L"Image Frame initializing is failed." );

	if ( FAILED ( encodeFrame->WriteSource ( source, nullptr ) ) )
		throw ref new Platform::FailureException ( L"WIC Encoding is failed." );

	if ( FAILED ( encodeFrame->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Frame Committing is failed." );
	if ( FAILED ( encoder->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Committing is failed." );
}