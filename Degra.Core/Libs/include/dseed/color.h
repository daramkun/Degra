#ifndef __DSEED_COLOR_H__
#define __DSEED_COLOR_H__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Pixel Format
	//
	////////////////////////////////////////////////////////////////////////////////////////
	enum pixelformat_t : int32_t
	{
		// Unknown Pixel Format
		pixelformat_unknown = 0x00000000,

		// 8-bit Grayscale Pixel Format
		pixelformat_grayscale8 = 0x00010001,
		// 32-bit Floating-Point Grayscale Pixel Format
		pixelformat_grayscalef = 0x00010004,
		// 32-bit Integer RGBA Pixel Format
		pixelformat_rgba8888 = 0x00010104,
		// 128-bit Floating-Point RGBA Pixel Format
		pixelformat_rgbaf = 0x00010110,
		// 24-bit RGB Pixel Format
		pixelformat_rgb888 = 0x00010103,
		// 32-bit Integer BGRA Pixel Format
		pixelformat_bgra8888 = 0x00010204,
		// 32-bit Integer BGRA Pixel Format
		pixelformat_bgr888 = 0x00010203,
		// 16-bit Integer BGRA Pixel Format
		pixelformat_bgra4444 = 0x00010202,

		// 16-bit Integer BGR Pixel Format(Legacy)
		pixelformat_bgr565 = 0x00010302,

		// 8-bit Indexed BGRA Pixel Format
		pixelformat_bgra8888_indexed8 = 0x00020104,
		// 8-bit Indexed BGR Pixel Format
		pixelformat_bgr888_indexed8 = 0x00020103,

		// 24-bit YCbCr:4:4:4 Pixel Format
		pixelformat_yuv888 = 0x00030003,
		// 32-bit YCbCr:4:4:4 and Alpha Pixel Format
		pixelformat_yuva8888 = 0x00030004,

		// 32-bit YCbCr:4:2:2(YUYV or YUY2) Packed Pixel Format
		pixelformat_yuyv8888 = 0x00040204,
		// YCbCr:4:2:0(NV12) Packed Pixel Format
		pixelformat_yyyyuv888888 = 0x00040406,
		pixelformat_nv12 = pixelformat_yyyyuv888888,

		// BC1(DXT1) Compressed Pixel Format
		pixelformat_bc1 = 0x00100001, pixelformat_dxt1 = pixelformat_bc1,
		// BC2(DXT3) Compressed Pixel Format
		pixelformat_bc2 = 0x00100002, pixelformat_dxt3 = pixelformat_bc2,
		// BC3(DXT5) Compressed Pixel Format
		pixelformat_bc3 = 0x00100003, pixelformat_dxt5 = pixelformat_bc3,
		// BC4(ATI1) Compressed Pixel Format
		pixelformat_bc4 = 0x00100004, pixelformat_ati1 = pixelformat_bc4,
		// BC5(ATI2) Compressed Pixel Format
		pixelformat_bc5 = 0x00100005, pixelformat_ati2 = pixelformat_bc5,
		// BC6 Compressed Pixel Format(Only support Signed-bit Format now)
		pixelformat_bc6h = 0x00100006,
		// BC7 Compressed Pixel Format
		pixelformat_bc7 = 0x00100007,

		// ETC Compressed Pixel Format
		pixelformat_etc1 = 0x00110001,
		// ETC2 Compressed Pixel Format
		pixelformat_etc2 = 0x00110002,
		// ETC2 with Alpha Compressed Pixel Format
		pixelformat_etc2a = 0x00110102,
		// ETC2 with Dedicated Alpha Compressed Pixel Format
		pixelformat_etc2a8 = 0x00110103,

		// PVRTC Compressed Pixel Formats
		pixelformat_pvrtc_2bpp = 0x00120001,
		pixelformat_pvrtc_2abpp = 0x00120101,
		pixelformat_pvrtc_4bpp = 0x00120002,
		pixelformat_pvrtc_4abpp = 0x00120102,
		// PVRTC2 Compressed Pixel Formats
		pixelformat_pvrtc2_2bpp = 0x00120003,
		pixelformat_pvrtc2_4bpp = 0x00120004,

		// ASTC Compressed Pixel Formats
		pixelformat_astc_4x4 = 0x00130001,
		pixelformat_astc_5x4 = 0x00130002,
		pixelformat_astc_5x5 = 0x00130003,
		pixelformat_astc_6x5 = 0x00130004,
		pixelformat_astc_6x6 = 0x00130005,
		pixelformat_astc_8x5 = 0x00130006,
		pixelformat_astc_8x6 = 0x00130007,
		pixelformat_astc_8x8 = 0x00130008,
		pixelformat_astc_10x5 = 0x00130009,
		pixelformat_astc_10x6 = 0x0013000a,
		pixelformat_astc_10x8 = 0x0013000b,
		pixelformat_astc_10x10 = 0x0013000c,
		pixelformat_astc_12x10 = 0x0013000d,
		pixelformat_astc_12x12 = 0x0013000e,

		// Depth-Stencil Pixel Formats (Not support in dseed Bitmap object, Only for Texture objects)
		pixelformat_depth16 = 0x01000002,
		pixelformat_depth24stencil8 = 0x01000104,
		pixelformat_depth32 = 0x01000004,
	};

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Types Prototypes
	//
	////////////////////////////////////////////////////////////////////////////////////////

	struct rgba;
	struct rgb;
	struct rgbaf;
	struct bgra;
	struct bgr;
	struct bgra4;
	struct bgr565;

	struct grayscale;
	struct grayscalef;

	struct yuva;
	struct yuv;

	struct color_processor;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Utility Procedures
	//
	////////////////////////////////////////////////////////////////////////////////////////

	// Get Stride value
	DSEEDEXP size_t get_bitmap_stride (pixelformat_t format, int32_t width) noexcept;
	// Get Bitmap Plane size
	DSEEDEXP size_t get_bitmap_plane_size (pixelformat_t format, int32_t width, int32_t height) noexcept;
	// Get Total Bitmap Planes Buffer size
	DSEEDEXP size_t get_bitmap_total_size (pixelformat_t format, const dseed::size3i& size) noexcept;
	// Get Mipmap Size
	DSEEDEXP dseed::size3i get_mipmap_size (int mipLevel, const dseed::size3i& size, bool cubemap) noexcept;
	// Get Maximum Mip-Levels
	DSEEDEXP size_t get_maximum_mip_levels (const dseed::size3i& size, bool cubemap) noexcept;
	// Saturate Integer color value
	constexpr uint8_t saturate8 (int32_t v) noexcept { return (uint8_t)maximum (0, minimum (255, v)); }
	constexpr uint8_t saturate4 (int32_t v) noexcept { return (uint8_t)maximum (0, minimum (15, v)); }
	constexpr uint8_t saturate5 (int32_t v) noexcept { return (uint8_t)maximum (0, minimum (31, v)); }
	constexpr uint8_t saturate6 (int32_t v) noexcept { return (uint8_t)maximum (0, minimum (63, v)); }
	// Saturate Floating point color value
	constexpr float saturatef (float v) noexcept { return maximum (0.000f, minimum (1.000f, v)); }
	// Saturate Custom color value
	template<class T, T min, T max>
	constexpr T saturate (T v) noexcept { return maximum<T> (min, minimum<T> (max, v)); }

	// https://stackoverflow.com/questions/1737726/how-to-perform-rgb-yuv-conversion-in-c-c , Modified
	// RGB -> YUV
	constexpr int32_t add128shift8 (int32_t v) noexcept { return (v + 128) >> 8; }
	constexpr uint8_t rgb2y (int32_t r, int32_t g, int32_t b) noexcept { return dseed::saturate8 (add128shift8 ((66 * r) + (129 * g) + (25 * b)) + 16); }
	constexpr uint8_t rgb2u (int32_t r, int32_t g, int32_t b) noexcept { return dseed::saturate8 (add128shift8 ((-38 * r) - (74 * g) + (112 * b)) + 128); }
	constexpr uint8_t rgb2v (int32_t r, int32_t g, int32_t b) noexcept { return dseed::saturate8 (add128shift8 ((112 * r) - (94 * g) - (18 * b)) + 128); }
	// YUV -> RGB
	constexpr int32_t __c (int32_t y) noexcept { return y - 16; }
	constexpr int32_t __d (int32_t u) noexcept { return u - 128; }
	constexpr int32_t __e (int32_t v) noexcept { return v - 128; }
	constexpr uint8_t yuv2r (int32_t y, int32_t u, int32_t v) noexcept { return dseed::saturate8 (add128shift8 ((298 * __c (y)) + (409 * __e (v)))); }
	constexpr uint8_t yuv2g (int32_t y, int32_t u, int32_t v) noexcept { return dseed::saturate8 (add128shift8 ((298 * __c (y)) - (100 * __d (u)) - (208 * __e (v)))); }
	constexpr uint8_t yuv2b (int32_t y, int32_t u, int32_t v) noexcept { return dseed::saturate8 (add128shift8 ((298 * __c (y)) + (516 * __d (u)))); }

	template<class color_t> constexpr pixelformat_t type2format () noexcept { static_assert (true, "Not support type."); return pixelformat_unknown; }
	template<> constexpr pixelformat_t type2format<rgba> () noexcept { return pixelformat_rgba8888; }
	template<> constexpr pixelformat_t type2format<rgb> () noexcept { return pixelformat_rgb888; }
	template<> constexpr pixelformat_t type2format<rgbaf> () noexcept { return pixelformat_rgbaf; }
	template<> constexpr pixelformat_t type2format<bgra> () noexcept { return pixelformat_bgra8888; }
	template<> constexpr pixelformat_t type2format<bgr> () noexcept { return pixelformat_bgr888; }
	template<> constexpr pixelformat_t type2format<bgra4> () noexcept { return pixelformat_bgra4444; }
	template<> constexpr pixelformat_t type2format<bgr565> () noexcept { return pixelformat_bgr565; }
	template<> constexpr pixelformat_t type2format<grayscale> () noexcept { return pixelformat_grayscale8; }
	template<> constexpr pixelformat_t type2format<grayscalef> () noexcept { return pixelformat_grayscalef; }
	template<> constexpr pixelformat_t type2format<yuva> () noexcept { return pixelformat_yuva8888; }
	template<> constexpr pixelformat_t type2format<yuv> () noexcept { return pixelformat_yuv888; }

	template<class color_t> constexpr pixelformat_t type2indexedformat () noexcept { static_assert (true, "Not support type."); return pixelformat_unknown; }
	template<> constexpr pixelformat_t type2indexedformat<bgra> () noexcept { return pixelformat_bgra8888_indexed8; }
	template<> constexpr pixelformat_t type2indexedformat<bgr> () noexcept { return pixelformat_bgr888_indexed8; }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Red/Green/Blue Color Types
	//
	////////////////////////////////////////////////////////////////////////////////////////

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif

	struct rgba
	{
		union
		{
			struct { uint8_t r, g, b, a; };
			uint32_t color;
		};
		rgba () noexcept = default;
		inline rgba (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
			: r (r), g (g), b (b), a (a)
		{ }
		inline rgba (uint32_t rgba) noexcept
			: color (rgba)
		{ }

		static inline rgba max_color () noexcept { return rgba (255, 255, 255, 255); }
		static inline rgba min_color () noexcept { return rgba (0, 0, 0, 0); }

		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline rgba& operator+= (const rgba& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline rgba& operator-= (const rgba& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline rgba& operator*= (const rgba& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline rgba& operator/= (const rgba& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline rgba operator+ (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline rgba operator- (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline rgba operator* (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline rgba operator/ (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline rgba operator* (const rgba& c1, double factor) noexcept { return rgba ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline rgba operator/ (const rgba& c1, double factor) noexcept { return rgba ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline rgba operator& (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline rgba operator| (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline rgba operator^ (const rgba& c1, const rgba& c2) noexcept { return rgba (c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b, c1.a ^ c2.a); }
	inline rgba operator~ (const rgba& c) noexcept { return rgba (~c.r, ~c.g, ~c.b, ~c.a); }

	struct rgb
	{
		union
		{
			struct { uint8_t r, g, b; };
			uint24_t color;
		};
		rgb () noexcept = default;
		inline rgb (uint8_t r, uint8_t g, uint8_t b) noexcept
			: r (r), g (g), b (b)
		{ }
		inline rgb (uint24_t rgb) noexcept
			: color (rgb)
		{ }

		static inline rgb max_color () noexcept { return rgb (255, 255, 255); }
		static inline rgb min_color () noexcept { return rgb (0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline rgb& operator+= (const rgb& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline rgb& operator-= (const rgb& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline rgb& operator*= (const rgb& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline rgb& operator/= (const rgb& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline rgb operator+ (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline rgb operator- (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline rgb operator* (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline rgb operator/ (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline rgb operator* (const rgb& c1, double factor) noexcept { return rgb ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline rgb operator/ (const rgb& c1, double factor) noexcept { return rgb ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline rgb operator& (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline rgb operator| (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline rgb operator^ (const rgb& c1, const rgb& c2) noexcept { return rgb (c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b); }
	inline rgb operator~ (const rgb& c) noexcept { return rgb (~c.r, ~c.g, ~c.b); }

	struct rgbaf
	{
		union
		{
			struct { float r, g, b, a; };
			float4 color;
		};
		rgbaf () noexcept = default;
		inline rgbaf (float r, float g, float b, float a = 1.00000000f) noexcept
			: r (r), g (g), b (b), a (a)
		{ }
		inline rgbaf (const float4& rgba) noexcept
			: color (rgba)
		{ }

		static inline rgbaf max_color () noexcept { return rgbaf (1, 1, 1, 1); }
		static inline rgbaf min_color () noexcept { return rgbaf (0, 0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;
		inline operator float4 () const noexcept { return color; }

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline rgbaf& operator+= (const rgbaf& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline rgbaf& operator-= (const rgbaf& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline rgbaf& operator*= (const rgbaf& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline rgbaf& operator/= (const rgbaf& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline rgbaf operator+ (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline rgbaf operator- (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline rgbaf operator- (const rgbaf& c) noexcept { return rgbaf (-c.r, -c.g, -c.b, -c.a); }
	inline rgbaf operator* (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline rgbaf operator/ (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline rgbaf operator* (const rgbaf& c1, double factor) noexcept { return rgbaf ((float)(c1.r * factor), (float)(c1.g * factor), (float)(c1.b * factor), (float)(c1.a * factor)); }
	inline rgbaf operator/ (const rgbaf& c1, double factor) noexcept { return rgbaf ((float)(c1.r / factor), (float)(c1.g / factor), (float)(c1.b / factor), (float)(c1.a / factor)); }
	
	struct bgra
	{
		union
		{
			struct { uint8_t b, g, r, a; };
			uint32_t color;
		};
		bgra () noexcept = default;
		inline bgra (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
			: r (r), g (g), b (b), a (a)
		{ }
		inline bgra (uint32_t bgra) noexcept
			: color (bgra)
		{ }

		static inline bgra max_color () noexcept { return bgra (255, 255, 255, 255); }
		static inline bgra min_color () noexcept { return bgra (0, 0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline bgra& operator+= (const bgra& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline bgra& operator-= (const bgra& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline bgra& operator*= (const bgra& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline bgra& operator/= (const bgra& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline bgra operator+ (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline bgra operator- (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline bgra operator* (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline bgra operator/ (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline bgra operator* (const bgra& c1, double factor) noexcept { return bgra ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline bgra operator/ (const bgra& c1, double factor) noexcept { return bgra ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline bgra operator& (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline bgra operator| (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline bgra operator^ (const bgra& c1, const bgra& c2) noexcept { return bgra (c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b, c1.a ^ c2.a); }
	inline bgra operator~ (const bgra& c) noexcept { return bgra (~c.r, ~c.g, ~c.b, ~c.a); }

	struct bgr
	{
		union
		{
			struct { uint8_t b, g, r; };
			uint24_t color;
		};
		bgr () noexcept = default;
		inline bgr (uint8_t r, uint8_t g, uint8_t b) noexcept
			: r (r), g (g), b (b)
		{ }
		inline bgr (uint24_t bgr) noexcept
			: color (bgr)
		{ }

		static inline bgr max_color () noexcept { return bgr (255, 255, 255); }
		static inline bgr min_color () noexcept { return bgr (0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline bgr& operator+= (const bgr& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline bgr& operator-= (const bgr& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline bgr& operator*= (const bgr& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline bgr& operator/= (const bgr& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline bgr operator+ (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline bgr operator- (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline bgr operator* (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline bgr operator/ (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline bgr operator* (const bgr& c1, double factor) noexcept { return bgr ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline bgr operator/ (const bgr& c1, double factor) noexcept { return bgr ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline bgr operator& (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline bgr operator| (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline bgr operator^ (const bgr& c1, const bgr& c2) noexcept { return bgr (c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b); }
	inline bgr operator~ (const bgr& c) noexcept { return bgr (~c.r, ~c.g, ~c.b); }

	struct bgra4
	{
		union
		{
			struct { uint8_t b : 4, g : 4, r : 4, a : 4; };
			uint16_t color;
		};
		bgra4 () noexcept = default;
		inline bgra4 (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 15) noexcept
			: r (r), g (g), b (b), a (a)
		{ }
		inline bgra4 (uint16_t bgra) noexcept
			: color (bgra)
		{ }

		static inline bgra4 max_color () noexcept { return bgra4 (15, 15, 15, 15); }
		static inline bgra4 min_color () noexcept { return bgra4 (0, 0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline bgra4& operator+= (const bgra4& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline bgra4& operator-= (const bgra4& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline bgra4& operator*= (const bgra4& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline bgra4& operator/= (const bgra4& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline bgra4 operator+ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline bgra4 operator- (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline bgra4 operator* (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline bgra4 operator/ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline bgra4 operator* (const bgra4& c1, double factor) noexcept { return bgra4 ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline bgra4 operator/ (const bgra4& c1, double factor) noexcept { return bgra4 ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline bgra4 operator& (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline bgra4 operator| (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline bgra4 operator^ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4 ((c1.r ^ c2.r) & 0xf, (c1.g ^ c2.g) & 0xf, (c1.b ^ c2.b) & 0xf, (c1.a ^ c2.a) & 0xf); }
	inline bgra4 operator~ (const bgra4& c) noexcept { return bgra4 ((~c.r) & 0xf, (~c.g) & 0xf, (~c.b) & 0xf, (~c.a) & 0xf); }

	struct bgr565
	{
		union
		{
			struct { uint8_t b : 5, g : 6, r : 5; };
			uint16_t color;
		};
		bgr565 () noexcept = default;
		inline bgr565 (uint8_t r, uint8_t g, uint8_t b) noexcept
			: r (r), g (g), b (b)
		{ }
		inline bgr565 (uint16_t bgr) noexcept
			: color (bgr)
		{ }

		static inline bgr565 max_color () noexcept { return bgr565 (31, 63, 31); }
		static inline bgr565 min_color () noexcept { return bgr565 (0, 0, 0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline bgr565& operator+= (const bgr565& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline bgr565& operator-= (const bgr565& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline bgr565& operator*= (const bgr565& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline bgr565& operator/= (const bgr565& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline bgr565 operator+ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline bgr565 operator- (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline bgr565 operator* (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline bgr565 operator/ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline bgr565 operator* (const bgr565& c1, double factor) noexcept { return bgr565 ((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline bgr565 operator/ (const bgr565& c1, double factor) noexcept { return bgr565 ((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline bgr565 operator& (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline bgr565 operator| (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 (c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline bgr565 operator^ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565 ((c1.r ^ c2.r) & 0x1f, (c1.g ^ c2.g) & 0x3f, (c1.b ^ c2.b) & 0x1f); }
	inline bgr565 operator~ (const bgr565& c) noexcept { return bgr565 ((~c.r) & 0x1f, (~c.g) & 0x3f, (~c.b) & 0x1f); }

#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Grayscale Color Types
	//
	////////////////////////////////////////////////////////////////////////////////////////

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif

	struct grayscale
	{
		uint8_t color;
		grayscale () noexcept = default;
		inline grayscale (uint8_t grayscale) noexcept : color (grayscale) { }
		inline grayscale (const rgb& rgb) noexcept
			: color ((uint8_t)maximum (0.0, minimum (255.0, +0.2627 * rgb.r + +0.678 * rgb.g + +0.0593 * rgb.b)))
		{ }

		static inline grayscale max_color () noexcept { return grayscale (255); }
		static inline grayscale min_color () noexcept { return grayscale (0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline grayscale& operator+= (const grayscale& c) noexcept { color += c.color; return *this; }
		inline grayscale& operator-= (const grayscale& c) noexcept { color -= c.color; return *this; }
		inline grayscale& operator*= (const grayscale& c) noexcept { color *= c.color; return *this; }
		inline grayscale& operator/= (const grayscale& c) noexcept { color /= c.color; return *this; }
	};
	inline grayscale operator+ (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color + c2.color); }
	inline grayscale operator- (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color - c2.color); }
	inline grayscale operator* (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color * c2.color); }
	inline grayscale operator/ (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color / c2.color); }
	inline grayscale operator* (const grayscale& c1, double factor) noexcept { return grayscale ((uint8_t)(c1.color * factor)); }
	inline grayscale operator/ (const grayscale& c1, double factor) noexcept { return grayscale ((uint8_t)(c1.color / factor)); }
	inline grayscale operator& (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color & c2.color); }
	inline grayscale operator| (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color | c2.color); }
	inline grayscale operator^ (const grayscale& c1, const grayscale& c2) noexcept { return grayscale (c1.color ^ c2.color); }
	inline grayscale operator~ (const grayscale& c) noexcept { return grayscale (~c.color); }

	struct grayscalef
	{
		float color;
		grayscalef () noexcept = default;
		inline grayscalef (float grayscale) : color (grayscale) { }
		inline grayscalef (const rgbaf& rgbaf)
			: color ((float)maximum (0.0, minimum (1.0, (+0.2627 * rgbaf.r + +0.678 * rgbaf.g + +0.0593 * rgbaf.b))))
		{ }

		static inline grayscalef max_color () noexcept { return grayscalef (1); }
		static inline grayscalef min_color () noexcept { return grayscalef (0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;

		inline operator yuv () const noexcept;
		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline grayscalef& operator+= (const grayscalef& c) noexcept { color += c.color; return *this; }
		inline grayscalef& operator-= (const grayscalef& c) noexcept { color -= c.color; return *this; }
		inline grayscalef& operator*= (const grayscalef& c) noexcept { color *= c.color; return *this; }
		inline grayscalef& operator/= (const grayscalef& c) noexcept { color /= c.color; return *this; }
	};
	inline grayscalef operator+ (const grayscalef& c1, const grayscalef& c2) noexcept { return grayscalef (c1.color + c2.color); }
	inline grayscalef operator- (const grayscalef& c1, const grayscalef& c2) noexcept { return grayscalef (c1.color - c2.color); }
	inline grayscalef operator- (const grayscalef& c) noexcept { return grayscalef (-c.color); }
	inline grayscalef operator* (const grayscalef& c1, const grayscalef& c2) noexcept { return grayscalef (c1.color * c2.color); }
	inline grayscalef operator/ (const grayscalef& c1, const grayscalef& c2) noexcept { return grayscalef (c1.color / c2.color); }
	inline grayscalef operator* (const grayscalef& c1, double factor) noexcept { return grayscalef ((float)(c1.color * factor)); }
	inline grayscalef operator/ (const grayscalef& c1, double factor) noexcept { return grayscalef ((float)(c1.color / factor)); }

#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Y/Cb(U)/Cr(V) Color Types
	//
	////////////////////////////////////////////////////////////////////////////////////////

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif

	struct yuva
	{
		union
		{
			struct { uint8_t v, u, y, a; };
			uint32_t color;
		};
		yuva () = default;
		inline yuva (uint8_t y, uint8_t u, uint8_t v, uint8_t a = 255) noexcept
			: y (y), u (u), v (v), a (a)
		{ }
		inline yuva (uint32_t yuva) noexcept
			: color (yuva)
		{ }

		static inline yuva max_color () noexcept { return yuva (255, 255, 255, 255); }
		static inline yuva min_color () noexcept { return yuva (0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuv () const noexcept;

		inline operator color_processor () const noexcept;

		inline yuva& operator+= (const yuva& c) noexcept { y += c.y; u += c.u; v += c.v; a += c.a; return *this; }
		inline yuva& operator-= (const yuva& c) noexcept { y -= c.y; u -= c.u; v -= c.v; a -= c.a; return *this; }
		inline yuva& operator*= (const yuva& c) noexcept { y *= c.y; u *= c.u; v *= c.v; a *= c.a; return *this; }
		inline yuva& operator/= (const yuva& c) noexcept { y /= c.y; u /= c.u; v /= c.v; a /= c.a; return *this; }
	};
	inline yuva operator+ (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y + c2.y, c1.u + c2.u, c1.v + c2.v, c1.a + c2.a); }
	inline yuva operator- (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y - c2.y, c1.u - c2.u, c1.v - c2.v, c1.a - c2.a); }
	inline yuva operator* (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y * c2.y, c1.u * c2.u, c1.v * c2.v, c1.a * c2.a); }
	inline yuva operator/ (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y / c2.y, c1.u / c2.u, c1.v / c2.v, c1.a / c2.a); }
	inline yuva operator* (const yuva& c1, double factor) noexcept { return yuva ((uint8_t)(c1.y * factor), (uint8_t)(c1.u * factor), (uint8_t)(c1.v * factor), (uint8_t)(c1.a * factor)); }
	inline yuva operator/ (const yuva& c1, double factor) noexcept { return yuva ((uint8_t)(c1.y / factor), (uint8_t)(c1.u / factor), (uint8_t)(c1.v / factor), (uint8_t)(c1.a / factor)); }
	inline yuva operator& (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y & c2.y, c1.u & c2.u, c1.v & c2.v, c1.a & c2.a); }
	inline yuva operator| (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y | c2.y, c1.u | c2.u, c1.v | c2.v, c1.a | c2.a); }
	inline yuva operator^ (const yuva& c1, const yuva& c2) noexcept { return yuva (c1.y ^ c2.y, c1.u ^ c2.u, c1.v ^ c2.v, c1.a ^ c2.a); }
	inline yuva operator~ (const yuva& c) noexcept { return yuva (~c.y, ~c.u, ~c.v, ~c.a); }

	struct yuv
	{
		union
		{
			struct { uint8_t y, u, v; };
			uint24_t color;
		};
		yuv () noexcept = default;
		inline yuv (uint8_t y, uint8_t u, uint8_t v) noexcept
			: y (y), u (u), v (v)
		{ }
		inline yuv (uint24_t yuv) noexcept
			: color (yuv)
		{ }

		static inline yuv max_color () noexcept { return yuv (255, 255, 255); }
		static inline yuv min_color () noexcept { return yuv (0); }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuva () const noexcept;

		inline operator color_processor () const noexcept;

		inline yuv& operator+= (const yuv& c) noexcept { y += c.y; u += c.u; v += c.v; return *this; }
		inline yuv& operator-= (const yuv& c) noexcept { y -= c.y; u -= c.u; v -= c.v; return *this; }
		inline yuv& operator*= (const yuv& c) noexcept { y *= c.y; u *= c.u; v *= c.v; return *this; }
		inline yuv& operator/= (const yuv& c) noexcept { y /= c.y; u /= c.u; v /= c.v; return *this; }
	};
	inline yuv operator+ (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y + c2.y, c1.u + c2.u, c1.v + c2.v); }
	inline yuv operator- (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y - c2.y, c1.u - c2.u, c1.v - c2.v); }
	inline yuv operator* (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y * c2.y, c1.u * c2.u, c1.v * c2.v); }
	inline yuv operator/ (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y / c2.y, c1.u / c2.u, c1.v / c2.v); }
	inline yuv operator* (const yuv& c1, double factor) noexcept { return yuv ((uint8_t)(c1.y * factor), (uint8_t)(c1.u * factor), (uint8_t)(c1.v * factor)); }
	inline yuv operator/ (const yuv& c1, double factor) noexcept { return yuv ((uint8_t)(c1.y / factor), (uint8_t)(c1.u / factor), (uint8_t)(c1.v / factor)); }
	inline yuv operator& (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y & c2.y, c1.u & c2.u, c1.v & c2.v); }
	inline yuv operator| (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y | c2.y, c1.u | c2.u, c1.v | c2.v); }
	inline yuv operator^ (const yuv& c1, const yuv& c2) noexcept { return yuv (c1.y ^ c2.y, c1.u ^ c2.u, c1.v ^ c2.v); }
	inline yuv operator~ (const yuv& c) noexcept { return yuv (~c.y, ~c.u, ~c.v); }

#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Color Processing Unit
	//
	////////////////////////////////////////////////////////////////////////////////////////

	struct color_processor
	{
		float a, b, c, d;
		color_processor () noexcept : a (0), b (0), c (0), d (0) { }
		color_processor (float a, float b, float c, float d) noexcept
			: a (a), b (b), c (c), d (d) { }

		inline operator rgba () const noexcept;
		inline operator rgb () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra () const noexcept;
		inline operator bgr () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator grayscale () const noexcept;
		inline operator grayscalef () const noexcept;

		inline operator yuva () const noexcept;
		inline operator yuv () const noexcept;

		inline color_processor& operator+= (const color_processor& cp) noexcept { a += cp.a; b += cp.b; c += cp.c; d += cp.d; return *this; }
		inline color_processor& operator-= (const color_processor& cp) noexcept { a -= cp.a; b -= cp.b; c -= cp.c; d -= cp.d; return *this; }
		inline color_processor& operator*= (const color_processor& cp) noexcept { a *= cp.a; b *= cp.b; c *= cp.c; d *= cp.d; return *this; }
		inline color_processor& operator/= (const color_processor& cp) noexcept { a /= cp.a; b /= cp.b; c /= cp.c; d /= cp.d; return *this; }

		inline void restore_alpha (const rgba& oc) noexcept { d = (float)oc.a; }
		inline void restore_alpha (const rgb& oc) noexcept { }
		inline void restore_alpha (const rgbaf& oc) noexcept { d = oc.a; }
		inline void restore_alpha (const bgra& oc) noexcept { d = (float)oc.a; }
		inline void restore_alpha (const bgr& oc) noexcept { }
		inline void restore_alpha (const bgra4& oc) noexcept { d = (float)oc.a; }
		inline void restore_alpha (const bgr565& oc) noexcept { }
		inline void restore_alpha (const grayscale& oc) noexcept { }
		inline void restore_alpha (const grayscalef& oc) noexcept { }
		inline void restore_alpha (const yuva& oc) noexcept { d = (float)oc.a; }
		inline void restore_alpha (const yuv& oc) noexcept { }
	};

	inline color_processor operator+ (const color_processor& c1, const color_processor& c2) noexcept { return color_processor (c1.a + c2.a, c1.b + c2.b, c1.c + c2.c, c1.d + c2.d); }
	inline color_processor operator- (const color_processor& c1, const color_processor& c2) noexcept { return color_processor (c1.a - c2.a, c1.b - c2.b, c1.c - c2.c, c1.d - c2.d); }
	inline color_processor operator- (const color_processor& c) noexcept { return color_processor (-c.a, -c.b, -c.c, -c.d); }
	inline color_processor operator* (const color_processor& c1, const color_processor& c2) noexcept { return color_processor (c1.a * c2.a, c1.b * c2.b, c1.c * c2.c, c1.d * c2.d); }
	inline color_processor operator/ (const color_processor& c1, const color_processor& c2) noexcept { return color_processor (c1.a / c2.a, c1.b / c2.b, c1.c / c2.c, c1.d / c2.d); }
	inline color_processor operator* (const color_processor& c1, double factor) noexcept { return color_processor ((float)(c1.a * factor), (float)(c1.b * factor), (float)(c1.c * factor), (float)(c1.d * factor)); }
	inline color_processor operator/ (const color_processor& c1, double factor) noexcept { float rf = 1 / (float)factor; return color_processor ((float)(c1.a * rf), (float)(c1.b * rf), (float)(c1.c * rf), (float)(c1.d * rf)); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Conversion Implementations
	//
	////////////////////////////////////////////////////////////////////////////////////////

	inline rgba::operator rgb() const noexcept { return rgb (r, g, b); }
	inline rgba::operator rgbaf() const noexcept { return rgbaf (r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
	inline rgba::operator bgra() const noexcept { return bgra (r, g, b, a); }
	inline rgba::operator bgr() const noexcept { return bgr (r, g, b); }
	inline rgba::operator bgra4() const noexcept { return bgra4 (r / 17, g / 17, b / 17, a / 17); }
	inline rgba::operator bgr565() const noexcept { return bgr565 (r / 8, g / 4, b / 8); }
	inline rgba::operator grayscale() const noexcept { return grayscale (rgb2y (r, g, b)); }
	inline rgba::operator grayscalef() const noexcept { return grayscalef (rgb2y (r, g, b) / 255.0f); }
	inline rgba::operator yuv() const noexcept { return yuv (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b)); }
	inline rgba::operator yuva() const noexcept { return yuva (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b), a); }
	inline rgba::operator color_processor() const noexcept
	{
		return color_processor ((float)r, (float)g, (float)b, (float)a);
	}

	inline rgb::operator rgba() const noexcept { return rgba (r, g, b, 255); }
	inline rgb::operator rgbaf() const noexcept { return rgbaf (r / 255.0f, g / 255.0f, b / 255.0f, 1); }
	inline rgb::operator bgra() const noexcept { return bgra (r, g, b, 255); }
	inline rgb::operator bgr() const noexcept { return bgr (r, g, b); }
	inline rgb::operator bgra4() const noexcept { return bgra4 (r / 17, g / 17, b / 17, 15); }
	inline rgb::operator bgr565() const noexcept { return bgr565 (r / 8, g / 4, b / 8); }
	inline rgb::operator grayscale() const noexcept { return grayscale (rgb2y (r, g, b)); }
	inline rgb::operator grayscalef() const noexcept { return grayscalef (rgb2y (r, g, b) / 255.0f); }
	inline rgb::operator yuv() const noexcept { return yuv (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b)); }
	inline rgb::operator yuva() const noexcept { return yuva (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b), 255); }
	inline rgb::operator color_processor() const noexcept
	{
		return color_processor ((float)r, (float)g, (float)b, 0);
	}

	inline rgbaf::operator rgba() const noexcept { return rgba ((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255)); }
	inline rgbaf::operator rgb() const noexcept { return rgb ((uint8_t)r * 255, (uint8_t)g * 255, (uint8_t)(b * 255)); }
	inline rgbaf::operator bgra() const noexcept { return bgra ((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255)); }
	inline rgbaf::operator bgr() const noexcept { return bgr ((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)); }
	inline rgbaf::operator bgra4() const noexcept { return bgra4 ((uint8_t)(r * 15), (uint8_t)(g * 15), (uint8_t)(b * 15), (uint8_t)(a * 15)); }
	inline rgbaf::operator bgr565() const noexcept { return bgr565 ((uint8_t)(r * 31), (uint8_t)(g * 63), (uint8_t)(b * 31)); }
	inline rgbaf::operator grayscale() const noexcept { return grayscale (rgb2y ((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255))); }
	inline rgbaf::operator grayscalef() const noexcept { return grayscalef (0.299f * r + 0.587f * g + 0.114f * b); }
	inline rgbaf::operator yuv() const noexcept { bgr bgr = *this; return (yuv)bgr; }
	inline rgbaf::operator yuva() const noexcept { bgra bgra = *this; return (yuva)bgra; }
	inline rgbaf::operator color_processor() const noexcept
	{
		return color_processor (r, g, b, a);
	}

	inline bgra::operator rgba() const noexcept { return rgba (r, g, b, a); }
	inline bgra::operator rgb() const noexcept { return rgb (r, g, b); }
	inline bgra::operator rgbaf() const noexcept { return rgbaf (r / 255.0f, g / 255.0f, b / 255.0f); }
	inline bgra::operator bgr() const noexcept { return bgr (r, g, b); }
	inline bgra::operator bgra4() const noexcept { return bgra4 (r / 17, g / 17, b / 17, a / 17); }
	inline bgra::operator bgr565() const noexcept { return bgr565 (r / 8, g / 4, b / 8); }
	inline bgra::operator grayscale() const noexcept { return grayscale (rgb2y (r, g, b)); }
	inline bgra::operator grayscalef() const noexcept { return grayscalef (rgb2y (r, g, b) / 255.0f); }
	inline bgra::operator yuv() const noexcept { return yuv (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b)); }
	inline bgra::operator yuva() const noexcept { return yuva (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b), a); }
	inline bgra::operator color_processor() const noexcept
	{
		return color_processor ((float)b, (float)g, (float)r, (float)a);
	}

	inline bgr::operator rgba() const noexcept { return rgba (r, g, b, 255); }
	inline bgr::operator rgb() const noexcept { return rgb (r, g, b); }
	inline bgr::operator rgbaf() const noexcept { return rgbaf (r / 255.0f, g / 255.0f, b / 255.0f, 1); }
	inline bgr::operator bgra() const noexcept { return bgra (r, g, b, 255); }
	inline bgr::operator bgra4() const noexcept { return bgra4 (r / 17, g / 17, b / 17, 15); }
	inline bgr::operator bgr565() const noexcept { return bgr565 (r / 8, g / 4, b / 8); }
	inline bgr::operator grayscale() const noexcept { return grayscale (rgb2y (r, g, b)); }
	inline bgr::operator grayscalef() const noexcept { return grayscalef (rgb2y (r, g, b) / 255.0f); }
	inline bgr::operator yuv() const noexcept { return yuv (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b)); }
	inline bgr::operator yuva() const noexcept { return yuva (rgb2y (r, g, b), rgb2u (r, g, b), rgb2v (r, g, b), 255); }
	inline bgr::operator color_processor() const noexcept
	{
		return color_processor ((float)b, (float)g, (float)r, 0);
	}

	inline bgra4::operator rgba() const noexcept { return rgba (r * 17, g * 17, b * 17, a * 17); }
	inline bgra4::operator rgb() const noexcept { return rgb (r * 17, g * 17, b * 17); }
	inline bgra4::operator rgbaf() const noexcept { return rgbaf (r / 15.0f, g / 15.0f, b / 15.0f, a / 15.0f); }
	inline bgra4::operator bgra() const noexcept { return bgra (r * 17, g * 17, b * 17, a * 17); }
	inline bgra4::operator bgr() const noexcept { return bgr (r * 17, g * 17, b * 17); }
	inline bgra4::operator bgr565() const noexcept { return bgr565 (r * 2, g * 4, b * 2); }
	inline bgra4::operator grayscale() const noexcept { return grayscale (rgb2y (r * 17, g * 17, b * 17)); }
	inline bgra4::operator grayscalef() const noexcept { return grayscalef (rgb2y (r * 17, g * 17, b * 17) / 255.0f); }
	inline bgra4::operator yuv() const noexcept { return yuv (rgb2y (r * 17, g * 17, b * 17), rgb2u (r * 17, g * 17, b * 17), rgb2v (r * 17, g * 17, b * 17)); }
	inline bgra4::operator yuva() const noexcept { return yuva (rgb2y (r * 17, g * 17, b * 17), rgb2u (r * 17, g * 17, b * 17), rgb2v (r * 17, g * 17, b * 17), a * 17); }
	inline bgra4::operator color_processor() const noexcept
	{
		return color_processor ((float)b, (float)g, (float)r, (float)a);
	}

	inline bgr565::operator rgba() const noexcept { return rgba (r * 8, g * 4, b * 8, 255); }
	inline bgr565::operator rgb() const noexcept { return rgb (r * 8, g * 4, b * 8); }
	inline bgr565::operator rgbaf() const noexcept { return rgbaf (r / 31.0f, g / 63.0f, b / 31.0f, 1); }
	inline bgr565::operator bgra() const noexcept { return bgra (r * 8, g * 4, b * 8, 255); }
	inline bgr565::operator bgr() const noexcept { return bgr (r * 8, g * 4, b * 8); }
	inline bgr565::operator bgra4() const noexcept { return bgra4 (r / 2, g / 4, b / 2, 15); }
	inline bgr565::operator grayscale() const noexcept { return grayscale (rgb2y (r * 8, g * 4, b * 8)); }
	inline bgr565::operator grayscalef() const noexcept { return grayscalef (rgb2y (r * 8, g * 4, b * 8) / 255.0f); }
	inline bgr565::operator yuv() const noexcept { return yuv (rgb2y (r * 8, g * 4, b * 8), rgb2u (r * 8, g * 4, b * 8), rgb2v (r * 8, g * 4, b * 8)); }
	inline bgr565::operator yuva() const noexcept { return yuva (rgb2y (r * 8, g * 4, b * 8), rgb2u (r * 8, g * 4, b * 8), rgb2v (r * 8, g * 4, b * 8), 255); }
	inline bgr565::operator color_processor() const noexcept
	{
		return color_processor ((float)b, (float)g, (float)r, 0);
	}

	inline grayscale::operator rgba () const noexcept { return rgba (color, color, color, 255); }
	inline grayscale::operator rgb () const noexcept { return rgb (color, color, color); }
	inline grayscale::operator rgbaf () const noexcept { float t = color / 255.0f; return rgbaf (t, t, t, 1.0f); }
	inline grayscale::operator bgra () const noexcept { return bgra (color, color, color, 255); }
	inline grayscale::operator bgr () const noexcept { return bgr (color, color, color); }
	inline grayscale::operator bgra4 () const noexcept { uint8_t t = color / 17; return bgra4 (t, t, t, 15); }
	inline grayscale::operator bgr565 () const noexcept { return bgr565 (color / 8, color / 4, color / 8); }
	inline grayscale::operator grayscalef () const noexcept { return grayscalef (color / 255.0f); }
	inline grayscale::operator yuv () const noexcept { return yuv (color, 0, 0); }
	inline grayscale::operator yuva () const noexcept { return yuva (color, 0, 0, 255); }
	inline grayscale::operator color_processor() const noexcept
	{
		return color_processor ((float)color, 0, 0, 0);
	}

	inline grayscalef::operator rgba () const noexcept { uint8_t t = (uint8_t)(color * 255); return rgba (t, t, t, 255); }
	inline grayscalef::operator rgb () const noexcept { uint8_t t = (uint8_t)(color * 255); return rgb (t, t, t); }
	inline grayscalef::operator rgbaf () const noexcept { return rgbaf (color, color, color, 1.0f); }
	inline grayscalef::operator bgra () const noexcept { uint8_t t = (uint8_t)(color * 255); return bgra (t, t, t, 255); }
	inline grayscalef::operator bgr () const noexcept { uint8_t t = (uint8_t)(color * 255); return bgr (t, t, t); }
	inline grayscalef::operator bgra4 () const noexcept { uint8_t t = (uint8_t)(color * 15); return bgra4 (t, t, t, 15); }
	inline grayscalef::operator bgr565 () const noexcept { return bgr565 ((uint8_t)(color * 8), (uint8_t)(color * 4), (uint8_t)(color * 8)); }
	inline grayscalef::operator grayscale () const noexcept { return grayscale ((uint8_t)(color * 255)); }
	inline grayscalef::operator yuv () const noexcept { return yuv ((uint8_t)(color * 255), 0, 0); }
	inline grayscalef::operator yuva () const noexcept { return yuva ((uint8_t)(color * 255), 0, 0, 255); }
	inline grayscalef::operator color_processor() const noexcept
	{
		return color_processor (color, 0, 0, 0);
	}

	inline yuva::operator rgba () const noexcept { return rgba (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v), a); }
	inline yuva::operator rgb () const noexcept { return rgb (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v)); }
	inline yuva::operator rgbaf () const noexcept { return (rgbaf)(bgra)* this; }
	inline yuva::operator bgra () const noexcept { return bgra (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v), a); }
	inline yuva::operator bgr () const noexcept { return bgr (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v)); }
	inline yuva::operator bgra4 () const noexcept { return bgra4 (yuv2r (y, u, v) / 17, yuv2g (y, u, v) / 17, yuv2b (y, u, v) / 17, a / 17); }
	inline yuva::operator bgr565 () const noexcept { return bgr565 (yuv2r (y, u, v) / 8, yuv2g (y, u, v) / 4, yuv2b (y, u, v) / 8); }
	inline yuva::operator grayscale () const noexcept { return grayscale (y); }
	inline yuva::operator grayscalef () const noexcept { return grayscalef (y / 255.0f); }
	inline yuva::operator yuv () const noexcept { return yuv (y, u, v); }
	inline yuva::operator color_processor() const noexcept
	{
		return color_processor ((float)y, (float)u, (float)v, (float)a);
	}

	inline yuv::operator rgba () const noexcept { return rgba (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v), 255); }
	inline yuv::operator rgb () const noexcept { return rgb (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v)); }
	inline yuv::operator rgbaf () const noexcept { return (rgbaf)(bgra)* this; }
	inline yuv::operator bgra () const noexcept { return bgra (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v), 255); }
	inline yuv::operator bgr () const noexcept { return bgr (yuv2r (y, u, v), yuv2g (y, u, v), yuv2b (y, u, v)); }
	inline yuv::operator bgra4 () const noexcept { return bgra4 (yuv2r (y, u, v) / 17, yuv2g (y, u, v) / 17, yuv2b (y, u, v) / 17, 15); }
	inline yuv::operator bgr565 () const noexcept { return bgr565 (yuv2r (y, u, v) / 8, yuv2g (y, u, v) / 4, yuv2b (y, u, v) / 8); }
	inline yuv::operator grayscale () const noexcept { return grayscale (y); }
	inline yuv::operator grayscalef () const noexcept { return grayscalef (y / 255.0f); }
	inline yuv::operator yuva () const noexcept { return yuva (y, u, v, 255); }
	inline yuv::operator color_processor() const noexcept
	{
		return color_processor ((float)y, (float)u, (float)v, 0);
	}

	inline color_processor::operator rgba () const noexcept
	{
		return rgba (
			saturate8 ((int32_t)a),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)c),
			saturate8 ((int32_t)d)
		);
	}
	inline color_processor::operator rgb () const noexcept
	{
		return rgb (
			saturate8 ((int32_t)a),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)c)
		);
	}
	inline color_processor::operator rgbaf () const noexcept
	{
		return rgbaf (
			saturatef (a),
			saturatef (b),
			saturatef (c),
			saturatef (d)
		);
	}
	inline color_processor::operator bgra () const noexcept
	{
		return bgra (
			saturate8 ((int32_t)c),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)a),
			saturate8 ((int32_t)d)
		);
	}
	inline color_processor::operator bgr () const noexcept
	{
		return bgr (
			saturate8 ((int32_t)c),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)a)
		);
	}
	inline color_processor::operator bgra4 () const noexcept
	{
		return bgra4 (
			saturate4 ((int32_t)c),
			saturate4 ((int32_t)b),
			saturate4 ((int32_t)a),
			saturate4 ((int32_t)d)
		);
	}
	inline color_processor::operator bgr565 () const noexcept
	{
		return bgr565 (
			saturate5 ((int32_t)c),
			saturate6 ((int32_t)b),
			saturate5 ((int32_t)a)
		);
	}
	inline color_processor::operator grayscale () const noexcept
	{
		return grayscale (
			saturate8 ((int32_t)a)
		);
	}
	inline color_processor::operator grayscalef () const noexcept
	{
		return grayscalef (
			saturatef (a)
		);
	}
	inline color_processor::operator yuva () const noexcept
	{
		return yuva (
			saturate8 ((int32_t)a),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)c),
			saturate8 ((int32_t)d)
		);
	}
	inline color_processor::operator yuv() const noexcept
	{
		return yuv (
			saturate8 ((int32_t)a),
			saturate8 ((int32_t)b),
			saturate8 ((int32_t)c)
		);
	}

	namespace colours
	{
		inline bgra transparent () noexcept { return bgra (0); }

		inline bgra black () noexcept { return bgra (0xff000000); }
		inline bgra white () noexcept { return bgra (0xffffffff); }

		inline bgra red () noexcept { return bgra (0xff0000ff); }
		inline bgra green () noexcept { return bgra (0xff00ff00); }
		inline bgra blue () noexcept { return bgra (0xffff0000); }

		inline bgra cyan () noexcept { return bgra (0xffffff00); }
		inline bgra magenta () noexcept { return bgra (0xffff00ff); }
		inline bgra yellow () noexcept { return bgra (0xff00ffff); }

		inline bgra aliceblue () noexcept { return bgra (0xfffff8f0); }
		inline bgra antiquewhite () noexcept { return bgra (0xffd7ebfa); }
		inline bgra aqua () noexcept { return bgra (0xffffff00); }
		inline bgra aquamarine () noexcept { return bgra (0xffd4ff7f); }
		inline bgra azure () noexcept { return bgra (0xfffffff0); }
		inline bgra beige () noexcept { return bgra (0xffdcf5f5); }
		inline bgra bisque () noexcept { return bgra (0xffc4e4ff); }
		inline bgra blanchedalmond () noexcept { return bgra (0xffcdebff); }
		inline bgra blueviolet () noexcept { return bgra (0xffe22b8a); }
		inline bgra brown () noexcept { return bgra (0xff2a2aa5); }
		inline bgra burlywood () noexcept { return bgra (0xff87b8de); }
		inline bgra cadetblue () noexcept { return bgra (0xffa09e5f); }
		inline bgra chartreuse () noexcept { return bgra (0xff00ff7f); }
		inline bgra chocolate () noexcept { return bgra (0xff1e69d2); }
		inline bgra coral () noexcept { return bgra (0xff507fff); }
		inline bgra cornflowerblue () noexcept { return bgra (0xffed9564); }
		inline bgra cornsilk () noexcept { return bgra (0xffdcf8ff); }
		inline bgra crimson () noexcept { return bgra (0xff3c14dc); }
		inline bgra darkblue () noexcept { return bgra (0xff8b0000); }
		inline bgra darkcyan () noexcept { return bgra (0xff8b8b00); }
		inline bgra darkgoldenrod () noexcept { return bgra (0xff0b86b8); }
		inline bgra darkgray () noexcept { return bgra (0xffa9a9a9); }
		inline bgra darkgreen () noexcept { return bgra (0xff006400); }
		inline bgra darkkhaki () noexcept { return bgra (0xff6bb7bd); }
		inline bgra darkmagenta () noexcept { return bgra (0xff8b008b); }
		inline bgra darkolivegreen () noexcept { return bgra (0xff2f6b55); }
		inline bgra darkorange () noexcept { return bgra (0xff008cff); }
		inline bgra darkorchid () noexcept { return bgra (0xffcc3299); }
		inline bgra darkred () noexcept { return bgra (0xff00008b); }
		inline bgra darksalmon () noexcept { return bgra (0xff7a96e9); }
		inline bgra darkseaGreen () noexcept { return bgra (0xff8bbc8f); }
		inline bgra darkslateBlue () noexcept { return bgra (0xff8b3d48); }
		inline bgra darkslateGray () noexcept { return bgra (0xff4f4f2f); }
		inline bgra darkturquoise () noexcept { return bgra (0xffd1ce00); }
		inline bgra darkviolet () noexcept { return bgra (0xffd30094); }
		inline bgra deeppink () noexcept { return bgra (0xff9314ff); }
		inline bgra deepskyblue () noexcept { return bgra (0xffffbf00); }
		inline bgra dimgray () { return bgra (0xff696969); }
		inline bgra dodgerblue () noexcept { return bgra (0xffff901e); }
		inline bgra firebrick () noexcept { return bgra (0xff2222b2); }
		inline bgra floralwhite () noexcept { return bgra (0xfff0faff); }
		inline bgra forestgreen () noexcept { return bgra (0xff228b22); }
		inline bgra fuchsia () noexcept { return bgra (0xffff00ff); }
		inline bgra gainsboro () noexcept { return bgra (0xffdcdcdc); }
		inline bgra ghostwhite () noexcept { return bgra (0xfffff8f8); }
		inline bgra gold () noexcept { return bgra (0xff00d7ff); }
		inline bgra goldenrod () noexcept { return bgra (0xff20a5da); }
		inline bgra gray () noexcept { return bgra (0xff808080); }
		inline bgra greenyellow () noexcept { return bgra (0xff2fffad); }
		inline bgra honeydew () noexcept { return bgra (0xfff0fff0); }
		inline bgra hotpink () noexcept { return bgra (0xffb469ff); }
		inline bgra indianred () noexcept { return bgra (0xff5c5ccd); }
		inline bgra indigo () noexcept { return bgra (0xff82004b); }
		inline bgra ivory () noexcept { return bgra (0xfff0ffff); }
		inline bgra khaki () noexcept { return bgra (0xff8ce6f0); }
		inline bgra lavender () noexcept { return bgra (0xfffae6e6); }
		inline bgra lavenderblush () noexcept { return bgra (0xfff5f0ff); }
		inline bgra lawngreen () noexcept { return bgra (0xff00fc7c); }
		inline bgra lemonchiffon () noexcept { return bgra (0xffcdfaff); }
		inline bgra lightblue () noexcept { return bgra (0xffe6d8ad); }
		inline bgra lightcoral () noexcept { return bgra (0xff8080f0); }
		inline bgra lightcyan () noexcept { return bgra (0xffffffe0); }
		inline bgra lightgoldenrodyellow () noexcept { return bgra (0xffd2fafa); }
		inline bgra lightgray () noexcept { return bgra (0xffd3d3d3); }
		inline bgra lightgreen () noexcept { return bgra (0xff90ee90); }
		inline bgra lightpink () noexcept { return bgra (0xffc1b6ff); }
		inline bgra lightsalmon () noexcept { return bgra (0xff7aa0ff); }
		inline bgra lightseaGreen () noexcept { return bgra (0xffaab220); }
		inline bgra lightskyBlue () noexcept { return bgra (0xffface87); }
		inline bgra lightslateGray () noexcept { return bgra (0xff998877); }
		inline bgra lightsteelBlue () noexcept { return bgra (0xffdec4b0); }
		inline bgra lightyellow () noexcept { return bgra (0xffe0ffff); }
		inline bgra lime () noexcept { return bgra (0xff00ff00); }
		inline bgra limegreen () noexcept { return bgra (0xff32cd32); }
		inline bgra linen () noexcept { return bgra (0xffe6f0fa); }
		inline bgra maroon () noexcept { return bgra (0xff000080); }
		inline bgra mediumaquamarine () noexcept { return bgra (0xffaacd66); }
		inline bgra mediumblue () noexcept { return bgra (0xffcd0000); }
		inline bgra mediumorchid () noexcept { return bgra (0xffd355ba); }
		inline bgra mediumpurple () noexcept { return bgra (0xffdb7093); }
		inline bgra mediumseagreen () noexcept { return bgra (0xff71b33c); }
		inline bgra mediumslateblue () noexcept { return bgra (0xffee687b); }
		inline bgra mediumspringgreen () noexcept { return bgra (0xff9afa00); }
		inline bgra mediumturquoise () noexcept { return bgra (0xffccd148); }
		inline bgra mediumvioletred () noexcept { return bgra (0xff8515c7); }
		inline bgra midnightblue () noexcept { return bgra (0xff701919); }
		inline bgra mintcream () noexcept { return bgra (0xfffafff5); }
		inline bgra mistyrose () noexcept { return bgra (0xffe1e4ff); }
		inline bgra moccasin () noexcept { return bgra (0xffb5e4ff); }
		inline bgra navajowhite () noexcept { return bgra (0xffaddeff); }
		inline bgra navy () noexcept { return bgra (0xff800000); }
		inline bgra oldlace () noexcept { return bgra (0xffe6f5fd); }
		inline bgra olive () noexcept { return bgra (0xff008080); }
		inline bgra olivedrab () noexcept { return bgra (0xff238e6b); }
		inline bgra orange () noexcept { return bgra (0xff00a5ff); }
		inline bgra orangered () noexcept { return bgra (0xff0045ff); }
		inline bgra orchid () noexcept { return bgra (0xffd670da); }
		inline bgra palegoldenrod () noexcept { return bgra (0xffaae8ee); }
		inline bgra palegreen () noexcept { return bgra (0xff98fb98); }
		inline bgra paleturquoise () noexcept { return bgra (0xffeeeeaf); }
		inline bgra palevioletred () noexcept { return bgra (0xff9370db); }
		inline bgra papayawhip () noexcept { return bgra (0xffd5efff); }
		inline bgra peachpuff () noexcept { return bgra (0xffb9daff); }
		inline bgra peru () noexcept { return bgra (0xff3f85cd); }
		inline bgra pink () noexcept { return bgra (0xffcbc0ff); }
		inline bgra plum () noexcept { return bgra (0xffdda0dd); }
		inline bgra powderblue () noexcept { return bgra (0xffe6e0b0); }
		inline bgra purple () noexcept { return bgra (0xff800080); }
		inline bgra rosybrown () noexcept { return bgra (0xff8f8fbc); }
		inline bgra royalblue () noexcept { return bgra (0xffe16941); }
		inline bgra saddlebrown () noexcept { return bgra (0xff13458b); }
		inline bgra salmon () noexcept { return bgra (0xff7280fa); }
		inline bgra sandybrown () noexcept { return bgra (0xff60a4f4); }
		inline bgra seagreen () noexcept { return bgra (0xff578b2e); }
		inline bgra seashell () noexcept { return bgra (0xffeef5ff); }
		inline bgra sienna () noexcept { return bgra (0xff2d52a0); }
		inline bgra silver () noexcept { return bgra (0xffc0c0c0); }
		inline bgra skyblue () noexcept { return bgra (0xffebce87); }
		inline bgra slateblue () noexcept { return bgra (0xffcd5a6a); }
		inline bgra slategray () noexcept { return bgra (0xff908070); }
		inline bgra snow () noexcept { return bgra (0xfffafaff); }
		inline bgra springgreen () noexcept { return bgra (0xff7fff00); }
		inline bgra steelblue () noexcept { return bgra (0xffb48246); }
		inline bgra tan () noexcept { return bgra (0xff8cb4d2); }
		inline bgra teal () noexcept { return bgra (0xff808000); }
		inline bgra thistle () noexcept { return bgra (0xffd8bfd8); }
		inline bgra tomato () noexcept { return bgra (0xff4763ff); }
		inline bgra turquoise () noexcept { return bgra (0xffd0e040); }
		inline bgra violet () noexcept { return bgra (0xffee82ee); }
		inline bgra webgreen () noexcept { return bgra (0xff008000); }
		inline bgra wheat () noexcept { return bgra (0xffb3def5); }
		inline bgra whitesmoke () noexcept { return bgra (0xfff5f5f5); }
		inline bgra yellowgreen () noexcept { return bgra (0xff32cd9a); }
	}
}

#endif