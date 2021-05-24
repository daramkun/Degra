///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Functions 1
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
#if !DONT_USE_SSE
	static inline f32x4_t sqrt(const f32x4_t& v) noexcept { return _mm_sqrt_ps(v); }
	static inline f32x4_t rcp(const f32x4_t& v) noexcept { return _mm_rcp_ps(v); }
	static inline f32x4_t rsqrt(const f32x4_t& v) noexcept { return _mm_rsqrt_ps(v); }
	static inline f32x4_t round(const f32x4_t& v) noexcept { return _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC); }
	static inline f32x4_t floor(const f32x4_t& v) noexcept { return _mm_round_ps(v, _MM_FROUND_FLOOR); }
	static inline f32x4_t ceil(const f32x4_t& v) noexcept { return _mm_round_ps(v, _MM_FROUND_CEIL); }
	static inline f32x4_t abs(const f32x4_t& v) noexcept { static auto sign = _mm_set1_epi32(0x7fffffff); return _mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(v), sign)); }
	static inline f32x4_t minimum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_min_ps(v1, v2); }
	static inline f32x4_t maximum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_max_ps(v1, v2); }

	static inline i32x4_t abs32(const i32x4_t& v) noexcept { return _mm_abs_epi32(v); }
	static inline i32x4_t minv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_min_epi32(v1, v2); }
	static inline i32x4_t maxv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_max_epi32(v1, v2); }

	static inline i32x4_t abs16(const i32x4_t& v) noexcept { return _mm_abs_epi16(v); }
	static inline i32x4_t minv16(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_min_epi16(v1, v2); }
	static inline i32x4_t maxv16(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_max_epi16(v1, v2); }

	static inline i32x4_t abs8(const i32x4_t& v) noexcept { return _mm_abs_epi8(v); }
	static inline i32x4_t minv8(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_min_epi8(v1, v2); }
	static inline i32x4_t maxv8(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_max_epi8(v1, v2); }
#elif !DONT_USE_NEON
	static inline f32x4_t sqrt(const f32x4_t& v) noexcept { return vrecpeq_f32(vrsqrteq_f32(v)); }
	static inline f32x4_t rcp(const f32x4_t& v) noexcept { return vrecpeq_f32(v); }
	static inline f32x4_t rsqrt(const f32x4_t& v) noexcept { return vrsqrteq_f32(v); }
	static inline f32x4_t round(const f32x4_t& v) noexcept
	{
		static int32x4_t signExtract = vdupq_n_s32(-2147483648);
		static float32x4_t roundValue = vdupq_n_f32(0.5);

		int32x4_t reinterpretInt = vreinterpretq_s32_f32(v);
		int32x4_t signSignal = vandq_s32(reinterpretInt, signExtract);

		float32x4_t plusValue = vreinterpretq_f32_s32(vorrq_s32(vreinterpretq_s32_f32(roundValue), signSignal));

		return vaddq_f32(v, plusValue);
	}
	static inline f32x4_t floor(const f32x4_t& v) noexcept { return casti32tof32(castf32toi32(v)); }
	static inline f32x4_t ceil(const f32x4_t& v) noexcept { static float32x4_t lessone = vdupq_n_f32(0.99999999f); return floor(add32(v, lessone)); }
	static inline f32x4_t abs(const f32x4_t& v) noexcept { return vabsq_f32(v); }
	static inline f32x4_t minimum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vminq_f32(v1, v2); }
	static inline f32x4_t maximum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vmaxq_f32(v1, v2); }

	static inline i32x4_t abs32(const i32x4_t& v) noexcept { return vabsq_s32(v); }
	static inline i32x4_t minv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vminq_s32(v1, v2); }
	static inline i32x4_t maxv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vmaxq_s32(v1, v2); }

	static inline i32x4_t abs16(const i32x4_t& v) noexcept { return vabsq_s16(v); }
	static inline i32x4_t minv16(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vminq_s16(v1, v2); }
	static inline i32x4_t maxv16(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vmaxq_s16(v1, v2); }

	static inline i32x4_t abs8(const i32x4_t& v) noexcept { return vabsq_s8(v); }
	static inline i32x4_t minv8(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vminq_s8(v1, v2); }
	static inline i32x4_t maxv8(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vmaxq_s8(v1, v2); }
