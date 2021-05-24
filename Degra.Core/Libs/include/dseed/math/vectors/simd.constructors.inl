#pragma warning (disable: 26495)

///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{

	inline f32x4_t::f32x4_t() noexcept { }
	inline f32x4_t::f32x4_t(float v) noexcept
#if DONT_USE_SIMD
		: _x(v), _y(v), _z(v), _w(v)
#elif !DONT_USE_SSE
		: _intrinsic(_mm_set1_ps(v))
#elif !DONT_USE_NEON
		: _intrinsic(vmovq_n_f32(v))
#endif
	{ }
	inline f32x4_t::f32x4_t(float x, float y, float z, float w) noexcept
#if DONT_USE_SIMD
		: _x(x), _y(y), _z(z), _w(w)
#elif !DONT_USE_SSE
		: _intrinsic(_mm_set_ps(w, z, y, x))
#endif
	{
#if !DONT_USE_NEON
		float temp[] = { x, y, z, w };
		_intrinsic = vld1q_f32(temp);
#endif
	}
	inline f32x4_t::f32x4_t(const float arr[4]) noexcept
	{
#if !DONT_USE_SSE
		_intrinsic = _mm_load_ps(arr);
#elif !DONT_USE_NEON
		_intrinsic = vld1q_f32(arr);
#elif DONT_USE_SIMD
		_x = arr[0]; _y = arr[1]; _z = arr[2]; _w = arr[3];
#endif
	}
	inline f32x4_t::f32x4_t(const f32x4_t* v) noexcept
#if DONT_USE_SIMD
		: _x(v->_x), _y(v->_y), _z(v->_z), _w(v->_w)
#elif !DONT_USE_SSE || !DONT_USE_NEON
		: _intrinsic(v->_intrinsic)
#endif
	{ }
	inline f32x4_t::f32x4_t(const f32x4_t& v) noexcept
#if DONT_USE_SIMD
		: _x(v._x), _y(v._y), _z(v._z), _w(v._w)
#elif !DONT_USE_SSE || !DONT_USE_NEON
		: _intrinsic(v._intrinsic)
#endif
	{ }
#if !DONT_USE_SSE
	inline f32x4_t::f32x4_t(__m128 v) noexcept : _intrinsic(v) { }
#elif !DONT_USE_NEON
	f32x4_t(float32x4_t v) : _intrinsoc(v) noexcept { }
#endif

	inline f32x4_t f32x4_t::load(const float arr[4]) noexcept
	{
		return f32x4_t(arr);
	}
	inline void f32x4_t::store(float arr[4], const f32x4_t* v) noexcept
	{
#if !DONT_USE_SSE
		_mm_store_ps(arr, v->_intrinsic);
#elif !DONT_USE_NEON
		vst1q_f32(arr, v->_intrinsic);
#elif DONT_USE_SIMD
		memcpy(arr, v, sizeof(vector4f));
#endif
	}

	inline i32x4_t::i32x4_t() noexcept { }
	inline i32x4_t::i32x4_t(int v) noexcept
#if DONT_USE_SIMD
		: _x(v), _y(v), _z(v), _w(v)
#elif !DONT_USE_SSE
		: _intrinsic(_mm_set1_epi32(v))
#elif !DONT_USE_NEON
		: _intrinsic(vmovq_n_i32(v))
#endif
	{ }
	inline i32x4_t::i32x4_t(int x, int y, int z, int w) noexcept
#if DONT_USE_SIMD
		: _x(x), _y(y), _z(z), _w(w)
#elif !DONT_USE_SSE
		: _intrinsic(_mm_set_epi32(w, z, y, x))
#endif
	{
#if !DONT_USE_NEON
		int arr[4] = { x, y, z, w };
		_intrinsic = vld1q_f32(arr);
#endif
	}
	inline i32x4_t::i32x4_t(const int arr[4]) noexcept
	{
#if !DONT_USE_SSE
		_intrinsic = _mm_load_si128((const __m128i*)arr);
#elif !DONT_USE_NEON
		_intrinsic = vld1q_i32(arr);
#elif DONT_USE_SIMD
		_x = arr[0]; _y = arr[1]; _z = arr[2]; _w = arr[3];
#endif
	}
	inline i32x4_t::i32x4_t(const i32x4_t* v) noexcept
#if DONT_USE_SIMD
		: _x(v->_x), _y(v->_y), _z(v->_z), _w(v->_w)
#elif !DONT_USE_SSE
		: _intrinsic(v->_intrinsic)
#elif !DONT_USE_NEON
		: _intrinsic(v->_intrinsic)
#endif
	{ }
	inline i32x4_t::i32x4_t(const i32x4_t& v) noexcept
#if DONT_USE_SIMD
		: _x(v._x), _y(v._y), _z(v._z), _w(v._w)
#elif !DONT_USE_SSE
		: _intrinsic(v._intrinsic)
#elif !DONT_USE_NEON
		: _intrinsic(v._intrinsic)
#endif
	{ }
#if !DONT_USE_SSE
	inline i32x4_t::i32x4_t(__m128i v) noexcept { _intrinsic = v; }
#elif !DONT_USE_NEON
	i32x4_t::i32x4_t(int32x4_t v) noexcept { _intrinsic = v; }
#endif

	inline i32x4_t i32x4_t::load(const int arr[4]) noexcept
	{
		return i32x4_t(arr);
	}
	inline void i32x4_t::store(int arr[4], const i32x4_t* v) noexcept
	{
#if !DONT_USE_SSE
		_mm_store_si128((__m128i*)arr, v->_intrinsic);
#elif !DONT_USE_NEON
		vst1q_i32(arr, v->_intrinsic);
#elif DONT_USE_SIMD
		memcpy(arr, v, sizeof(i32x4_t));
#endif
	}
}