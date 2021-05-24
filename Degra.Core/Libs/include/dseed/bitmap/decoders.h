#ifndef __DSEED_BITMAP_DECODERS_H__
#define __DSEED_BITMAP_DECODERS_H__

namespace dseed::bitmaps
{
	using decoder_creator_func = error_t(*)(dseed::io::stream*, dseed::bitmaps::bitmap_array**);

	DSEEDEXP error_t create_windows_imaging_codec_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;

	DSEEDEXP error_t create_dib_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_tga_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_ico_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_cur_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;

	DSEEDEXP error_t create_dds_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_ktx_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_pkm_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_astc_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;

	DSEEDEXP error_t create_png_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_jpeg_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_jpeg_bitmap_decoder_yuv(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_jpeg2000_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_webp_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_tiff_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
	DSEEDEXP error_t create_gif_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder) noexcept;
}

#endif