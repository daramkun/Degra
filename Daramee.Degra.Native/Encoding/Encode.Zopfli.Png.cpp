#include "../DegraCore.h"

#include <Shlwapi.h>

#include <zopflipng/zopflipng_lib.h>

#	if defined ( DEBUG )
#		pragma comment ( lib, "zopflid.lib" )
#		pragma comment ( lib, "zopflipngd.lib" )
#	else
#		pragma comment ( lib, "zopfli.lib" )
#		pragma comment ( lib, "zopflipng.lib" )
#	endif

void Encode_Zopfli_PNG ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source )
{
	CComPtr<IStream> firstPassStream;
	if ( FAILED ( CreateStreamOnHGlobal ( nullptr, true, &firstPassStream ) ) )
		throw ref new Platform::FailureException ( L"Out of memory or Invalid arguments" );

	Encode_WIC_PNG ( wicFactory, firstPassStream, source );

	STATSTG firstPassStatStg;
	firstPassStream->Stat ( &firstPassStatStg, 0 );
	firstPassStream->Seek ( { 0 }, STREAM_SEEK_SET, nullptr );

	WICPixelFormatGUID pixelFormat;
	source->GetPixelFormat ( &pixelFormat );

	ZopfliPNGOptions options;
	options.lossy_8bit = pixelFormat == GUID_WICPixelFormat8bppIndexed;
	options.use_zopfli = true;
	options.auto_filter_strategy = true;
	options.lossy_transparent = true;
	options.num_iterations *= 4;
	options.num_iterations_large *= 4;

	std::vector<byte> firstPassData ( firstPassStatStg.cbSize.QuadPart );
	firstPassStream->Read ( firstPassData.data (), ( ULONG ) firstPassData.size (), nullptr );

	std::vector<byte> secondPassData;
	ZopfliPNGOptimize ( firstPassData, options, false, &secondPassData );

	stream->Write ( secondPassData.data (), ( ULONG ) secondPassData.size (), nullptr );
}