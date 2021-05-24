#ifndef __DSEED_COLOR_H__
#define __DSEED_COLOR_H__

namespace dseed::color
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Pixel Format
	//
	////////////////////////////////////////////////////////////////////////////////////////
	enum class pixelformat : unsigned int
	{
		// Unknown Pixel Format
		unknown = 0x00000000,

		// RGB-type Pixel Formats
		rgba8 = 0x01010104,
		rgb8 = 0x01010003,
		rgbaf = 0x01010110,
		bgra8 = 0x01020104,
		bgr8 = 0x01020003,
		bgra4 = 0x01020102,
		bgr565 = 0x01020002,

		// Grayscale Pixel Formats
		r8 = 0x01030001,
		rf = 0x01030004,

		// YUV-type Pixel Formats
		yuva8 = 0x02010104,
		yuv8 = 0x02010003,

		// HSV-type Pixel Foramts
		hsva8 = 0x02020104,
		hsv8 = 0x02020003,

		// YUV Chroma Subsampled Pixel Formats
		yuyv8 = 0x03010001,
		nv12 = 0x03010002,

		// Indexed Pixel Formats
		bgra8_indexed8 = 0xfd010104,
		bgr8_indexed8 = 0xfd010103,

		// Depth-Stencil Pixel Formats
		depth16 = 0xfe010002,
		depth24stencil8 = 0xfe020004,
		depth32 = 0xfe010004,

		// S3TC Compressed Pixel Formats
		bc1 = 0xff010001, dxt1 = bc1,
		bc2 = 0xff010002, dxt3 = bc2,
		bc3 = 0xff010003, dxt5 = bc3,
		bc4 = 0xff010004, ati1 = bc4,
		bc5 = 0xff010005, ati2 = bc5,
		bc6 = 0xff010006,
		bc7 = 0xff010007,

		// ETC Compressed Pixel Formats
		etc1 = 0xff020001,
		etc2 = 0xff020002,
		etc2a = 0xff020003,

		// PVRTC Compressed Pixel Formats,
		pvrtc_2bpp = 0xff030001,
		pvrtc_2abpp = 0xff030101,
		pvrtc_4bpp = 0xff030002,
		pvrtc_4abpp = 0xff030102,
		pvrtc2_2bpp = 0xff030103,
		pvrtc2_4bpp = 0xff030104,

		// ASTC Compressed Pixel Formats
		astc4x4 = 0xff040001,
		astc5x4 = 0xff040002,
		astc5x5 = 0xff040003,
		astc6x5 = 0xff040004,
		astc6x6 = 0xff040005,
		astc8x5 = 0xff040006,
		astc8x6 = 0xff040007,
		astc8x8 = 0xff040008,
		astc10x5 = 0xff040009,
		astc10x6 = 0xff04000a,
		astc10x8 = 0xff04000b,
		astc10x10 = 0xff04000c,
		astc12x10 = 0xff04000d,
		astc12x12 = 0xff04000e,
	};

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Type Prototypes
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct colorv;

	struct rgba8;
	struct rgb8;
	struct rgbaf;

	struct bgra8;
	struct bgr8;
	struct bgra4;
	struct bgr565;

	struct r8;
	struct rf;

	struct yuva8;
	struct yuv8;

	struct hsva8;
	struct hsv8;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Utilities
	//
	////////////////////////////////////////////////////////////////////////////////////////

	DSEEDEXP size_t calc_bitmap_stride(pixelformat pf, size_t width) noexcept;
	DSEEDEXP size_t calc_bitmap_plane_size(pixelformat pf, const size2i& size) noexcept;
	DSEEDEXP size_t calc_bitmap_total_size(pixelformat pf, const size3i& size) noexcept;

	DSEEDEXP size3i calc_mipmap_size(int mipLevel, const size3i& size, bool cubemap = false) noexcept;
	DSEEDEXP size_t calc_maximum_mipmap_levels(const dseed::size3i& size, bool cubemap = false) noexcept;

	constexpr uint8_t saturate8(int32_t v) noexcept { return (uint8_t)maximum(0, minimum(255, v)); }
	constexpr uint8_t saturate6(int32_t v) noexcept { return (uint8_t)maximum(0, minimum(63, v)); }
	constexpr uint8_t saturate5(int32_t v) noexcept { return (uint8_t)maximum(0, minimum(31, v)); }
	constexpr uint8_t saturate4(int32_t v) noexcept { return (uint8_t)maximum(0, minimum(15, v)); }
	constexpr float saturatef(float v) noexcept { return maximum(0.0000000000f, minimum(1.0000000000f, v)); }

	// https://stackoverflow.com/questions/1737726/how-to-perform-rgb-yuv-conversion-in-c-c , Modified
	// RGB -> YUV
	constexpr int32_t __add128shift8(int32_t v) noexcept { return (v + 128) >> 8; }
	constexpr uint8_t rgb2y(int32_t r, int32_t g, int32_t b) noexcept { return saturate8(__add128shift8((66 * r) + (129 * g) + (25 * b)) + 16); }
	constexpr uint8_t rgb2u(int32_t r, int32_t g, int32_t b) noexcept { return saturate8(__add128shift8((-38 * r) - (74 * g) + (112 * b)) + 128); }
	constexpr uint8_t rgb2v(int32_t r, int32_t g, int32_t b) noexcept { return saturate8(__add128shift8((112 * r) - (94 * g) - (18 * b)) + 128); }
	// YUV -> RGB
	constexpr int32_t __c(int32_t y) noexcept { return y - 16; }
	constexpr int32_t __d(int32_t u) noexcept { return u - 128; }
	constexpr int32_t __e(int32_t v) noexcept { return v - 128; }
	constexpr uint8_t yuv2r(int32_t y, int32_t u, int32_t v) noexcept { return saturate8(__add128shift8((298 * __c(y)) + (409 * __e(v)))); }
	constexpr uint8_t yuv2g(int32_t y, int32_t u, int32_t v) noexcept { return saturate8(__add128shift8((298 * __c(y)) - (100 * __d(u)) - (208 * __e(v)))); }
	constexpr uint8_t yuv2b(int32_t y, int32_t u, int32_t v) noexcept { return saturate8(__add128shift8((298 * __c(y)) + (516 * __d(u)))); }

	// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
	// RGB -> HSV
	constexpr void rgb2hsv(int32_t r, int32_t g, int32_t b, uint8_t& h, uint8_t& s, uint8_t& v) noexcept
	{
		uint8_t rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b)
			, rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

		if ((v = rgbMax) == 0)
			h = s = 0;
		else if ((s = 255 * long(rgbMax - rgbMin) / v) == 0)
			h = 0;
		else
		{
			if (rgbMax == r)		h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
			else if (rgbMax == g)	h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
			else					h = 171 + 43 * (r - g) / (rgbMax - rgbMin);
		}
	}
	// HSV -> RGB
	constexpr void hsv2rgb(int32_t h, int32_t s, int32_t v, uint8_t& r, uint8_t& g, uint8_t& b) noexcept
	{
		if (s == 0)
			r = g = b = v;
		else
		{
			uint8_t region = h / 43, remainder = (h - (region * 43)) * 6;

			uint8_t p = (v * (255 - s)) >> 8;
			uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
			uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

			switch (region)
			{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
			}
		}
	}

	template<class T> constexpr pixelformat type2format() noexcept { static_assert (true, "Not support type."); return pixelformat::unknown; }
	template<> constexpr pixelformat type2format<rgba8>() noexcept { return pixelformat::rgba8; }
	template<> constexpr pixelformat type2format<rgb8>() noexcept { return pixelformat::rgb8; }
	template<> constexpr pixelformat type2format<rgbaf>() noexcept { return pixelformat::rgbaf; }
	template<> constexpr pixelformat type2format<bgra8>() noexcept { return pixelformat::bgra8; }
	template<> constexpr pixelformat type2format<bgr8>() noexcept { return pixelformat::bgr8; }
	template<> constexpr pixelformat type2format<bgra4>() noexcept { return pixelformat::bgra4; }
	template<> constexpr pixelformat type2format<bgr565>() noexcept { return pixelformat::bgr565; }
	template<> constexpr pixelformat type2format<r8>() noexcept { return pixelformat::r8; }
	template<> constexpr pixelformat type2format<rf>() noexcept { return pixelformat::rf; }
	template<> constexpr pixelformat type2format<yuva8>() noexcept { return pixelformat::yuva8; }
	template<> constexpr pixelformat type2format<yuv8>() noexcept { return pixelformat::yuv8; }
	template<> constexpr pixelformat type2format<hsva8>() noexcept { return pixelformat::hsva8; }
	template<> constexpr pixelformat type2format<hsv8>() noexcept { return pixelformat::hsv8; }

	template<class T> constexpr pixelformat type2indexedformat() noexcept { static_assert (true, "Not support type."); return pixelformat::unknown; }
	template<> constexpr pixelformat type2indexedformat<bgra8>() noexcept { return pixelformat::bgra8_indexed8; }
	template<> constexpr pixelformat type2indexedformat<bgr8>() noexcept { return pixelformat::bgr8_indexed8; }

	template<class T> constexpr bool hasalpha() noexcept { return false; }
	template<> constexpr bool hasalpha<rgba8>() noexcept { return true; }
	template<> constexpr bool hasalpha<rgbaf>() noexcept { return true; }
	template<> constexpr bool hasalpha<bgra8>() noexcept { return true; }
	template<> constexpr bool hasalpha<bgra4>() noexcept { return true; }
	template<> constexpr bool hasalpha<yuva8>() noexcept { return true; }
	template<> constexpr bool hasalpha<hsva8>() noexcept { return true; }
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

	struct rgba8
	{
		union
		{
			struct { uint8_t r, g, b, a; };
			uint32_t color;
		};
		rgba8() noexcept = default;
		inline rgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
			: r(r), g(g), b(b), a(a)
		{ }
		inline rgba8(uint32_t rgba) noexcept
			: color(rgba)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline rgba8 max_color() noexcept { return rgba8(255, 255, 255, 255); }
		static inline rgba8 min_color() noexcept { return rgba8(0, 0, 0, 0); }

		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline rgba8& operator+= (const rgba8& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline rgba8& operator-= (const rgba8& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline rgba8& operator*= (const rgba8& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline rgba8& operator/= (const rgba8& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline rgba8 operator+ (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline rgba8 operator- (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline rgba8 operator* (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline rgba8 operator/ (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline rgba8 operator* (const rgba8& c1, double factor) noexcept { return rgba8((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline rgba8 operator/ (const rgba8& c1, double factor) noexcept { return rgba8((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline rgba8 operator& (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline rgba8 operator| (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline rgba8 operator^ (const rgba8& c1, const rgba8& c2) noexcept { return rgba8(c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b, c1.a ^ c2.a); }
	inline rgba8 operator~ (const rgba8& c) noexcept { return rgba8(~c.r, ~c.g, ~c.b, ~c.a); }
	inline bool operator== (const rgba8& c1, const rgba8& c2) noexcept { return c1.color == c2.color; }

	struct rgb8
	{
		union
		{
			struct { uint8_t r, g, b; };
			uint24_t color;
		};
		rgb8() noexcept = default;
		inline rgb8(uint8_t r, uint8_t g, uint8_t b) noexcept
			: r(r), g(g), b(b)
		{ }
		inline rgb8(uint24_t rgb) noexcept
			: color(rgb)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline rgb8 max_color() noexcept { return rgb8(255, 255, 255); }
		static inline rgb8 min_color() noexcept { return rgb8(0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline rgb8& operator+= (const rgb8& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline rgb8& operator-= (const rgb8& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline rgb8& operator*= (const rgb8& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline rgb8& operator/= (const rgb8& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline rgb8 operator+ (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline rgb8 operator- (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline rgb8 operator* (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline rgb8 operator/ (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline rgb8 operator* (const rgb8& c1, double factor) noexcept { return rgb8((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline rgb8 operator/ (const rgb8& c1, double factor) noexcept { return rgb8((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline rgb8 operator& (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline rgb8 operator| (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline rgb8 operator^ (const rgb8& c1, const rgb8& c2) noexcept { return rgb8(c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b); }
	inline rgb8 operator~ (const rgb8& c) noexcept { return rgb8(~c.r, ~c.g, ~c.b); }
	inline bool operator== (const rgb8& c1, const rgb8& c2) noexcept { return static_cast<int>(c1.color) == static_cast<int>(c2.color); }

	struct rgbaf
	{
		union
		{
			struct { float r, g, b, a; };
			f32x4_t color;
		};
		rgbaf() noexcept { };
		inline rgbaf(float r, float g, float b, float a = 1.00000000f) noexcept
			: r(r), g(g), b(b), a(a)
		{ }
		inline rgbaf(const rgbaf& rgba) noexcept
			: color(rgba.color)
		{ }
		inline rgbaf(const f32x4_t& rgba) noexcept
			: color(rgba)
		{ }

		inline float& operator [] (int index) noexcept { return reinterpret_cast<float*>(this)[index]; }
		inline const float& operator [] (int index) const noexcept { return reinterpret_cast<const float*>(this)[index]; }

		static inline rgbaf max_color() noexcept { return rgbaf(1, 1, 1, 1); }
		static inline rgbaf min_color() noexcept { return rgbaf(0, 0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;
		inline operator f32x4_t () const noexcept { return color; }

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline rgbaf& operator+= (const rgbaf& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline rgbaf& operator-= (const rgbaf& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline rgbaf& operator*= (const rgbaf& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline rgbaf& operator/= (const rgbaf& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline rgbaf operator+ (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline rgbaf operator- (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline rgbaf operator- (const rgbaf& c) noexcept { return rgbaf(-c.r, -c.g, -c.b, -c.a); }
	inline rgbaf operator* (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline rgbaf operator/ (const rgbaf& c1, const rgbaf& c2) noexcept { return rgbaf(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline rgbaf operator* (const rgbaf& c1, double factor) noexcept { return rgbaf((float)(c1.r * factor), (float)(c1.g * factor), (float)(c1.b * factor), (float)(c1.a * factor)); }
	inline rgbaf operator/ (const rgbaf& c1, double factor) noexcept { return rgbaf((float)(c1.r / factor), (float)(c1.g / factor), (float)(c1.b / factor), (float)(c1.a / factor)); }
	inline bool operator== (const rgbaf& c1, const rgbaf& c2) noexcept
	{
		f32x4_t a = c1.color, b = c2.color;
		return equalsb(a, b);
	}

	struct bgra8
	{
		union
		{
			struct { uint8_t b, g, r, a; };
			uint32_t color;
		};
		bgra8() noexcept = default;
		inline bgra8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
			: r(r), g(g), b(b), a(a)
		{ }
		inline bgra8(uint32_t bgra) noexcept
			: color(bgra)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline bgra8 max_color() noexcept { return bgra8(255, 255, 255, 255); }
		static inline bgra8 min_color() noexcept { return bgra8(0, 0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline bgra8& operator+= (const bgra8& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline bgra8& operator-= (const bgra8& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline bgra8& operator*= (const bgra8& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline bgra8& operator/= (const bgra8& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline bgra8 operator+ (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline bgra8 operator- (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline bgra8 operator* (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline bgra8 operator/ (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline bgra8 operator* (const bgra8& c1, double factor) noexcept { return bgra8((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline bgra8 operator/ (const bgra8& c1, double factor) noexcept { return bgra8((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline bgra8 operator& (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline bgra8 operator| (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline bgra8 operator^ (const bgra8& c1, const bgra8& c2) noexcept { return bgra8(c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b, c1.a ^ c2.a); }
	inline bgra8 operator~ (const bgra8& c) noexcept { return bgra8(~c.r, ~c.g, ~c.b, ~c.a); }
	inline bool operator== (const bgra8& c1, const bgra8& c2) noexcept { return c1.color == c2.color; }

	struct bgr8
	{
		union
		{
			struct { uint8_t b, g, r; };
			uint24_t color;
		};
		bgr8() noexcept = default;
		inline bgr8(uint8_t r, uint8_t g, uint8_t b) noexcept
			: r(r), g(g), b(b)
		{ }
		inline bgr8(uint24_t bgr) noexcept
			: color(bgr)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline bgr8 max_color() noexcept { return bgr8(255, 255, 255); }
		static inline bgr8 min_color() noexcept { return bgr8(0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline bgr8& operator+= (const bgr8& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline bgr8& operator-= (const bgr8& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline bgr8& operator*= (const bgr8& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline bgr8& operator/= (const bgr8& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline bgr8 operator+ (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline bgr8 operator- (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline bgr8 operator* (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline bgr8 operator/ (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline bgr8 operator* (const bgr8& c1, double factor) noexcept { return bgr8((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline bgr8 operator/ (const bgr8& c1, double factor) noexcept { return bgr8((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline bgr8 operator& (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline bgr8 operator| (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline bgr8 operator^ (const bgr8& c1, const bgr8& c2) noexcept { return bgr8(c1.r ^ c2.r, c1.g ^ c2.g, c1.b ^ c2.b); }
	inline bgr8 operator~ (const bgr8& c) noexcept { return bgr8(~c.r, ~c.g, ~c.b); }
	inline bool operator== (const bgr8& c1, const bgr8& c2) noexcept { return static_cast<int>(c1.color) == static_cast<int>(c2.color); }

	struct bgra4
	{
		union
		{
			struct { uint8_t b : 4, g : 4, r : 4, a : 4; };
			uint16_t color;
		};
		bgra4() noexcept = default;
		inline bgra4(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 15) noexcept
			: r(r), g(g), b(b), a(a)
		{ }
		inline bgra4(uint16_t bgra) noexcept
			: color(bgra)
		{ }

		inline uint8_t operator [] (int index) const noexcept
		{
			switch (index)
			{
			case 0: return b;
			case 1: return g;
			case 2: return r;
			case 3: return a;
			default: return 0xff;
			}
		}

		static inline bgra4 max_color() noexcept { return bgra4(15, 15, 15, 15); }
		static inline bgra4 min_color() noexcept { return bgra4(0, 0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline bgra4& operator+= (const bgra4& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
		inline bgra4& operator-= (const bgra4& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
		inline bgra4& operator*= (const bgra4& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
		inline bgra4& operator/= (const bgra4& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	};
	inline bgra4 operator+ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline bgra4 operator- (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	inline bgra4 operator* (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }
	inline bgra4 operator/ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a); }
	inline bgra4 operator* (const bgra4& c1, double factor) noexcept { return bgra4((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor), (uint8_t)(c1.a * factor)); }
	inline bgra4 operator/ (const bgra4& c1, double factor) noexcept { return bgra4((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor), (uint8_t)(c1.a / factor)); }
	inline bgra4 operator& (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b, c1.a & c2.a); }
	inline bgra4 operator| (const bgra4& c1, const bgra4& c2) noexcept { return bgra4(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b, c1.a | c2.a); }
	inline bgra4 operator^ (const bgra4& c1, const bgra4& c2) noexcept { return bgra4((c1.r ^ c2.r) & 0xf, (c1.g ^ c2.g) & 0xf, (c1.b ^ c2.b) & 0xf, (c1.a ^ c2.a) & 0xf); }
	inline bgra4 operator~ (const bgra4& c) noexcept { return bgra4((~c.r) & 0xf, (~c.g) & 0xf, (~c.b) & 0xf, (~c.a) & 0xf); }
	inline bool operator== (const bgra4& c1, const bgra4& c2) noexcept { return c1.color == c2.color; }

	struct bgr565
	{
		union
		{
			struct { uint8_t b : 5, g : 6, r : 5; };
			uint16_t color;
		};
		bgr565() noexcept = default;
		inline bgr565(uint8_t r, uint8_t g, uint8_t b) noexcept
			: r(r), g(g), b(b)
		{ }
		inline bgr565(uint16_t bgr) noexcept
			: color(bgr)
		{ }

		inline uint8_t operator [] (int index) const noexcept
		{
			switch (index)
			{
			case 0: return b;
			case 1: return g;
			case 2: return r;
			default: return 0xff;
			}
		}

		static inline bgr565 max_color() noexcept { return bgr565(31, 63, 31); }
		static inline bgr565 min_color() noexcept { return bgr565(0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline bgr565& operator+= (const bgr565& c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
		inline bgr565& operator-= (const bgr565& c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
		inline bgr565& operator*= (const bgr565& c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
		inline bgr565& operator/= (const bgr565& c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	};
	inline bgr565 operator+ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	inline bgr565 operator- (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	inline bgr565 operator* (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b); }
	inline bgr565 operator/ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b); }
	inline bgr565 operator* (const bgr565& c1, double factor) noexcept { return bgr565((uint8_t)(c1.r * factor), (uint8_t)(c1.g * factor), (uint8_t)(c1.b * factor)); }
	inline bgr565 operator/ (const bgr565& c1, double factor) noexcept { return bgr565((uint8_t)(c1.r / factor), (uint8_t)(c1.g / factor), (uint8_t)(c1.b / factor)); }
	inline bgr565 operator& (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r & c2.r, c1.g & c2.g, c1.b & c2.b); }
	inline bgr565 operator| (const bgr565& c1, const bgr565& c2) noexcept { return bgr565(c1.r | c2.r, c1.g | c2.g, c1.b | c2.b); }
	inline bgr565 operator^ (const bgr565& c1, const bgr565& c2) noexcept { return bgr565((c1.r ^ c2.r) & 0x1f, (c1.g ^ c2.g) & 0x3f, (c1.b ^ c2.b) & 0x1f); }
	inline bgr565 operator~ (const bgr565& c) noexcept { return bgr565((~c.r) & 0x1f, (~c.g) & 0x3f, (~c.b) & 0x1f); }
	inline bool operator== (const bgr565& c1, const bgr565& c2) noexcept { return c1.color == c2.color; }

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

	struct r8
	{
		uint8_t color;
		r8() noexcept = default;
		inline r8(uint8_t grayscale) noexcept : color(grayscale) { }
		inline r8(const rgb8& rgb) noexcept
			: color((uint8_t)maximum(0.0, minimum(255.0, +0.2627 * rgb.r + +0.678 * rgb.g + +0.0593 * rgb.b)))
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline r8 max_color() noexcept { return r8(255); }
		static inline r8 min_color() noexcept { return r8(0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline r8& operator+= (const r8& c) noexcept { color += c.color; return *this; }
		inline r8& operator-= (const r8& c) noexcept { color -= c.color; return *this; }
		inline r8& operator*= (const r8& c) noexcept { color *= c.color; return *this; }
		inline r8& operator/= (const r8& c) noexcept { color /= c.color; return *this; }
	};
	inline r8 operator+ (const r8& c1, const r8& c2) noexcept { return r8(c1.color + c2.color); }
	inline r8 operator- (const r8& c1, const r8& c2) noexcept { return r8(c1.color - c2.color); }
	inline r8 operator* (const r8& c1, const r8& c2) noexcept { return r8(c1.color * c2.color); }
	inline r8 operator/ (const r8& c1, const r8& c2) noexcept { return r8(c1.color / c2.color); }
	inline r8 operator* (const r8& c1, double factor) noexcept { return r8((uint8_t)(c1.color * factor)); }
	inline r8 operator/ (const r8& c1, double factor) noexcept { return r8((uint8_t)(c1.color / factor)); }
	inline r8 operator& (const r8& c1, const r8& c2) noexcept { return r8(c1.color & c2.color); }
	inline r8 operator| (const r8& c1, const r8& c2) noexcept { return r8(c1.color | c2.color); }
	inline r8 operator^ (const r8& c1, const r8& c2) noexcept { return r8(c1.color ^ c2.color); }
	inline r8 operator~ (const r8& c) noexcept { return r8(~c.color); }
	inline bool operator== (const r8& c1, const r8& c2) noexcept { return c1.color == c2.color; }

	struct rf
	{
		float color;
		rf() noexcept = default;
		inline rf(float grayscale) : color(grayscale) { }
		inline rf(const rgbaf& rgbaf)
			: color((float)maximum(0.0, minimum(1.0, (+0.2627 * rgbaf.r + +0.678 * rgbaf.g + +0.0593 * rgbaf.b))))
		{ }

		inline float& operator [] (int index) noexcept { return reinterpret_cast<float*>(this)[index]; }
		inline const float& operator [] (int index) const noexcept { return reinterpret_cast<const float*>(this)[index]; }

		static inline rf max_color() noexcept { return rf(1); }
		static inline rf min_color() noexcept { return rf(0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;

		inline operator yuv8 () const noexcept;
		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline rf& operator+= (const rf& c) noexcept { color += c.color; return *this; }
		inline rf& operator-= (const rf& c) noexcept { color -= c.color; return *this; }
		inline rf& operator*= (const rf& c) noexcept { color *= c.color; return *this; }
		inline rf& operator/= (const rf& c) noexcept { color /= c.color; return *this; }
	};
	inline rf operator+ (const rf& c1, const rf& c2) noexcept { return rf(c1.color + c2.color); }
	inline rf operator- (const rf& c1, const rf& c2) noexcept { return rf(c1.color - c2.color); }
	inline rf operator- (const rf& c) noexcept { return rf(-c.color); }
	inline rf operator* (const rf& c1, const rf& c2) noexcept { return rf(c1.color * c2.color); }
	inline rf operator/ (const rf& c1, const rf& c2) noexcept { return rf(c1.color / c2.color); }
	inline rf operator* (const rf& c1, double factor) noexcept { return rf((float)(c1.color * factor)); }
	inline rf operator/ (const rf& c1, double factor) noexcept { return rf((float)(c1.color / factor)); }
	inline bool operator== (const rf& c1, const rf& c2) noexcept { return c1.color == c2.color; }

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

	struct yuva8
	{
		union
		{
			struct { uint8_t v, u, y, a; };
			uint32_t color;
		};
		yuva8() = default;
		inline yuva8(uint8_t y, uint8_t u, uint8_t v, uint8_t a = 255) noexcept
			: y(y), u(u), v(v), a(a)
		{ }
		inline yuva8(uint32_t yuva) noexcept
			: color(yuva)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline yuva8 max_color() noexcept { return yuva8(255, 255, 255, 255); }
		static inline yuva8 min_color() noexcept { return yuva8(0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuv8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline yuva8& operator+= (const yuva8& c) noexcept { y += c.y; u += c.u; v += c.v; a += c.a; return *this; }
		inline yuva8& operator-= (const yuva8& c) noexcept { y -= c.y; u -= c.u; v -= c.v; a -= c.a; return *this; }
		inline yuva8& operator*= (const yuva8& c) noexcept { y *= c.y; u *= c.u; v *= c.v; a *= c.a; return *this; }
		inline yuva8& operator/= (const yuva8& c) noexcept { y /= c.y; u /= c.u; v /= c.v; a /= c.a; return *this; }
	};
	inline yuva8 operator+ (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y + c2.y, c1.u + c2.u, c1.v + c2.v, c1.a + c2.a); }
	inline yuva8 operator- (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y - c2.y, c1.u - c2.u, c1.v - c2.v, c1.a - c2.a); }
	inline yuva8 operator* (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y * c2.y, c1.u * c2.u, c1.v * c2.v, c1.a * c2.a); }
	inline yuva8 operator/ (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y / c2.y, c1.u / c2.u, c1.v / c2.v, c1.a / c2.a); }
	inline yuva8 operator* (const yuva8& c1, double factor) noexcept { return yuva8((uint8_t)(c1.y * factor), (uint8_t)(c1.u * factor), (uint8_t)(c1.v * factor), (uint8_t)(c1.a * factor)); }
	inline yuva8 operator/ (const yuva8& c1, double factor) noexcept { return yuva8((uint8_t)(c1.y / factor), (uint8_t)(c1.u / factor), (uint8_t)(c1.v / factor), (uint8_t)(c1.a / factor)); }
	inline yuva8 operator& (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y & c2.y, c1.u & c2.u, c1.v & c2.v, c1.a & c2.a); }
	inline yuva8 operator| (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y | c2.y, c1.u | c2.u, c1.v | c2.v, c1.a | c2.a); }
	inline yuva8 operator^ (const yuva8& c1, const yuva8& c2) noexcept { return yuva8(c1.y ^ c2.y, c1.u ^ c2.u, c1.v ^ c2.v, c1.a ^ c2.a); }
	inline yuva8 operator~ (const yuva8& c) noexcept { return yuva8(~c.y, ~c.u, ~c.v, ~c.a); }
	inline bool operator== (const yuva8& c1, const yuva8& c2) noexcept { return c1.color == c2.color; }

	struct yuv8
	{
		union
		{
			struct { uint8_t y, u, v; };
			uint24_t color;
		};
		yuv8() noexcept = default;
		inline yuv8(uint8_t y, uint8_t u, uint8_t v) noexcept
			: y(y), u(u), v(v)
		{ }
		inline yuv8(uint24_t yuv) noexcept
			: color(yuv)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline yuv8 max_color() noexcept { return yuv8(255, 255, 255); }
		static inline yuv8 min_color() noexcept { return yuv8(0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuva8 () const noexcept;

		inline operator hsv8 () const noexcept;
		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline yuv8& operator+= (const yuv8& c) noexcept { y += c.y; u += c.u; v += c.v; return *this; }
		inline yuv8& operator-= (const yuv8& c) noexcept { y -= c.y; u -= c.u; v -= c.v; return *this; }
		inline yuv8& operator*= (const yuv8& c) noexcept { y *= c.y; u *= c.u; v *= c.v; return *this; }
		inline yuv8& operator/= (const yuv8& c) noexcept { y /= c.y; u /= c.u; v /= c.v; return *this; }
	};
	inline yuv8 operator+ (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y + c2.y, c1.u + c2.u, c1.v + c2.v); }
	inline yuv8 operator- (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y - c2.y, c1.u - c2.u, c1.v - c2.v); }
	inline yuv8 operator* (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y * c2.y, c1.u * c2.u, c1.v * c2.v); }
	inline yuv8 operator/ (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y / c2.y, c1.u / c2.u, c1.v / c2.v); }
	inline yuv8 operator* (const yuv8& c1, double factor) noexcept { return yuv8((uint8_t)(c1.y * factor), (uint8_t)(c1.u * factor), (uint8_t)(c1.v * factor)); }
	inline yuv8 operator/ (const yuv8& c1, double factor) noexcept { return yuv8((uint8_t)(c1.y / factor), (uint8_t)(c1.u / factor), (uint8_t)(c1.v / factor)); }
	inline yuv8 operator& (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y & c2.y, c1.u & c2.u, c1.v & c2.v); }
	inline yuv8 operator| (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y | c2.y, c1.u | c2.u, c1.v | c2.v); }
	inline yuv8 operator^ (const yuv8& c1, const yuv8& c2) noexcept { return yuv8(c1.y ^ c2.y, c1.u ^ c2.u, c1.v ^ c2.v); }
	inline yuv8 operator~ (const yuv8& c) noexcept { return yuv8(~c.y, ~c.u, ~c.v); }
	inline bool operator== (const yuv8& c1, const yuv8& c2) noexcept { return static_cast<int>(c1.color) == static_cast<int>(c2.color); }

#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Hue/Saturation/Value Color Types
	//
	////////////////////////////////////////////////////////////////////////////////////////

#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif

	struct hsva8
	{
		union
		{
			struct { uint8_t h, s, v, a; };
			uint32_t color;
		};
		hsva8() = default;
		inline hsva8(uint8_t h, uint8_t s, uint8_t v, uint8_t a = 255) noexcept
			: h(h), s(s), v(v), a(a)
		{ }
		inline hsva8(uint32_t hsva) noexcept
			: color(hsva)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline hsva8 max_color() noexcept { return hsva8(255, 255, 255, 255); }
		static inline hsva8 min_color() noexcept { return hsva8(0, 0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuva8 () const noexcept;
		inline operator yuv8 () const noexcept;

		inline operator hsv8 () const noexcept;

		inline operator colorv () const noexcept;

		inline hsva8& operator+= (const hsva8& c) noexcept { h += c.h; s += c.s; v += c.v; a += c.a; return *this; }
		inline hsva8& operator-= (const hsva8& c) noexcept { h -= c.h; s -= c.s; v -= c.v; a -= c.a; return *this; }
		inline hsva8& operator*= (const hsva8& c) noexcept { h *= c.h; s *= c.s; v *= c.v; a *= c.a; return *this; }
		inline hsva8& operator/= (const hsva8& c) noexcept { h /= c.h; s /= c.s; v /= c.v; a /= c.a; return *this; }
	};
	inline hsva8 operator+ (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h + c2.h, c1.s + c2.s, c1.v + c2.v, c1.a + c2.a); }
	inline hsva8 operator- (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h - c2.h, c1.s - c2.s, c1.v - c2.v, c1.a - c2.a); }
	inline hsva8 operator* (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h * c2.h, c1.s * c2.s, c1.v * c2.v, c1.a * c2.a); }
	inline hsva8 operator/ (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h / c2.h, c1.s / c2.s, c1.v / c2.v, c1.a / c2.a); }
	inline hsva8 operator* (const hsva8& c1, double factor) noexcept { return hsva8((uint8_t)(c1.h * factor), (uint8_t)(c1.s * factor), (uint8_t)(c1.v * factor), (uint8_t)(c1.a * factor)); }
	inline hsva8 operator/ (const hsva8& c1, double factor) noexcept { return hsva8((uint8_t)(c1.h / factor), (uint8_t)(c1.s / factor), (uint8_t)(c1.v / factor), (uint8_t)(c1.a / factor)); }
	inline hsva8 operator& (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h & c2.h, c1.s & c2.s, c1.v & c2.v, c1.a & c2.a); }
	inline hsva8 operator| (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h | c2.h, c1.s | c2.s, c1.v | c2.v, c1.a | c2.a); }
	inline hsva8 operator^ (const hsva8& c1, const hsva8& c2) noexcept { return hsva8(c1.h ^ c2.h, c1.s ^ c2.s, c1.v ^ c2.v, c1.a ^ c2.a); }
	inline hsva8 operator~ (const hsva8& c) noexcept { return hsva8(~c.h, ~c.s, ~c.v, ~c.a); }
	inline bool operator== (const hsva8& c1, const hsva8& c2) noexcept { return c1.color == c2.color; }

	struct hsv8
	{
		union
		{
			struct { uint8_t h, s, v; };
			uint24_t color;
		};
		hsv8() noexcept = default;
		inline hsv8(uint8_t h, uint8_t s, uint8_t v) noexcept
			: h(h), s(s), v(v)
		{ }
		inline hsv8(uint24_t hsv) noexcept
			: color(hsv)
		{ }

		inline uint8_t& operator [] (int index) noexcept { return reinterpret_cast<uint8_t*>(this)[index]; }
		inline const uint8_t& operator [] (int index) const noexcept { return reinterpret_cast<const uint8_t*>(this)[index]; }

		static inline hsv8 max_color() noexcept { return hsv8(255, 255, 255); }
		static inline hsv8 min_color() noexcept { return hsv8(0, 0, 0); }

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuva8 () const noexcept;
		inline operator yuv8 () const noexcept;

		inline operator hsva8 () const noexcept;

		inline operator colorv () const noexcept;

		inline hsv8& operator+= (const hsv8& c) noexcept { h += c.h; s += c.s; v += c.v; return *this; }
		inline hsv8& operator-= (const hsv8& c) noexcept { h -= c.h; s -= c.s; v -= c.v; return *this; }
		inline hsv8& operator*= (const hsv8& c) noexcept { h *= c.h; s *= c.s; v *= c.v; return *this; }
		inline hsv8& operator/= (const hsv8& c) noexcept { h /= c.h; s /= c.s; v /= c.v; return *this; }
	};
	inline hsv8 operator+ (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h + c2.h, c1.s + c2.s, c1.v + c2.v); }
	inline hsv8 operator- (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h - c2.h, c1.s - c2.s, c1.v - c2.v); }
	inline hsv8 operator* (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h * c2.h, c1.s * c2.s, c1.v * c2.v); }
	inline hsv8 operator/ (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h / c2.h, c1.s / c2.s, c1.v / c2.v); }
	inline hsv8 operator* (const hsv8& c1, double factor) noexcept { return hsv8((uint8_t)(c1.h * factor), (uint8_t)(c1.s * factor), (uint8_t)(c1.v * factor)); }
	inline hsv8 operator/ (const hsv8& c1, double factor) noexcept { return hsv8((uint8_t)(c1.h / factor), (uint8_t)(c1.s / factor), (uint8_t)(c1.v / factor)); }
	inline hsv8 operator& (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h & c2.h, c1.s & c2.s, c1.v & c2.v); }
	inline hsv8 operator| (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h | c2.h, c1.s | c2.s, c1.v | c2.v); }
	inline hsv8 operator^ (const hsv8& c1, const hsv8& c2) noexcept { return hsv8(c1.h ^ c2.h, c1.s ^ c2.s, c1.v ^ c2.v); }
	inline hsv8 operator~ (const hsv8& c) noexcept { return hsv8(~c.h, ~c.s, ~c.v); }
	inline bool operator== (const hsv8& c1, const hsv8& c2) noexcept { return static_cast<int>(c1.color) == static_cast<int>(c2.color); }

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

	struct colorv
	{
		f32x4_t vector;

		inline colorv() noexcept : vector((float)0) { }
		inline colorv(float a, float b, float c, float d) noexcept
			: vector(a, b, c, d) { }
		inline colorv(const f32x4_t& v) : vector(v) { }

		inline float& operator [] (int index) noexcept
		{
			return reinterpret_cast<float*>(this)[index];
		}
		inline const float& operator [] (int index) const noexcept
		{
			return reinterpret_cast<const float*>(this)[index];
		}

		inline void store(WRITES(4) float* a) const noexcept
		{
			f32x4_t::store(a, &vector);
		}

		inline operator rgba8 () const noexcept;
		inline operator rgb8 () const noexcept;
		inline operator rgbaf () const noexcept;
		inline operator bgra8 () const noexcept;
		inline operator bgr8 () const noexcept;
		inline operator bgra4 () const noexcept;
		inline operator bgr565 () const noexcept;

		inline operator r8 () const noexcept;
		inline operator rf () const noexcept;

		inline operator yuva8 () const noexcept;
		inline operator yuv8 () const noexcept;

		inline operator hsva8 () const noexcept;
		inline operator hsv8 () const noexcept;

		inline colorv& operator+= (const colorv& cp) noexcept { vector = addfv(vector, cp.vector); return *this; }
		inline colorv& operator-= (const colorv& cp) noexcept { vector = subtractfv(vector, cp.vector); return *this; }
		inline colorv& operator*= (const colorv& cp) noexcept { vector = multiplyfv(vector, cp.vector); return *this; }
		inline colorv& operator/= (const colorv& cp) noexcept { vector = dividefv(vector, cp.vector); return *this; }

		inline void restore_alpha(const rgba8& oc) noexcept { this->operator[](3) = (float)oc.a; }
		inline void restore_alpha(const rgb8& oc) noexcept { }
		inline void restore_alpha(const rgbaf& oc) noexcept { this->operator[](3) = oc.a; }
		inline void restore_alpha(const bgra8& oc) noexcept { this->operator[](3) = (float)oc.a; }
		inline void restore_alpha(const bgr8& oc) noexcept { }
		inline void restore_alpha(const bgra4& oc) noexcept { this->operator[](3) = (float)oc.a; }
		inline void restore_alpha(const bgr565& oc) noexcept { }
		inline void restore_alpha(const r8& oc) noexcept { }
		inline void restore_alpha(const rf& oc) noexcept { }
		inline void restore_alpha(const yuva8& oc) noexcept { this->operator[](3) = (float)oc.a; }
		inline void restore_alpha(const yuv8& oc) noexcept { }
		inline void restore_alpha(const hsva8& oc) noexcept { this->operator[](3) = (float)oc.a; }
		inline void restore_alpha(const hsv8& oc) noexcept { }
	};

	inline colorv operator+ (const colorv& c1, const colorv& c2) noexcept { return colorv(addfv(c1.vector, c2.vector)); }
	inline colorv operator- (const colorv& c1, const colorv& c2) noexcept { return colorv(subtractfv(c1.vector, c2.vector)); }
	inline colorv operator- (const colorv& c) noexcept { return colorv(negatefv(c.vector)); }
	inline colorv operator* (const colorv& c1, const colorv& c2) noexcept { return colorv(multiplyfv(c1.vector, c2.vector)); }
	inline colorv operator/ (const colorv& c1, const colorv& c2) noexcept { return colorv(dividefv(c1.vector, c2.vector)); }
	inline colorv operator* (const colorv& c1, double factor) noexcept { return colorv(multiplyfv(c1.vector, (float)factor)); }
	inline colorv operator/ (const colorv& c1, double factor) noexcept { return colorv(dividefv(c1.vector, (float)factor)); }
	inline bool operator== (const colorv& c1, const colorv& c2) noexcept { return equalsb(c1.vector, c2.vector); }
	inline bool operator!= (const colorv& c1, const colorv& c2) noexcept { return not_equalsb(c1.vector, c2.vector); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Conversion Implementations
	//
	////////////////////////////////////////////////////////////////////////////////////////

	inline rgba8::operator rgb8() const noexcept { return rgb8(r, g, b); }
	inline rgba8::operator rgbaf() const noexcept { return rgbaf(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
	inline rgba8::operator bgra8() const noexcept { return bgra8(r, g, b, a); }
	inline rgba8::operator bgr8() const noexcept { return bgr8(r, g, b); }
	inline rgba8::operator bgra4() const noexcept { return bgra4(r / 17, g / 17, b / 17, a / 17); }
	inline rgba8::operator bgr565() const noexcept { return bgr565(r / 8, g / 4, b / 8); }
	inline rgba8::operator r8() const noexcept { return r8(rgb2y(r, g, b)); }
	inline rgba8::operator rf() const noexcept { return rf(rgb2y(r, g, b) / 255.0f); }
	inline rgba8::operator yuv8() const noexcept { return yuv8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b)); }
	inline rgba8::operator yuva8() const noexcept { return yuva8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b), a); }
	inline rgba8::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsv8(h, s, v); }
	inline rgba8::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsva8(h, s, v, a); }
	inline rgba8::operator colorv() const noexcept
	{
		return colorv((float)r, (float)g, (float)b, (float)a);
	}

	inline rgb8::operator rgba8() const noexcept { return rgba8(r, g, b, 255); }
	inline rgb8::operator rgbaf() const noexcept { return rgbaf(r / 255.0f, g / 255.0f, b / 255.0f, 1); }
	inline rgb8::operator bgra8() const noexcept { return bgra8(r, g, b, 255); }
	inline rgb8::operator bgr8() const noexcept { return bgr8(r, g, b); }
	inline rgb8::operator bgra4() const noexcept { return bgra4(r / 17, g / 17, b / 17, 15); }
	inline rgb8::operator bgr565() const noexcept { return bgr565(r / 8, g / 4, b / 8); }
	inline rgb8::operator r8() const noexcept { return r8(rgb2y(r, g, b)); }
	inline rgb8::operator rf() const noexcept { return rf(rgb2y(r, g, b) / 255.0f); }
	inline rgb8::operator yuv8() const noexcept { return yuv8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b)); }
	inline rgb8::operator yuva8() const noexcept { return yuva8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b), 255); }
	inline rgb8::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsv8(h, s, v); }
	inline rgb8::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsva8(h, s, v, 255); }
	inline rgb8::operator colorv() const noexcept
	{
		return colorv((float)r, (float)g, (float)b, 0);
	}

	inline rgbaf::operator rgba8() const noexcept { return rgba8((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255)); }
	inline rgbaf::operator rgb8() const noexcept { return rgb8((uint8_t)r * 255, (uint8_t)g * 255, (uint8_t)(b * 255)); }
	inline rgbaf::operator bgra8() const noexcept { return bgra8((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255)); }
	inline rgbaf::operator bgr8() const noexcept { return bgr8((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)); }
	inline rgbaf::operator bgra4() const noexcept { return bgra4((uint8_t)(r * 15), (uint8_t)(g * 15), (uint8_t)(b * 15), (uint8_t)(a * 15)); }
	inline rgbaf::operator bgr565() const noexcept { return bgr565((uint8_t)(r * 31), (uint8_t)(g * 63), (uint8_t)(b * 31)); }
	inline rgbaf::operator r8() const noexcept { return r8(rgb2y((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255))); }
	inline rgbaf::operator rf() const noexcept { return rf(0.299f * r + 0.587f * g + 0.114f * b); }
	inline rgbaf::operator yuv8() const noexcept { bgr8 bgr = *this; return (yuv8)bgr; }
	inline rgbaf::operator yuva8() const noexcept { bgra8 bgra = *this; return (yuva8)bgra; }
	inline rgbaf::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), h, s, v); return hsv8(h, s, v); }
	inline rgbaf::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), h, s, v); return hsva8(h, s, v, (uint8_t)(a * 255)); }
	inline rgbaf::operator colorv() const noexcept
	{
		return colorv(f32x4_t::load((const float*)this));
	}

	inline bgra8::operator rgba8() const noexcept { return rgba8(r, g, b, a); }
	inline bgra8::operator rgb8() const noexcept { return rgb8(r, g, b); }
	inline bgra8::operator rgbaf() const noexcept { return rgbaf(r / 255.0f, g / 255.0f, b / 255.0f); }
	inline bgra8::operator bgr8() const noexcept { return bgr8(r, g, b); }
	inline bgra8::operator bgra4() const noexcept { return bgra4(r / 17, g / 17, b / 17, a / 17); }
	inline bgra8::operator bgr565() const noexcept { return bgr565(r / 8, g / 4, b / 8); }
	inline bgra8::operator r8() const noexcept { return r8(rgb2y(r, g, b)); }
	inline bgra8::operator rf() const noexcept { return rf(rgb2y(r, g, b) / 255.0f); }
	inline bgra8::operator yuv8() const noexcept { return yuv8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b)); }
	inline bgra8::operator yuva8() const noexcept { return yuva8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b), a); }
	inline bgra8::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsv8(h, s, v); }
	inline bgra8::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsva8(h, s, v, a); }
	inline bgra8::operator colorv() const noexcept
	{
		return colorv((float)b, (float)g, (float)r, (float)a);
	}

	inline bgr8::operator rgba8() const noexcept { return rgba8(r, g, b, 255); }
	inline bgr8::operator rgb8() const noexcept { return rgb8(r, g, b); }
	inline bgr8::operator rgbaf() const noexcept { return rgbaf(r / 255.0f, g / 255.0f, b / 255.0f, 1); }
	inline bgr8::operator bgra8() const noexcept { return bgra8(r, g, b, 255); }
	inline bgr8::operator bgra4() const noexcept { return bgra4(r / 17, g / 17, b / 17, 15); }
	inline bgr8::operator bgr565() const noexcept { return bgr565(r / 8, g / 4, b / 8); }
	inline bgr8::operator r8() const noexcept { return r8(rgb2y(r, g, b)); }
	inline bgr8::operator rf() const noexcept { return rf(rgb2y(r, g, b) / 255.0f); }
	inline bgr8::operator yuv8() const noexcept { return yuv8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b)); }
	inline bgr8::operator yuva8() const noexcept { return yuva8(rgb2y(r, g, b), rgb2u(r, g, b), rgb2v(r, g, b), 255); }
	inline bgr8::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsv8(h, s, v); }
	inline bgr8::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r, g, b, h, s, v); return hsva8(h, s, v, 255); }
	inline bgr8::operator colorv() const noexcept
	{
		return colorv((float)b, (float)g, (float)r, 0);
	}

	inline bgra4::operator rgba8() const noexcept { return rgba8(r * 17, g * 17, b * 17, a * 17); }
	inline bgra4::operator rgb8() const noexcept { return rgb8(r * 17, g * 17, b * 17); }
	inline bgra4::operator rgbaf() const noexcept { return rgbaf(r / 15.0f, g / 15.0f, b / 15.0f, a / 15.0f); }
	inline bgra4::operator bgra8() const noexcept { return bgra8(r * 17, g * 17, b * 17, a * 17); }
	inline bgra4::operator bgr8() const noexcept { return bgr8(r * 17, g * 17, b * 17); }
	inline bgra4::operator bgr565() const noexcept { return bgr565(r * 2, g * 4, b * 2); }
	inline bgra4::operator r8() const noexcept { return r8(rgb2y(r * 17, g * 17, b * 17)); }
	inline bgra4::operator rf() const noexcept { return rf(rgb2y(r * 17, g * 17, b * 17) / 255.0f); }
	inline bgra4::operator yuv8() const noexcept { return yuv8(rgb2y(r * 17, g * 17, b * 17), rgb2u(r * 17, g * 17, b * 17), rgb2v(r * 17, g * 17, b * 17)); }
	inline bgra4::operator yuva8() const noexcept { return yuva8(rgb2y(r * 17, g * 17, b * 17), rgb2u(r * 17, g * 17, b * 17), rgb2v(r * 17, g * 17, b * 17), a * 17); }
	inline bgra4::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r * 17, g * 17, b * 17, h, s, v); return hsv8(h, s, v); }
	inline bgra4::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r * 17, g * 17, b * 17, h, s, v); return hsva8(h, s, v, a * 17); }
	inline bgra4::operator colorv() const noexcept
	{
		return colorv((float)b, (float)g, (float)r, (float)a);
	}

	inline bgr565::operator rgba8() const noexcept { return rgba8(r * 8, g * 4, b * 8, 255); }
	inline bgr565::operator rgb8() const noexcept { return rgb8(r * 8, g * 4, b * 8); }
	inline bgr565::operator rgbaf() const noexcept { return rgbaf(r / 31.0f, g / 63.0f, b / 31.0f, 1); }
	inline bgr565::operator bgra8() const noexcept { return bgra8(r * 8, g * 4, b * 8, 255); }
	inline bgr565::operator bgr8() const noexcept { return bgr8(r * 8, g * 4, b * 8); }
	inline bgr565::operator bgra4() const noexcept { return bgra4(r / 2, g / 4, b / 2, 15); }
	inline bgr565::operator r8() const noexcept { return r8(rgb2y(r * 8, g * 4, b * 8)); }
	inline bgr565::operator rf() const noexcept { return rf(rgb2y(r * 8, g * 4, b * 8) / 255.0f); }
	inline bgr565::operator yuv8() const noexcept { return yuv8(rgb2y(r * 8, g * 4, b * 8), rgb2u(r * 8, g * 4, b * 8), rgb2v(r * 8, g * 4, b * 8)); }
	inline bgr565::operator yuva8() const noexcept { return yuva8(rgb2y(r * 8, g * 4, b * 8), rgb2u(r * 8, g * 4, b * 8), rgb2v(r * 8, g * 4, b * 8), 255); }
	inline bgr565::operator hsv8() const noexcept { uint8_t h, s, v; rgb2hsv(r * 8, g * 4, b * 8, h, s, v); return hsv8(h, s, v); }
	inline bgr565::operator hsva8() const noexcept { uint8_t h, s, v; rgb2hsv(r * 8, g * 4, b * 8, h, s, v); return hsva8(h, s, v, 255); }
	inline bgr565::operator colorv() const noexcept
	{
		return colorv((float)b, (float)g, (float)r, 0);
	}

	inline r8::operator rgba8 () const noexcept { return rgba8(color, color, color, 255); }
	inline r8::operator rgb8 () const noexcept { return rgb8(color, color, color); }
	inline r8::operator rgbaf () const noexcept { float t = color / 255.0f; return rgbaf(t, t, t, 1.0f); }
	inline r8::operator bgra8 () const noexcept { return bgra8(color, color, color, 255); }
	inline r8::operator bgr8 () const noexcept { return bgr8(color, color, color); }
	inline r8::operator bgra4 () const noexcept { uint8_t t = color / 17; return bgra4(t, t, t, 15); }
	inline r8::operator bgr565 () const noexcept { return bgr565(color / 8, color / 4, color / 8); }
	inline r8::operator rf () const noexcept { return rf(color / 255.0f); }
	inline r8::operator yuv8 () const noexcept { return yuv8(color, 0, 0); }
	inline r8::operator yuva8 () const noexcept { return yuva8(color, 0, 0, 255); }
	inline r8::operator hsv8() const noexcept { return hsv8(0, 0, color); }
	inline r8::operator hsva8() const noexcept { return hsva8(0, 0, color, 255); }
	inline r8::operator colorv() const noexcept
	{
		return colorv((float)color, 0, 0, 0);
	}

	inline rf::operator rgba8 () const noexcept { uint8_t t = (uint8_t)(color * 255); return rgba8(t, t, t, 255); }
	inline rf::operator rgb8 () const noexcept { uint8_t t = (uint8_t)(color * 255); return rgb8(t, t, t); }
	inline rf::operator rgbaf () const noexcept { return rgbaf(color, color, color, 1.0f); }
	inline rf::operator bgra8 () const noexcept { uint8_t t = (uint8_t)(color * 255); return bgra8(t, t, t, 255); }
	inline rf::operator bgr8 () const noexcept { uint8_t t = (uint8_t)(color * 255); return bgr8(t, t, t); }
	inline rf::operator bgra4 () const noexcept { uint8_t t = (uint8_t)(color * 15); return bgra4(t, t, t, 15); }
	inline rf::operator bgr565 () const noexcept { return bgr565((uint8_t)(color * 8), (uint8_t)(color * 4), (uint8_t)(color * 8)); }
	inline rf::operator r8 () const noexcept { return r8((uint8_t)(color * 255)); }
	inline rf::operator yuv8 () const noexcept { return yuv8((uint8_t)(color * 255), 0, 0); }
	inline rf::operator yuva8 () const noexcept { return yuva8((uint8_t)(color * 255), 0, 0, 255); }
	inline rf::operator hsv8() const noexcept { return hsv8(0, 0, (uint8_t)(color * 255)); }
	inline rf::operator hsva8() const noexcept { return hsva8(0, 0, (uint8_t)(color * 255), 255); }
	inline rf::operator colorv() const noexcept
	{
		return colorv(color, 0, 0, 0);
	}

	inline yuva8::operator rgba8 () const noexcept { return rgba8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v), a); }
	inline yuva8::operator rgb8 () const noexcept { return rgb8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v)); }
	inline yuva8::operator rgbaf () const noexcept { return (rgbaf)(bgra8)*this; }
	inline yuva8::operator bgra8 () const noexcept { return bgra8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v), a); }
	inline yuva8::operator bgr8 () const noexcept { return bgr8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v)); }
	inline yuva8::operator bgra4 () const noexcept { return bgra4(yuv2r(y, u, v) / 17, yuv2g(y, u, v) / 17, yuv2b(y, u, v) / 17, a / 17); }
	inline yuva8::operator bgr565 () const noexcept { return bgr565(yuv2r(y, u, v) / 8, yuv2g(y, u, v) / 4, yuv2b(y, u, v) / 8); }
	inline yuva8::operator r8 () const noexcept { return r8(y); }
	inline yuva8::operator rf () const noexcept { return rf(y / 255.0f); }
	inline yuva8::operator yuv8 () const noexcept { return yuv8(y, u, v); }
	inline yuva8::operator hsv8() const noexcept { return (hsv8)(rgb8)*this; }
	inline yuva8::operator hsva8() const noexcept { return (hsva8)(rgba8)*this; }
	inline yuva8::operator colorv() const noexcept
	{
		return colorv((float)y, (float)u, (float)v, (float)a);
	}

	inline yuv8::operator rgba8 () const noexcept { return rgba8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v), 255); }
	inline yuv8::operator rgb8 () const noexcept { return rgb8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v)); }
	inline yuv8::operator rgbaf () const noexcept { return (rgbaf)(rgba8)*this; }
	inline yuv8::operator bgra8 () const noexcept { return bgra8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v), 255); }
	inline yuv8::operator bgr8 () const noexcept { return bgr8(yuv2r(y, u, v), yuv2g(y, u, v), yuv2b(y, u, v)); }
	inline yuv8::operator bgra4 () const noexcept { return bgra4(yuv2r(y, u, v) / 17, yuv2g(y, u, v) / 17, yuv2b(y, u, v) / 17, 15); }
	inline yuv8::operator bgr565 () const noexcept { return bgr565(yuv2r(y, u, v) / 8, yuv2g(y, u, v) / 4, yuv2b(y, u, v) / 8); }
	inline yuv8::operator r8 () const noexcept { return r8(y); }
	inline yuv8::operator rf () const noexcept { return rf(y / 255.0f); }
	inline yuv8::operator yuva8 () const noexcept { return yuva8(y, u, v, 255); }
	inline yuv8::operator hsv8() const noexcept { return (hsv8)(rgb8)*this; }
	inline yuv8::operator hsva8() const noexcept { return (hsva8)(rgb8)*this; }
	inline yuv8::operator colorv() const noexcept
	{
		return colorv((float)y, (float)u, (float)v, 0);
	}

	inline hsva8::operator rgba8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return rgba8(r, g, b, a); }
	inline hsva8::operator rgb8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return rgb8(r, g, b); }
	inline hsva8::operator rgbaf () const noexcept { return (rgbaf)(rgba8)*this; }
	inline hsva8::operator bgra8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgra8(r, g, b, a); }
	inline hsva8::operator bgr8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgr8(r, g, b); }
	inline hsva8::operator bgra4 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgra4(r / 17, g / 17, b / 17, a / 17); }
	inline hsva8::operator bgr565 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgr8(r / 8, g / 4, b / 8); }
	inline hsva8::operator r8 () const noexcept { return r8(v); }
	inline hsva8::operator rf () const noexcept { return rf(v / 255.0f); }
	inline hsva8::operator yuva8 () const noexcept { return (yuva8)(rgba8)*this; }
	inline hsva8::operator yuv8() const noexcept { return (yuv8)(rgb8)*this; }
	inline hsva8::operator hsv8() const noexcept { return hsv8(h, s, v); }
	inline hsva8::operator colorv() const noexcept
	{
		return colorv((float)h, (float)s, (float)v, (float)a);
	}

	inline hsv8::operator rgba8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return rgba8(r, g, b, 255); }
	inline hsv8::operator rgb8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return rgb8(r, g, b); }
	inline hsv8::operator rgbaf () const noexcept { return (rgbaf)(rgba8)*this; }
	inline hsv8::operator bgra8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgra8(r, g, b, 255); }
	inline hsv8::operator bgr8 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgr8(r, g, b); }
	inline hsv8::operator bgra4 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgra4(r / 17, g / 17, b / 17, 15); }
	inline hsv8::operator bgr565 () const noexcept { uint8_t r, g, b; hsv2rgb(h, s, v, r, g, b); return bgr8(r / 8, g / 4, b / 8); }
	inline hsv8::operator r8 () const noexcept { return r8(v); }
	inline hsv8::operator rf () const noexcept { return rf(v / 255.0f); }
	inline hsv8::operator yuva8 () const noexcept { return (yuva8)(rgba8)*this; }
	inline hsv8::operator yuv8() const noexcept { return (yuv8)(rgb8)*this; }
	inline hsv8::operator hsva8() const noexcept { return hsva8(h, s, v, 255); }
	inline hsv8::operator colorv() const noexcept
	{
		return colorv((float)h, (float)s, (float)v, 0);
	}

	inline colorv::operator rgba8 () const noexcept
	{
		float arr[4];
		store(arr);
		return rgba8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2]),
			saturate8((int32_t)arr[3])
		);
	}
	inline colorv::operator rgb8 () const noexcept
	{
		float arr[4];
		store(arr);
		return rgb8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2])
		);
	}
	inline colorv::operator rgbaf () const noexcept
	{
		float arr[4];
		store(arr);
		return rgbaf(
			saturatef(arr[0]),
			saturatef(arr[1]),
			saturatef(arr[2]),
			saturatef(arr[3])
		);
	}
	inline colorv::operator bgra8 () const noexcept
	{
		float arr[4];
		store(arr);
		return bgra8(
			saturate8((int32_t)arr[2]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[3])
		);
	}
	inline colorv::operator bgr8 () const noexcept
	{
		float arr[4];
		store(arr);
		return bgr8(
			saturate8((int32_t)arr[2]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[0])
		);
	}
	inline colorv::operator bgra4 () const noexcept
	{
		float arr[4];
		store(arr);
		return bgra4(
			saturate4((int32_t)arr[2]),
			saturate4((int32_t)arr[1]),
			saturate4((int32_t)arr[0]),
			saturate4((int32_t)arr[3])
		);
	}
	inline colorv::operator bgr565 () const noexcept
	{
		float arr[4];
		store(arr);
		return bgr565(
			saturate5((int32_t)arr[2]),
			saturate6((int32_t)arr[1]),
			saturate5((int32_t)arr[0])
		);
	}
	inline colorv::operator r8 () const noexcept
	{
		float arr[4];
		store(arr);
		return r8(
			saturate8((int32_t)arr[0])
		);
	}
	inline colorv::operator rf () const noexcept
	{
		float arr[4];
		store(arr);
		return rf(
			saturatef(arr[0])
		);
	}
	inline colorv::operator yuva8 () const noexcept
	{
		float arr[4];
		store(arr);
		return yuva8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2]),
			saturate8((int32_t)arr[3])
		);
	}
	inline colorv::operator yuv8() const noexcept
	{
		float arr[4];
		store(arr);
		return yuv8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2])
		);
	}
	inline colorv::operator hsva8() const noexcept
	{
		float arr[4];
		store(arr);
		return hsva8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2]),
			saturate8((int32_t)arr[3])
		);
	}
	inline colorv::operator hsv8() const noexcept
	{
		float arr[4];
		store(arr);
		return hsv8(
			saturate8((int32_t)arr[0]),
			saturate8((int32_t)arr[1]),
			saturate8((int32_t)arr[2])
		);
	}
}

namespace dseed::color::defs
{
	inline bgra8 transparent() noexcept { return bgra8(0); }

	inline bgra8 black() noexcept { return bgra8(0xff000000); }
	inline bgra8 white() noexcept { return bgra8(0xffffffff); }

	inline bgra8 red() noexcept { return bgra8(0xff0000ff); }
	inline bgra8 green() noexcept { return bgra8(0xff00ff00); }
	inline bgra8 blue() noexcept { return bgra8(0xffff0000); }

	inline bgra8 cyan() noexcept { return bgra8(0xffffff00); }
	inline bgra8 magenta() noexcept { return bgra8(0xffff00ff); }
	inline bgra8 yellow() noexcept { return bgra8(0xff00ffff); }

	inline bgra8 aliceblue() noexcept { return bgra8(0xfffff8f0); }
	inline bgra8 antiquewhite() noexcept { return bgra8(0xffd7ebfa); }
	inline bgra8 aqua() noexcept { return bgra8(0xffffff00); }
	inline bgra8 aquamarine() noexcept { return bgra8(0xffd4ff7f); }
	inline bgra8 azure() noexcept { return bgra8(0xfffffff0); }
	inline bgra8 beige() noexcept { return bgra8(0xffdcf5f5); }
	inline bgra8 bisque() noexcept { return bgra8(0xffc4e4ff); }
	inline bgra8 blanchedalmond() noexcept { return bgra8(0xffcdebff); }
	inline bgra8 blueviolet() noexcept { return bgra8(0xffe22b8a); }
	inline bgra8 brown() noexcept { return bgra8(0xff2a2aa5); }
	inline bgra8 burlywood() noexcept { return bgra8(0xff87b8de); }
	inline bgra8 cadetblue() noexcept { return bgra8(0xffa09e5f); }
	inline bgra8 chartreuse() noexcept { return bgra8(0xff00ff7f); }
	inline bgra8 chocolate() noexcept { return bgra8(0xff1e69d2); }
	inline bgra8 coral() noexcept { return bgra8(0xff507fff); }
	inline bgra8 cornflowerblue() noexcept { return bgra8(0xffed9564); }
	inline bgra8 cornsilk() noexcept { return bgra8(0xffdcf8ff); }
	inline bgra8 crimson() noexcept { return bgra8(0xff3c14dc); }
	inline bgra8 darkblue() noexcept { return bgra8(0xff8b0000); }
	inline bgra8 darkcyan() noexcept { return bgra8(0xff8b8b00); }
	inline bgra8 darkgoldenrod() noexcept { return bgra8(0xff0b86b8); }
	inline bgra8 darkgray() noexcept { return bgra8(0xffa9a9a9); }
	inline bgra8 darkgreen() noexcept { return bgra8(0xff006400); }
	inline bgra8 darkkhaki() noexcept { return bgra8(0xff6bb7bd); }
	inline bgra8 darkmagenta() noexcept { return bgra8(0xff8b008b); }
	inline bgra8 darkolivegreen() noexcept { return bgra8(0xff2f6b55); }
	inline bgra8 darkorange() noexcept { return bgra8(0xff008cff); }
	inline bgra8 darkorchid() noexcept { return bgra8(0xffcc3299); }
	inline bgra8 darkred() noexcept { return bgra8(0xff00008b); }
	inline bgra8 darksalmon() noexcept { return bgra8(0xff7a96e9); }
	inline bgra8 darkseaGreen() noexcept { return bgra8(0xff8bbc8f); }
	inline bgra8 darkslateBlue() noexcept { return bgra8(0xff8b3d48); }
	inline bgra8 darkslateGray() noexcept { return bgra8(0xff4f4f2f); }
	inline bgra8 darkturquoise() noexcept { return bgra8(0xffd1ce00); }
	inline bgra8 darkviolet() noexcept { return bgra8(0xffd30094); }
	inline bgra8 deeppink() noexcept { return bgra8(0xff9314ff); }
	inline bgra8 deepskyblue() noexcept { return bgra8(0xffffbf00); }
	inline bgra8 dimgray() { return bgra8(0xff696969); }
	inline bgra8 dodgerblue() noexcept { return bgra8(0xffff901e); }
	inline bgra8 firebrick() noexcept { return bgra8(0xff2222b2); }
	inline bgra8 floralwhite() noexcept { return bgra8(0xfff0faff); }
	inline bgra8 forestgreen() noexcept { return bgra8(0xff228b22); }
	inline bgra8 fuchsia() noexcept { return bgra8(0xffff00ff); }
	inline bgra8 gainsboro() noexcept { return bgra8(0xffdcdcdc); }
	inline bgra8 ghostwhite() noexcept { return bgra8(0xfffff8f8); }
	inline bgra8 gold() noexcept { return bgra8(0xff00d7ff); }
	inline bgra8 goldenrod() noexcept { return bgra8(0xff20a5da); }
	inline bgra8 gray() noexcept { return bgra8(0xff808080); }
	inline bgra8 greenyellow() noexcept { return bgra8(0xff2fffad); }
	inline bgra8 honeydew() noexcept { return bgra8(0xfff0fff0); }
	inline bgra8 hotpink() noexcept { return bgra8(0xffb469ff); }
	inline bgra8 indianred() noexcept { return bgra8(0xff5c5ccd); }
	inline bgra8 indigo() noexcept { return bgra8(0xff82004b); }
	inline bgra8 ivory() noexcept { return bgra8(0xfff0ffff); }
	inline bgra8 khaki() noexcept { return bgra8(0xff8ce6f0); }
	inline bgra8 lavender() noexcept { return bgra8(0xfffae6e6); }
	inline bgra8 lavenderblush() noexcept { return bgra8(0xfff5f0ff); }
	inline bgra8 lawngreen() noexcept { return bgra8(0xff00fc7c); }
	inline bgra8 lemonchiffon() noexcept { return bgra8(0xffcdfaff); }
	inline bgra8 lightblue() noexcept { return bgra8(0xffe6d8ad); }
	inline bgra8 lightcoral() noexcept { return bgra8(0xff8080f0); }
	inline bgra8 lightcyan() noexcept { return bgra8(0xffffffe0); }
	inline bgra8 lightgoldenrodyellow() noexcept { return bgra8(0xffd2fafa); }
	inline bgra8 lightgray() noexcept { return bgra8(0xffd3d3d3); }
	inline bgra8 lightgreen() noexcept { return bgra8(0xff90ee90); }
	inline bgra8 lightpink() noexcept { return bgra8(0xffc1b6ff); }
	inline bgra8 lightsalmon() noexcept { return bgra8(0xff7aa0ff); }
	inline bgra8 lightseaGreen() noexcept { return bgra8(0xffaab220); }
	inline bgra8 lightskyBlue() noexcept { return bgra8(0xffface87); }
	inline bgra8 lightslateGray() noexcept { return bgra8(0xff998877); }
	inline bgra8 lightsteelBlue() noexcept { return bgra8(0xffdec4b0); }
	inline bgra8 lightyellow() noexcept { return bgra8(0xffe0ffff); }
	inline bgra8 lime() noexcept { return bgra8(0xff00ff00); }
	inline bgra8 limegreen() noexcept { return bgra8(0xff32cd32); }
	inline bgra8 linen() noexcept { return bgra8(0xffe6f0fa); }
	inline bgra8 maroon() noexcept { return bgra8(0xff000080); }
	inline bgra8 mediumaquamarine() noexcept { return bgra8(0xffaacd66); }
	inline bgra8 mediumblue() noexcept { return bgra8(0xffcd0000); }
	inline bgra8 mediumorchid() noexcept { return bgra8(0xffd355ba); }
	inline bgra8 mediumpurple() noexcept { return bgra8(0xffdb7093); }
	inline bgra8 mediumseagreen() noexcept { return bgra8(0xff71b33c); }
	inline bgra8 mediumslateblue() noexcept { return bgra8(0xffee687b); }
	inline bgra8 mediumspringgreen() noexcept { return bgra8(0xff9afa00); }
	inline bgra8 mediumturquoise() noexcept { return bgra8(0xffccd148); }
	inline bgra8 mediumvioletred() noexcept { return bgra8(0xff8515c7); }
	inline bgra8 midnightblue() noexcept { return bgra8(0xff701919); }
	inline bgra8 mintcream() noexcept { return bgra8(0xfffafff5); }
	inline bgra8 mistyrose() noexcept { return bgra8(0xffe1e4ff); }
	inline bgra8 moccasin() noexcept { return bgra8(0xffb5e4ff); }
	inline bgra8 navajowhite() noexcept { return bgra8(0xffaddeff); }
	inline bgra8 navy() noexcept { return bgra8(0xff800000); }
	inline bgra8 oldlace() noexcept { return bgra8(0xffe6f5fd); }
	inline bgra8 olive() noexcept { return bgra8(0xff008080); }
	inline bgra8 olivedrab() noexcept { return bgra8(0xff238e6b); }
	inline bgra8 orange() noexcept { return bgra8(0xff00a5ff); }
	inline bgra8 orangered() noexcept { return bgra8(0xff0045ff); }
	inline bgra8 orchid() noexcept { return bgra8(0xffd670da); }
	inline bgra8 palegoldenrod() noexcept { return bgra8(0xffaae8ee); }
	inline bgra8 palegreen() noexcept { return bgra8(0xff98fb98); }
	inline bgra8 paleturquoise() noexcept { return bgra8(0xffeeeeaf); }
	inline bgra8 palevioletred() noexcept { return bgra8(0xff9370db); }
	inline bgra8 papayawhip() noexcept { return bgra8(0xffd5efff); }
	inline bgra8 peachpuff() noexcept { return bgra8(0xffb9daff); }
	inline bgra8 peru() noexcept { return bgra8(0xff3f85cd); }
	inline bgra8 pink() noexcept { return bgra8(0xffcbc0ff); }
	inline bgra8 plum() noexcept { return bgra8(0xffdda0dd); }
	inline bgra8 powderblue() noexcept { return bgra8(0xffe6e0b0); }
	inline bgra8 purple() noexcept { return bgra8(0xff800080); }
	inline bgra8 rosybrown() noexcept { return bgra8(0xff8f8fbc); }
	inline bgra8 royalblue() noexcept { return bgra8(0xffe16941); }
	inline bgra8 saddlebrown() noexcept { return bgra8(0xff13458b); }
	inline bgra8 salmon() noexcept { return bgra8(0xff7280fa); }
	inline bgra8 sandybrown() noexcept { return bgra8(0xff60a4f4); }
	inline bgra8 seagreen() noexcept { return bgra8(0xff578b2e); }
	inline bgra8 seashell() noexcept { return bgra8(0xffeef5ff); }
	inline bgra8 sienna() noexcept { return bgra8(0xff2d52a0); }
	inline bgra8 silver() noexcept { return bgra8(0xffc0c0c0); }
	inline bgra8 skyblue() noexcept { return bgra8(0xffebce87); }
	inline bgra8 slateblue() noexcept { return bgra8(0xffcd5a6a); }
	inline bgra8 slategray() noexcept { return bgra8(0xff908070); }
	inline bgra8 snow() noexcept { return bgra8(0xfffafaff); }
	inline bgra8 springgreen() noexcept { return bgra8(0xff7fff00); }
	inline bgra8 steelblue() noexcept { return bgra8(0xffb48246); }
	inline bgra8 tan() noexcept { return bgra8(0xff8cb4d2); }
	inline bgra8 teal() noexcept { return bgra8(0xff808000); }
	inline bgra8 thistle() noexcept { return bgra8(0xffd8bfd8); }
	inline bgra8 tomato() noexcept { return bgra8(0xff4763ff); }
	inline bgra8 turquoise() noexcept { return bgra8(0xffd0e040); }
	inline bgra8 violet() noexcept { return bgra8(0xffee82ee); }
	inline bgra8 webgreen() noexcept { return bgra8(0xff008000); }
	inline bgra8 wheat() noexcept { return bgra8(0xffb3def5); }
	inline bgra8 whitesmoke() noexcept { return bgra8(0xfff5f5f5); }
	inline bgra8 yellowgreen() noexcept { return bgra8(0xff32cd9a); }
}

#endif