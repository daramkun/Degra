#ifndef __DSEED_MATH_SIMD_NOSIMD_VECTORI_INL__
#define __DSEED_MATH_SIMD_NOSIMD_VECTORI_INL__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_def FASTCALL addvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3]);
	}
	inline vectori_def FASTCALL subtractvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3]);
	}
	inline vectori_def FASTCALL negatevi (const vectori_def& v) noexcept
	{
		return vectori_def (-v.v[0], -v.v[1], -v.v[2], -v.v[3]);
	}
	inline vectori_def FASTCALL multiplyvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2], v1.v[3] * v2.v[3]);
	}
	inline vectori_def FASTCALL multiplyvi (const vectori_def& v, int s) noexcept
	{
		return vectori_def (v.v[0] * s, v.v[1] * s, v.v[2] * s, v.v[3] * s);
	}
	inline vectori_def FASTCALL multiplyvi (int s, const vectori_def& v) noexcept
	{
		return multiplyvi (v, s);
	}
	inline vectori_def FASTCALL dividevi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2], v1.v[3] / v2.v[3]);
	}
	inline vectori_def FASTCALL dividevi (const vectori_def& v, int s) noexcept
	{
		return vectori_def (v.v[0] / s, v.v[1] / s, v.v[2] / s, v.v[3] / s);
	}
	inline vectori_def FASTCALL fmavi (const vectori_def& mv1, const vectori_def& mv2, const vectori_def& av) noexcept
	{
		return addvi (multiplyvi (mv1, mv2), av);
	}
	inline vectori_def FASTCALL fmsvi (const vectori_def& mv1, const vectori_def& mv2, const vectori_def& sv) noexcept
	{
		return subtractvi (multiplyvi (mv1, mv2), sv);
	}
	inline vectori_def FASTCALL fnmsvi (const vectori_def& sv, const vectori_def& mv1, const vectori_def& mv2) noexcept
	{
		return subtractvi (sv, multiplyvi (mv1, mv2));
	}
	inline vectori_def FASTCALL andvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] & v2.v[0], v1.v[1] & v2.v[1], v1.v[2] & v2.v[2], v1.v[3] & v2.v[3]);
	}
	inline vectori_def FASTCALL orvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] | v2.v[0], v1.v[1] | v2.v[1], v1.v[2] | v2.v[2], v1.v[3] | v2.v[3]);
	}
	inline vectori_def FASTCALL xorvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.v[0] ^ v2.v[0], v1.v[1] ^ v2.v[1], v1.v[2] ^ v2.v[2], v1.v[3] ^ v2.v[3]);
	}
	inline vectori_def FASTCALL shiftlvi (const vectori_def& v, int s) noexcept
	{
		return vectori_def (v.v[0] << s, v.v[1] << s, v.v[2] << s, v.v[3] << s);
	}
	inline vectori_def FASTCALL shiftrvi (const vectori_def& v, int s) noexcept
	{
		return vectori_def (v.v[0] >> s, v.v[1] >> s, v.v[2] >> s, v.v[3] >> s);
	}
	inline vectori_def FASTCALL notvi (const vectori_def& v) noexcept
	{
		return vectori_def (~v.v[0], ~v.v[1], ~v.v[2], ~v.v[3]);
	}
	inline vectori_def FASTCALL equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () == v2.x () ? -1 : 0,
			v1.y () == v2.y () ? -1 : 0,
			v1.z () == v2.z () ? -1 : 0,
			v1.w () == v2.w () ? -1 : 0
		);
	}
	inline vectori_def FASTCALL not_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () != v2.x () ? -1 : 0,
			v1.y () != v2.y () ? -1 : 0,
			v1.z () != v2.z () ? -1 : 0,
			v1.w () != v2.w () ? -1 : 0
		);
	}
	inline vectori_def FASTCALL lesservi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () < v2.x () ? -1 : 0,
			v1.y () < v2.y () ? -1 : 0,
			v1.z () < v2.z () ? -1 : 0,
			v1.w () < v2.w () ? -1 : 0
		);
	}
	inline vectori_def FASTCALL lesser_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () <= v2.x () ? -1 : 0,
			v1.y () <= v2.y () ? -1 : 0,
			v1.z () <= v2.z () ? -1 : 0,
			v1.w () <= v2.w () ? -1 : 0
		);
	}
	inline vectori_def FASTCALL greatervi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () > v2.x () ? -1 : 0,
			v1.y () > v2.y () ? -1 : 0,
			v1.z () > v2.z () ? -1 : 0,
			v1.w () > v2.w () ? -1 : 0
		);
	}
	inline vectori_def FASTCALL greater_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			v1.x () >= v2.x () ? -1 : 0,
			v1.y () >= v2.y () ? -1 : 0,
			v1.z () >= v2.z () ? -1 : 0,
			v1.w () >= v2.w () ? -1 : 0
		);
	}

	inline vectori_def FASTCALL addvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] + ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL subtractvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] - ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL negatevi8 (const vectori_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = -ia[i];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] * ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi8 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] * s;

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi8 (int s, const vectori_def& v) noexcept
	{
		return multiplyvi8 (v, s);
	}
	inline vectori_def FASTCALL dividevi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] / ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL dividevi8 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int8_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia[i] / s;

		return ret;
	}
	inline vectori_def FASTCALL equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] == ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vectori_def FASTCALL not_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] != ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesservi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] < ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesser_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] <= ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greatervi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] > ia2[i] ? 0xff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greater_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int8_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int8_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		for (int i = 0; i < 16; ++i)
			retia[i] = ia1[i] >= ia2[i] ? 0xff : 0;

		return ret;
	}

	inline vectori_def FASTCALL addvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] + ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL subtractvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] - ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL negatevi16 (const vectori_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = -ia[i];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] * ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi16 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia[i] * s;

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi16 (int s, const vectori_def& v) noexcept
	{
		return multiplyvi16 (v, s);
	}
	inline vectori_def FASTCALL dividevi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] / ia2[i];

		return ret;
	}
	inline vectori_def FASTCALL dividevi16 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia[i] / s;

		return ret;
	}
	inline vectori_def FASTCALL shiftlvi16 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia[i] << s;

		return ret;
	}
	inline vectori_def FASTCALL shiftrvi16 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int16_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia[i] >> s;

		return ret;
	}
	inline vectori_def FASTCALL equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] == ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL not_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] != ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesservi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] < ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesser_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] <= ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greatervi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] > ia2[i] ? 0xffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greater_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int16_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int16_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		for (int i = 0; i < 8; ++i)
			retia[i] = ia1[i] >= ia2[i] ? 0xffff : 0;

		return ret;
	}

	inline vectori_def FASTCALL addvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] + ia2[0];
		retia[1] = ia1[1] + ia2[1];

		return ret;
	}
	inline vectori_def FASTCALL subtractvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] - ia2[0];
		retia[1] = ia1[1] - ia2[1];

		return ret;
	}
	inline vectori_def FASTCALL negatevi64 (const vectori_def& v) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = -ia[0];
		retia[1] = -ia[1];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] * ia2[0];
		retia[1] = ia1[1] * ia2[1];

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi64 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] * s;
		retia[1] = ia[1] * s;

		return ret;
	}
	inline vectori_def FASTCALL multiplyvi64 (int s, const vectori_def& v) noexcept
	{
		return multiplyvi64 (v, s);
	}
	inline vectori_def FASTCALL dividevi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] / ia2[0];
		retia[1] = ia1[1] / ia2[1];

		return ret;
	}
	inline vectori_def FASTCALL dividevi64 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] / s;
		retia[1] = ia[1] / s;

		return ret;
	}
	inline vectori_def FASTCALL shiftlvi64 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] << s;
		retia[1] = ia[1] << s;

		return ret;
	}
	inline vectori_def FASTCALL shiftrvi64 (const vectori_def& v, int s) noexcept
	{
		auto ia = reinterpret_cast<const int64_t*> (v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia[0] >> s;
		retia[1] = ia[1] >> s;

		return ret;
	}
	inline vectori_def FASTCALL equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] == ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] == ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL not_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] != ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] != ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesservi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] < ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] < ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL lesser_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] <= ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] <= ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greatervi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] > ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] > ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}
	inline vectori_def FASTCALL greater_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		auto ia1 = reinterpret_cast<const int64_t*> (v1.v);
		auto ia2 = reinterpret_cast<const int64_t*> (v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = ia1[0] >= ia2[0] ? 0xffffffffffffffff : 0;
		retia[1] = ia1[1] >= ia2[1] ? 0xffffffffffffffff : 0;

		return ret;
	}

	inline vectori_def FASTCALL dotvi2d (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.x () * v2.x () + v1.y () * v2.y ());
	}
	inline vectori_def FASTCALL dotvi3d (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.x () * v2.x () + v1.y () * v2.y () + v1.z () * v2.z ());
	}
	inline vectori_def FASTCALL dotvi4d (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (v1.x () * v2.x () + v1.y () * v2.y () + v1.z () * v2.z () + v1.w () * v2.w ());
	}

	inline vectori_def FASTCALL absvi (const vectori_def& v) noexcept
	{
		return vectori_def (abs (v.x ()), abs (v.y ()), abs (v.z ()), abs (v.w ()));
	}
	inline vectori_def FASTCALL minvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			minimum (v1.x (), v2.x ()),
			minimum (v1.y (), v2.y ()),
			minimum (v1.z (), v2.z ()),
			minimum (v1.w (), v2.w ())
		);
	}
	inline vectori_def FASTCALL maxvi (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		return vectori_def (
			maximum (v1.x (), v2.x ()),
			maximum (v1.y (), v2.y ()),
			maximum (v1.z (), v2.z ()),
			maximum (v1.w (), v2.w ())
		);
	}

	inline vectori_def FASTCALL absvi64 (const vectori_def& v) noexcept
	{
		const int64_t* ia = reinterpret_cast<const int64_t*>(v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = abs (ia[0]);
		retia[1] = abs (ia[1]);

		return ret;
	}
	inline vectori_def FASTCALL minvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int64_t* ia1 = reinterpret_cast<const int64_t*>(v1.v);
		const int64_t* ia2 = reinterpret_cast<const int64_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = minimum (ia1[0], ia2[0]);
		retia[1] = minimum (ia1[1], ia2[1]);

		return ret;
	}
	inline vectori_def FASTCALL maxvi64 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int64_t* ia1 = reinterpret_cast<const int64_t*>(v1.v);
		const int64_t* ia2 = reinterpret_cast<const int64_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int64_t*>(ret.v);

		retia[0] = maximum (ia1[0], ia2[0]);
		retia[1] = maximum (ia1[1], ia2[1]);

		return ret;
	}

	inline vectori_def FASTCALL absvi16 (const vectori_def& v) noexcept
	{
		const int16_t* ia = reinterpret_cast<const int16_t*>(v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = abs (ia[0]); retia[1] = abs (ia[1]); retia[2] = abs (ia[2]); retia[3] = abs (ia[3]);
		retia[4] = abs (ia[4]); retia[5] = abs (ia[5]); retia[6] = abs (ia[6]); retia[7] = abs (ia[7]);

		return ret;
	}
	inline vectori_def FASTCALL minvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int16_t* ia1 = reinterpret_cast<const int16_t*>(v1.v);
		const int16_t* ia2 = reinterpret_cast<const int16_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = minimum (ia1[0], ia2[0]);
		retia[1] = minimum (ia1[1], ia2[1]);
		retia[2] = minimum (ia1[2], ia2[2]);
		retia[3] = minimum (ia1[3], ia2[3]);
		retia[4] = minimum (ia1[4], ia2[4]);
		retia[5] = minimum (ia1[5], ia2[5]);
		retia[6] = minimum (ia1[6], ia2[6]);
		retia[7] = minimum (ia1[7], ia2[7]);

		return ret;
	}
	inline vectori_def FASTCALL maxvi16 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int16_t* ia1 = reinterpret_cast<const int16_t*>(v1.v);
		const int16_t* ia2 = reinterpret_cast<const int16_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int16_t*>(ret.v);

		retia[0] = maximum (ia1[0], ia2[0]);
		retia[1] = maximum (ia1[1], ia2[1]);
		retia[2] = maximum (ia1[2], ia2[2]);
		retia[3] = maximum (ia1[3], ia2[3]);
		retia[4] = maximum (ia1[4], ia2[4]);
		retia[5] = maximum (ia1[5], ia2[5]);
		retia[6] = maximum (ia1[6], ia2[6]);
		retia[7] = maximum (ia1[7], ia2[7]);

		return ret;
	}

	inline vectori_def FASTCALL absvi8 (const vectori_def& v) noexcept
	{
		const int8_t* ia = reinterpret_cast<const int8_t*>(v.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

		retia[0] = abs (ia[0]); retia[1] = abs (ia[1]); retia[2] = abs (ia[2]); retia[3] = abs (ia[3]);
		retia[4] = abs (ia[4]); retia[5] = abs (ia[5]); retia[6] = abs (ia[6]); retia[7] = abs (ia[7]);
		retia[8] = abs (ia[8]); retia[9] = abs (ia[9]); retia[10] = abs (ia[10]); retia[11] = abs (ia[11]);
		retia[12] = abs (ia[12]); retia[13] = abs (ia[13]); retia[14] = abs (ia[14]); retia[15] = abs (ia[15]);

		return ret;
	}
	inline vectori_def FASTCALL minvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int8_t* ia1 = reinterpret_cast<const int8_t*>(v1.v);
		const int8_t* ia2 = reinterpret_cast<const int8_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

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
	inline vectori_def FASTCALL maxvi8 (const vectori_def& v1, const vectori_def& v2) noexcept
	{
		const int8_t* ia1 = reinterpret_cast<const int8_t*>(v1.v);
		const int8_t* ia2 = reinterpret_cast<const int8_t*>(v2.v);

		vectori_def ret;
		auto retia = reinterpret_cast<int8_t*>(ret.v);

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
}

#endif