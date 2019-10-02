#ifndef __DSEED_BITMAP_BITMAP_H__
#define __DSEED_BITMAP_BITMAP_H__

namespace dseed
{
	// Bitmap Type
	enum bitmaptype_t
	{
		// 2D (width * height)
		bitmaptype_2d,
		// 2D Cube-map(width * height * 6)
		bitmaptype_2d_cubemap,
		// 3D (width * height * depth)
		bitmaptype_3d,
	};

	// Cube-map Index
	enum cubemapindex_t
	{
		cubemapindex_positive_x = 0,
		cubemapindex_negative_x = 1,
		cubemapindex_positive_y = 2,
		cubemapindex_negative_y = 3,
		cubemapindex_positive_z = 4,
		cubemapindex_negative_z = 5,
		cubemapindex_size = 6,
	};

	// Palette Object
	class DSEEDEXP palette : public object
	{
	public:
		virtual size_t size () = 0;
		virtual int bits_per_pixel () = 0;

	public:
		virtual error_t copy_palette (void* dest) = 0;

	public:
		virtual void resize (size_t size) = 0;
		virtual error_t pixels_pointer (void** ptr) = 0;
	};

	DSEEDEXP error_t create_palette (const void* pixels, int bpp, size_t pixelsCount, palette** palette);
	
	// Bitmap Object
	class DSEEDEXP bitmap : public object
	{
	public:
		virtual bitmaptype_t type () = 0;

	public:
		virtual pixelformat_t format () = 0;
		virtual size3i size () = 0;

	public:
		virtual error_t palette (palette** palette) = 0;

	public:
		virtual error_t copy_pixels_per_line (void* dest, size_t y, size_t depth = 0) = 0;
		virtual error_t copy_pixels (void* dest, size_t depth = 0) = 0;

	public:
		virtual error_t pixels_pointer_per_line (void** ptr, size_t y, size_t depth = 0) = 0;
		virtual error_t pixels_pointer (void** ptr, size_t depth = 0) = 0;

	public:
		virtual error_t read_pixels (void* buffer, const point2i& pos, const size2i& size, size_t depth = 0) = 0;
		virtual error_t write_pixels (const void* data, const point2i& pos, const size2i& size, size_t depth = 0) = 0;

	public:
		virtual error_t extra_info (attributes** attr) = 0;
	};

	DSEEDEXP error_t create_bitmap (bitmaptype_t type, const size3i& size, pixelformat_t format, palette* palette, bitmap** bitmap);

	enum frametype_t
	{
		frametype_plain,
		frametype_mipmap,
	};

	class DSEEDEXP bitmap_decoder : public object
	{
	public:
		virtual error_t decode_frame (size_t i, bitmap** bitmap, timespan_t* duration) = 0;
		virtual size_t frame_count () = 0;
		virtual frametype_t frame_type () = 0;
	};

	// Decoder Creation Function Prototype
	using createbitmapdecoder_fn = error_t (*) (dseed::stream*, bitmap_decoder**);
	// Add Bitmap Decoder to Detection Queue
	DSEEDEXP error_t add_bitmap_decoder (createbitmapdecoder_fn fn);
	// Detect Bitmap Decoder from Stream
	DSEEDEXP error_t detect_bitmap_decoder (dseed::stream* stream, dseed::bitmap_decoder** decoder);

	// Bitmap Encoder
	class DSEEDEXP bitmap_encoder : public object
	{
	public:
		virtual error_t encode_frame (bitmap* bitmap, timespan_t duration) = 0;
		virtual frametype_t frame_type () = 0;

	public:
		virtual error_t commit () = 0;
	};

	// Bitmap Encoder Options Base Object
	struct DSEEDEXP bitmap_encoder_options
	{
		size_t options_size;
	protected:
		bitmap_encoder_options (size_t options_size) : options_size (options_size) { }
	};

	// Encoder Creation Function Prototype
	using createbitmapencoder_fn = error_t (*) (dseed::stream*, const bitmap_encoder_options*, bitmap_encoder**);

