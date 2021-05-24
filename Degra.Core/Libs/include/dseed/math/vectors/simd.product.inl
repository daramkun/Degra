///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Dot, Product Product
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4_t dot2(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		return _mm_dp_ps(v1, v2, 0x3f);
#elif !DONT_USE_NEON
		float32x2_t temp = vmul_f32(vget_low_f32(v1), vget_low_f32(v2));
		temp = vadd_f32(temp, temp);
		return vcombine_f32(temp, temp);
#elif DONT_USE_SIMD
		return f32x4_t(dot2f(v1, v2));
#endif
	}
	static inline f32x4_t dot3(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		return _mm_dp_ps(v1, v2, 0x7f);
#elif !DONT_USE_NEON
		float32x4_t temp = vmulq_f32(v1, v2);
		float32x2_t tv1 = vpadd_f32(vget_low_f32(temp), vget_low_f32(temp));
		float32x2_t tv2 = vdup_lane_f32(vget_high_f32(temp), 0);
		tv1 = vadd_f32(tv1, tv2);
		return vcombine_f32(tv1, tv1);
#elif DONT_USE_SIMD
		return f32x4_t(dot3f(v1, v2));
#endif
	}
	static inline f32x4_t dot4(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		return _mm_dp_ps(v1, v2, 0xff);
#elif !DONT_USE_NEON
		float32x4_t temp = vmulq_f32(v1, v2);
		float32x2_t tv1 = vget_low_f32(temp);
		float32x2_t tv2 = vget_high_f32(temp);
		tv1 = vadd_f32(tv1, tv2);
		tv1 = vpadd_f32(tv1, tv1);
		return vcombine_f32(tv1, tv1);
#elif DONT_USE_SIMD
		return f32x4_t(dot4f(v1, v2));
#endif
	}

	static inline float dot2f(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot2(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y();
#endif
	}
	static inline float dot3f(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot2(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
#endif
	}
	static inline float dot4f(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot2(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z() + v1.w() * v2.w();
#endif
	}

	static inline i32x4_t dot2(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		const auto temp = multiply32(v1, v2);
		return add32(temp, temp.vy()).vx();
#elif !DONT_USE_NEON
		int32x2_t temp = vmul_s32(vget_low_s32(v1), vget_low_s32(v2));
		temp = vadd_s32(temp, temp);
		return vcombine_s32(temp, temp);
#elif DONT_USE_SIMD
		return i32x4_t(dot2i(v1, v2));
#endif
	}
	static inline i32x4_t dot3(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		const auto temp = multiply32(v1, v2);
		return add32(temp, add32(temp.vy(), temp.vz())).vx();
#elif !DONT_USE_NEON
		int32x4_t temp = vmulq_s32(v1, v2);
		int32x2_t tv1 = vpadd_s32(vget_low_s32(temp), vget_low_s32(temp));
		int32x2_t tv2 = vdup_lane_s32(vget_high_s32(temp), 0);
		tv1 = vadd_s32(tv1, tv2);
		return vcombine_s32(tv1, tv1);
#elif DONT_USE_SIMD
		return i32x4_t(dot3i(v1, v2));
#endif
	}
	static inline i32x4_t dot4(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SSE
		const auto temp = multiply32(v1, v2);
		return add32(temp, add32(temp.vy(), add32(temp.vz(), temp.vw()))).vx();
#elif !DONT_USE_NEON
		int32x4_t temp = vmulq_s32(v1, v2);
		int32x2_t tv1 = vget_low_s32(temp);
		int32x2_t tv2 = vget_high_s32(temp);
		tv1 = vadd_s32(tv1, tv2);
		tv1 = vpadd_s32(tv1, tv1);
		return vcombine_s32(tv1, tv1);
#elif DONT_USE_SIMD
		return i32x4_t(dot4i(v1, v2));
#endif
	}

	static inline int dot2i(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot2(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y();
#endif
	}
	static inline int dot3i(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot3(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
#endif
	}
	static inline int dot4i(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		return dot4(v1, v2).x();
#elif DONT_USE_SIMD
		return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z() + v1.w() * v2.w();
#endif
	}

	static inline f32x4_t cross2(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		const auto v2_ = f32x4_t::shuffle<permute::y, permute::x, permute::w, permute::w>(v2);
		return multiplyfv(v1, v2_);
#elif DONT_USE_SIMD
		return f32x4_t(v1.x() * v2.y(), v1.y() * v2.x(), 0, 0);
#endif
	}
	static inline f32x4_t cross3(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		const auto v1_1 = f32x4_t::shuffle<permute::y, permute::z, permute::x, permute::w>(v1);
		const auto v2_1 = f32x4_t::shuffle<permute::z, permute::x, permute::y, permute::w>(v2);
		const auto v1_2 = f32x4_t::shuffle<permute::z, permute::x, permute::y, permute::w>(v1);
		const auto v2_2 = f32x4_t::shuffle<permute::y, permute::z, permute::x, permute::w>(v2);
		return subtractfv(multiplyfv(v1_1, v2_1), multiplyfv(v1_2, v2_2));
#elif DONT_USE_SIMD
		return f32x4_t(
			(v1.y() * v2.z()) - (v1.z() * v2.y()),
			(v1.z() * v2.x()) - (v1.x() * v2.z()),
			(v1.x() * v2.y()) - (v1.y() * v2.x()),
			0
		);
#endif
	}
	static inline f32x4_t cross4(const f32x4_t& v1, const f32x4_t& v2, const f32x4_t& v3) noexcept
	{
		// NEED TO CONVERT TO SIMD OPERATIONS
		return f32x4_t(
			+(v1.y() * ((v2.z() * v3.w()) - (v2.w() * v3.z()))) - (v1.z() * ((v2.y() * v3.w()) - (v2.w() * v3.y()))) + (v1.w() * ((v2.y() * v3.z()) - (v2.z() * v3.y()))),
			-(v1.x() * ((v2.z() * v3.w()) - (v2.w() * v3.z()))) + (v1.z() * ((v2.x() * v3.w()) - (v2.w() * v3.x()))) - (v1.w() * ((v2.x() * v3.z()) - (v2.z() * v3.x()))),
			+(v1.x() * ((v2.y() * v3.w()) - (v2.w() * v3.y()))) - (v1.y() * ((v2.x() * v3.w()) - (v2.w() * v3.x()))) + (v1.w() * ((v2.x() * v3.y()) - (v2.y() * v3.x()))),
			-(v1.x() * ((v2.y() * v3.z()) - (v2.z() * v3.y()))) + (v1.y() * ((v2.x() * v3.z()) - (v2.z() * v3.x()))) - (v1.z() * ((v2.x() * v3.y()) - (v2.y() * v3.x())))
		);
	}

	static inline i32x4_t cross2(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		const auto v2_ = i32x4_t::shuffle<permute::y, permute::x, permute::w, permute::w>(v2);
		return multiply32(v1, v2_);
#elif DONT_USE_SIMD
		return i32x4_t(v1.x() * v2.y(), v1.y() * v2.x(), 0, 0);
#endif
	}
	static inline i32x4_t cross3(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
#if !DONT_USE_SIMD
		const auto v1_1 = i32x4_t::shuffle<permute::y, permute::z, permute::x, permute::w>(v1);
		const auto v2_1 = i32x4_t::shuffle<permute::z, permute::x, permute::y, permute::w>(v2);
		const auto v1_2 = i32x4_t::shuffle<permute::z, permute::x, permute::y, permute::w>(v1);
		const auto v2_2 = i32x4_t::shuffle<permute::y, permute::z, permute::x, permute::w>(v2);
		return subtract32(multiply32(v1_1, v2_1), multiply32(v1_2, v2_2));
#elif DONT_USE_SIMD
		return i32x4_t(
			(v1.y() * v2.z()) - (v1.z() * v2.y()),
			(v1.z() * v2.x()) - (v1.x() * v2.z()),
			(v1.x() * v2.y()) - (v1.y() * v2.x()),
			0
		);
#endif
	}
	static inline i32x4_t cross4(const i32x4_t& v1, const i32x4_t& v2, const i32x4_t& v3) noexcept
	{
		// NEED TO CONVERT TO SIMD OPERATIONS
		return i32x4_t(
			+(v1.y() * ((v2.z() * v3.w()) - (v2.w() * v3.z()))) - (v1.z() * ((v2.y() * v3.w()) - (v2.w() * v3.y()))) + (v1.w() * ((v2.y() * v3.z()) - (v2.z() * v3.y()))),
			-(v1.x() * ((v2.z() * v3.w()) - (v2.w() * v3.z()))) + (v1.z() * ((v2.x() * v3.w()) - (v2.w() * v3.x()))) - (v1.w() * ((v2.x() * v3.z()) - (v2.z() * v3.x()))),
			+(v1.x() * ((v2.y() * v3.w()) - (v2.w() * v3.y()))) - (v1.y() * ((v2.x() * v3.w()) - (v2.w() * v3.x()))) + (v1.w() * ((v2.x() * v3.y()) - (v2.y() * v3.x()))),
			-(v1.x() * ((v2.y() * v3.z()) - (v2.z() * v3.y()))) + (v1.y() * ((v2.x() * v3.z()) - (v2.z() * v3.x()))) - (v1.z() * ((v2.x() * v3.y()) - (v2.y() * v3.x())))
		);
	}
}