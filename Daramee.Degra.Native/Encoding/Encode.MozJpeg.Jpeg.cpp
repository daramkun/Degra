#include "../DegraCore.h"

#if defined ( _M_AMD64 ) || defined ( _M_IA32 )
#	include <jpeglib.h>
#	include <turbojpeg.h>
//#	pragma comment ( lib, "jpeg-static.lib" )
#	if defined ( DEBUG )
#		pragma comment ( lib, "turbojpegd-static.lib" )
#	else
#		pragma comment ( lib, "turbojpeg-static.lib" )
#	endif
#endif

#pragma region Stream Initialize
struct degra_destination_mgr
{
	jpeg_destination_mgr pub;

	IStream* stream;
	JOCTET* buffer;
};

#define OUTPUT_BUF_SIZE										4096

void init_stream_destination ( j_compress_ptr cinfo )
{
	degra_destination_mgr* dest = ( degra_destination_mgr* ) cinfo->dest;

	dest->buffer = ( JOCTET* ) ( *cinfo->mem->alloc_small ) ( ( j_common_ptr ) cinfo, JPOOL_IMAGE,
			OUTPUT_BUF_SIZE * sizeof ( JOCTET ) );

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

boolean empty_stream_output_buffer ( j_compress_ptr cinfo )
{
	degra_destination_mgr* dest = ( degra_destination_mgr* ) cinfo->dest;

	ULONG written;
	if ( FAILED ( dest->stream->Write ( dest->buffer, OUTPUT_BUF_SIZE, &written ) ) || written != OUTPUT_BUF_SIZE )
		return FALSE;

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return TRUE;
}

void term_stream_destination ( j_compress_ptr cinfo )
{
	degra_destination_mgr* dest = ( degra_destination_mgr* ) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

	if ( datacount > 0 ) {
		ULONG written;
		if ( FAILED ( dest->stream->Write ( dest->buffer, ( ULONG ) datacount, &written ) ) || written != datacount )
			throw ref new Platform::FailureException ( L"Stream Written is failed." );
	}
	dest->stream->Commit ( 0 );
}

void jpeg_stream_dest ( j_compress_ptr cinfo, IStream* stream )
{
	degra_destination_mgr* dest;

	if ( cinfo->dest == nullptr )
	{
		cinfo->dest = ( jpeg_destination_mgr* ) ( *cinfo->mem->alloc_small ) ( ( j_common_ptr ) cinfo, JPOOL_PERMANENT, sizeof ( degra_destination_mgr ) );
	}
	else if ( cinfo->dest->init_destination != init_stream_destination )
	{
		throw ref new Platform::FailureException ( L"MozJPEG Stream Initialize is failed." );
	}

	dest = ( degra_destination_mgr* ) cinfo->dest;
	dest->pub.init_destination = init_stream_destination;
	dest->pub.empty_output_buffer = empty_stream_output_buffer;
	dest->pub.term_destination = term_stream_destination;
	dest->stream = stream;
}
#pragma endregion

void Encode_MozJpeg_Jpeg ( IStream* stream, IWICBitmapSource* source, int quality )
{
#if defined ( _M_AMD64 ) || defined ( _M_IA32 )
	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error ( &jerr );
	jpeg_create_compress ( &cinfo );

	jpeg_stream_dest ( &cinfo, stream );

	UINT width, height;
	source->GetSize ( &width, &height );

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_EXT_BGR;

	jpeg_set_defaults ( &cinfo );

	jpeg_set_quality ( &cinfo, quality, true );

	jpeg_start_compress ( &cinfo, true );

	int row_stride = 4 * ( ( width * ( ( 24 + 7 ) / 8 ) + 3 ) / 4 );

	byte* row_pointer = new byte [ row_stride ];
	
	while ( cinfo.next_scanline < cinfo.image_height ) {

		JSAMPROW rows [ 1 ];
		rows [ 0 ] = row_pointer;

		WICRect rect = { 0, ( int ) cinfo.next_scanline, ( int ) width, 1 };
		source->CopyPixels ( &rect, row_stride, row_stride, row_pointer );

		jpeg_write_scanlines ( &cinfo, rows, 1 );
	}

	delete [] row_pointer;

	jpeg_finish_compress ( &cinfo );
	jpeg_destroy_compress ( &cinfo );
#else
	throw ref new Platform::NotImplementedException ();
#endif
}