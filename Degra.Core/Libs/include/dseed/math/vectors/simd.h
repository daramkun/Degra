#ifndef __DSEED_VECTORS_SIMD_DEF_H__
#define __DSEED_VECTORS_SIMD_DEF_H__

#if ARCH_X86SET && !DONT_USE_SSE
#	include <xmmintrin.h>									// SSE
#	include <emmintrin.h>									// SSE 2
#	include <pmmintrin.h>									// SSE 3
#	include <smmintrin.h>									// SSE 4.x
#	include <nmmintrin.h>									// SSE 4.x
#	include <immintrin.h>									// AVX
#elif ARCH_ARMSET && !DONT_USE_NEON
#	if ARCH_ARM
#		include <arm_neon.h>								// 32-bit NEON
#	elif ARCH_ARM64
#		include <arm64_neon.h>								// 64-bit NEON
#	endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Vector Definitions
//
///////////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	enum class permute
	{
		x = 0, y = 1, z = 2, w = 3
	};

	enum class shuffle1
	{
		x1 = 0, y1 = 1, z1 = 2, w1 = 3,
		x2 = 4, y2 = 5, z2 = 6, w2 = 7
	};

	enum class shuffle2
	{
		x2 = 0, y2 = 1, z2 = 2, w2 = 3,
		x1 = 4, y1 = 5, z1 = 6, w1 = 7
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Floating-Point Vector
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	struct DSEEDEXP f32x4_t
	{
	public:
		union {
			struct { float _x, _y, _z, _w; };
			float _arr[4];
#if !DONT_USE_SSE
			__m128 _intrinsic;
#elif !DONT_USE_NEON
			float32x4_t _intrinsic;
#endif
		};

	public:
		inline f32x4_t() noexcept;
		inline f32x4_t(float v) noexcept;
		inline f32x4_t(float x, float y, float z, float w) noexcept;
		inline f32x4_t(const float arr[4]) noexcept;
		inline f32x4_t(const f32x4_t* v) noexcept;
		inline f32x4_t(const f32x4_t& v) noexcept;
#if !DONT_USE_SSE
		inline f32x4_t(__m128 v) noexcept;
#elif !DONT_USE_NEON
		inline f32x4_t(float32x4_t v) noexcept;
#endif

	public:
		static inline f32x4_t load(const float arr[4]) noexcept;
		static inline void store(float arr[4], const f32x4_t* v) noexcept;

#if !DONT_USE_SSE
		inline operator __m128() const noexcept { return _intrinsic; }
#elif !DONT_USE_NEON
		inline operator float32x4_t() const noexcept { return _intrinsic; }
#elif DONT_USE_SIMD
		inline float& arr(int index) noexcept { return ((float*)this)[index]; }
		inline float& operator[](int index) noexcept { return arr(index); }
#endif

	public:
#if !DONT_USE_SSE
		inline float x() const noexcept { return _mm_cvtss_f32(vx()); }
		inline float y() const noexcept { return _mm_cvtss_f32(vy()); }
		inline float z() const noexcept { return _mm_cvtss_f32(vz()); }
		inline float w() const noexcept { return _mm_cvtss_f32(vw()); }

		inline f32x4_t vx() const noexcept { return _mm_permute_ps(_intrinsic, _MM_SHUFFLE(0, 0, 0, 0)); }
		inline f32x4_t vy() const noexcept { return _mm_permute_ps(_intrinsic, _MM_SHUFFLE(1, 1, 1, 1)); }
		inline f32x4_t vz() const noexcept { return _mm_permute_ps(_intrinsic, _MM_SHUFFLE(2, 2, 2, 2)); }
		inline f32x4_t vw() const noexcept { return _mm_permute_ps(_intrinsic, _MM_SHUFFLE(3, 3, 3, 3)); }
#elif !DONT_USE_NEON
		inline float x() const noexcept { return vgetq_lane_f32(_intrinsic, 0); }
		inline float y() const noexcept { return vgetq_lane_f32(_intrinsic, 1); }
		inline float z() const noexcept { return vgetq_lane_f32(_intrinsic, 2); }
		inline float w() const noexcept { return vgetq_lane_f32(_intrinsic, 3); }

		inline f32x4_t vx() const noexcept { return vdupq_lane_f32(vget_low_f32(_intrinsic), 0); }
		inline f32x4_t vy() const noexcept { return vdupq_lane_f32(vget_low_f32(_intrinsic), 1); }
		inline f32x4_t vz() const noexcept { return vdupq_lane_f32(vget_high_f32(_intrinsic), 0); }
		inline f32x4_t vw() const noexcept { return vdupq_lane_f32(vget_high_f32(_intrinsic), 1); }
#elif DONT_USE_SIMD
		inline float x() const noexcept { return _x; }
		inline float y() const noexcept { return _y; }
		inline float z() const noexcept { return _z; }
		inline float w() const noexcept { return _w; }

		inline f32x4_t vx() const noexcept { return f32x4_t(_x); }
		inline f32x4_t vy() const noexcept { return f32x4_t(_y); }
		inline f32x4_t vz() const noexcept { return f32x4_t(_z); }
		inline f32x4_t vw() const noexcept { return f32x4_t(_w); }
#endif

	public:
		template<permute x, permute y, permute z, permute w>
		static inline f32x4_t shuffle(const f32x4_t& v) noexcept;
		template<shuffle1 x, shuffle1 y, shuffle2 z, shuffle2 w>
		static inline f32x4_t shuffle(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	} ALIGNPACK(4);

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Integer Vector
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	struct DSEEDEXP i32x4_t
	{
	public:
		union {
			struct { int _x, _y, _z, _w; };
			int _arr[4];
#if !DONT_USE_SSE
			__m128i _intrinsic;
#elif !DONT_USE_NEON
			int32x4_t _intrinsic;
#endif
		};

	public:
		inline i32x4_t() noexcept;
		inline i32x4_t(int v) noexcept;
		inline i32x4_t(int x, int y, int z, int w) noexcept;
		inline i32x4_t(const int arr[4]) noexcept;
		inline i32x4_t(const i32x4_t* v) noexcept;
		inline i32x4_t(const i32x4_t& v) noexcept;
#if !DONT_USE_SSE
		inline i32x4_t(__m128i v) noexcept;
#elif !DONT_USE_NEON
		inline i32x4_t(int32x4_t v) noexcept;
#endif

	public:
		static inline i32x4_t load(const int arr[4]) noexcept;
		static inline void store(int arr[4], const i32x4_t* v) noexcept;

#if !DONT_USE_SSE
		inline operator __m128i() const noexcept { return _intrinsic; }
#elif !DONT_USE_NEON
		inline operator int32x4_t() const noexcept { return _intrinsic; }
#elif DONT_USE_SIMD
		inline int arr(int index) noexcept { return ((int*)this)[index]; }
		inline int operator[](int index) noexcept { return arr(index); }
#endif

	public:
#if !DONT_USE_SSE
		inline int x() const noexcept { return _mm_cvtss_si32(_mm_castsi128_ps(vx())); }
		inline int y() const noexcept { return _mm_cvtss_si32(_mm_castsi128_ps(vy())); }
		inline int z() const noexcept { return _mm_cvtss_si32(_mm_castsi128_ps(vz())); }
		inline int w() const noexcept { return _mm_cvtss_si32(_mm_castsi128_ps(vw())); }

		inline i32x4_t vx() const noexcept { return _mm_shuffle_epi32(_intrinsic, _MM_SHUFFLE(0, 0, 0, 0)); }
		inline i32x4_t vy() const noexcept { return _mm_shuffle_epi32(_intrinsic, _MM_SHUFFLE(1, 1, 1, 1)); }
		inline i32x4_t vz() const noexcept { return _mm_shuffle_epi32(_intrinsic, _MM_SHUFFLE(2, 2, 2, 2)); }
		inline i32x4_t vw() const noexcept { return _mm_shuffle_epi32(_intrinsic, _MM_SHUFFLE(3, 3, 3, 3)); }
#elif !DONT_USE_NEON
		inline float x() const noexcept { return vgetq_lane_i32(_intrinsic, 0); }
		inline float y() const noexcept { return vgetq_lane_i32(_intrinsic, 1); }
		inline float z() const noexcept { return vgetq_lane_i32(_intrinsic, 2); }
		inline float w() const noexcept { return vgetq_lane_i32(_intrinsic, 3); }

		inline i32x4_t vx() const noexcept { return vdupq_lane_i32(vget_low_i32(_intrinsic), 0); }
		inline i32x4_t vy() const noexcept { return vdupq_lane_i32(vget_low_i32(_intrinsic), 1); }
		inline i32x4_t vz() const noexcept { return vdupq_lane_i32(vget_high_i32(_intrinsic), 0); }
		inline i32x4_t vw() const noexcept { return vdupq_lane_i32(vget_high_i32(_intrinsic), 1); }
#elif DONT_USE_SIMD
		inline float x() const noexcept { return _x; }
		inline float y() const noexcept { return _y; }
		inline float z() const noexcept { return _z; }
		inline float w() const noexcept { return _w; }

		inline i32x4_t vx() const noexcept { return i32x4_t(_x); }
		inline i32x4_t vy() const noexcept { return i32x4_t(_y); }
		inline i32x4_t vz() const noexcept { return i32x4_t(_z); }
		inline i32x4_t vw() const noexcept { return i32x4_t(_w); }
#endif

	public:
		template<permute x, permute y, permute z, permute w>
		static inline i32x4_t shuffle(const i32x4_t& v) noexcept;
		template<shuffle1 x, shuffle1 y, shuffle2 z, shuffle2 w>
		static inline i32x4_t shuffle(const i32x4_t& v1, const i32x4_t& v2) noexcept;
		static inline i32x4_t shuffle8(const i32x4_t& v, const i32x4_t& mask) noexcept;
	} ALIGNPACK(4);

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Type cast
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline i32x4_t castf32toi32(const f32x4_t& v) noexcept;
	static inline f32x4_t casti32tof32(const i32x4_t& v) noexcept;
	static inline i32x4_t casti32toi24(const i32x4_t& v) noexcept;
	static inline i32x4_t casti32toi16(const i32x4_t& v) noexcept;
	static inline i32x4_t casti32toi8(const i32x4_t& v) noexcept;
	static inline i32x4_t casti24toi32(const i32x4_t& v) noexcept;
	static inline i32x4_t casti24toi16(const i32x4_t& v) noexcept;
	static inline i32x4_t casti24toi8(const i32x4_t& v) noexcept;
	static inline i32x4_t casti16toi32(const i32x4_t& v) noexcept;
	static inline i32x4_t casti16toi24(const i32x4_t& v) noexcept;
	static inline i32x4_t casti16toi8(const i32x4_t& v) noexcept;
	static inline i32x4_t casti8toi32(const i32x4_t& v) noexcept;
	static inline i32x4_t casti8toi24(const i32x4_t& v) noexcept;
	static inline i32x4_t casti8toi16(const i32x4_t& v) noexcept;
	static inline i32x4_t reinterpretftoi(const f32x4_t& v) noexcept;
	static inline f32x4_t reinterpretitof(const i32x4_t& v) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Length
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t length2_squared(const f32x4_t& v) noexcept;
	static inline f32x4_t length3_squared(const f32x4_t& v) noexcept;
	static inline f32x4_t length4_squared(const f32x4_t& v) noexcept;

	static inline float length2_squaredf(const f32x4_t& v) noexcept;
	static inline float length3_squaredf(const f32x4_t& v) noexcept;
	static inline float length4_squaredf(const f32x4_t& v) noexcept;

	static inline f32x4_t length2(const f32x4_t& v) noexcept;
	static inline f32x4_t length3(const f32x4_t& v) noexcept;
	static inline f32x4_t length4(const f32x4_t& v) noexcept;

	static inline float length2f(const f32x4_t& v) noexcept;
	static inline float length3f(const f32x4_t& v) noexcept;
	static inline float length4f(const f32x4_t& v) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Normalize
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t normalize2(const f32x4_t& v) noexcept;
	static inline f32x4_t normalize3(const f32x4_t& v) noexcept;
	static inline f32x4_t normalize4(const f32x4_t& v) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Dot, Product Product
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t dot2(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t dot3(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t dot4(const f32x4_t& v1, const f32x4_t& v2) noexcept;

	static inline float dot2f(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline float dot3f(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline float dot4f(const f32x4_t& v1, const f32x4_t& v2) noexcept;

	static inline i32x4_t dot2(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t dot3(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t dot4(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	static inline int dot2i(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline int dot3i(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline int dot4i(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	static inline f32x4_t cross2(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t cross3(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t cross4(const f32x4_t& v1, const f32x4_t& v2, const f32x4_t& v3) noexcept;

	static inline i32x4_t cross2(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t cross3(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t cross4(const i32x4_t& v1, const i32x4_t& v2, const i32x4_t& v3) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Arithmetic Operations
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t addfv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t subtractfv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t negatefv(const f32x4_t& v) noexcept;
	static inline f32x4_t multiplyfv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t multiplyfv(const f32x4_t& v, float s) noexcept;
	static inline f32x4_t multiplyfv(float s, const f32x4_t& v) noexcept { return multiplyfv(v, s); }
	static inline f32x4_t fmaf(const f32x4_t& mv1, const f32x4_t& mv2, const f32x4_t& av) noexcept;
	static inline f32x4_t fmsf(const f32x4_t& mv1, const f32x4_t& mv2, const f32x4_t& sv) noexcept;
	static inline f32x4_t fnmsf(const f32x4_t& sv, const f32x4_t& mv1, const f32x4_t& mv2) noexcept;
	static inline f32x4_t dividefv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t dividefv(const f32x4_t& v, float s) noexcept;

	static inline i32x4_t add32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t subtract32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t negate32(const i32x4_t& v) noexcept;
	static inline i32x4_t multiply32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t multiply32(const i32x4_t& v, int s) noexcept;
	static inline i32x4_t multiply32(int s, const i32x4_t& v) noexcept { return multiply32(v, s); }

	static inline i32x4_t add16(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t subtract16(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t negate16(const i32x4_t& v) noexcept;
	static inline i32x4_t multiply16(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t multiply16(const i32x4_t& v, int s) noexcept;
	static inline i32x4_t multiply16(int s, const i32x4_t& v) noexcept { return multiply16(v, s); }

	static inline i32x4_t add8(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t subtract8(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t negate8(const i32x4_t& v) noexcept;
	static inline i32x4_t multiply8(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t multiply8(const i32x4_t& v, int s) noexcept;
	static inline i32x4_t multiply8(int s, const i32x4_t& v) noexcept { return multiply8(v, s); }

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Bit Operations
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t andfv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t orfv(const f32x4_t & v1, const f32x4_t & v2) noexcept;
	static inline f32x4_t xorfv(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t notfv(const f32x4_t& v) noexcept;

	static inline i32x4_t and32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t or32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t xor32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t not32(const i32x4_t& v) noexcept;
	static inline i32x4_t shiftl32(const i32x4_t& v, int move) noexcept;
	static inline i32x4_t shiftr32(const i32x4_t& v, int move) noexcept;

	static inline i32x4_t shiftl16(const i32x4_t& v, int move) noexcept;
	static inline i32x4_t shiftr16(const i32x4_t& v, int move) noexcept;

	static inline i32x4_t shiftl8(const i32x4_t& v, int move) noexcept;
	static inline i32x4_t shiftr8(const i32x4_t& v, int move) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Comparer
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t equals(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t not_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t lesser(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t lesser_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t greater(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t greater_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept;

	static inline bool equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline bool not_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline bool lesserb(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline bool lesser_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline bool greaterb(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline bool greater_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept;

	static inline i32x4_t equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t not_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t lesser32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t lesser_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t greater32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t greater_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	static inline bool equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline bool not_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline bool lesser32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline bool lesser_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline bool greater32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline bool greater_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4D Vector Functions
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t sqrt(const f32x4_t& v) noexcept;
	static inline f32x4_t rcp(const f32x4_t& v) noexcept;
	static inline f32x4_t rsqrt(const f32x4_t& v) noexcept;
	static inline f32x4_t round(const f32x4_t& v) noexcept;
	static inline f32x4_t floor(const f32x4_t& v) noexcept;
	static inline f32x4_t ceil(const f32x4_t& v) noexcept;
	static inline f32x4_t abs(const f32x4_t& v) noexcept;
	static inline f32x4_t minimum(const f32x4_t& v1, const f32x4_t& v2) noexcept;
	static inline f32x4_t maximum(const f32x4_t& v1, const f32x4_t& v2) noexcept;

	static inline i32x4_t abs32(const i32x4_t& v) noexcept;
	static inline i32x4_t minv32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t maxv32(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t minimum(const i32x4_t& v1, const i32x4_t& v2) noexcept { return minv32(v1, v2); }
	static inline i32x4_t maximum(const i32x4_t& v1, const i32x4_t& v2) noexcept { return maxv32(v1, v2); }

	static inline i32x4_t abs16(const i32x4_t& v) noexcept;
	static inline i32x4_t minv16(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t maxv16(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	static inline i32x4_t abs8(const i32x4_t& v) noexcept;
	static inline i32x4_t minv8(const i32x4_t& v1, const i32x4_t& v2) noexcept;
	static inline i32x4_t maxv8(const i32x4_t& v1, const i32x4_t& v2) noexcept;

	static inline f32x4_t sin(const f32x4_t& v) noexcept;
	static inline f32x4_t cos(const f32x4_t& v) noexcept;
	static inline void sincos(const f32x4_t& v, f32x4_t* sinOut, f32x4_t* cosOut) noexcept;
	static inline f32x4_t tan(const f32x4_t& v) noexcept;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Matrix Definitions
//
///////////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Definitions
	//
	///////////////////////////////////////////////////////////////////////////////////////////////
	struct DSEEDEXP f32x4x4_t
	{
	public:
		f32x4_t _c1, _c2, _c3, _c4;

	public:
		inline f32x4x4_t() noexcept { }
		inline f32x4x4_t(float v) noexcept : _c1(v), _c2(v), _c3(v), _c4(v) { }
		inline f32x4x4_t(const f32x4_t& c1, const f32x4_t& c2, const f32x4_t& c3, const f32x4_t& c4) noexcept
			: _c1(c1), _c2(c2), _c3(c3), _c4(c4)
		{ }
		inline f32x4x4_t(const float arr[16]) noexcept
			: _c1(arr), _c2(arr + 4), _c3(arr + 8), _c4(arr + 12)
		{ }
		inline f32x4x4_t(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44) noexcept
			: _c1(m11, m12, m13, m14), _c2(m21, m22, m23, m24), _c3(m31, m32, m33, m34), _c4(m41, m42, m43, m44)
		{ }

	public:
		static inline f32x4x4_t load(const float arr[16]) noexcept
		{
			return f32x4x4_t(arr);
		}
		static inline void store(float arr[16], const f32x4x4_t* v) noexcept
		{
			f32x4_t::store(arr, &v->_c1);
			f32x4_t::store(arr + 4, &v->_c2);
			f32x4_t::store(arr + 8, &v->_c3);
			f32x4_t::store(arr + 12, &v->_c4);
		}

	public:
		inline f32x4_t& column1() noexcept { return _c1; }
		inline f32x4_t& column2() noexcept { return _c2; }
		inline f32x4_t& column3() noexcept { return _c3; }
		inline f32x4_t& column4() noexcept { return _c4; }

		inline const f32x4_t& column1() const noexcept { return _c1; }
		inline const f32x4_t& column2() const noexcept { return _c2; }
		inline const f32x4_t& column3() const noexcept { return _c3; }
		inline const f32x4_t& column4() const noexcept { return _c4; }

	public:
		inline float m11() const noexcept { return _c1.x(); }
		inline float m12() const noexcept { return _c1.y(); }
		inline float m13() const noexcept { return _c1.z(); }
		inline float m14() const noexcept { return _c1.w(); }

		inline float m21() const noexcept { return _c2.x(); }
		inline float m22() const noexcept { return _c2.y(); }
		inline float m23() const noexcept { return _c2.z(); }
		inline float m24() const noexcept { return _c2.w(); }

		inline float m31() const noexcept { return _c3.x(); }
		inline float m32() const noexcept { return _c3.y(); }
		inline float m33() const noexcept { return _c3.z(); }
		inline float m34() const noexcept { return _c3.w(); }

		inline float m41() const noexcept { return _c4.x(); }
		inline float m42() const noexcept { return _c4.y(); }
		inline float m43() const noexcept { return _c4.z(); }
		inline float m44() const noexcept { return _c4.w(); }
	} ALIGNPACK(4);

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Transpose, Invert
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4x4_t transpose(const f32x4x4_t& m) noexcept;
	static inline float determinant(const f32x4x4_t& m) noexcept;
	static inline f32x4x4_t invert(const f32x4x4_t& m, float* determinant) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Arithmetic Operations
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4x4_t addfv(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t subtractfv(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t negatefv(const f32x4x4_t& m) noexcept;
	static inline f32x4x4_t multiplyfv(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t multiplyfv(const f32x4x4_t& m, float s) noexcept;
	static inline f32x4x4_t multiplyfv(float s, const f32x4x4_t& m) noexcept { return multiplyfv(m, s); }
	static inline f32x4x4_t dividefv(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t dividefv(const f32x4x4_t& m, float s) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Bit Operations
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4x4_t andfv(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept;
	static inline f32x4x4_t orfv(const f32x4x4_t & v1, const f32x4x4_t & v2) noexcept;
	static inline f32x4x4_t xorfv(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept;
	static inline f32x4x4_t notfv(const f32x4x4_t& v) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Comparer
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4x4_t equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t not_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t lesser(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t lesser_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t greater(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline f32x4x4_t greater_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;

	static inline bool equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline bool not_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline bool lesserb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline bool lesser_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline bool greaterb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;
	static inline bool greater_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Functions
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4x4_t sqrt(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t rcp(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t rsqrt(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t round(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t floor(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t ceil(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t abs(const f32x4x4_t& v) noexcept;
	static inline f32x4x4_t minimum(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept;
	static inline f32x4x4_t maximum(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 4x4 Matrix Transform
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	static inline f32x4_t transform2(const f32x4_t& v, const f32x4x4_t& m) noexcept;
	static inline f32x4_t transform3(const f32x4_t& v, const f32x4x4_t& m) noexcept;
	static inline f32x4_t transform4(const f32x4_t& v, const f32x4x4_t& m) noexcept;

	static inline f32x4_t transform_normal2(const f32x4_t& v, const f32x4x4_t& m) noexcept;
	static inline f32x4_t transform_normal3(const f32x4_t& v, const f32x4x4_t& m) noexcept;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Operator Overloads
//
///////////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4_t operator+(const f32x4_t& v1, const f32x4_t& v2) noexcept { return addfv(v1, v2); }
	static inline f32x4_t operator-(const f32x4_t& v1, const f32x4_t& v2) noexcept { return subtractfv(v1, v2); }
	static inline f32x4_t operator-(const f32x4_t& v) noexcept { return negatefv(v); }
	static inline f32x4_t operator*(const f32x4_t& v1, const f32x4_t& v2) noexcept { return multiplyfv(v1, v2); }
	static inline f32x4_t operator*(const f32x4_t& v, float s) noexcept { return multiplyfv(v, s); }
	static inline f32x4_t operator*(float s, const f32x4_t& v) noexcept { return multiplyfv(s, v); }
	static inline f32x4_t operator/(const f32x4_t& v1, const f32x4_t& v2) noexcept { return dividefv(v1, v2); }
	static inline f32x4_t operator/(const f32x4_t& v, float s) noexcept { return dividefv(v, s); }
	static inline f32x4_t operator&(const f32x4_t& v1, const f32x4_t& v2) noexcept { return andfv (v1, v2); }
	static inline f32x4_t operator|(const f32x4_t& v1, const f32x4_t& v2) noexcept { return orfv (v1, v2); }
	static inline f32x4_t operator^(const f32x4_t& v1, const f32x4_t& v2) noexcept { return xorfv (v1, v2); }
	static inline f32x4_t operator~(const f32x4_t& v) noexcept { return notfv(v); }
	static inline f32x4_t operator<<(const f32x4_t& v, int move) noexcept { return reinterpretitof(shiftl32(reinterpretftoi(v), move)); }
	static inline f32x4_t operator>>(const f32x4_t& v, int move) noexcept { return reinterpretitof(shiftr32(reinterpretftoi(v), move)); }
	static inline f32x4_t operator==(const f32x4_t& v1, const f32x4_t& v2) noexcept { return equalsb(v1, v2); }
	static inline f32x4_t operator!=(const f32x4_t& v1, const f32x4_t& v2) noexcept { return not_equalsb(v1, v2); }
	static inline f32x4_t operator<(const f32x4_t& v1, const f32x4_t& v2) noexcept { return lesserb(v1, v2); }
	static inline f32x4_t operator<=(const f32x4_t& v1, const f32x4_t& v2) noexcept { return lesser_equalsb(v1, v2); }
	static inline f32x4_t operator>(const f32x4_t& v1, const f32x4_t& v2) noexcept { return greaterb(v1, v2); }
	static inline f32x4_t operator>=(const f32x4_t& v1, const f32x4_t& v2) noexcept { return greater_equalsb(v1, v2); }

	static inline f32x4_t operator*(const f32x4_t& v, const f32x4x4_t& m) noexcept { return transform4(v, m); }

	static inline i32x4_t operator+(const i32x4_t& v1, const i32x4_t& v2) noexcept { return add32(v1, v2); }
	static inline i32x4_t operator-(const i32x4_t& v1, const i32x4_t& v2) noexcept { return subtract32(v1, v2); }
	static inline i32x4_t operator-(const i32x4_t& v) noexcept { return negate32(v); }
	static inline i32x4_t operator*(const i32x4_t& v1, const i32x4_t& v2) noexcept { return multiply32(v1, v2); }
	static inline i32x4_t operator*(const i32x4_t& v, int s) noexcept { return multiply32(v, s); }
	static inline i32x4_t operator*(int s, const i32x4_t& v) noexcept { return multiply32(s, v); }
	static inline i32x4_t operator/(const i32x4_t& v1, const i32x4_t& v2) noexcept { return castf32toi32(dividefv(casti32tof32(v1), casti32tof32(v2))); }
	static inline i32x4_t operator/(const i32x4_t& v, int s) noexcept { return castf32toi32(dividefv(casti32tof32(v), (float)s)); }
	static inline i32x4_t operator&(const i32x4_t& v1, const i32x4_t& v2) noexcept { return and32(v1, v2); }
	static inline i32x4_t operator|(const i32x4_t& v1, const i32x4_t& v2) noexcept { return or32(v1, v2); }
	static inline i32x4_t operator^(const i32x4_t& v1, const i32x4_t& v2) noexcept { return xor32(v1, v2); }
	static inline i32x4_t operator~(const i32x4_t& v) noexcept { return not32(v); }
	static inline i32x4_t operator<<(const i32x4_t& v, int move) noexcept { return shiftl32(v, move); }
	static inline i32x4_t operator>>(const i32x4_t& v, int move) noexcept { return shiftr32(v, move); }
	static inline i32x4_t operator==(const i32x4_t& v1, const i32x4_t& v2) noexcept { return equals32b(v1, v2); }
	static inline i32x4_t operator!=(const i32x4_t& v1, const i32x4_t& v2) noexcept { return not_equals32b(v1, v2); }
	static inline i32x4_t operator<(const i32x4_t& v1, const i32x4_t& v2) noexcept { return lesser32b(v1, v2); }
	static inline i32x4_t operator<=(const i32x4_t& v1, const i32x4_t& v2) noexcept { return lesser_equals32b(v1, v2); }
	static inline i32x4_t operator>(const i32x4_t& v1, const i32x4_t& v2) noexcept { return greater32b(v1, v2); }
	static inline i32x4_t operator>=(const i32x4_t& v1, const i32x4_t& v2) noexcept { return greater_equals32b(v1, v2); }
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 4x4 Matrix Operator Overloads
//
///////////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4x4_t operator+(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return addfv(v1, v2); }
	static inline f32x4x4_t operator-(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return subtractfv(v1, v2); }
	static inline f32x4x4_t operator-(const f32x4x4_t& v) noexcept { return negatefv(v); }
	static inline f32x4x4_t operator*(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return multiplyfv(v1, v2); }
	static inline f32x4x4_t operator*(const f32x4x4_t& v, float s) noexcept { return multiplyfv(v, s); }
	static inline f32x4x4_t operator*(float s, const f32x4x4_t& v) noexcept { return multiplyfv(s, v); }
	static inline f32x4x4_t operator/(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return dividefv(v1, v2); }
	static inline f32x4x4_t operator/(const f32x4x4_t& v, float s) noexcept { return dividefv(v, s); }
	static inline f32x4x4_t operator&(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return andfv (v1, v2); }
	static inline f32x4x4_t operator|(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return orfv (v1, v2); }
	static inline f32x4x4_t operator^(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return xorfv (v1, v2); }
	static inline f32x4x4_t operator~(const f32x4x4_t& v) noexcept { return notfv(v); }
	static inline f32x4x4_t operator<<(const f32x4x4_t& v, int move) noexcept
	{
		return f32x4x4_t(
			v.column1() << move,
			v.column2() << move,
			v.column3() << move,
			v.column4() << move
		);
	}
	static inline f32x4x4_t operator>>(const f32x4x4_t& v, int move) noexcept
	{
		return f32x4x4_t(
			v.column1() >> move,
			v.column2() >> move,
			v.column3() >> move,
			v.column4() >> move
		);
	}
	static inline f32x4x4_t operator==(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return equalsb(v1, v2); }
	static inline f32x4x4_t operator!=(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return not_equalsb(v1, v2); }
	static inline f32x4x4_t operator<(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return lesserb(v1, v2); }
	static inline f32x4x4_t operator<=(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return lesser_equalsb(v1, v2); }
	static inline f32x4x4_t operator>(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return greaterb(v1, v2); }
	static inline f32x4x4_t operator>=(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept { return greater_equalsb(v1, v2); }
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector andfv 4x4 Matrix Implementations
//
///////////////////////////////////////////////////////////////////////////////////////////////
#	include "simd.constructors.inl"
#	include "simd.shuffle.inl"
#	include "simd.cast.inl"
#	include "simd.length.inl"
#	include "simd.normalize.inl"
#	include "simd.product.inl"
#	include "simd.arithmetic.inl"
#	include "simd.bit.inl"
#	include "simd.compare.inl"
#	include "simd.functions.inl"
#	include "simd.matrixfuncs.inl"
#	include "simd.transform.inl"

#endif