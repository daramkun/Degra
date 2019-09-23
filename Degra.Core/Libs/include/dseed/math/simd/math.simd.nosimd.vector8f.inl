#ifndef __DSEED_MATH_SIMD_NOSIMD_VECTOR8F_INL__
#define __DSEED_MATH_SIMD_NOSIMD_VECTOR8F_INL__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 256-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vector8f_def FASTCALL addv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3],
			v1.v[4] + v2.v[4], v1.v[5] + v2.v[5], v1.v[6] + v2.v[6], v1.v[7] + v2.v[7]
		);
	}
	inline vector8f_def FASTCALL subtractv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3],
			v1.v[4] - v2.v[4], v1.v[5] - v2.v[5], v1.v[6] - v2.v[6], v1.v[7] - v2.v[7]
		);
	}
	inline vector8f_def FASTCALL negatev8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (-v.v[0], -v.v[1], -v.v[2], -v.v[3], -v.v[4], -v.v[5], -v.v[6], -v.v[7]);
	}
	inline vector8f_def FASTCALL multiplyv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2], v1.v[3] * v2.v[3],
			v1.v[4] * v2.v[4], v1.v[5] * v2.v[5], v1.v[6] * v2.v[6], v1.v[7] * v2.v[7]
		);
	}
	inline vector8f_def FASTCALL multiplyv8f (const vector8f_def& v, float s) noexcept
	{
		return vector8f_def (
			v.v[0] * s, v.v[1] * s, v.v[2] * s, v.v[3] * s,
			v.v[4] * s, v.v[5] * s, v.v[6] * s, v.v[7] * s
		);
	}
	inline vector8f_def FASTCALL multiplyv8f (float s, const vector8f_def& v) noexcept
	{
		return multiplyv8f (v, s);
	}
	inline vector8f_def FASTCALL dividev8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2], v1.v[3] / v2.v[3],
			v1.v[4] / v2.v[4], v1.v[5] / v2.v[5], v1.v[6] / v2.v[6], v1.v[7] / v2.v[7]
		);
	}
	inline vector8f_def FASTCALL dividev8f (const vector8f_def& v, float s) noexcept
	{
		return vector8f_def (
			v.v[0] / s, v.v[1] / s, v.v[2] / s, v.v[3] / s,
			v.v[4] / s, v.v[5] / s, v.v[6] / s, v.v[7] / s
		);
	}
	inline vector8f_def FASTCALL fmav8f (const vector8f_def& mv1, const vector8f_def& mv2, const vector8f_def& av) noexcept
	{
		return addv8f (multiplyv8f (mv1, mv2), av);
	}
	inline vector8f_def FASTCALL fmsv8f (const vector8f_def& mv1, const vector8f_def& mv2, const vector8f_def& sv) noexcept
	{
		return subtractv8f (multiplyv8f (mv1, mv2), sv);
	}
	inline vector8f_def FASTCALL fnmsv8f (const vector8f_def& sv, const vector8f_def& mv1, const vector8f_def& mv2) noexcept
	{
		return subtractv8f (sv, multiplyv8f (mv1, mv2));
	}
	inline vector8f_def FASTCALL andv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (andv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2)));
	}
	inline vector8f_def FASTCALL orv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (orv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2)));
	}
	inline vector8f_def FASTCALL xorv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (xorv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2)));
	}
	inline vector8f_def FASTCALL notv8f (const vector8f_def& v) noexcept
	{
		return reinterpret8_i32_to_f32 (notv8i (reinterpret8_f32_to_i32 (v)));
	}
	inline vector8f_def FASTCALL equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () == v2.x1 () ? -1 : 0,
			v1.y1 () == v2.y1 () ? -1 : 0,
			v1.z1 () == v2.z1 () ? -1 : 0,
			v1.w1 () == v2.w1 () ? -1 : 0,
			v1.x2 () == v2.x2 () ? -1 : 0,
			v1.y2 () == v2.y2 () ? -1 : 0,
			v1.z2 () == v2.z2 () ? -1 : 0,
			v1.w2 () == v2.w2 () ? -1 : 0));
	}
	inline vector8f_def FASTCALL not_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () != v2.x1 () ? -1 : 0,
			v1.y1 () != v2.y1 () ? -1 : 0,
			v1.z1 () != v2.z1 () ? -1 : 0,
			v1.w1 () != v2.w1 () ? -1 : 0,
			v1.x2 () != v2.x2 () ? -1 : 0,
			v1.y2 () != v2.y2 () ? -1 : 0,
			v1.z2 () != v2.z2 () ? -1 : 0,
			v1.w2 () != v2.w2 () ? -1 : 0));
	}
	inline vector8f_def FASTCALL lesserv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () < v2.x1 () ? -1 : 0,
			v1.y1 () < v2.y1 () ? -1 : 0,
			v1.z1 () < v2.z1 () ? -1 : 0,
			v1.w1 () < v2.w1 () ? -1 : 0,
			v1.x2 () < v2.x2 () ? -1 : 0,
			v1.y2 () < v2.y2 () ? -1 : 0,
			v1.z2 () < v2.z2 () ? -1 : 0,
			v1.w2 () < v2.w2 () ? -1 : 0));
	}
	inline vector8f_def FASTCALL lesser_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () <= v2.x1 () ? -1 : 0,
			v1.y1 () <= v2.y1 () ? -1 : 0,
			v1.z1 () <= v2.z1 () ? -1 : 0,
			v1.w1 () <= v2.w1 () ? -1 : 0,
			v1.x2 () <= v2.x2 () ? -1 : 0,
			v1.y2 () <= v2.y2 () ? -1 : 0,
			v1.z2 () <= v2.z2 () ? -1 : 0,
			v1.w2 () <= v2.w2 () ? -1 : 0));
	}
	inline vector8f_def FASTCALL greaterv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () > v2.x1 () ? -1 : 0,
			v1.y1 () > v2.y1 () ? -1 : 0,
			v1.z1 () > v2.z1 () ? -1 : 0,
			v1.w1 () > v2.w1 () ? -1 : 0,
			v1.x2 () > v2.x2 () ? -1 : 0,
			v1.y2 () > v2.y2 () ? -1 : 0,
			v1.z2 () > v2.z2 () ? -1 : 0,
			v1.w2 () > v2.w2 () ? -1 : 0));
	}
	inline vector8f_def FASTCALL greater_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return reinterpret8_i32_to_f32 (vector8i_def (
			v1.x1 () >= v2.x1 () ? -1 : 0,
			v1.y1 () >= v2.y1 () ? -1 : 0,
			v1.z1 () >= v2.z1 () ? -1 : 0,
			v1.w1 () >= v2.w1 () ? -1 : 0,
			v1.x2 () >= v2.x2 () ? -1 : 0,
			v1.y2 () >= v2.y2 () ? -1 : 0,
			v1.z2 () >= v2.z2 () ? -1 : 0,
			v1.w2 () >= v2.w2 () ? -1 : 0));
	}

	inline vector8f_def FASTCALL dotv8f2d (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (dotvf2d (v1.vector1 (), v2.vector1 ()), dotvf2d (v1.vector2 (), v2.vector2 ()));
	}
	inline vector8f_def FASTCALL dotv8f3d (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (dotvf3d (v1.vector1 (), v2.vector1 ()), dotvf3d (v1.vector2 (), v2.vector2 ()));
	}
	inline vector8f_def FASTCALL dotv8f4d (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (dotvf4d (v1.vector1 (), v2.vector1 ()), dotvf4d (v1.vector2 (), v2.vector2 ()));
	}

	inline vector8f_def FASTCALL sqrtv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			sqrtf (v.x1 ()), sqrtf (v.y1 ()), sqrtf (v.z1 ()), sqrtf (v.w1 ()),
			sqrtf (v.x2 ()), sqrtf (v.y2 ()), sqrtf (v.z2 ()), sqrtf (v.w2 ())
		);
	}
	inline vector8f_def FASTCALL rsqrtv8f (const vector8f_def& v) noexcept
	{
		return sqrtv8f (rcpv8f (v));
	}
	inline vector8f_def FASTCALL rcpv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			1 / v.x1 (), 1 / v.y1 (), 1 / v.z1 (), 1 / v.w1 (),
			1 / v.x2 (), 1 / v.y2 (), 1 / v.z2 (), 1 / v.w2 ()
		);
	}
	inline vector8f_def FASTCALL roundv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			roundf (v.x1 ()), roundf (v.y1 ()), roundf (v.z1 ()), roundf (v.w1 ()),
			roundf (v.x2 ()), roundf (v.y2 ()), roundf (v.z2 ()), roundf (v.w2 ())
		);
	}
	inline vector8f_def FASTCALL floorv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			floorf (v.x1 ()), floorf (v.y1 ()), floorf (v.z1 ()), floorf (v.w1 ()),
			floorf (v.x2 ()), floorf (v.y2 ()), floorf (v.z2 ()), floorf (v.w2 ())
		);
	}
	inline vector8f_def FASTCALL ceilv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			ceilf (v.x1 ()), ceilf (v.y1 ()), ceilf (v.z1 ()), ceilf (v.w1 ()),
			ceilf (v.x2 ()), ceilf (v.y2 ()), ceilf (v.z2 ()), ceilf (v.w2 ())
		);
	}
	inline vector8f_def FASTCALL absv8f (const vector8f_def& v) noexcept
	{
		return vector8f_def (
			fabs (v.x1 ()), fabs (v.y1 ()), fabs (v.z1 ()), fabs (v.w1 ()),
			fabs (v.x2 ()), fabs (v.y2 ()), fabs (v.z2 ()), fabs (v.w2 ())
		);
	}
	inline vector8f_def FASTCALL minv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			minimum (v1.x1 (), v2.x1 ()),
			minimum (v1.y1 (), v2.y1 ()),
			minimum (v1.z1 (), v2.z1 ()),
			minimum (v1.w1 (), v2.w1 ()),
			minimum (v1.x2 (), v2.x2 ()),
			minimum (v1.y2 (), v2.y2 ()),
			minimum (v1.z2 (), v2.z2 ()),
			minimum (v1.w2 (), v2.w2 ())
		);
	}
	inline vector8f_def FASTCALL maxv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept
	{
		return vector8f_def (
			maximum (v1.x1 (), v2.x1 ()),
			maximum (v1.y1 (), v2.y1 ()),
			maximum (v1.z1 (), v2.z1 ()),
			maximum (v1.w1 (), v2.w1 ()),
			maximum (v1.x2 (), v2.x2 ()),
			maximum (v1.y2 (), v2.y2 ()),
			maximum (v1.z2 (), v2.z2 ()),
			maximum (v1.w2 (), v2.w2 ())
		);
	}
}

#endif