#elif DONT_USE_SIMD
	static inline f32x4_t sqrt(const f32x4_t& v) noexcept { return f32x4_t(sqrtf(v.x()), sqrtf(v.y()), sqrtf(v.z()), sqrtf(v.w())); }
	static inline f32x4_t rcp(const f32x4_t& v) noexcept { return f32x4_t(1 / v.x(), 1 / v.y(), 1 / v.z(), 1 / v.w()); }
	static inline f32x4_t rsqrt(const f32x4_t& v) noexcept { return rcp(sqrt(v)); }
	static inline f32x4_t round(const f32x4_t& v) noexcept { return f32x4_t(roundf(v.x()), roundf(v.y()), roundf(v.z()), roundf(v.w())); }
	static inline f32x4_t floor(const f32x4_t& v) noexcept { return f32x4_t(floorf(v.x()), floorf(v.y()), floorf(v.z()), floorf(v.w())); }
	static inline f32x4_t ceil(const f32x4_t& v) noexcept { return f32x4_t(ceilf(v.x()), ceilf(v.y()), ceilf(v.z()), ceilf(v.w())); }
	static inline f32x4_t abs(const f32x4_t& v) noexcept { return f32x4_t(fabs(v.x()), fabs(v.y()), fabs(v.z()), fabs(v.w())); }
	static inline f32x4_t minimum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return f32x4_t(min(v1.x(), v2.x()), min(v1.y(), v2.y()), min(v1.z(), v2.z()), min(v1.w(), v2.w())); }
	static inline f32x4_t maximum(const f32x4_t& v1, const f32x4_t& v2) noexcept { return f32x4_t(max(v1.x(), v2.x()), max(v1.y(), v2.y()), max(v1.z(), v2.z()), max(v1.w(), v2.w())); }

	static inline i32x4_t abs32(const i32x4_t& v) noexcept { return i32x4_t(v.x() & 0x7fffffff, v.y() & 0x7fffffff, v.z() & 0x7fffffff, v.w() & 0x7fffffff); }
	static inline i32x4_t minv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return i32x4_t(min(v1.x(), v2.x()), min(v1.y(), v2.y()), min(v1.z(), v2.z()), min(v1.w(), v2.w())); }
	static inline i32x4_t maxv32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return i32x4_t(max(v1.x(), v2.x()), max(v1.y(), v2.y()), max(v1.z(), v2.z()), max(v1.w(), v2.w())); }

	static inline i32x4_t abs16(const i32x4_t& v) noexcept
	{
		const short* arr = (const short*)&v;
		short result[8] = {
			arr[0] & 0x7fff,
			arr[1] & 0x7fff,
			arr[2] & 0x7fff,
			arr[3] & 0x7fff,
			arr[4] & 0x7fff,
			arr[5] & 0x7fff,
			arr[6] & 0x7fff,
			arr[7] & 0x7fff,
		};
		return i32x4_t(result);
	}
	static inline i32x4_t minv16(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		const short* arr1 = (const short*)&v1;
		const short* arr2 = (const short*)&v2;
		short result[8] = {
			min(arr1[0], arr2[0]),
			min(arr1[1], arr2[1]),
			min(arr1[2], arr2[2]),
			min(arr1[3], arr2[3]),
			min(arr1[4], arr2[4]),
			min(arr1[5], arr2[5]),
			min(arr1[6], arr2[6]),
			min(arr1[7], arr2[7]),
		};
		return i32x4_t(result);
	}
	static inline i32x4_t maxv16(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		const short* arr1 = (const short*)&v1;
		const short* arr2 = (const short*)&v2;
		short result[8] = {
			max(arr1[0], arr2[0]),
			max(arr1[1], arr2[1]),
			max(arr1[2], arr2[2]),
			max(arr1[3], arr2[3]),
			max(arr1[4], arr2[4]),
			max(arr1[5], arr2[5]),
			max(arr1[6], arr2[6]),
			max(arr1[7], arr2[7]),
		};
		return i32x4_t(result);
	}

	static inline i32x4_t abs8(const i32x4_t& v) noexcept
	{
		const char* arr = (const char*)&v;
		char result[16] = {
			arr[0] & 0x7f,
			arr[1] & 0x7f,
			arr[2] & 0x7f,
			arr[3] & 0x7f,
			arr[4] & 0x7f,
			arr[5] & 0x7f,
			arr[6] & 0x7f,
			arr[7] & 0x7f,
			arr[8] & 0x7f,
			arr[9] & 0x7f,
			arr[10] & 0x7f,
			arr[11] & 0x7f,
			arr[12] & 0x7f,
			arr[13] & 0x7f,
			arr[14] & 0x7f,
			arr[15] & 0x7f,
		};
		return i32x4_t(result);
	}
	static inline i32x4_t minv8(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		const char* arr1 = (const char*)&v1;
		const char* arr2 = (const char*)&v2;
		char result[16] = {
			min(arr1[0], arr2[0]),
			min(arr1[1], arr2[1]),
			min(arr1[2], arr2[2]),
			min(arr1[3], arr2[3]),
			min(arr1[4], arr2[4]),
			min(arr1[5], arr2[5]),
			min(arr1[6], arr2[6]),
			min(arr1[7], arr2[7]),
			min(arr1[8], arr2[8]),
			min(arr1[9], arr2[9]),
			min(arr1[10], arr2[10]),
			min(arr1[11], arr2[11]),
			min(arr1[12], arr2[12]),
			min(arr1[13], arr2[13]),
			min(arr1[14], arr2[14]),
			min(arr1[15], arr2[15]),
		};
		return i32x4_t(result);
	}
	static inline i32x4_t maxv8(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		const char* arr1 = (const char*)&v1;
		const char* arr2 = (const char*)&v2;
		char result[16] = {
			max(arr1[0], arr2[0]),
			max(arr1[1], arr2[1]),
			max(arr1[2], arr2[2]),
			max(arr1[3], arr2[3]),
			max(arr1[4], arr2[4]),
			max(arr1[5], arr2[5]),
			max(arr1[6], arr2[6]),
			max(arr1[7], arr2[7]),
			max(arr1[8], arr2[8]),
			max(arr1[9], arr2[9]),
			max(arr1[10], arr2[10]),
			max(arr1[11], arr2[11]),
			max(arr1[12], arr2[12]),
			max(arr1[13], arr2[13]),
			max(arr1[14], arr2[14]),
			max(arr1[15], arr2[15]),
		};
		return i32x4_t(result);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Functions 2
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
#if !DONT_USE_SSE
	static inline f32x4_t sin(const f32x4_t& v) noexcept
	{
		f32x4_t sin;
		sincos(v, &sin, nullptr);
		return sin;
	}
	static inline f32x4_t cos(const f32x4_t& v) noexcept
	{
		f32x4_t cos;
		sincos(v, nullptr, &cos);
		return cos;
	}
	static inline void sincos(const f32x4_t& v, f32x4_t* sinOut, f32x4_t* cosOut) noexcept
	{
		// http://gruntthepeon.free.fr/ssemath/sse_mathfun.h
		static auto sign_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000)), sign_mask_inv = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)),
			quater_pi = _mm_set1_ps(4 / 3.14159265359f), onef = _mm_set1_ps(1), half = _mm_set1_ps(0.5f);
		static auto one = _mm_set1_epi32(1), one_inv = _mm_set1_epi32(~1), two = _mm_set1_epi32(2), four = _mm_set1_epi32(4);
		static auto minus_cephes_dp1 = _mm_set1_ps(-0.78515625f)
			, minus_cephes_dp2 = _mm_set1_ps(static_cast<float>(-2.4187564849853515625e-4))
			, minus_cephes_dp3 = _mm_set1_ps(static_cast<float>(-3.77489497744594108e-8));
		static auto sincof_p0 = _mm_set1_ps(static_cast<float>(-1.9515295891e-4))
			, sincof_p1 = _mm_set1_ps(static_cast<float>(8.3321608736e-3))
			, sincof_p2 = _mm_set1_ps(static_cast<float>(-1.6666654611e-1));
		static auto coscof_p0 = _mm_set1_ps(static_cast<float>(2.443315711809948e-005))
			, coscof_p1 = _mm_set1_ps(static_cast<float>(-1.388731625493765e-003))
			, coscof_p2 = _mm_set1_ps(static_cast<float>(4.166664568298827e-002));

		__m128 xmm1, xmm2 = _mm_setzero_ps(), xmm3, sign_bit_sin, y;
		__m128i emm0, emm2, emm4;

		sign_bit_sin = v;
		auto x = _mm_and_ps(v, sign_mask_inv);
		sign_bit_sin = _mm_and_ps(sign_bit_sin, sign_mask);

		y = _mm_mul_ps(x, quater_pi);

		emm2 = _mm_cvttps_epi32(y);

		emm2 = _mm_add_epi32(emm2, one);
		emm2 = _mm_and_si128(emm2, one_inv);
		y = _mm_cvtepi32_ps(emm2);

		emm4 = emm2;

		emm0 = _mm_and_si128(emm2, four);
		emm0 = _mm_slli_epi32(emm0, 29);
		auto swap_sign_bit_sin = _mm_castsi128_ps(emm0);

		emm2 = _mm_and_si128(emm2, two);
		emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
		auto poly_mask = _mm_castsi128_ps(emm2);

		xmm1 = minus_cephes_dp1;
		xmm2 = minus_cephes_dp2;
		xmm3 = minus_cephes_dp3;
		xmm1 = _mm_mul_ps(y, xmm1);
		xmm2 = _mm_mul_ps(y, xmm2);
		xmm3 = _mm_mul_ps(y, xmm3);
		x = _mm_add_ps(x, xmm1);
		x = _mm_add_ps(x, xmm2);
		x = _mm_add_ps(x, xmm3);

		emm4 = _mm_sub_epi32(emm4, two);
		emm4 = _mm_andnot_si128(emm4, four);
		emm4 = _mm_slli_epi32(emm4, 29);
		auto sign_bit_cos = _mm_castsi128_ps(emm4);

		sign_bit_sin = _mm_xor_ps(sign_bit_sin, swap_sign_bit_sin);

		__m128 z = _mm_mul_ps(x, x);
		y = coscof_p0;

		y = _mm_mul_ps(y, z);
		y = _mm_add_ps(y, coscof_p1);
		y = _mm_mul_ps(y, z);
		y = _mm_add_ps(y, coscof_p2);
		y = _mm_mul_ps(y, z);
		y = _mm_mul_ps(y, z);
		__m128 tmp = _mm_mul_ps(z, half);
		y = _mm_sub_ps(y, tmp);
		y = _mm_add_ps(y, onef);

		auto y2 = sincof_p0;
		y2 = _mm_mul_ps(y2, z);
		y2 = _mm_add_ps(y2, sincof_p1);
		y2 = _mm_mul_ps(y2, z);
		y2 = _mm_add_ps(y2, sincof_p2);
		y2 = _mm_mul_ps(y2, z);
		y2 = _mm_mul_ps(y2, x);
		y2 = _mm_add_ps(y2, x);

		xmm3 = poly_mask;
		auto ysin2 = _mm_and_ps(xmm3, y2);
		auto ysin1 = _mm_andnot_ps(xmm3, y);
		y2 = _mm_sub_ps(y2, ysin2);
		y = _mm_sub_ps(y, ysin1);

		xmm1 = _mm_add_ps(ysin1, ysin2);
		xmm2 = _mm_add_ps(y, y2);

		if (sinOut) *sinOut = _mm_xor_ps(xmm1, sign_bit_sin);
		if (cosOut) *cosOut = _mm_xor_ps(xmm2, sign_bit_cos);
	}
	static inline f32x4_t tan(const f32x4_t& v) noexcept
	{
		f32x4_t sin, cos;
		sincos(v, &sin, &cos);
		return dividefv(sin, cos);
	}
