///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Shuffles
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
#if !DONT_USE_NEON
	namespace __armshuffleutil
	{
		template<uint8_t x, uint8_t y>
		inline float32x2_t __getpart(const float32x4_t& v1, const float32x4_t& v2) const noexcept
		{
			float32x2_t ret;
			if (x <= 0 && x >= 3) ret = vmov_n_f32(vgetq_lane_f32(v1, x));
			else if (x <= 4 && x >= 7) ret = vmov_n_f32(vgetq_lane_f32(v2, 4 - x));
			else assert(true);

			if (y <= 0 && y >= 3) ret = vset_lane_f32(vgetq_lane_f32(v2, y), ret, 1);
			else if (y <= 4 && y >= 7) ret = vset_lane_f32(vgetq_lane_f32(v1, 4 - y), ret, 1);
			else assert(true);

			return ret;
		}
		template<> inline float32x2_t __getpart<0, 1>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vget_low_f32(v1); }
		template<> inline float32x2_t __getpart<2, 3>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vget_high_f32(v1); }
		template<> inline float32x2_t __getpart<4, 5>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vget_low_f32(v2); }
		template<> inline float32x2_t __getpart<6, 7>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vget_high_f32(v2); }
		template<> inline float32x2_t __getpart<1, 0>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vrev64_f32(vget_low_f32(v1)); }
		template<> inline float32x2_t __getpart<3, 2>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vrev64_f32(vget_high_f32(v1)); }
		template<> inline float32x2_t __getpart<5, 4>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vrev64_f32(vget_low_f32(v2)); }
		template<> inline float32x2_t __getpart<7, 6>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vrev64_f32(vget_high_f32(v2)); }
		template<> inline float32x2_t __getpart<0, 0>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_low_f32(v1), 0); }
		template<> inline float32x2_t __getpart<1, 1>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_low_f32(v1), 1); }
		template<> inline float32x2_t __getpart<2, 2>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_high_f32(v1), 0); }
		template<> inline float32x2_t __getpart<3, 3>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_high_f32(v1), 1); }
		template<> inline float32x2_t __getpart<4, 4>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_low_f32(v2), 0); }
		template<> inline float32x2_t __getpart<5, 5>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_low_f32(v2), 1); }
		template<> inline float32x2_t __getpart<6, 6>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_high_f32(v2), 0); }
		template<> inline float32x2_t __getpart<7, 7>(const float32x4_t& v1, const float32x4_t& v2) const noexcept { return vdup_lane_f32(vget_high_f32(v2), 1); }

		template<uint8_t x, uint8_t y>
		inline int32x2_t __getpart(const int32x2_t& v1, const int32x2_t& v2) const noexcept
		{
			int32x2_t ret;
			if (x <= 0 && x >= 3) ret = vmov_n_i32(vgetq_lane_i32(v1, x));
			else if (x <= 4 && x >= 7) ret = vmov_n_i32(vgetq_lane_i32(v2, 4 - x));
			else assert(true);

			if (y <= 0 && y >= 3) ret = vset_lane_i32(vgetq_lane_i32(v2, y), ret, 1);
			else if (y <= 4 && y >= 7) ret = vset_lane_i32(vgetq_lane_i32(v1, 4 - y), ret, 1);
			else assert(true);

			return ret;
		}
		template<> inline int32x2_t __getpart<0, 1>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vget_low_i32(v1); }
		template<> inline int32x2_t __getpart<2, 3>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vget_high_i32(v1); }
		template<> inline int32x2_t __getpart<4, 5>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vget_low_i32(v2); }
		template<> inline int32x2_t __getpart<6, 7>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vget_high_i32(v2); }
		template<> inline int32x2_t __getpart<1, 0>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vrev64_i32(vget_low_i32(v1)); }
		template<> inline int32x2_t __getpart<3, 2>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vrev64_i32(vget_high_i32(v1)); }
		template<> inline int32x2_t __getpart<5, 4>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vrev64_i32(vget_low_i32(v2)); }
		template<> inline int32x2_t __getpart<7, 6>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vrev64_i32(vget_high_i32(v2)); }
		template<> inline int32x2_t __getpart<0, 0>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_low_i32(v1), 0); }
		template<> inline int32x2_t __getpart<1, 1>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_low_i32(v1), 1); }
		template<> inline int32x2_t __getpart<2, 2>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_high_i32(v1), 0); }
		template<> inline int32x2_t __getpart<3, 3>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_high_i32(v1), 1); }
		template<> inline int32x2_t __getpart<4, 4>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_low_i32(v2), 0); }
		template<> inline int32x2_t __getpart<5, 5>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_low_i32(v2), 1); }
		template<> inline int32x2_t __getpart<6, 6>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_high_i32(v2), 0); }
		template<> inline int32x2_t __getpart<7, 7>(const int32x2_t& v1, const int32x2_t& v2) const noexcept { return vdup_lane_i32(vget_high_i32(v2), 1); }
	}