	// Bitmap Pixel Reformatting
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV), Palette color, Chroma Subsampled YCbCr formats(YCbCr 4:2:2 aka YUYV, YCbCr 4:2:0 aka NV12)
	//    can be converted to each other.
	//  : Compressed color formats can be converted from/to RGBA only. (BC6H, BC7, ETC2, PVRTC, ASTC not implemented now)
	DSEEDEXP error_t reformat_bitmap (dseed::bitmap* original, dseed::pixelformat_t reformat, dseed::bitmap** bitmap);

	// Resize methods
	enum resize_t
	{
		// Nearest-Neighborhood
		resize_nearest,
		// Bilinear
		resize_bilinear,
		// Bicubic,
		resize_bicubic,
		// Lanczos
		resize_lanczos,
		// Lanczos x2
		resize_lanczos2,
		// Lanczos x3
		resize_lanczos3,
		// Lanczos x4
		resize_lanczos4,
		// Lanczos x5
		resize_lanczos5,
	};

	// Bitmap Rezie
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t resize_bitmap (dseed::bitmap* original, resize_t resize_method, const size3i& size, dseed::bitmap** bitmap);
	// Bitmap Crop
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t crop_bitmap (dseed::bitmap* original, const rectangle& area, dseed::bitmap** bitmap);

	struct DSEEDEXP bitmap_filter_mask
	{
		float mask[192];
		size_t width, height;
		bitmap_filter_mask () = default;
		bitmap_filter_mask (float* mask, size_t width, size_t height);
		bitmap_filter_mask (float mask, size_t width, size_t height);

		inline float get_mask (size_t x, size_t y) const noexcept { return mask[y * width + x]; }

		inline bitmap_filter_mask& operator*= (float factor) noexcept
		{
			for (size_t y = 0; y < height; ++y)
				for (size_t x = 0; x < width; ++x)
					mask[y * width + x] *= factor;
			return *this;
		}
		inline bitmap_filter_mask& operator/= (float factor) noexcept
		{
			for (size_t y = 0; y < height; ++y)
				for (size_t x = 0; x < width; ++x)
					mask[y * width + x] /= factor;
			return *this;
		}

		static void identity_3 (bitmap_filter_mask* mask);
		static void edge_detection_3 (bitmap_filter_mask* mask);
		static void sharpen_3 (bitmap_filter_mask* mask);
		static void gaussian_blur_3 (bitmap_filter_mask* mask);
		static void gaussian_blur_5 (bitmap_filter_mask* mask);
		static void unsharpmask_5 (bitmap_filter_mask* mask);
	};

	// Bitmap Filtering
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t filter_bitmap (dseed::bitmap* original, const bitmap_filter_mask& mask, dseed::bitmap** bitmap);

	// Bitmap Horizontal Flipping¡ê
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t flip_horizontal_bitmap (dseed::bitmap* original, dseed::bitmap** bitmap);
	// Bitmap Vertical Flipping¢Õ
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t flip_vertical_bitmap (dseed::bitmap* original, dseed::bitmap** bitmap);
	
	enum histogram_color_t
	{
		histogram_color_first,
		histogram_color_second,
		histogram_color_third,
		histogram_color_fourth,
	};

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif
	struct histogram
	{
		int histogram[256];
		size_t total_pixels;

		bool calced_table;
		int histogram_table[256];
	};
#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	// Generate Histogram from Bitmap
	DSEEDEXP error_t bitmap_generate_histogram (dseed::bitmap* original, histogram_color_t color, uint32_t depth, histogram* histogram);
	// Doing Histogram Equalization
	DSEEDEXP error_t histogram_equalization (histogram* histogram);
	// Apply Histogram to Bitmap
	DSEEDEXP error_t bitmap_apply_histogram (dseed::bitmap* original, histogram_color_t color, uint32_t depth, const histogram* histogram, dseed::bitmap** bitmap);
	// Bitmap Processing to Generate Histogram, Histogram Equalization, Apply Histogram
	DSEEDEXP error_t bitmap_auto_histogram_equalization (dseed::bitmap* original, histogram_color_t color, uint32_t depth, dseed::bitmap** bitmap);

	// Bitmap Binary Operation
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t bitmap_binary_operation (dseed::bitmap* b1, dseed::bitmap* b2, dseed::binary_operator_t op, dseed::bitmap** bitmap);
	// Bitmap Unary Operation
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t bitmap_unary_operation (dseed::bitmap* b, dseed::unary_operator_t op, dseed::bitmap** bitmap);

	enum colorcount_t : uint32_t
	{
		colorcount_unknown = 0,
		colorcount_1bpp_palettable = 2,
		colorcount_2bpp_palettable = 4,
		colorcount_4bpp_palettable = 16,
		colorcount_8bpp_palettable = 256,
		colorcount_cannot_palettable = 0xffffffff
	};

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif
	struct bitmap_properties
	{
		bool transparent;
		bool grayscale;
		colorcount_t colours;
	};
#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	DSEEDEXP error_t bitmap_determine_bitmap_properties (dseed::bitmap* bitmap, dseed::bitmap_properties* prop, int threshold = 10);

	// Detect Transparented Alpha Value from Bitmap
	DSEEDEXP error_t bitmap_detect_transparent (dseed::bitmap* bitmap, bool* transparent);
	// Check Is Bitmap Grayscale?
	DSEEDEXP error_t bitmap_detect_grayscale_bitmap (dseed::bitmap* bitmap, bool* grayscale, int threshold = 10);
	// Get Total Colours Count
	DSEEDEXP error_t bitmap_get_total_colours (dseed::bitmap* bitmap, colorcount_t* colours);
}

#include <dseed/bitmap/bitmap.decoders.h>
#include <dseed/bitmap/bitmap.encoders.h>

#endif