#elif !DONT_USE_NEON
	static inline f32x4_t sin(const f32x4_t& v) noexcept
	{
		f32x4_t sin;
		sincos(v, &sin, nullptr);
		return sin;
	}
	static inline f32x4_t cos(const f32x4_t& v) noexcept
	{
		f32x4_t cos;
		sincos(v, nullptr, &cos);
		return cos;
	}
	static inline void sincos(const f32x4_t& v, f32x4_t* sinOut, f32x4_t* cosOut) noexcept
	{
		// http://gruntthepeon.free.fr/ssemath/neon_mathfun.h
		static float32x4_t quater_pi = vdupq_n_f32(4 / 3.14159265359f), onef = vdupq_n_f32(1), half = vdupq_n_f32(0.5f);
		static uint32x4_t one = vdupq_n_u32(1), one_inv = vdupq_n_u32(~1), two = vdupq_n_u32(2), four = vdupq_n_u32(4);
		static float32x4_t minus_cephes_dp1 = vdupq_n_f32(-0.78515625f)
			, minus_cephes_dp2 = vdupq_n_f32((float)-2.4187564849853515625e-4)
			, minus_cephes_dp3 = vdupq_n_f32((float)-3.77489497744594108e-8);
		static float32x4_t sincof_p0 = vdupq_n_f32((float)-1.9515295891e-4)
			, sincof_p1 = vdupq_n_f32((float)8.3321608736e-3)
			, sincof_p2 = vdupq_n_f32((float)-1.6666654611e-1);
		static float32x4_t coscof_p0 = vdupq_n_f32((float)2.443315711809948e-005)
			, coscof_p1 = vdupq_n_f32((float)-1.388731625493765e-003)
			, coscof_p2 = vdupq_n_f32((float)4.166664568298827e-002);

		float32x4_t xmm1, xmm2, xmm3, y;

		uint32x4_t emm2;

		uint32x4_t sign_mask_sin, sign_mask_cos;
		sign_mask_sin = vcltq_f32(x, zero);
		x = vabsq_f32(x);

		y = vmulq_f32(x, vdupq_n_f32(quater_pi));

		emm2 = vcvtq_u32_f32(y);
		emm2 = vaddq_u32(emm2, one);
		emm2 = vandq_u32(emm2, one_inv);
		y = vcvtq_f32_u32(emm2);

		uint32x4_t poly_mask = vtstq_u32(emm2, two);

		xmm1 = vmulq_n_f32(y, minus_cephes_dp1);
		xmm2 = vmulq_n_f32(y, minus_cephes_dp2);
		xmm3 = vmulq_n_f32(y, minus_cephes_dp3);
		x = vaddq_f32(x, xmm1);
		x = vaddq_f32(x, xmm2);
		x = vaddq_f32(x, xmm3);

		sign_mask_sin = veorq_u32(sign_mask_sin, vtstq_u32(emm2, four));
		sign_mask_cos = vtstq_u32(vsubq_u32(emm2, two), four);

		float32x4_t z = vmulq_f32(x, x);
		float32x4_t y1, y2;

		y1 = vmulq_n_f32(z, coscof_p0);
		y2 = vmulq_n_f32(z, sincof_p0);
		y1 = vaddq_f32(y1, vdupq_n_f32(coscof_p1));
		y2 = vaddq_f32(y2, vdupq_n_f32(sincof_p1));
		y1 = vmulq_f32(y1, z);
		y2 = vmulq_f32(y2, z);
		y1 = vaddq_f32(y1, vdupq_n_f32(coscof_p2));
		y2 = vaddq_f32(y2, vdupq_n_f32(sincof_p2));
		y1 = vmulq_f32(y1, z);
		y2 = vmulq_f32(y2, z);
		y1 = vmulq_f32(y1, z);
		y2 = vmulq_f32(y2, x);
		y1 = vsubq_f32(y1, vmulq_f32(z, half));
		y2 = vaddq_f32(y2, x);
		y1 = vaddq_f32(y1, onef);

		/* select the correct result from the two polynoms */
		float32x4_t ys = vbslq_f32(poly_mask, y1, y2);
		float32x4_t yc = vbslq_f32(poly_mask, y2, y1);

		if (sinOut) *sinOut = vbslq_f32(sign_mask_sin, vnegq_f32(ys), ys);
		if (cosOut) *cosOut = vbslq_f32(sign_mask_cos, yc, vnegq_f32(yc));
	}
	static inline f32x4_t tan(const f32x4_t& v) noexcept
	{
		f32x4_t sin, cos;
		sincos(v, &sin, &cos);
		return dividefv(sin, cos);
	}
