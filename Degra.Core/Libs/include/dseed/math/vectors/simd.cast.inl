///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Type cast
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline i32x4_t castf32toi32(const f32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtps_epi32(v._intrinsic);
#elif !NO_USE_NEON
		return vcvtq_s32_f32(v._intrinsic);
#elif NO_USE_SIMD
		return i32x4_t(
			(int)v._x,
			(int)v._y,
			(int)v._z,
			(int)v._w
		);
#endif
	}
	static inline f32x4_t casti32tof32(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtepi32_ps(v._intrinsic);
#elif !NO_USE_NEON
		return vcvtq_f32_s32(v._intrinsic);
#elif NO_USE_SIMD
		return f32x4_t(
			(float)v._x,
			(float)v._y,
			(float)v._z,
			(float)v._w
		);
#endif
	}
	static inline i32x4_t casti32toi24(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i32_to_i24_shuffle = _mm_set_epi8(-1, -1, -1, -1, 14, 13, 12, 10, 9, 8, 6, 5, 4, 2, 1, 0);
		return _mm_shuffle_epi8(v._intrinsic, i32_to_i24_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		int24_t temp[6] = { (int24_t)v._x, (int24_t)v._y, (int24_t)v._z, (int24_t)v._w, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti32toi16(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto zero_int = _mm_setzero_si128();
		return _mm_packs_epi32(v._intrinsic, zero_int);
#elif !NO_USE_NEON
		return vcombine_s32(vmovn_s32(v._intrinsic), vdup_n_s16(0));
#elif NO_USE_SIMD
		int16_t temp[8] = { (int16_t)v._x, (int16_t)v._y, (int16_t)v._z, (int16_t)v._w, 0, 0, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti32toi8(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i32_to_i8_shuffle = _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, 8, 4, 0);
		return _mm_shuffle_epi8(v._intrinsic, i32_to_i8_shuffle);
#elif !NO_USE_NEON
		return vcombine_s32(vmovn_s16(casti32toi16(v)), vdup_n_s8(0));
#elif NO_USE_SIMD
		char temp[16] = { (int8_t)v._x, (int8_t)v._y, (int8_t)v._z, (int8_t)v._w, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti24toi32(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i24_to_i32_shuffle = _mm_set_epi8(-1, 11, 10, 9, -1, 8, 7, 6, -1, 5, 4, 3, -1, 2, 1, 0);
		return _mm_shuffle_epi8(v._intrinsic, i24_to_i32_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		const int24_t& original = (const int24_t&)v;
		int32_t temp[4] = { (int32_t)original[0], (int32_t)original[1], (int32_t)original[2], (int32_t)original[3] };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti24toi16(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i24_to_i16_shuffle = _mm_set_epi8(-1, -1, -1, -1, -1, -1, 13, 12, 10, 9, 7, 6, 4, 3, 1, 0);
		return _mm_shuffle_epi8(v._intrinsic, i24_to_i16_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		const int24_t& original = (const int24_t&)v;
		int16_t temp[8] = { (int16_t)original[0], (int16_t)original[1], (int16_t)original[2], (int16_t)original[3], (int16_t)original[4], 0, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti24toi8(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i24_to_i8_shuffle = _mm_set_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, 9, 6, 3, 0);
		return _mm_shuffle_epi8(v._intrinsic, i24_to_i8_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		const int24_t& original = (const int24_t&)v;
		int8_t temp[16] = { (int8_t)original[0], (int8_t)original[1], (int8_t)original[2], (int8_t)original[3], (int8_t)original[4], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti16toi32(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtepi16_epi32(v._intrinsic);
#elif !NO_USE_NEON
		return vmovl_s16(vget_low_s32(v._intrinsic));
#elif NO_USE_SIMD
		const int16_t& original = (const int16_t&)v;
		int32_t temp[4] = { (int32_t)original[0], (int32_t)original[1], (int32_t)original[2], (int32_t)original[3] };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti16toi24(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i16_to_i24_shuffle = _mm_set_epi8(-1, -1, 9, 8, -1, 7, 6, -1, 5, 4, -1, 3, 2, -1, 1, 0);
		return _mm_shuffle_epi8(v._intrinsic, i16_to_i24_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		const int16_t& original = (const int16_t&)v;
		int24_t temp[6] = { (int24_t)original[0], (int24_t)original[1], (int24_t)original[2], (int24_t)original[3], (int24_t)original[4], 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti16toi8(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtepi16_epi8(v._intrinsic);
#elif !NO_USE_NEON
		return vcombine_s32(vmovn_s16(v._intrinsic), vdup_n_s8(0));
#elif NO_USE_SIMD
		const int16_t& original = (const int16_t&)v;
		int8_t temp[16] = { (int8_t)original[0], (int8_t)original[1], (int8_t)original[2], (int8_t)original[3], (int8_t)original[4], (int8_t)original[5], (int8_t)original[6], (int8_t)original[7], 0, 0, 0, 0, 0, 0, 0, 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti8toi32(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtepi8_epi32(v._intrinsic);
#elif !NO_USE_NEON
		return vmovl_s16(vget_low_s32(casti8toi16(v)));
#elif NO_USE_SIMD
		const int8_t& original = (const int8_t&)v;
		int32_t temp[4] = { (int32_t)original[0], (int32_t)original[1], (int32_t)original[2], (int32_t)original[3] };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti8toi24(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		static const auto i8_to_i24_shuffle = _mm_set_epi8(-1, -1, -1, 4, -1, -1, 3, -1, -1, 2, -1, -1, 1, -1, -1, 0);
		return _mm_shuffle_epi8(v._intrinsic, i8_to_i24_shuffle);
#elif NO_USE_SIMD || !NO_USE_NEON
		const int8_t& original = (const int8_t&)v;
		int24_t temp[6] = { (int24_t)original[0], (int24_t)original[1], (int24_t)original[2], (int24_t)original[3], (int24_t)original[4], 0 };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t casti8toi16(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_cvtepi8_epi16(v._intrinsic);
#elif !NO_USE_NEON
		return vmovl_s8(vget_low_s32(v._intrinsic));
#elif NO_USE_SIMD
		const int8_t& original = (const int8_t&)v;
		int16_t temp[8] = { (int16_t)original[0], (int16_t)original[1], (int16_t)original[2], (int16_t)original[3], (int16_t)original[4], (int16_t)original[5], (int16_t)original[6], (int16_t)original[7] };
		return i32x4_t((const int32_t&)temp);
#endif
	}
	static inline i32x4_t reinterpretftoi(const f32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_castps_si128(v._intrinsic);
#elif !NO_USE_NEON
		return vreinterpretq_s32_f32(v._intrinsic);
#elif NO_USE_SIMD
		return &(reinterpret_cast<const i32x4_t&>(v));
#endif
	}
	static inline f32x4_t reinterpretitof(const i32x4_t& v) noexcept
	{
#if !NO_USE_SSE
		return _mm_castsi128_ps(v._intrinsic);
#elif !NO_USE_NEON
		return vreinterpretq_f32_s32(v._intrinsic);
#elif NO_USE_SIMD
		return &(reinterpret_cast<const f32x4_t&>(v));
#endif
	}
}