///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Bit Operations
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
#if !DONT_USE_SSE
	static inline f32x4_t andfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_and_ps(v1, v2); }
	static inline f32x4_t orfv (const f32x4_t & v1, const f32x4_t & v2) noexcept { return _mm_or_ps(v1, v2); }
	static inline f32x4_t xorfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_xor_ps(v1, v2); }
	static inline f32x4_t notfv(const f32x4_t& v) noexcept { return reinterpretitof(not32(reinterpretftoi(v))); }

	static inline i32x4_t and32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_and_si128(v1, v2); }
	static inline i32x4_t or32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_or_si128(v1, v2); }
	static inline i32x4_t xor32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_xor_si128(v1, v2); }
	static inline i32x4_t not32(const i32x4_t& v) noexcept { return _mm_xor_si128(v, _mm_cmpeq_epi32(v, v)); }
	static inline i32x4_t shiftl32(const i32x4_t& v, int move) noexcept { return _mm_slli_epi32(v, move); }
	static inline i32x4_t shiftr32(const i32x4_t& v, int move) noexcept { return _mm_srai_epi32(v, move); }

	static inline i32x4_t shiftl16(const i32x4_t& v, int move) noexcept { return _mm_slli_epi16(v, move); }
	static inline i32x4_t shiftr16(const i32x4_t& v, int move) noexcept { return _mm_srai_epi16(v, move); }
#elif !DONT_USE_NEON
	static inline f32x4_t andfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return vandq_s32(v1, v2); }
	static inline f32x4_t orfv (const f32x4_t & v1, const f32x4_t & v2) noexcept { return vorrq_s32(v1, v2); }
	static inline f32x4_t xorfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return veorq_s32(v1, v2); }
	static inline f32x4_t notfv(const f32x4_t& v) noexcept { return vmvnq_s32(v); }

	static inline i32x4_t and32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vandq_s32(v1, v2); }
	static inline i32x4_t or32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vorrq_s32(v1, v2); }
	static inline i32x4_t xor32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return veorq_s32(v1, v2); }
	static inline i32x4_t not32(const i32x4_t& v) noexcept { return vmvnq_s32(v); }
	static inline i32x4_t shiftl32(const i32x4_t& v, int move) noexcept { return vshlq_n_s32(v, s); }
	static inline i32x4_t shiftr32(const i32x4_t& v, int move) noexcept { return vshrq_n_s32(v, s); }

	static inline i32x4_t shiftl16(const i32x4_t& v, int move) noexcept { return vshlq_n_s16(v, move); }
	static inline i32x4_t shiftr16(const i32x4_t& v, int move) noexcept { return vshrq_n_s16(v, move); }
#elif DONT_USE_SIMD
	static inline f32x4_t andfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return reinterpretitof(and32(reinterpretftoi(v1), reinterpretftoi(v2))); }
	static inline f32x4_t orfv (const f32x4_t & v1, const f32x4_t & v2) noexcept { return reinterpretitof(or32(reinterpretftoi(v1), reinterpretftoi(v2))); }
	static inline f32x4_t xorfv (const f32x4_t& v1, const f32x4_t& v2) noexcept { return reinterpretitof(xor32(reinterpretftoi(v1), reinterpretftoi(v2))); }
	static inline f32x4_t notfv(const f32x4_t& v) noexcept { return reinterpretitof(not32(reinterpretftoi(v))); }

	static inline i32x4_t and32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return i32x4_t(v1.x() & v2.x(), v1.y() & v2.y(), v1.z() & v2.z(), v1.w() & v2.w()); }
	static inline i32x4_t or32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return i32x4_t(v1.x() | v2.x(), v1.y() | v2.y(), v1.z() | v2.z(), v1.w() | v2.w()); }
	static inline i32x4_t xor32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return i32x4_t(v1.x() ^ v2.x(), v1.y() ^ v2.y(), v1.z() ^ v2.z(), v1.w() ^ v2.w()); }
	static inline i32x4_t not32(const i32x4_t& v) noexcept { return i32x4_t(~v.x(), ~v.y(), ~v.z(), ~v.w()); }
	static inline i32x4_t shiftl32(const i32x4_t& v, int move) noexcept { return i32x4_t(v.x() << move, v.y() << move, v.z() << move, v.w() << move); }
	static inline i32x4_t shiftr32(const i32x4_t& v, int move) noexcept { return i32x4_t(v.x() >> move, v.y() >> move, v.z() >> move, v.w() >> move); }

	static inline i32x4_t shiftl16(const i32x4_t& v, int move) noexcept
	{
		const short* arr = (short*)&v;
		short result[8] = {
			arr[0] << move,
			arr[1] << move,
			arr[2] << move,
			arr[3] << move,
			arr[4] << move,
			arr[5] << move,
			arr[6] << move,
			arr[7] << move
		};
		return i32x4_t(result);
	}
	static inline i32x4_t shiftr16(const i32x4_t& v, int move) noexcept
	{
		const short* arr = (short*)&v;
		short result[8] = {
			arr[0] >> move,
			arr[1] >> move,
			arr[2] >> move,
			arr[3] >> move,
			arr[4] >> move,
			arr[5] >> move,
			arr[6] >> move,
			arr[7] >> move
		};
		return i32x4_t(arr);
	}
#endif

	static inline i32x4_t shiftl8(const i32x4_t& v, int move) noexcept { return and32(shiftl16(v, move), (0xff00 << move) & 0xff); }
	static inline i32x4_t shiftr8(const i32x4_t& v, int move) noexcept { return and32(shiftr16(v, move), (0xff >> move) & 0xff00); }
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 4x4 Matrix Bit Operations
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4x4_t andfv (const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept
	{
		return f32x4x4_t(
			andfv (v1.column1(), v2.column1()),
			andfv (v1.column2(), v2.column2()),
			andfv (v1.column3(), v2.column3()),
			andfv (v1.column4(), v2.column4())
		);
	}
	static inline f32x4x4_t orfv (const f32x4x4_t & v1, const f32x4x4_t & v2) noexcept
	{
		return f32x4x4_t(
			orfv (v1.column1(), v2.column1()),
			orfv (v1.column2(), v2.column2()),
			orfv (v1.column3(), v2.column3()),
			orfv (v1.column4(), v2.column4())
		);
	}
	static inline f32x4x4_t xorfv (const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept
	{
		return f32x4x4_t(
			xorfv (v1.column1(), v2.column1()),
			xorfv (v1.column2(), v2.column2()),
			xorfv (v1.column3(), v2.column3()),
			xorfv (v1.column4(), v2.column4())
		);
	}
	static inline f32x4x4_t notfv(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			notfv(v.column1()),
			notfv(v.column2()),
			notfv(v.column3()),
			notfv(v.column4())
		);
	}
}