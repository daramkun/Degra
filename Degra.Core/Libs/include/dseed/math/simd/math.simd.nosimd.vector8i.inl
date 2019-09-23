#ifndef __DSEED_MATH_SIMD_NOSIMD_VECTOR8I_INL__
#define __DSEED_MATH_SIMD_NOSIMD_VECTOR8I_INL__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 256-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vector8i_def FASTCALL addv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3],
			v1.v[4] + v2.v[4], v1.v[5] + v2.v[5], v1.v[6] + v2.v[6], v1.v[7] + v2.v[7]
		);
	}
	inline vector8i_def FASTCALL subtractv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3],
			v1.v[4] - v2.v[4], v1.v[5] - v2.v[5], v1.v[6] - v2.v[6], v1.v[7] - v2.v[7]
		);
	}
	inline vector8i_def FASTCALL negatev8i (const vector8i_def& v) noexcept
	{
		return vector8i_def (-v.v[0], -v.v[1], -v.v[2], -v.v[3], -v.v[4], -v.v[5], -v.v[6], -v.v[7]);
	}
	inline vector8i_def FASTCALL multiplyv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2], v1.v[3] * v2.v[3],
			v1.v[4] * v2.v[4], v1.v[5] * v2.v[5], v1.v[6] * v2.v[6], v1.v[7] * v2.v[7]
		);
	}
	inline vector8i_def FASTCALL multiplyv8i (const vector8i_def& v, int s) noexcept
	{
		return vector8i_def (
			v.v[0] * s, v.v[1] * s, v.v[2] * s, v.v[3] * s,
			v.v[4] * s, v.v[5] * s, v.v[6] * s, v.v[7] * s
		);
	}
	inline vector8i_def FASTCALL multiplyv8i (int s, const vector8i_def& v) noexcept
	{
		return multiplyv8i (v, s);
	}
	inline vector8i_def FASTCALL dividev8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2], v1.v[3] / v2.v[3],
			v1.v[4] / v2.v[4], v1.v[5] / v2.v[5], v1.v[6] / v2.v[6], v1.v[7] / v2.v[7]
		);
	}
	inline vector8i_def FASTCALL dividev8i (const vector8i_def& v, int s) noexcept
	{
		return vector8i_def (
			v.v[0] / s, v.v[1] / s, v.v[2] / s, v.v[3] / s,
			v.v[4] / s, v.v[5] / s, v.v[6] / s, v.v[7] / s
		);
	}
	inline vector8i_def FASTCALL fmav8i (const vector8i_def& mv1, const vector8i_def& mv2, const vector8i_def& av) noexcept
	{
		return addv8i (multiplyv8i (mv1, mv2), av);
	}
	inline vector8i_def FASTCALL fmsv8i (const vector8i_def& mv1, const vector8i_def& mv2, const vector8i_def& sv) noexcept
	{
		return subtractv8i (multiplyv8i (mv1, mv2), sv);
	}
	inline vector8i_def FASTCALL fnmsv8i (const vector8i_def& sv, const vector8i_def& mv1, const vector8i_def& mv2) noexcept
	{
		return subtractv8i (sv, multiplyv8i (mv1, mv2));
	}
	inline vector8i_def FASTCALL andv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] & v2.v[0], v1.v[1] & v2.v[1], v1.v[2] & v2.v[2], v1.v[3] & v2.v[3],
			v1.v[4] & v2.v[4], v1.v[5] & v2.v[5], v1.v[6] & v2.v[6], v1.v[7] & v2.v[7]
		);
	}
	inline vector8i_def FASTCALL orv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] | v2.v[0], v1.v[1] | v2.v[1], v1.v[2] | v2.v[2], v1.v[3] | v2.v[3],
			v1.v[4] | v2.v[4], v1.v[5] | v2.v[5], v1.v[6] | v2.v[6], v1.v[7] | v2.v[7]
		);
	}
	inline vector8i_def FASTCALL xorv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.v[0] ^ v2.v[0], v1.v[1] ^ v2.v[1], v1.v[2] ^ v2.v[2], v1.v[3] ^ v2.v[3],
			v1.v[4] ^ v2.v[4], v1.v[5] ^ v2.v[5], v1.v[6] ^ v2.v[6], v1.v[7] ^ v2.v[7]
		);
	}
	inline vector8i_def FASTCALL shiftlv8i (const vector8i_def& v, int s) noexcept
	{
		return vector8i_def (
			v.v[0] << s, v.v[1] << s, v.v[2] << s, v.v[3] << s,
			v.v[4] << s, v.v[5] << s, v.v[6] << s, v.v[7] << s
		);
	}
	inline vector8i_def FASTCALL shiftrv8i (const vector8i_def& v, int s) noexcept
	{
		return vector8i_def (
			v.v[0] >> s, v.v[1] >> s, v.v[2] >> s, v.v[3] >> s,
			v.v[4] >> s, v.v[5] >> s, v.v[6] >> s, v.v[7] >> s
		);
	}
	inline vector8i_def FASTCALL notv8i (const vector8i_def& v) noexcept
	{
		return vector8i_def (~v.v[0], ~v.v[1], ~v.v[2], ~v.v[3], ~v.v[4], ~v.v[5], ~v.v[6], ~v.v[7]);
	}
	inline vector8i_def FASTCALL equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () == v2.x1 () ? -1 : 0,
			v1.y1 () == v2.y1 () ? -1 : 0,
			v1.z1 () == v2.z1 () ? -1 : 0,
			v1.w1 () == v2.w1 () ? -1 : 0,
			v1.x2 () == v2.x2 () ? -1 : 0,
			v1.y2 () == v2.y2 () ? -1 : 0,
			v1.z2 () == v2.z2 () ? -1 : 0,
			v1.w2 () == v2.w2 () ? -1 : 0
		);
	}
	inline vector8i_def FASTCALL not_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () != v2.x1 () ? -1 : 0,
			v1.y1 () != v2.y1 () ? -1 : 0,
			v1.z1 () != v2.z1 () ? -1 : 0,
			v1.w1 () != v2.w1 () ? -1 : 0,
			v1.x2 () != v2.x2 () ? -1 : 0,
			v1.y2 () != v2.y2 () ? -1 : 0,
			v1.z2 () != v2.z2 () ? -1 : 0,
			v1.w2 () != v2.w2 () ? -1 : 0
		);
	}
	inline vector8i_def FASTCALL lesserv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () < v2.x1 () ? -1 : 0,
			v1.y1 () < v2.y1 () ? -1 : 0,
			v1.z1 () < v2.z1 () ? -1 : 0,
			v1.w1 () < v2.w1 () ? -1 : 0,
			v1.x2 () < v2.x2 () ? -1 : 0,
			v1.y2 () < v2.y2 () ? -1 : 0,
			v1.z2 () < v2.z2 () ? -1 : 0,
			v1.w2 () < v2.w2 () ? -1 : 0
		);
	}
	inline vector8i_def FASTCALL lesser_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () <= v2.x1 () ? -1 : 0,
			v1.y1 () <= v2.y1 () ? -1 : 0,
			v1.z1 () <= v2.z1 () ? -1 : 0,
			v1.w1 () <= v2.w1 () ? -1 : 0,
			v1.x2 () <= v2.x2 () ? -1 : 0,
			v1.y2 () <= v2.y2 () ? -1 : 0,
			v1.z2 () <= v2.z2 () ? -1 : 0,
			v1.w2 () <= v2.w2 () ? -1 : 0
		);
	}
	inline vector8i_def FASTCALL greaterv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () > v2.x1 () ? -1 : 0,
			v1.y1 () > v2.y1 () ? -1 : 0,
			v1.z1 () > v2.z1 () ? -1 : 0,
			v1.w1 () > v2.w1 () ? -1 : 0,
			v1.x2 () > v2.x2 () ? -1 : 0,
			v1.y2 () > v2.y2 () ? -1 : 0,
			v1.z2 () > v2.z2 () ? -1 : 0,
			v1.w2 () > v2.w2 () ? -1 : 0
		);
	}
	inline vector8i_def FASTCALL greater_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
			v1.x1 () >= v2.x1 () ? -1 : 0,
			v1.y1 () >= v2.y1 () ? -1 : 0,
			v1.z1 () >= v2.z1 () ? -1 : 0,
			v1.w1 () >= v2.w1 () ? -1 : 0,
			v1.x2 () >= v2.x2 () ? -1 : 0,
			v1.y2 () >= v2.y2 () ? -1 : 0,
			v1.z2 () >= v2.z2 () ? -1 : 0,
			v1.w2 () >= v2.w2 () ? -1 : 0
		);
	}

	inline vector8i_def FASTCALL addv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] + ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL subtractv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] - ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL negatev8i8 (const vector8i_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = -ia[i];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] * ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i8 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia[i] * s;

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i8 (int s, const vector8i_def& v) noexcept
	{
		return multiplyv8i8 (v, s);
	}
	inline vector8i_def FASTCALL dividev8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] / ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL dividev8i8 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia[i] / s;

		return ret;
	}
	inline vector8i_def FASTCALL equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] == ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL not_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] != ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesserv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] < ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesser_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] <= ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greaterv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] > ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greater_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = ia1[i] >= ia2[i] ? 0xff : 0;

		return ret;
	}

	inline vector8i_def FASTCALL addv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] + ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL subtractv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] - ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL negatev8i16 (const vector8i_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = -ia[i];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] * ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i16 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] * s;

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i16 (int s, const vector8i_def& v) noexcept
	{
		return multiplyv8i16 (v, s);
	}
	inline vector8i_def FASTCALL dividev8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] / ia2[i];

		return ret;
	}
	inline vector8i_def FASTCALL dividev8i16 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] / s;

		return ret;
	}
	inline vector8i_def FASTCALL shiftlv8i16 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] << s;

		return ret;
	}
	inline vector8i_def FASTCALL shiftrv8i16 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] >> s;

		return ret;
	}
	inline vector8i_def FASTCALL equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] == ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL not_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] != ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesserv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] < ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesser_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] <= ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greaterv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] > ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greater_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] >= ia2[i] ? 0xffff : 0;

		return ret;
	}

	inline vector8i_def FASTCALL addv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] + ia2[0];
		retia[1] = ia1[1] + ia2[1];
		retia[2] = ia1[2] + ia2[2];
		retia[3] = ia1[3] + ia2[3];

		return ret;
	}
	inline vector8i_def FASTCALL subtractv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] - ia2[0];
		retia[1] = ia1[1] - ia2[1];
		retia[2] = ia1[2] - ia2[2];
		retia[3] = ia1[3] - ia2[3];

		return ret;
	}
	inline vector8i_def FASTCALL negatev8i64 (const vector8i_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = -ia[0];
		retia[1] = -ia[1];
		retia[2] = -ia[2];
		retia[3] = -ia[3];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] * ia2[0];
		retia[1] = ia1[1] * ia2[1];
		retia[2] = ia1[2] * ia2[2];
		retia[3] = ia1[3] * ia2[3];

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i64 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] * s;
		retia[1] = ia[1] * s;
		retia[2] = ia[2] * s;
		retia[3] = ia[3] * s;

		return ret;
	}
	inline vector8i_def FASTCALL multiplyv8i64 (int s, const vector8i_def& v) noexcept
	{
		return multiplyv8i64 (v, s);
	}
	inline vector8i_def FASTCALL dividev8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] / ia2[0];
		retia[1] = ia1[1] / ia2[1];
		retia[2] = ia1[2] / ia2[2];
		retia[3] = ia1[3] / ia2[3];

		return ret;
	}
	inline vector8i_def FASTCALL dividev8i64 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] / s;
		retia[1] = ia[1] / s;
		retia[2] = ia[2] / s;
		retia[3] = ia[3] / s;

		return ret;
	}
	inline vector8i_def FASTCALL shiftlv8i64 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] << s;
		retia[1] = ia[1] << s;
		retia[2] = ia[2] << s;
		retia[3] = ia[3] << s;

		return ret;
	}
	inline vector8i_def FASTCALL shiftrv8i64 (const vector8i_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] >> s;
		retia[1] = ia[1] >> s;
		retia[2] = ia[2] >> s;
		retia[3] = ia[3] >> s;

		return ret;
	}
	inline vector8i_def FASTCALL equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] == ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] == ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] == ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] == ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL not_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] != ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] != ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] != ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] != ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesserv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] < ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] < ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] < ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] < ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL lesser_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] <= ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] <= ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] <= ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] <= ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greaterv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] > ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] > ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] > ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] > ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vector8i_def FASTCALL greater_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] >= ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] >= ia2[1] ? 0xffffffffffffffff : 0;
		retia[2] = ia1[2] >= ia2[2] ? 0xffffffffffffffff : 0;
		retia[3] = ia1[3] >= ia2[3] ? 0xffffffffffffffff : 0;

		return ret;
	}

	inline vector8i_def FASTCALL dotv8i2d (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (dotvi2d (v1.vector1 (), v2.vector1 ()), dotvi2d (v1.vector2 (), v2.vector2 ()));
	}
	inline vector8i_def FASTCALL dotv8i3d (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (dotvi3d (v1.vector1 (), v2.vector1 ()), dotvi3d (v1.vector2 (), v2.vector2 ()));
	}
	inline vector8i_def FASTCALL dotv8i4d (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (dotvi4d (v1.vector1 (), v2.vector1 ()), dotvi4d (v1.vector2 (), v2.vector2 ()));
	}

	inline vector8i_def FASTCALL absv8i (const vector8i_def& v) noexcept
	{
		return vector8i_def (
			abs (v.x1 ()), abs (v.y1 ()), abs (v.z1 ()), abs (v.w1 ()),
			abs (v.x2 ()), abs (v.y2 ()), abs (v.z2 ()), abs (v.w2 ())
		);
	}
	inline vector8i_def FASTCALL minv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
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
	inline vector8i_def FASTCALL maxv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		return vector8i_def (
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

	inline vector8i_def FASTCALL absv8i64 (const vector8i_def& v) noexcept
	{
		const int64_t* ia = reinterpret_cast<const int64_t*>(v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = abs (ia[0]);
		retia[1] = abs (ia[1]);
		retia[2] = abs (ia[2]);
		retia[3] = abs (ia[3]);

		return ret;
	}
	inline vector8i_def FASTCALL minv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int64_t* ia1 = reinterpret_cast<const int64_t*>(v1.v);
		const int64_t* ia2 = reinterpret_cast<const int64_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = minimum (ia1[0], ia2[0]);
		retia[1] = minimum (ia1[1], ia2[1]);
		retia[2] = minimum (ia1[2], ia2[2]);
		retia[3] = minimum (ia1[3], ia2[3]);

		return ret;
	}
	inline vector8i_def FASTCALL maxv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int64_t* ia1 = reinterpret_cast<const int64_t*>(v1.v);
		const int64_t* ia2 = reinterpret_cast<const int64_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = maximum (ia1[0], ia2[0]);
		retia[1] = maximum (ia1[1], ia2[1]);
		retia[2] = maximum (ia1[2], ia2[2]);
		retia[3] = maximum (ia1[3], ia2[3]);

		return ret;
	}

	inline vector8i_def FASTCALL absv8i16 (const vector8i_def& v) noexcept
	{
		const int16_t* ia = reinterpret_cast<const int16_t*>(v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = abs (ia[0]); retia[1] = abs (ia[1]); retia[2] = abs (ia[2]); retia[3] = abs (ia[3]);
		retia[4] = abs (ia[4]); retia[5] = abs (ia[5]); retia[6] = abs (ia[6]); retia[7] = abs (ia[7]);
		retia[8] = abs (ia[8]); retia[9] = abs (ia[9]); retia[10] = abs (ia[10]); retia[11] = abs (ia[11]);
		retia[12] = abs (ia[12]); retia[13] = abs (ia[13]); retia[14] = abs (ia[14]); retia[15] = abs (ia[15]);

		return ret;
	}
	inline vector8i_def FASTCALL minv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int16_t* ia1 = reinterpret_cast<const int16_t*>(v1.v);
		const int16_t* ia2 = reinterpret_cast<const int16_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = minimum (ia1[0], ia2[0]); retia[1] = minimum (ia1[1], ia2[1]);
		retia[2] = minimum (ia1[2], ia2[2]); retia[3] = minimum (ia1[3], ia2[3]);
		retia[4] = minimum (ia1[4], ia2[4]); retia[5] = minimum (ia1[5], ia2[5]);
		retia[6] = minimum (ia1[6], ia2[6]); retia[7] = minimum (ia1[7], ia2[7]);
		retia[8] = minimum (ia1[8], ia2[8]); retia[9] = minimum (ia1[9], ia2[9]);
		retia[10] = minimum (ia1[10], ia2[10]); retia[11] = minimum (ia1[11], ia2[11]);
		retia[12] = minimum (ia1[12], ia2[12]); retia[13] = minimum (ia1[13], ia2[13]);
		retia[14] = minimum (ia1[14], ia2[14]); retia[15] = minimum (ia1[15], ia2[15]);

		return ret;
	}
	inline vector8i_def FASTCALL maxv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int16_t* ia1 = reinterpret_cast<const int16_t*>(v1.v);
		const int16_t* ia2 = reinterpret_cast<const int16_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = maximum (ia1[0], ia2[0]); retia[1] = maximum (ia1[1], ia2[1]);
		retia[2] = maximum (ia1[2], ia2[2]); retia[3] = maximum (ia1[3], ia2[3]);
		retia[4] = maximum (ia1[4], ia2[4]); retia[5] = maximum (ia1[5], ia2[5]);
		retia[6] = maximum (ia1[6], ia2[6]); retia[7] = maximum (ia1[7], ia2[7]);
		retia[8] = maximum (ia1[8], ia2[8]); retia[9] = maximum (ia1[9], ia2[9]);
		retia[10] = maximum (ia1[10], ia2[10]); retia[11] = maximum (ia1[11], ia2[11]);
		retia[12] = maximum (ia1[12], ia2[12]); retia[13] = maximum (ia1[13], ia2[13]);
		retia[14] = maximum (ia1[14], ia2[14]); retia[15] = maximum (ia1[15], ia2[15]);

		return ret;
	}

	inline vector8i_def FASTCALL absv8i8 (const vector8i_def& v) noexcept
	{
		const int8_t* ia = reinterpret_cast<const int8_t*>(v.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = abs (ia[i]);

		return ret;
	}
	inline vector8i_def FASTCALL minv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int8_t* ia1 = reinterpret_cast<const int8_t*>(v1.v);
		const int8_t* ia2 = reinterpret_cast<const int8_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = minimum (ia1[i], ia2[i]);

		return ret;
	}
	inline vector8i_def FASTCALL maxv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept
	{
		const int8_t* ia1 = reinterpret_cast<const int8_t*>(v1.v);
		const int8_t* ia2 = reinterpret_cast<const int8_t*>(v2.v);

		vector8i_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 32; ++i)
			retia[i] = maximum (ia1[i], ia2[i]);

		return ret;
	}
}

#endif