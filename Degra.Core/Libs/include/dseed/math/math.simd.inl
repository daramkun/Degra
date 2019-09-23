#ifndef __DSEED_MATH_SIMD_INL__
#define __DSEED_MATH_SIMD_INL__

#include <cmath>
#include <cassert>

#if COMPILER_MSVC
#	pragma warning (disable : 4556)
#endif

#if COMPILER_MSVC
#	if ARCH_X86SET
#		define FASTCALL										__vectorcall
#	else
#		define FASTCALL										__fastcall
#	endif
#else
#	define FASTCALL											__fastcall
#endif

// Most of Sources from DirectXMath's Collision

#include "simd/math.simd.nosimd.inl"
#include "simd/math.simd.x86.inl"
#include "simd/math.simd.arm.inl"

namespace dseed
{
#if ARCH_X86SET
	using vectorf = vectorf_x86;
	using vectori = vectori_x86;
	using vector8f = vector8f_x86;
	using vector8i = vector8i_x86;
	inline vectorf selectcontrolvf (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept { return selectcontrolvf_x86 (v0, v1, v2, v3); }
#elif ARCH_ARMSET
	using vectorf = vectorf_arm;
	using vectori = vectori_arm;
	using vector8f = vector8f_def;
	using vector8i = vector8i_def;
	inline vectorf selectcontrolvf (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept { return (vectorf)selectcontrolvf_def (v0, v1, v2, v3); }
#else
	using vectorf = vectorf_def;
	using vectori = vectori_def;
	using vector8f = vector8f_def;
	using vector8i = vector8i_def;
	inline vectorf selectcontrolvf (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept { return selectcontrolvf_def (v0, v1, v2, v3); }
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// SIMD-operation Type definitions
	//
	////////////////////////////////////////////////////////////////////////////////////////

	// Shuffle Index
	enum shuffle_t : uint8_t {
		shuffle_ax1 = 0, shuffle_ay1 = 1, shuffle_az1 = 2, shuffle_aw1 = 3,
		shuffle_bx2 = 0, shuffle_by2 = 1, shuffle_bz2 = 2, shuffle_bw2 = 3,
		shuffle_ax2 = 4, shuffle_ay2 = 5, shuffle_az2 = 6, shuffle_aw2 = 7,
		shuffle_bx1 = 4, shuffle_by1 = 5, shuffle_bz1 = 6, shuffle_bw1 = 7,
	};
	// Permute Index
	enum permute_t : uint8_t {
		permute_x = 0, permute_y = 1, permute_z = 2, permute_w = 3,
	};

	// 32-bit Floating Point 4x4 Matrix
	struct matrixf
	{
	public:
		vectorf column1, column2, column3, column4;

	public:
		matrixf () = default;
		inline matrixf (const vectorf& c1, const vectorf& c2, const vectorf& c3, const vectorf& c4)
			: column1 (c1), column2 (c2), column3 (c3), column4 (c4)
		{ }
	};

	inline int FASTCALL movemask_matrix (const matrixf& m) noexcept
	{
		return (movemaskvf (m.column1) & 0x000F)
			+ ((movemaskvf (m.column2) << 8) & 0x00F0)
			+ ((movemaskvf (m.column3) << 16) & 0x0F00)
			+ ((movemaskvf (m.column4) << 24) & 0xF000);
	}

	inline vectorf FASTCALL operator+ (const vectorf& v1, const vectorf& v2) noexcept { return addvf (v1, v2); }
	inline vectorf FASTCALL operator- (const vectorf& v1, const vectorf& v2) noexcept { return subtractvf (v1, v2); }
	inline vectorf FASTCALL operator- (const vectorf& v) noexcept { return negatevf (v); }
	inline vectorf FASTCALL operator* (const vectorf& v1, const vectorf& v2) noexcept { return multiplyvf (v1, v2); }
	inline vectorf FASTCALL operator* (const vectorf& v, float s) noexcept { return multiplyvf (v, s); }
	inline vectorf FASTCALL operator* (float s, const vectorf& v) noexcept { return multiplyvf (s, v); }
	inline vectorf FASTCALL operator/ (const vectorf& v1, const vectorf& v2) noexcept { return dividevf (v1, v2); }
	inline vectorf FASTCALL operator/ (const vectorf& v, float s) noexcept { return dividevf (v, s); }
	inline vectorf FASTCALL operator& (const vectorf& v1, const vectorf& v2) noexcept { return andvf (v1, v2); }
	inline vectorf FASTCALL operator| (const vectorf& v1, const vectorf& v2) noexcept { return orvf (v1, v2); }
	inline vectorf FASTCALL operator^ (const vectorf& v1, const vectorf& v2) noexcept { return xorvf (v1, v2); }
	inline vectorf FASTCALL operator~ (const vectorf& v) noexcept { return notvf (v); }
	inline bool FASTCALL operator== (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (equalsvf (v1, v2)) == 0xF; }
	inline bool FASTCALL operator!= (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (not_equalsvf (v1, v2)) != 0; }
	inline bool FASTCALL operator< (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (lesservf (v1, v2)) == 0xF; }
	inline bool FASTCALL operator<= (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (lesser_equalsvf (v1, v2)) == 0xF; }
	inline bool FASTCALL operator> (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (greatervf (v1, v2)) == 0xF; }
	inline bool FASTCALL operator>= (const vectorf& v1, const vectorf& v2) noexcept { return movemaskvf (greater_equalsvf (v1, v2)) == 0xF; }

	inline vectori FASTCALL operator+ (const vectori& v1, const vectori& v2) noexcept { return addvi (v1, v2); }
	inline vectori FASTCALL operator- (const vectori& v1, const vectori& v2) noexcept { return subtractvi (v1, v2); }
	inline vectori FASTCALL operator- (const vectori& v) noexcept { return negatevi (v); }
	inline vectori FASTCALL operator* (const vectori& v1, const vectori& v2) noexcept { return multiplyvi (v1, v2); }
	inline vectori FASTCALL operator* (const vectori& v, int s) noexcept { return multiplyvi (v, s); }
	inline vectori FASTCALL operator* (int s, const vectori& v) noexcept { return multiplyvi (s, v); }
	inline vectori FASTCALL operator/ (const vectori& v1, const vectori& v2) noexcept { return dividevi (v1, v2); }
	inline vectori FASTCALL operator/ (const vectori& v, int s) noexcept { return dividevi (v, s); }
	inline vectori FASTCALL operator& (const vectori& v1, const vectori& v2) noexcept { return andvi (v1, v2); }
	inline vectori FASTCALL operator| (const vectori& v1, const vectori& v2) noexcept { return orvi (v1, v2); }
	inline vectori FASTCALL operator^ (const vectori& v1, const vectori& v2) noexcept { return xorvi (v1, v2); }
	inline vectori FASTCALL operator~ (const vectori& v) noexcept { return notvi (v); }
	inline bool FASTCALL operator== (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (equalsvi (v1, v2)) == 0xFFFF; }
	inline bool FASTCALL operator!= (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (not_equalsvi (v1, v2)) != 0; }
	inline bool FASTCALL operator< (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (lesservi (v1, v2)) == 0xFFFF; }
	inline bool FASTCALL operator<= (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (lesser_equalsvi (v1, v2)) == 0xFFFF; }
	inline bool FASTCALL operator> (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (greatervi (v1, v2)) == 0xFFFF; }
	inline bool FASTCALL operator>= (const vectori& v1, const vectori& v2) noexcept { return movemaskvi (greater_equalsvi (v1, v2)) == 0xFFFF; }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector type Vector operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	
	inline vectorf FASTCALL crossvf2d (const vectorf& v1, const vectorf& v2) noexcept
	{
		vectorf ret = v2.permute32<1, 0, 1, 0> ();
		ret = ret * v1;
		vectorf temp = ret.splat_y ();
		ret = ret - temp;
		ret = ret.permute32<0, 0, 3, 3> ();
		return ret;
	}
	inline vectorf FASTCALL crossvf3d (const vectorf& v1, const vectorf& v2) noexcept
	{
		vectorf temp1 = v1.permute32<1, 2, 0, 3> ();
		vectorf temp2 = v2.permute32<2, 0, 1, 3> ();
		vectorf ret = temp1 * temp2;
		temp1 = temp1.permute32<1, 2, 0, 3> ();
		temp2 = temp2.permute32<2, 0, 1, 3> ();
		temp1 = temp1 * temp2;
		return ret - temp1;
	}
	inline vectorf FASTCALL crossvf4d (const vectorf& v1, const vectorf& v2, const vectorf& v3) noexcept
	{
		vectorf ret = v2.permute32 <2, 3, 1, 3> ();
		vectorf temp3 = v3.permute32 <3, 2, 3, 1> ();
		ret = ret * temp3;

		vectorf temp2 = v2.permute32 <3, 2, 3, 1> ();
		temp3 = temp3.permute32 <1, 0, 3, 1> ();
		temp2 = temp2 * temp3;
		ret = ret - temp2;

		vectorf temp1 = v1.permute32<1, 0, 0, 0> ();
		ret = ret * temp1;

		temp2 = v2.permute32<1, 3, 0, 2> ();
		temp3 = v3.permute32<3, 0, 3, 0> ();
		temp3 = temp3 * temp2;

		temp2 = temp2.permute32<1, 2, 1, 2> ();
		temp1 = v3.permute32<1, 3, 0, 2> ();
		temp2 = temp2 * temp1;
		temp3 = temp3 - temp2;

		temp1 = v1.permute32 <2, 2, 1, 1> ();
		temp1 = temp1 * temp3;
		ret = ret - temp1;

		temp2 = v2.permute32<1, 2, 0, 1> ();
		temp3 = v3.permute32<2, 0, 1, 0> ();
		temp3 = temp3 * temp2;

		temp2 = temp2.permute32<1, 2, 0, 2> ();
		temp1 = v3.permute32<1, 2, 0, 1> ();
		temp1 = temp1 * temp2;
		temp3 = temp3 - temp1;

		temp1 = v1.permute32<3, 3, 3, 2> ();
		temp3 = temp3 * temp1;
		ret = ret + temp3;

		return ret;
	}

	inline vectorf FASTCALL length_squaredvf2d (const vectorf& v) noexcept
	{
		return dotvf2d (v, v);
	}
	inline vectorf FASTCALL length_squaredvf3d (const vectorf& v) noexcept
	{
		return dotvf3d (v, v);
	}
	inline vectorf FASTCALL length_squaredvf4d (const vectorf& v) noexcept
	{
		return dotvf4d (v, v);
	}
	inline vectorf FASTCALL lengthvf2d (const vectorf& v) noexcept
	{
		return sqrtvf (length_squaredvf2d (v));
	}
	inline vectorf FASTCALL lengthvf3d (const vectorf& v) noexcept
	{
		return sqrtvf (length_squaredvf3d (v));
	}
	inline vectorf FASTCALL lengthvf4d (const vectorf& v) noexcept
	{
		return sqrtvf (length_squaredvf4d (v));
	}

	inline vectorf FASTCALL normalizevf2d (const vectorf& v) noexcept { return v / lengthvf2d (v); }
	inline vectorf FASTCALL normalizevf3d (const vectorf& v) noexcept { return v / lengthvf3d (v); }
	inline vectorf FASTCALL normalizevf4d (const vectorf& v) noexcept { return v / lengthvf4d (v); }
	inline vectorf FASTCALL normalize_plane (const vectorf& v) noexcept { return normalizevf3d (v); }

	inline vectorf FASTCALL is_unit2 (const vectorf& v) noexcept { return equalsvf (length_squaredvf2d (v), vectorf (1)); }
	inline vectorf FASTCALL is_unit3 (const vectorf& v) noexcept { return equalsvf (length_squaredvf3d (v), vectorf (1)); }
	inline vectorf FASTCALL is_unit4 (const vectorf& v) noexcept { return equalsvf (length_squaredvf4d (v), vectorf (1)); }

	template<uint32_t elements>
	inline vectorf FASTCALL shift_leftvf (const vectorf& v) noexcept
	{
		return v.permute32<elements & 3, (elements + 1) & 3, (elements + 2) & 3, (elements + 3) & 3> ();
	}
	template<uint32_t elements>
	inline vectorf FASTCALL shift_rightvf (const vectorf& v) noexcept
	{
		return v.permute32<(4 - elements) & 3, (5 - elements) & 3, (6 - elements) & 3, (7 - elements) & 3> ();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector Type Utility operations
	//
	////////////////////////////////////////////////////////////////////////////////////////

	// Get Power vector (v1^v2)
	inline vectorf FASTCALL powvf (const vectorf& v1, const vectorf& v2) noexcept
	{
//#if ( ARCH_X86SET ) && !defined ( NO_INTRINSIC )
//		return _mm_pow_ps (v1, v2);
//#else
		float v[4] = { powf (v1.x (), v2.x ()), powf (v1.y (), v2.y ()), powf (v1.z (), v2.z ()), powf (v1.w (), v2.w ()) };
		return vectorf (v);
//#endif
	}
	// Get Sine vector (sin(v))
	inline vectorf FASTCALL sinvf (const vectorf& v) noexcept
	{
		return vectorf (sinf (v.x ()), sinf (v.y ()), sinf (v.z ()), sinf (v.w ()));
	}
	inline vectorf FASTCALL sinhvf (const vectorf& v) noexcept
	{
		return vectorf (sinhf (v.x ()), sinhf (v.y ()), sinhf (v.z ()), sinhf (v.w ()));
	}
	// Get Cosine vector (cos(v))
	inline vectorf FASTCALL cosvf (const vectorf& v) noexcept
	{
		return vectorf (cosf (v.x ()), cosf (v.y ()), cosf (v.z ()), cosf (v.w ()));
	}
	inline vectorf FASTCALL coshvf (const vectorf& v) noexcept
	{
		return vectorf (coshf (v.x ()), coshf (v.y ()), coshf (v.z ()), coshf (v.w ()));
	}
	// Get Tangent vector (tan(v))
	inline vectorf FASTCALL tanvf (const vectorf& v) noexcept
	{
		return vectorf (tanf (v.x ()), tanf (v.y ()), tanf (v.z ()), tanf (v.w ()));
	}
	inline vectorf FASTCALL tanhvf (const vectorf& v) noexcept
	{
		return vectorf (tanhf (v.x ()), tanhf (v.y ()), tanhf (v.z ()), tanhf (v.w ()));
	}
	// Get Arc-Sine vector (asin(v))
	inline vectorf FASTCALL asinvf (const vectorf& v) noexcept
	{
		return vectorf (asinf (v.x ()), asinf (v.y ()), asinf (v.z ()), asinf (v.w ()));
	}
	inline vectorf FASTCALL asinhvf (const vectorf& v) noexcept
	{
		return vectorf (asinhf (v.x ()), asinhf (v.y ()), asinhf (v.z ()), asinhf (v.w ()));
	}
	// Get Arc-Cosine vector (acos(v))
	inline vectorf FASTCALL acosvf (const vectorf& v) noexcept
	{
		return vectorf (acosf (v.x ()), acosf (v.y ()), acosf (v.z ()), acosf (v.w ()));
	}
	inline vectorf FASTCALL acoshvf (const vectorf& v) noexcept
	{
		return vectorf (acoshf (v.x ()), acoshf (v.y ()), acoshf (v.z ()), acoshf (v.w ()));
	}
	// Get Arc-Tangent vector (atan(v))
	inline vectorf FASTCALL atanvf (const vectorf& v) noexcept
	{
		return vectorf (atanf (v.x ()), atanf (v.y ()), atanf (v.z ()), atanf (v.w ()));
	}
	inline vectorf FASTCALL atanhvf (const vectorf& v) noexcept
	{
		return vectorf (atanhf (v.x ()), atanhf (v.y ()), atanhf (v.z ()), atanhf (v.w ()));
	}
	inline vectorf FASTCALL atan2vf (const vectorf& y, const vectorf& x) noexcept
	{
		return vectorf (atan2f (y.x (), x.x ()), atan2f (y.y (), x.y ()),
			atan2f (y.z (), x.z ()), atan2f (y.w (), x.w ()));
	}
	template<uint32_t leftRotate, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3>
	inline vectorf FASTCALL insertvf (const vectorf& v1, const vectorf& v2) noexcept
	{
		return selectvf (v1, shift_leftvf<leftRotate> (v2), selectcontrolvf (s0 & 1, s1 & 1, s2 & 2, s3 & 3));
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Quaternion operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf FASTCALL quaternion_add (const vectorf& q1, const vectorf& q2) noexcept
	{
		return addvf (q1, q2);
	}
	inline vectorf FASTCALL quaternion_subtract (const vectorf& q1, const vectorf& q2)noexcept
	{
		return subtractvf (q1, q2);
	}
	inline vectorf FASTCALL quaternion_multiply (const vectorf& q1, const vectorf& q2) noexcept
	{
		vectorf q2x = q2.splat_x ();
		vectorf q2y = q2.splat_y ();
		vectorf q2z = q2.splat_z ();
		vectorf q2w = q2.splat_w ();

		vectorf result = q2w * q1;
		q2x = (q2x * q1.permute32<3, 2, 1, 0> ()) * vectorf (1, -1, 1, -1);
		q2y = (q2y * q1.permute32<1, 0, 3, 2> ()) * vectorf (1, 1, -1, -1);
		q2z = (q2z * q1.permute32<3, 2, 1, 0> ()) * vectorf (-1, 1, 1, -1);
		q2y = q2y + q2z;

		return result + q2x + q2y;
	}
	inline vectorf FASTCALL quaternion_divide (const vectorf& q1, const vectorf& q2) noexcept
	{
		return dividevf (q1, q2);
	}

	inline vectorf FASTCALL lengthq (const vectorf& q) noexcept
	{
		return lengthvf4d (q);
	}
	inline vectorf FASTCALL normalizeq (const vectorf& q) noexcept
	{
		return normalizevf4d (q);
	}
	inline vectorf FASTCALL conjugateq (const vectorf& q) noexcept
	{
		return multiplyvf (q, vectorf (-1, -1, -1, 1));
	}
	inline vectorf invertq (const vectorf& q) noexcept
	{
		vectorf length = lengthvf4d (q);
		vectorf conjugate = conjugateq (q);

		if (length.x () <= single_epsilon)
			return vectorf (0, 0, 0, 0);

		return dividevf (conjugate, length);
	}
	inline vectorf FASTCALL from_matrixq (const matrixf& m) noexcept
	{
		static const vectorf XMPMMP (+1.0f, -1.0f, -1.0f, +1.0f);
		static const vectorf XMMPMP (-1.0f, +1.0f, -1.0f, +1.0f);
		static const vectorf XMMMPP (-1.0f, -1.0f, +1.0f, +1.0f);

		vectorf r0 = m.column1;
		vectorf r1 = m.column2;
		vectorf r2 = m.column3;

		vectorf r00 = r0.splat_x ();
		vectorf r11 = r1.splat_y ();
		vectorf r22 = r2.splat_z ();

		vectorf r11mr00 = r11 - r00;
		vectorf x2gey2 = lesser_equalsvf (r11mr00, vectorf (0, 0, 0, 0));

		vectorf r11pr00 = r11 + r00;
		vectorf z2gew2 = lesser_equalsvf (r11pr00, vectorf (0, 0, 0, 0));

		vectorf x2py2gez2pw2 = lesser_equalsvf (r22, vectorf (0, 0, 0, 0));

		vectorf t0 = XMPMMP * r00;
		vectorf t1 = XMMPMP * r11;
		vectorf t2 = XMMMPP * r22;

		vectorf x2y2z2w2 = t0 + t1;
		x2y2z2w2 = t2 + x2y2z2w2;
		x2y2z2w2 = x2y2z2w2 + vectorf (1, 1, 1, 1);

		t0 = vectorf::shuffle32<1, 2, 2, 1> (r0, r1);
		t1 = vectorf::shuffle32<0, 0, 0, 1> (r1, r2);
		t1 = t1.permute32<0, 2, 3, 1> ();
		vectorf xyxzyz = t0 + t1;

		t0 = vectorf::shuffle32<1, 0, 0, 0> (r2, r1);
		t1 = vectorf::shuffle32<2, 2, 2, 1> (r1, r0);
		t1 = t1.permute32<0, 2, 3, 1> ();
		vectorf xwywzw = t0 - t1;
		xwywzw = XMMPMP * xwywzw;

		t0 = vectorf::shuffle32<0, 1, 0, 0> (x2y2z2w2, xyxzyz);
		t1 = vectorf::shuffle32<2, 3, 2, 0> (x2y2z2w2, xwywzw);
		t2 = vectorf::shuffle32<1, 2, 0, 1> (xyxzyz, xwywzw);

		vectorf tensor0 = vectorf::shuffle32<0, 2, 0, 2> (t0, t2);
		vectorf tensor1 = vectorf::shuffle32<2, 1, 1, 3> (t0, t2);
		vectorf tensor2 = vectorf::shuffle32<0, 1, 0, 2> (t2, t1);
		vectorf tensor3 = vectorf::shuffle32<2, 3, 2, 1> (t2, t1);

		t0 = x2gey2 & tensor0;
		t1 = ~x2gey2 & tensor1;
		t0 = t0 | t1;
		t1 = z2gew2 & tensor2;
		t2 = ~z2gew2 & tensor3;
		t1 = t1 | t2;
		t0 = x2py2gez2pw2 & t0;
		t1 = ~x2py2gez2pw2 & t1;
		t2 = t0 | t1;

		t0 = lengthvf4d (t2);
		return t2 / t0;
	}
	inline vectorf FASTCALL from_yaw_pitch_rollq (float yaw, float pitch, float roll) noexcept
	{
		static vectorf sign = vectorf (1, -1, -1, 1);
		vectorf halfAngles = vectorf (pitch, yaw, roll, 0) * vectorf (0.5f, 0.5f, 0.5f, 0.5f);

		vectorf sinAngles = sinvf (halfAngles);
		vectorf cosAngles = cosvf (halfAngles);

		vectorf p0 = vectorf::shuffle32<shuffle_ax1, shuffle_bx1, shuffle_bx2, shuffle_bx2> (sinAngles, cosAngles);
		vectorf y0 = vectorf::shuffle32<shuffle_ax1, shuffle_bx1, shuffle_ax2, shuffle_ax2> (cosAngles, sinAngles);
		vectorf r0 = vectorf::shuffle32<shuffle_ax1, shuffle_ax1, shuffle_bx2, shuffle_ax2> (cosAngles, sinAngles);
		vectorf p1 = vectorf::shuffle32<shuffle_ax1, shuffle_bx1, shuffle_bx2, shuffle_bx2> (cosAngles, sinAngles);
		vectorf y1 = vectorf::shuffle32<shuffle_ax1, shuffle_bx1, shuffle_ax2, shuffle_ax2> (sinAngles, cosAngles);
		vectorf r1 = vectorf::shuffle32<shuffle_ax1, shuffle_ax1, shuffle_bx2, shuffle_ax2> (sinAngles, cosAngles);

		return (p0 * y0 * r0) + (p1 * y1 * r1);
	}
	inline matrixf FASTCALL to_matrixq (const vectorf& q) noexcept
	{
		static vectorf constant1110 = vectorf (1, 1, 1, 0);

		vectorf q0 = q + q;
		vectorf q1 = q * q0;

		vectorf v0 = vectorf::shuffle32<1, 0, 4, 3> (q1, constant1110);
		vectorf v1 = vectorf::shuffle32<2, 2, 5, 3> (q1, constant1110);
		vectorf r0 = constant1110 - v0;
		r0 = r0 - v1;

		v0 = q.permute32<0, 0, 1, 3> ();
		v1 = q0.permute32<2, 1, 2, 3> ();
		v0 = v0 * v1;

		v1 = q.splat_w ();
		vectorf v2 = q0.permute32<1, 2, 0, 3> ();
		v1 = v1 * v2;

		vectorf r1 = v0 + v1;
		vectorf r2 = v0 - v1;

		v0 = vectorf::shuffle32<1, 4, 1, 7> (r1, r2);
		v1 = vectorf::shuffle32<0, 6, 4, 2> (r1, r2);

		matrixf ret;
		ret.column1 = vectorf::shuffle32<0, 4, 1, 7> (r0, v0);
		ret.column2 = vectorf::shuffle32<2, 1, 3, 7> (r0, v0);
		ret.column3 = vectorf::shuffle32<4, 5, 6, 7> (r0, v1);
		ret.column4 = vectorf (0, 0, 0, 1);
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Matrix type Arithmetic operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline matrixf FASTCALL addmf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 + m2.column1, m1.column2 + m2.column2, m1.column3 + m2.column3, m1.column4 + m2.column4);
	}
	inline matrixf FASTCALL subtractmf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 + m2.column1, m1.column2 + m2.column2, m1.column3 + m2.column3, m1.column4 + m2.column4);
	}
	inline matrixf FASTCALL negatemf (const matrixf& m) noexcept
	{
		return matrixf (-m.column1, -m.column2, -m.column3, -m.column4);
	}
	inline matrixf FASTCALL multiplymf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (
			fmavf (m1.column1.splat_x (), m2.column1,
				fmavf (m1.column1.splat_y (), m2.column2,
					fmavf (m1.column1.splat_z (), m2.column3,
						m1.column1.splat_w () * m2.column4))),
			fmavf (m1.column2.splat_x (), m2.column1,
				fmavf (m1.column2.splat_y (), m2.column2,
					fmavf (m1.column2.splat_z (), m2.column3,
						m1.column2.splat_w () * m2.column4))),
			fmavf (m1.column3.splat_x (), m2.column1,
				fmavf (m1.column3.splat_y (), m2.column2,
					fmavf (m1.column3.splat_z (), m2.column3,
						m1.column3.splat_w () * m2.column4))),
			fmavf (m1.column4.splat_x (), m2.column1,
				fmavf (m1.column4.splat_y (), m2.column2,
					fmavf (m1.column4.splat_z (), m2.column3,
						m1.column4.splat_w () * m2.column4)))
		);
	}
	inline matrixf FASTCALL multiplymf (const matrixf& m, float s) noexcept
	{
		return matrixf (m.column1 * s, m.column2 * s, m.column3 * s, m.column4 * s);
	}
	inline matrixf FASTCALL multiplymf (float s, const matrixf& m) noexcept
	{
		return multiplymf (m, s);
	}
	inline matrixf FASTCALL dividemf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 / m2.column1, m1.column2 / m2.column2, m1.column3 / m2.column3, m1.column4 / m2.column4);
	}
	inline matrixf FASTCALL dividemf (const matrixf& m, float s) noexcept
	{
		return matrixf (m.column1 / s, m.column2 / s, m.column3 / s, m.column4 / s);
	}
	inline matrixf FASTCALL andmf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 & m2.column1, m1.column2 & m2.column2, m1.column3 & m2.column3, m1.column4 & m2.column4);
	}
	inline matrixf FASTCALL ormf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 | m2.column1, m1.column2 | m2.column2, m1.column3 | m2.column3, m1.column4 | m2.column4);
	}
	inline matrixf FASTCALL xormf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (m1.column1 ^ m2.column1, m1.column2 ^ m2.column2, m1.column3 ^ m2.column3, m1.column4 ^ m2.column4);
	}
	inline matrixf FASTCALL equalsmf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (equalsvf (m1.column1, m2.column1), equalsvf (m1.column2, m2.column2),
			equalsvf (m1.column3, m2.column3), equalsvf (m1.column4, m2.column4));
	}
	inline matrixf FASTCALL not_equalsmf (const matrixf& m1, const matrixf& m2) noexcept
	{
		return matrixf (equalsvf (m1.column1, m2.column1), not_equalsvf (m1.column2, m2.column2),
			not_equalsvf (m1.column3, m2.column3), not_equalsvf (m1.column4, m2.column4));
	}

	inline matrixf FASTCALL operator+ (const matrixf& m1, const matrixf& m2) noexcept { return addmf (m1, m2); }
	inline matrixf FASTCALL operator- (const matrixf& m1, const matrixf& m2) noexcept { return subtractmf (m1, m2); }
	inline matrixf FASTCALL operator- (const matrixf& m) noexcept { return negatemf (m); }
	inline matrixf FASTCALL operator* (const matrixf& m1, const matrixf& m2) noexcept { return multiplymf (m1, m2); }
	inline matrixf FASTCALL operator* (const matrixf& m, float s) noexcept { return multiplymf (m, s); }
	inline matrixf FASTCALL operator* (float s, const matrixf& m) noexcept { return multiplymf (s, m); }
	inline matrixf FASTCALL operator/ (const matrixf& m1, const matrixf& m2) noexcept { return dividemf (m1, m2); }
	inline matrixf FASTCALL operator/ (const matrixf& m, float s) noexcept { return dividemf (m, s); }
	inline matrixf FASTCALL operator& (const matrixf& m1, const matrixf& m2) noexcept { return andmf (m1, m2); }
	inline matrixf FASTCALL operator| (const matrixf& m1, const matrixf& m2) noexcept { return ormf (m1, m2); }
	inline matrixf FASTCALL operator^ (const matrixf& m1, const matrixf& m2) noexcept { return xormf (m1, m2); }
	inline bool FASTCALL operator== (const matrixf& m1, const matrixf& m2) noexcept { return movemask_matrix (equalsmf (m1, m2)) == 0xFFFF; }
	inline bool FASTCALL operator!= (const matrixf& m1, const matrixf& m2) noexcept { return movemask_matrix (not_equalsmf (m1, m2)) != 0; }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Matrix type Matrix operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline matrixf FASTCALL transposemf (const matrixf& m) noexcept
	{
		vectorf temp0 = vectorf::shuffle32<0, 1, 0, 1> (m.column1, m.column2);
		vectorf temp2 = vectorf::shuffle32<2, 3, 2, 3> (m.column1, m.column2);
		vectorf temp1 = vectorf::shuffle32<0, 1, 0, 1> (m.column3, m.column4);
		vectorf temp3 = vectorf::shuffle32<2, 3, 2, 3> (m.column3, m.column4);

		matrixf ret;
		ret.column1 = vectorf::shuffle32<0, 2, 0, 2> (temp0, temp1);
		ret.column2 = vectorf::shuffle32<1, 3, 1, 3> (temp0, temp1);
		ret.column3 = vectorf::shuffle32<0, 2, 0, 2> (temp2, temp3);
		ret.column4 = vectorf::shuffle32<1, 3, 1, 3> (temp2, temp3);

		return ret;
	}
	inline float FASTCALL determinantmf (const matrixf& m) noexcept
	{
		float m11 = m.column1.x (), m12 = m.column1.y (), m13 = m.column1.z (), m14 = m.column1.w ()
			, m21 = m.column2.x (), m22 = m.column2.y (), m23 = m.column2.z (), m24 = m.column2.w ();
		float m31 = m.column3.x (), m32 = m.column3.y (), m33 = m.column3.z (), m34 = m.column3.w ()
			, m41 = m.column4.x (), m42 = m.column4.y (), m43 = m.column4.z (), m44 = m.column4.w ();

		float n18 = (m33 * m44) - (m34 * m43), n17 = (m32 * m44) - (m34 * m42);
		float n16 = (m32 * m43) - (m33 * m42), n15 = (m31 * m44) - (m34 * m41);
		float n14 = (m31 * m43) - (m33 * m41), n13 = (m31 * m42) - (m32 * m41);

		return (((m11 * (((m22 * n18) - (m23 * n17)) + (m24 * n16))) -
			(m12 * (((m21 * n18) - (m23 * n15)) + (m24 * n14)))) +
			(m13 * (((m21 * n17) - (m22 * n15)) + (m24 * n13)))) -
			(m14 * (((m21 * n16) - (m22 * n14)) + (m23 * n13)));
	}
	inline matrixf FASTCALL invertmf (const matrixf& m, float& det) noexcept
	{
		static vectorf one = vectorf (1);

		matrixf transposed = transposemf (m);
		vectorf v00 = transposed.column3.permute32<0, 0, 1, 1> ();
		vectorf v10 = transposed.column4.permute32<2, 3, 2, 3> ();
		vectorf v01 = transposed.column1.permute32<0, 0, 1, 1> ();
		vectorf v11 = transposed.column2.permute32<2, 3, 2, 3> ();
		vectorf v02 = vectorf::shuffle32<0, 2, 0, 2> (transposed.column3, transposed.column1);
		vectorf v12 = vectorf::shuffle32<1, 3, 1, 3> (transposed.column4, transposed.column2);

		vectorf d0 = v00 * v10;
		vectorf d1 = v01 * v11;
		vectorf d2 = v02 * v12;

		v00 = transposed.column3.permute32<2, 3, 2, 3> ();
		v10 = transposed.column4.permute32<0, 0, 1, 1> ();
		v01 = transposed.column1.permute32<2, 3, 2, 3> ();
		v11 = transposed.column2.permute32<0, 0, 1, 1> ();
		v02 = vectorf::shuffle32<1, 3, 1, 3> (transposed.column3, transposed.column1);
		v12 = vectorf::shuffle32<0, 2, 0, 2> (transposed.column4, transposed.column2);

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;

		d0 = d0 - v00;
		d1 = d1 - v01;
		d2 = d2 - v02;

		v11 = vectorf::shuffle32<1, 3, 1, 1> (d0, d2);
		v00 = transposed.column2.permute32<1, 2, 0, 1> ();
		v10 = vectorf::shuffle32<2, 0, 3, 0> (v11, d0);
		v01 = transposed.column1.permute32<2, 0, 1, 0> ();
		v11 = vectorf::shuffle32<1, 2, 1, 2> (v11, d0);

		vectorf v13 = vectorf::shuffle32<1, 3, 3, 3> (d1, d2);
		v02 = transposed.column4.permute32<1, 2, 0, 1> ();
		v12 = vectorf::shuffle32<2, 0, 3, 0> (v13, d1);
		vectorf v03 = transposed.column3.permute32<2, 0, 1, 0> ();
		v13 = vectorf::shuffle32<1, 2, 1, 2> (v13, d1);

		vectorf c0 = v00 * v10;
		vectorf c2 = v01 * v11;
		vectorf c4 = v02 * v12;
		vectorf c6 = v03 * v13;

		v11 = vectorf::shuffle32<0, 1, 0, 0> (d0, d2);
		v00 = transposed.column2.permute32<2, 3, 1, 2> ();
		v10 = vectorf::shuffle32<3, 0, 1, 2> (d0, v11);
		v01 = transposed.column1.permute32<3, 2, 3, 1> ();
		v11 = vectorf::shuffle32<2, 1, 2, 0> (d0, v11);

		v13 = vectorf::shuffle32<0, 1, 2, 2> (d1, d2);
		v02 = transposed.column4.permute32<2, 3, 1, 2> ();
		v12 = vectorf::shuffle32<3, 0, 1, 2> (d1, v13);
		v03 = transposed.column3.permute32<3, 2, 3, 1> ();
		v13 = vectorf::shuffle32<2, 1, 2, 0> (d1, v13);

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;
		v03 = v03 * v13;
		c0 = c0 - v00;
		c2 = c2 - v01;
		c4 = c4 - v02;
		c6 = c6 - v03;

		v00 = transposed.column2.permute32<3, 0, 3, 0> ();
		v10 = vectorf::shuffle32<2, 2, 0, 1> (d0, d2);
		v10 = v10.permute32<0, 3, 2, 0> ();
		v01 = transposed.column1.permute32<1, 3, 0, 2> ();
		v11 = vectorf::shuffle32<0, 3, 0, 1> (d0, d2);
		v11 = v11.permute32<3, 0, 1, 2> ();
		v02 = transposed.column4.permute32<3, 0, 3, 0> ();
		v12 = vectorf::shuffle32<2, 2, 2, 3> (d1, d2);
		v12 = v12.permute32<0, 3, 2, 0> ();
		v03 = transposed.column3.permute32<1, 3, 0, 2> ();
		v13 = vectorf::shuffle32<0, 3, 2, 3> (d1, d2);
		v13 = v13.permute32<3, 0, 1, 2> ();

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;
		v03 = v03 * v13;
		vectorf c1 = c0 - v00;
		c0 = c0 + v00;
		vectorf c3 = c2 + v01;
		c2 = c2 - v01;
		vectorf c5 = c4 - v02;
		c4 = c4 + v02;
		vectorf c7 = c6 + v03;
		c6 = c6 - v03;

		c0 = vectorf::shuffle32<0, 2, 1, 3> (c0, c1);
		c2 = vectorf::shuffle32<0, 2, 1, 3> (c2, c3);
		c4 = vectorf::shuffle32<0, 2, 1, 3> (c4, c5);
		c6 = vectorf::shuffle32<0, 2, 1, 3> (c6, c7);
		c0 = c0.permute32<0, 2, 1, 3> ();
		c2 = c2.permute32<0, 2, 1, 3> ();
		c4 = c4.permute32<0, 2, 1, 3> ();
		c6 = c6.permute32<0, 2, 1, 3> ();

		vectorf temp = vectorf (dotvf4d (c0, transposed.column1));
		det = temp.x ();
		temp = one / temp;

		matrixf ret;
		ret.column1 = c0 * temp;
		ret.column2 = c2 * temp;
		ret.column3 = c4 * temp;
		ret.column4 = c6 * temp;

		return ret;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector * Quaternion Operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf FASTCALL rotatevf3d (const vectorf& v, const vectorf& q) noexcept
	{
		return (conjugateq (q) * v) * q;
	}
	inline vectorf FASTCALL inverse_rotatevf3d (const vectorf& v, const vectorf& q) noexcept
	{
		return (conjugateq (q * v)) * q;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector * Matrix Operations
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf FASTCALL transformvf2d (const vectorf& v, const matrixf& m) noexcept
	{
		return fmavf (v.splat_x (), m.column1, fmavf (v.splat_y (), m.column2, m.column4));
	}
	inline vectorf FASTCALL transform_normalvf2d (const vectorf& v, const matrixf& m) noexcept
	{
		return fmavf (v.splat_x (), m.column1, v.splat_y () * m.column2);
	}
	inline vectorf FASTCALL transformvf3d (const vectorf& v, const matrixf& m) noexcept
	{
		return fmavf (v.splat_x (), m.column1, fmavf (v.splat_y (), m.column2, fmavf (v.splat_z (), m.column3, m.column4)));
	}
	inline vectorf FASTCALL transform_normalvf3d (const vectorf& v, const matrixf& m) noexcept
	{
		return fmavf (v.splat_x (), m.column1, fmavf (v.splat_y (), m.column2, v.splat_z () * m.column3));
	}
	inline vectorf FASTCALL transformvf4d (const vectorf& v, const matrixf& m) noexcept
	{
		return fmavf (v.splat_x (), m.column1, fmavf (v.splat_y (), m.column2, fmavf (v.splat_z (), m.column3, v.splat_w () * m.column4)));
	}
	inline vectorf FASTCALL transform_plane (const vectorf& p, const matrixf& m) noexcept
	{
		return transformvf4d (p, m);
	}
	inline vectorf FASTCALL transform_plane (const vectorf& p, const vectorf& rot, const vectorf& tr) noexcept
	{
		vectorf vNormal = rotatevf3d (p, rot);
		vectorf vD = p.splat_w () * dotvf3d (vNormal, tr);
		return insertvf<0, 0, 0, 0, 1> (vNormal, vD);
	}
}

#if COMPILER_MSVC
#	pragma warning (default : 4556)
#endif

#endif