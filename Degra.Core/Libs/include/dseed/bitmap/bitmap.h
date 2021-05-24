#ifndef __DSEED_BITMAP_H__
#define __DSEED_BITMAP_H__

namespace dseed::bitmaps
{
	class DSEEDEXP palette : public object
	{
	public:
		virtual size_t size() noexcept = 0;
		virtual size_t bits_per_pixel() noexcept = 0;

	public:
		virtual error_t lock(void** ptr) noexcept = 0;
		virtual error_t unlock() noexcept = 0;

	public:
		virtual error_t copy_palette(void* buf) noexcept = 0;
	};

	DSEEDEXP error_t create_palette(const void* pixels, size_t bits_per_pixel, size_t size, palette** palette) noexcept;
	DSEEDEXP error_t create_palette(size_t bits_per_pixel, size_t size, palette** palette) noexcept;

	enum class bitmaptype
	{
		bitmap2d,
		bitmap2dcube,
		bitmap3d,
	};

	enum class cubemapindex
	{
		positive_x = 0,
		negative_x = 1,
		positive_y = 2,
		negative_y = 3,
		positive_z = 4,
		negative_z = 5,
	};

	class DSEEDEXP bitmap : public object
	{
	public:
		virtual bitmaptype type() noexcept = 0;
		virtual size3i size() noexcept = 0;
		virtual color::pixelformat format() noexcept = 0;

	public:
		virtual error_t palette(palette** palette) noexcept = 0;

	public:
		virtual error_t lock(void** ptr) noexcept = 0;
		virtual error_t unlock() noexcept = 0;

	public:
		virtual error_t copy_pixels(void* dest, size_t depth) = 0;

	public:
		virtual error_t read_pixels(const rect2i& area, void* ptr, size_t depth = 0) noexcept = 0;
		virtual error_t write_pixels(const rect2i& area, const void* ptr, size_t depth = 0) noexcept = 0;

	public:
		virtual error_t extra_info(attributes** attr) noexcept = 0;
	};

	DSEEDEXP error_t create_bitmap(const void* pixels, bitmaptype type, const size3i& size, color::pixelformat format, palette* palette, bitmap** bitmap) noexcept;
	DSEEDEXP error_t create_bitmap(bitmaptype type, const size3i& size, color::pixelformat format, palette* palette, bitmap** bitmap) noexcept;

	enum class arraytype
	{
		plain,
		mipmap,
	};

	class DSEEDEXP bitmap_array : public object
	{
	public:
		virtual arraytype type() noexcept = 0;
		virtual size_t size() noexcept = 0;

	public:
		virtual error_t at(size_t index, dseed::bitmaps::bitmap** bitmap) noexcept = 0;
	};

	DSEEDEXP error_t create_bitmap_array(arraytype type, size_t size, bitmap** bitmaps, bitmap_array** arr) noexcept;

	enum
	{
		bitmap_encoder_options_for_png,
		bitmap_encoder_options_for_jpeg,
		bitmap_encoder_options_for_webp,
		bitmap_encoder_options_for_wic,
	};

	struct DSEEDEXP bitmap_encoder_options
	{
		size_t options_size;
		size_t option_type;

	protected:
		bitmap_encoder_options(size_t options_size, size_t option_type) noexcept
			: options_size(options_size)
			, option_type(option_type)
		{ }
	};

	class DSEEDEXP bitmap_encoder : public object
	{
	public:
		virtual arraytype type() noexcept = 0;

	public:
		virtual error_t encode_frame(bitmap* bitmap) noexcept = 0;
		virtual error_t commit() noexcept = 0;
	};
}

namespace dseed::bitmaps
{
	enum class colorcount : uint32_t
	{
		unknown = 0,
		color_1bpp_palettable = 2,
		color_2bpp_palettable = 4,
		color_4bpp_palettable = 16,
		color_8bpp_palettable = 256,
		color_cannot_palettable = 0xffffffff
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
		colorcount colours;
	};
#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	DSEEDEXP error_t determine_bitmap_properties(bitmap* bitmap, bitmap_properties* prop, int threshold = 10);

	// Detect Transparented Alpha Value from Bitmap
	DSEEDEXP error_t detect_transparent(bitmap* bitmap, bool* transparent);
	// Check Is Bitmap Grayscale?
	DSEEDEXP error_t detect_grayscale_bitmap(bitmap* bitmap, bool* grayscale, int threshold = 10);
	// Get Total Colours Count
	DSEEDEXP error_t get_total_colours(bitmap* bitmap, colorcount* colours);
}

