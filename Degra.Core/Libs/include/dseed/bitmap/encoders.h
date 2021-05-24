#ifndef __DSEED_BITMAP_ENCODERS_H__
#define __DSEED_BITMAP_ENCODERS_H__

namespace dseed::bitmaps
{
#if COMPILER_MSVC
#	pragma pack (push, 1)
#else
#	pragma pack (1)
#endif
	struct DSEEDEXP png_encoder_options : public bitmap_encoder_options
	{
		png_encoder_options()
			: bitmap_encoder_options(sizeof(png_encoder_options), bitmap_encoder_options_for_png)
		{ }
	};

	struct DSEEDEXP jpeg_encoder_options : public bitmap_encoder_options
	{
		int quality;
		jpeg_encoder_options(int quality = 80)
			: bitmap_encoder_options(sizeof(jpeg_encoder_options), bitmap_encoder_options_for_jpeg)
			, quality(quality)
		{ }
	};

	struct DSEEDEXP webp_encoder_options : public bitmap_encoder_options
	{
		int quality;
		bool lossless;
		webp_encoder_options(int quality = 75, bool lossless = false)
			: bitmap_encoder_options(sizeof(webp_encoder_options), bitmap_encoder_options_for_webp)
			, quality(quality), lossless(lossless)
		{ }
	};

	enum class wic_encoder_format
	{
		bmp,
		gif,
		heif,
		jpeg,
		png,
		tiff,
		wmp,
	};

	struct DSEEDEXP wic_encoder_options : public bitmap_encoder_options
	{
		wic_encoder_format format;
		int quality;
		int compressionQuality;
		bool lossless;
		wic_encoder_options(wic_encoder_format format = wic_encoder_format::png, int quality = 80, int compressionQuality = 100, bool lossless = false)
			: bitmap_encoder_options(sizeof(wic_encoder_options), bitmap_encoder_options_for_wic)
			, format(format), quality(quality), compressionQuality(compressionQuality), lossless(lossless)
		{ }
	};
#if COMPILER_MSVC
#	pragma pack (pop)
#else
#	pragma pack ()
#endif

	using encoder_creator_func = error_t(*)(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);

	DSEEDEXP error_t create_dib_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_dds_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);

	DSEEDEXP error_t create_ico_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_cur_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);

	DSEEDEXP error_t create_png_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_jpeg_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_webp_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_gif_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
	DSEEDEXP error_t create_tiff_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);

	DSEEDEXP error_t create_wic_bitmap_encoder(dseed::io::stream* stream, const dseed::bitmaps::bitmap_encoder_options* options, dseed::bitmaps::bitmap_encoder** encoder);
}

#endif