#elif DONT_USE_SIMD
	static inline f32x4_t sin(const f32x4_t& v) noexcept { return f32x4_t(sinf(v.x()), sinf(v.y()), sinf(v.z()), sinf(v.w())); }
	static inline f32x4_t cos(const f32x4_t& v) noexcept { return f32x4_t(cosf(v.x()), cosf(v.y()), cosf(v.z()), cosf(v.w())); }
	static inline void sincos(const f32x4_t& v, f32x4_t* sinOut, f32x4_t* cosOut) noexcept
	{
		if (sinOut) *sinOut = sin(v);
		if (cosOut) *cosOut = cos(v);
	}
	static inline f32x4_t tan(const f32x4_t& v) noexcept { return f32x4_t(tanf(v.x()), tanf(v.y()), tanf(v.z()), tanf(v.w())); }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 4x4 Matrix Functions
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4x4_t sqrt(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			sqrt(v.column1()),
			sqrt(v.column2()),
			sqrt(v.column3()),
			sqrt(v.column4())
		);
	}
	static inline f32x4x4_t rcp(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			rcp(v.column1()),
			rcp(v.column2()),
			rcp(v.column3()),
			rcp(v.column4())
		);
	}
	static inline f32x4x4_t rsqrt(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			rsqrt(v.column1()),
			rsqrt(v.column2()),
			rsqrt(v.column3()),
			rsqrt(v.column4())
		);
	}
	static inline f32x4x4_t round(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			round(v.column1()),
			round(v.column2()),
			round(v.column3()),
			round(v.column4())
		);
	}
	static inline f32x4x4_t floor(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			floor(v.column1()),
			floor(v.column2()),
			floor(v.column3()),
			floor(v.column4())
		);
	}
	static inline f32x4x4_t ceil(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			ceil(v.column1()),
			ceil(v.column2()),
			ceil(v.column3()),
			ceil(v.column4())
		);
	}
	static inline f32x4x4_t abs(const f32x4x4_t& v) noexcept
	{
		return f32x4x4_t(
			abs(v.column1()),
			abs(v.column2()),
			abs(v.column3()),
			abs(v.column4())
		);
	}
	static inline f32x4x4_t minimum(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept
	{
		return f32x4x4_t(
			minimum(v1.column1(), v2.column1()),
			minimum(v1.column2(), v2.column2()),
			minimum(v1.column3(), v2.column3()),
			minimum(v1.column4(), v2.column4())
		);
	}
	static inline f32x4x4_t maximum(const f32x4x4_t& v1, const f32x4x4_t& v2) noexcept
	{
		return f32x4x4_t(
			maximum(v1.column1(), v2.column1()),
			maximum(v1.column2(), v2.column2()),
			maximum(v1.column3(), v2.column3()),
			maximum(v1.column4(), v2.column4())
		);
	}
}