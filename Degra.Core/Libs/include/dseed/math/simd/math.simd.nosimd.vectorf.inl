#ifndef __DSEED_MATH_SIMD_NOSIMD_VECTORF_INL__
#define __DSEED_MATH_SIMD_NOSIMD_VECTORF_INL__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf_def FASTCALL addvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3]);
	}
	inline vectorf_def FASTCALL subtractvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3]);
	}
	inline vectorf_def FASTCALL negatevf (const vectorf_def& v) noexcept
	{
		return vectorf_def (-v.v[0], -v.v[1], -v.v[2], -v.v[3]);
	}
	inline vectorf_def FASTCALL multiplyvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2], v1.v[3] * v2.v[3]);
	}
	inline vectorf_def FASTCALL multiplyvf (const vectorf_def& v, float s) noexcept
	{
		return vectorf_def (v.v[0] * s, v.v[1] * s, v.v[2] * s, v.v[3] * s);
	}
	inline vectorf_def FASTCALL multiplyvf (float s, const vectorf_def& v) noexcept
	{
		return multiplyvf (v, s);
	}
	inline vectorf_def FASTCALL dividevf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2], v1.v[3] / v2.v[3]);
	}
	inline vectorf_def FASTCALL dividevf (const vectorf_def& v, float s) noexcept
	{
		return vectorf_def (v.v[0] / s, v.v[1] / s, v.v[2] / s, v.v[3] / s);
	}
	inline vectorf_def FASTCALL fmavf (const vectorf_def& mv1, const vectorf_def& mv2, const vectorf_def& av) noexcept
	{
		return addvf (multiplyvf (mv1, mv2), av);
	}
	inline vectorf_def FASTCALL fmsvf (const vectorf_def& mv1, const vectorf_def& mv2, const vectorf_def& sv) noexcept
	{
		return subtractvf (multiplyvf (mv1, mv2), sv);
	}
	inline vectorf_def FASTCALL fnmsvf (const vectorf_def& sv, const vectorf_def& mv1, const vectorf_def& mv2) noexcept
	{
		return subtractvf (sv, multiplyvf (mv1, mv2));
	}
	inline vectorf_def FASTCALL andvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (andvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2)));
	}
	inline vectorf_def FASTCALL orvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (orvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2)));
	}
	inline vectorf_def FASTCALL xorvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (xorvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2)));
	}
	inline vectorf_def FASTCALL notvf (const vectorf_def& v) noexcept
	{
		return reinterpret_i32_to_f32 (notvi (reinterpret_f32_to_i32 (v)));
	}
	inline vectorf_def FASTCALL equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () == v2.x () ? -1 : 0,
			v1.y () == v2.y () ? -1 : 0,
			v1.z () == v2.z () ? -1 : 0,
			v1.w () == v2.w () ? -1 : 0));
	}
	inline vectorf_def FASTCALL not_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () != v2.x () ? -1 : 0,
			v1.y () != v2.y () ? -1 : 0,
			v1.z () != v2.z () ? -1 : 0,
			v1.w () != v2.w () ? -1 : 0));
	}
	inline vectorf_def FASTCALL lesservf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () < v2.x () ? -1 : 0,
			v1.y () < v2.y () ? -1 : 0,
			v1.z () < v2.z () ? -1 : 0,
			v1.w () < v2.w () ? -1 : 0));
	}
	inline vectorf_def FASTCALL lesser_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () <= v2.x () ? -1 : 0,
			v1.y () <= v2.y () ? -1 : 0,
			v1.z () <= v2.z () ? -1 : 0,
			v1.w () <= v2.w () ? -1 : 0));
	}
	inline vectorf_def FASTCALL greatervf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () > v2.x () ? -1 : 0,
			v1.y () > v2.y () ? -1 : 0,
			v1.z () > v2.z () ? -1 : 0,
			v1.w () > v2.w () ? -1 : 0));
	}
	inline vectorf_def FASTCALL greater_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return reinterpret_i32_to_f32 (vectori_def (
			v1.x () >= v2.x () ? -1 : 0,
			v1.y () >= v2.y () ? -1 : 0,
			v1.z () >= v2.z () ? -1 : 0,
			v1.w () >= v2.w () ? -1 : 0));
	}

	inline vectorf_def FASTCALL dotvf2d (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.x () * v2.x () + v1.y () * v2.y ());
	}
	inline vectorf_def FASTCALL dotvf3d (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.x () * v2.x () + v1.y () * v2.y () + v1.z () * v2.z ());
	}
	inline vectorf_def FASTCALL dotvf4d (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (v1.x () * v2.x () + v1.y () * v2.y () + v1.z () * v2.z () + v1.w () * v2.w ());
	}

	inline vectorf_def FASTCALL sqrtvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (sqrtf (v.x ()), sqrtf (v.y ()), sqrtf (v.z ()), sqrtf (v.w ()));
	}
	inline vectorf_def FASTCALL rsqrtvf (const vectorf_def& v) noexcept
	{
		return sqrtvf (rcpvf (v));
	}
	inline vectorf_def FASTCALL rcpvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (1 / v.x (), 1 / v.y (), 1 / v.z (), 1 / v.w ());
	}
	inline vectorf_def FASTCALL roundvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (roundf (v.x ()), roundf (v.y ()), roundf (v.z ()), roundf (v.w ()));
	}
	inline vectorf_def FASTCALL floorvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (floorf (v.x ()), floorf (v.y ()), floorf (v.z ()), floorf (v.w ()));
	}
	inline vectorf_def FASTCALL ceilvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (ceilf (v.x ()), ceilf (v.y ()), ceilf (v.z ()), ceilf (v.w ()));
	}
	inline vectorf_def FASTCALL absvf (const vectorf_def& v) noexcept
	{
		return vectorf_def (fabs (v.x ()), fabs (v.y ()), fabs (v.z ()), fabs (v.w ()));
	}
	inline vectorf_def FASTCALL minvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (
			minimum (v1.x (), v2.x ()),
			minimum (v1.y (), v2.y ()),
			minimum (v1.z (), v2.z ()),
			minimum (v1.w (), v2.w ())
		);
	}
	inline vectorf_def FASTCALL maxvf (const vectorf_def& v1, const vectorf_def& v2) noexcept
	{
		return vectorf_def (
			maximum (v1.x (), v2.x ()),
			maximum (v1.y (), v2.y ()),
			maximum (v1.z (), v2.z ()),
			maximum (v1.w (), v2.w ())
		);
	}

	inline vectorf_def FASTCALL selectvf (const vectorf_def& v1, const vectorf_def& v2, const vectorf_def& controlv) noexcept
	{
		vectori_def v1i = reinterpret_f32_to_i32 (v1), v2i = reinterpret_f32_to_i32 (v2);
		vectori_def ci = reinterpret_f32_to_i32 (controlv);
		vectori_def ret (
			(v1i.x () & ~ci.x ()) | (v2i.x () & ci.x ()),
			(v1i.y () & ~ci.y ()) | (v2i.y () & ci.y ()),
			(v1i.z () & ~ci.z ()) | (v2i.z () & ci.z ()),
			(v1i.w () & ~ci.w ()) | (v2i.w () & ci.w ())
		);
		return reinterpret_i32_to_f32 (ret);
	}
	inline vectorf_def FASTCALL selectcontrolvf_def (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept
	{
		uint32_t elements[] = { 0, 0xffffffff };
		return reinterpret_i32_to_f32 (vectori_def (
			elements[v0],
			elements[v1],
			elements[v2],
			elements[v3]
		));
	}
	inline bool FASTCALL inboundsvf3d (const vectorf_def& v, const vectorf_def& bounds) noexcept
	{
		return (((v.x () <= bounds.x () && v.x () >= -bounds.x ()) &&
			(v.y () <= bounds.y () && v.y () >= -bounds.y ()) &&
			(v.z () <= bounds.z () && v.z () >= -bounds.y ())) != 0);
	}
}

#endif