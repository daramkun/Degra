#include "../DegraCore.h"

DEFINE_PROCESS ( Process_Resize )
{
	UINT width, height;
	if ( FAILED ( source->GetSize ( &width, &height ) ) )
		throw ref new Platform::FailureException ( L"Getting Image Size is failed." );

	if ( height > args->MaximumHeight )
	{
		float scaleFactor = args->MaximumHeight / ( float ) height;
		CComPtr<IWICBitmapScaler> scaler;
		if ( FAILED ( wicFactory->CreateBitmapScaler ( &scaler ) ) )
			throw ref new Platform::FailureException ( L"Initializing Scale Image is failed." );

		if ( FAILED ( scaler->Initialize ( source,
			( UINT ) ( width * scaleFactor ), ( UINT ) ( height * scaleFactor ),
			args->ResizeBicubic ? WICBitmapInterpolationModeHighQualityCubic : WICBitmapInterpolationModeNearestNeighbor ) ) )
			throw ref new Platform::FailureException ( L"Scaling Image is failed." );

		return scaler.Detach ();
	}

	return source;
}