namespace dseed::bitmaps
{
	// Bitmap Pixel Reformatting
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV), Palette color, Chroma Subsampled YCbCr formats(YCbCr 4:2:2 aka YUYV, YCbCr 4:2:0 aka NV12)
	//    can be converted to each other.
	//  : Compressed color formats can be converted from/to RGBA only. (BC6H, BC7, ETC2, PVRTC, ASTC not implemented now)
	DSEEDEXP error_t reformat_bitmap(bitmap* original, dseed::color::pixelformat reformat, bitmap** bitmap);

	// Resize methods
	enum class resize
	{
		// Nearest-Neighborhood
		nearest,
		// Bilinear
		bilinear,
		// Bicubic,
		bicubic,
		// Lanczos
		lanczos,
		// Lanczos x2
		lanczos2,
		// Lanczos x3
		lanczos3,
		// Lanczos x4
		lanczos4,
		// Lanczos x5
		lanczos5,
	};

	// Bitmap Rezie
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t resize_bitmap(bitmap* original, resize resize_method, const size3i& size, bitmap** bitmap);
	// Bitmap Crop
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t crop_bitmap(bitmap* original, const rect2i& area, bitmap** bitmap);

	struct DSEEDEXP bitmap_filter_mask
	{
		float mask[192];
		size_t width, height;
		bitmap_filter_mask() = default;
		bitmap_filter_mask(float* mask, size_t width, size_t height);
		bitmap_filter_mask(float mask, size_t width, size_t height);

		inline float get_mask(size_t x, size_t y) const noexcept { return mask[y * width + x]; }

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

		static void identity_3(bitmap_filter_mask* mask);
		static void edge_detection_3(bitmap_filter_mask* mask);
		static void sharpen_3(bitmap_filter_mask* mask);
		static void gaussian_blur_3(bitmap_filter_mask* mask);
		static void gaussian_blur_5(bitmap_filter_mask* mask);
		static void unsharpmask_5(bitmap_filter_mask* mask);
	};

	// Bitmap Filtering
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t filter_bitmap(bitmap* original, const bitmap_filter_mask& mask, bitmap** bitmap);

	// Bitmap Horizontal Flipping¡ê
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t flip_horizontal_bitmap(bitmap* original, bitmap** bitmap);
	// Bitmap Vertical Flipping¢Õ
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t flip_vertical_bitmap(bitmap* original, bitmap** bitmap);

	enum class histogram_color
	{
		first,
		second,
		third,
		fourth,
	};

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif
	struct histogram
	{
		int histogram_data[256];
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
	DSEEDEXP error_t bitmap_generate_histogram(bitmap* original, histogram_color color, uint32_t depth, histogram* histogram);
	// Doing Histogram Equalization
	DSEEDEXP error_t histogram_equalization(histogram* histogram);
	// Apply Histogram to Bitmap
	DSEEDEXP error_t bitmap_apply_histogram(bitmap* original, histogram_color color, uint32_t depth, const histogram* histogram, bitmap** bitmap);
	// Bitmap Processing to Generate Histogram, Histogram Equalization, Apply Histogram
	DSEEDEXP error_t bitmap_auto_histogram_equalization(bitmap* original, histogram_color color, uint32_t depth, bitmap** bitmap);

	// Bitmap Binary Operation
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t bitmap_binary_operation(bitmap* b1, bitmap* b2, binary_operator op, bitmap** bitmap);
	// Bitmap Unary Operation
	//  : RGBA, RGB, BGRA, BGR, Grayscale, YCbCr(YUV, 4:4:4) only support.
	DSEEDEXP error_t bitmap_unary_operation(bitmap* b, unary_operator op, bitmap** bitmap);
}

#include "decoders.h"
#include "encoders.h"

namespace dseed::bitmaps
{
	// Decoder Creation Function Prototype
	using createbitmapdecoder_fn = error_t(*) (dseed::io::stream*, dseed::bitmaps::bitmap_array**);
	// Add Bitmap Decoder to Detection Queue
	DSEEDEXP error_t add_bitmap_decoder(createbitmapdecoder_fn fn);
	// Detect Bitmap Decoder from Stream
	DSEEDEXP error_t detect_bitmap_decoder(dseed::io::stream* stream, dseed::bitmaps::bitmap_array** decoder);
}

#endif