#endif

	template<permute x1, permute y1, permute z1, permute w1>
	inline f32x4_t f32x4_t::shuffle(const f32x4_t& v) noexcept
	{
#if !DONT_USE_SSE
		return _mm_permute_ps(v._intrinsic, static_cast<uint8_t>(_MM_SHUFFLE(
			dseed::to_underlying_type(w1),
			dseed::to_underlying_type(z1),
			dseed::to_underlying_type(y1),
			dseed::to_underlying_type(x1)
		)));
#elif !DONT_USE_NEON
		return shuffle<x1, y1, z1, w1>(this, this);
#elif DONT_USE_SIMD
		return f32x4_t(
			v._arr((int)x1),
			v._arr((int)y1),
			v._arr((int)z1),
			v._arr((int)w1)
		);
#endif
	}
	template<shuffle1 x1, shuffle1 y1, shuffle2 z1, shuffle2 w1>
	inline f32x4_t f32x4_t::shuffle(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		return _mm_shuffle_ps(v1._intrinsic, v2._intrinsic, static_cast<uint8_t>(_MM_SHUFFLE(
			dseed::to_underlying_type(w1),
			dseed::to_underlying_type(z1),
			dseed::to_underlying_type(y1),
			dseed::to_underlying_type(x1)
		)));
#elif !DONT_USE_NEON
		float32x2_t a = __getpart<x1, y1>(v1._intrinsic, v2._intrinsic);
		float32x2_t b = __getpart<z1, w1>(v2._intrinsic, v1._intrinsic);
		return vcombine_f32(a, b);
#elif DONT_USE_SIMD
		return f32x4_t(
			(x1 >= 0 && x1 <= 3) ? v1._arr[(int)x1] : v2._arr[4 - (int)x1],
			(y1 >= 0 && y1 <= 3) ? v1._arr[(int)y1] : v2._arr[4 - (int)y1],
			(z1 >= 0 && z1 <= 3) ? v2._arr[(int)z1] : v1._arr[4 - (int)z1],
			(w1 >= 0 && w1 <= 3) ? v2._arr[(int)w1] : v1._arr[4 - (int)w1]
		);
#endif
	}

	template<permute x1, permute y1, permute z1, permute w1>
	inline i32x4_t i32x4_t::shuffle(const i32x4_t& v) noexcept
	{
#if !DONT_USE_SSE
		return _mm_shuffle_epi32(v._intrinsic, _MM_SHUFFLE(
			dseed::to_underlying_type(w1),
			dseed::to_underlying_type(z1),
			dseed::to_underlying_type(y1),
			dseed::to_underlying_type(x1)
		));
#elif !DONT_USE_NEON
		return shuffle<x1, y1, z1, w1>(this, this);
#elif DONT_USE_SIMD
		return i32x4_t(
			v._arr[(int)x1],
			v._arr[(int)y1],
			v._arr[(int)z1],
			v._arr[(int)w1]
		);
#endif
	}
	template<shuffle1 x1, shuffle1 y1, shuffle2 z1, shuffle2 w1>
	inline i32x4_t i32x4_t::shuffle(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(v1._intrinsic), _mm_castsi128_ps(v2._intrinsic), _MM_SHUFFLE(
			dseed::to_underlying_type(w1),
			dseed::to_underlying_type(z1),
			dseed::to_underlying_type(y1),
			dseed::to_underlying_type(x1)
		)));
#elif !DONT_USE_NEON
		int32x2_t a = __getpart<x1, y1>(v1._intrinsic, v2._intrinsic);
		int32x2_t b = __getpart<z1, w1>(v2._intrinsic, v1._intrinsic);
		return vcombine_i32(a, b);
#elif DONT_USE_SIMD
		return i32x4_t(
			(x1 >= 0 && x1 <= 3) ? v1._arr[(int)x1] : v2._arr[4 - (int)x1],
			(y1 >= 0 && y1 <= 3) ? v1._arr[(int)y1] : v2._arr[4 - (int)y1],
			(z1 >= 0 && z1 <= 3) ? v2._arr[(int)z1] : v1._arr[4 - (int)z1],
			(w1 >= 0 && w1 <= 3) ? v2._arr[(int)w1] : v1._arr[4 - (int)w1]
		);
#endif
	}
	inline i32x4_t i32x4_t::shuffle8(const i32x4_t& v, const i32x4_t& mask) noexcept
	{
#if !DONT_USE_SSE
		return _mm_shuffle_epi8(v, mask);
#elif !DONT_USE_NEON
		// https://github.com/DLTcollab/sse2neon/blob/master/sse2neon.h
		int8x16_t tbl = vreinterpretq_s8_m128i(a);
		uint8x16_t idx = vreinterpretq_u8_m128i(b);
		uint8x16_t idx_masked = vandq_u8(idx, vdupq_n_u8(0x8f));
#	if defined(__aarch64__)
		return vreinterpretq_m128i_s8(vqtbl1q_s8(tbl, idx_masked));
#	elif defined(__GNUC__)
		int8x16_t ret;
		__asm__ __volatile__(
			"vtbl.8  %e[ret], {%e[tbl], %f[tbl]}, %e[idx]\n"
			"vtbl.8  %f[ret], {%e[tbl], %f[tbl]}, %f[idx]\n"
			: [ret] "=&w"(ret)
			: [tbl] "w"(tbl), [idx] "w"(idx_masked));
		return vreinterpretq_m128i_s8(ret);
#	else
		int8x8x2_t a_split = { vget_low_s8(tbl), vget_high_s8(tbl) };
		return vreinterpretq_m128i_s8(
			vcombine_s8(vtbl2_s8(a_split, vget_low_u8(idx_masked)),
				vtbl2_s8(a_split, vget_high_u8(idx_masked))));
#	endif
#elif DONT_USE_SIMD
		const uint8_t* varr = reinterpret_cast<const uint8_t*>(&v);
		const uint8_t* maskarr = reinterpret_cast<const uint8_t*>(&mask);
		uint8_t* result[16];
		for (int i = 0; i < 16; ++i)
			result[i] = varr[maskarr[i]];
		return i32x4_t(reinterpret_cast<const int32_t*>(result));
#endif
	}
}