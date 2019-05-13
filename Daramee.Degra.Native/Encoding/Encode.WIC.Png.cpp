#include "../DegraCore.h"

void Encode_WIC_PNG ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source )
{
	CComPtr<IWICBitmapEncoder> encoder;
	if ( FAILED ( wicFactory->CreateEncoder ( GUID_ContainerFormatPng, &GUID_VendorMicrosoft, &encoder ) ) )
		throw ref new Platform::FailureException ( L"Encoder Create is failed." );

	if ( FAILED ( encoder->Initialize ( stream, WICBitmapEncoderNoCache ) ) )
		throw ref new Platform::FailureException ( L"Encoder Initialize is failed." );

	CComPtr<IWICBitmapFrameEncode> encodeFrame;
	CComPtr<IPropertyBag2> options;
	if ( FAILED ( encoder->CreateNewFrame ( &encodeFrame, &options ) ) )
		throw ref new Platform::FailureException ( L"New Frame creation is failed." );

	PROPBAG2 filter = {};
	filter.pstrName = ( LPOLESTR ) L"FilterOption";
	filter.vt = VT_R4;

	VARIANT filterVar = {};
	filterVar.vt = VT_UI1;
	filterVar.bVal = WICPngFilterAdaptive;

	if ( FAILED ( options->Write ( 0, &filter, &filterVar ) ) )
		throw ref new Platform::FailureException ( L"PNG Option setting is failed." );

	if ( FAILED ( encodeFrame->Initialize ( options ) ) )
		throw ref new Platform::FailureException ( L"Image Frame initializing is failed." );

	if ( FAILED ( encodeFrame->WriteSource ( source, nullptr ) ) )
		throw ref new Platform::FailureException ( L"WIC Encoding is failed." );

	if ( FAILED ( encodeFrame->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Frame Committing is failed." );
	if ( FAILED ( encoder->Commit () ) )
		throw ref new Platform::FailureException ( L"Image Committing is failed." );
}