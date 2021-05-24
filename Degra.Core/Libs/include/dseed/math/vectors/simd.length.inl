///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Length
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4_t length2_squared(const f32x4_t& v) noexcept
	{
		return dot2(v, v);
	}
	static inline f32x4_t length3_squared(const f32x4_t& v) noexcept
	{
		return dot3(v, v);
	}
	static inline f32x4_t length4_squared(const f32x4_t& v) noexcept
	{
		return dot4(v, v);
	}

	static inline float length2_squaredf(const f32x4_t& v) noexcept
	{
		return dot2f(v, v);
	}
	static inline float length3_squaredf(const f32x4_t& v) noexcept
	{
		return dot3f(v, v);
	}
	static inline float length4_squaredf(const f32x4_t& v) noexcept
	{
		return dot4f(v, v);
	}

	static inline f32x4_t length2(const f32x4_t& v) noexcept
	{
#if !DONT_USE_SIMD
		return sqrt(length2_squared(v));
#else
		return f32x4_t(length2f(v));
#endif
	}
	static inline f32x4_t length3(const f32x4_t& v) noexcept
	{
#if !DONT_USE_SIMD
		return sqrt(length3_squared(v));
#else
		return f32x4_t(length3f(v));
#endif
	}
	static inline f32x4_t length4(const f32x4_t& v) noexcept
	{
#if !DONT_USE_SIMD
		return sqrt(length4_squared(v));
#else
		return f32x4_t(length4f(v));
#endif
	}

	static inline float length2f(const f32x4_t& v) noexcept
	{
#if DONT_USE_SIMD
		return sqrtf(length2_squaredf(v));
#else
		return length2(v).x();
#endif
	}
	static inline float length3f(const f32x4_t& v) noexcept
	{
#if DONT_USE_SIMD
		return sqrtf(length3_squaredf(v));
#else
		return length3(v).x();
#endif
	}
	static inline float length4f(const f32x4_t& v) noexcept
	{
#if DONT_USE_SIMD
		return sqrtf(length4_squaredf(v));
#else
		return length4(v).x();
#endif
	}
}