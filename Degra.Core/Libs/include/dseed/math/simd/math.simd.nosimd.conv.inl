#ifndef __DSEED_MATH_SIMD_NOSIMD_CONV_INL__
#define __DSEED_MATH_SIMD_NOSIMD_CONV_INL__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation vector conversions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_def FASTCALL conv_f32_to_i32 (const vectorf_def& v) noexcept
	{
		return vectori_def ((int)v.x (), (int)v.y (), (int)v.z (), (int)v.w ());
	}
	inline vector8i_def FASTCALL conv8_f32_to_i32 (const vector8f_def& v) noexcept
	{
		return vector8i_def (
			(int)v.x1 (), (int)v.y1 (), (int)v.z1 (), (int)v.w1 (),
			(int)v.x2 (), (int)v.y2 (), (int)v.z2 (), (int)v.w2 ()
		);
	}
	inline vectori_def FASTCALL reinterpret_f32_to_i32 (const vectorf_def& v) noexcept
	{
		const int* reinterpret = reinterpret_cast<const int*>(v.v);
		return vectori_def (reinterpret);
	}
	inline vector8i_def FASTCALL reinterpret8_f32_to_i32 (const vector8f_def& v) noexcept
	{
		auto reinterpret = reinterpret_cast<const int*>(v.v);
		return vector8i_def (reinterpret);
	}

	inline vectorf_def FASTCALL conv_i32_to_f32 (const vectori_def& v) noexcept
	{
		return vectorf_def ((float)v.x (), (float)v.y (), (float)v.z (), (float)v.w ());
	}
	inline vector8f_def FASTCALL conv8_i32_to_f32 (const vector8i_def& v) noexcept
	{
		return vector8f_def (
			(float)v.x1 (), (float)v.y1 (), (float)v.z1 (), (float)v.w1 (),
			(float)v.x2 (), (float)v.y2 (), (float)v.z2 (), (float)v.w2 ()
		);
	}
	inline vectorf_def FASTCALL reinterpret_i32_to_f32 (const vectori_def& v) noexcept
	{
		const float* reinterpret = reinterpret_cast<const float*>(v.v);
		return vectorf_def (reinterpret);
	}
	inline vector8f_def FASTCALL reinterpret8_i32_to_f32 (const vector8i_def& v) noexcept
	{
		auto reinterpret = reinterpret_cast<const float*>(v.v);
		return vector8f_def (reinterpret);
	}

	inline vectori_def FASTCALL conv_i64_to_i32 (const vectori_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int64_t conv[2] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i64_to_i32 (const vector8i_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int64_t conv[4] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		conv[2] = origin[2];
		conv[3] = origin[3];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i64_to_i16 (const vectori_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int16_t conv[8] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i64_to_i16 (const vector8i_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int16_t conv[8] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		conv[2] = origin[2];
		conv[3] = origin[3];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i64_to_i8 (const vectori_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int8_t conv[16] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i64_to_i8 (const vector8i_def& v) noexcept
	{
		const int32_t* origin = (const int32_t*)v.v;
		int8_t conv[32] = {};
		conv[0] = origin[0];
		conv[1] = origin[1];
		conv[2] = origin[2];
		conv[3] = origin[3];
		return vector8i_def ((const int*)conv);
	}

	inline vectori_def FASTCALL conv_i32_to_i64 (const vectori_def& v) noexcept
	{
		int64_t conv[2] = {};
		conv[0] = (int64_t)v.v[0]; conv[1] = (int64_t)v.v[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i32_to_i64 (const vector8i_def& v) noexcept
	{
		int64_t conv[4] = {};
		conv[0] = (int64_t)v.v[0]; conv[1] = (int64_t)v.v[1];
		conv[2] = (int64_t)v.v[2]; conv[3] = (int64_t)v.v[3];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i32_to_i16 (const vectori_def& v) noexcept
	{
		short conv[8] = {};
		conv[0] = (short)v.v[0]; conv[1] = (short)v.v[1];
		conv[2] = (short)v.v[2]; conv[3] = (short)v.v[3];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i32_to_i16 (const vector8i_def& v) noexcept
	{
		short conv[16] = {};
		conv[0] = (short)v.v[0]; conv[1] = (short)v.v[1];
		conv[2] = (short)v.v[2]; conv[3] = (short)v.v[3];
		conv[4] = (short)v.v[4]; conv[5] = (short)v.v[5];
		conv[6] = (short)v.v[6]; conv[7] = (short)v.v[7];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i32_to_i8 (const vectori_def& v) noexcept
	{
		char conv[16] = {};
		conv[0] = (char)v.v[0]; conv[1] = (char)v.v[1];
		conv[2] = (char)v.v[2]; conv[3] = (char)v.v[3];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i32_to_i8 (const vector8i_def& v) noexcept
	{
		char conv[32] = {};
		conv[0] = (char)v.v[0]; conv[1] = (char)v.v[1];
		conv[2] = (char)v.v[2]; conv[3] = (char)v.v[3];
		conv[4] = (char)v.v[4]; conv[5] = (char)v.v[5];
		conv[6] = (char)v.v[6]; conv[7] = (char)v.v[7];
		return vector8i_def ((const int*)conv);
	}

	inline vectori_def FASTCALL conv_i16_to_i64 (const vectori_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		int64_t conv[2] = {};
		conv[0] = (int64_t)origin[0];
		conv[1] = (int64_t)origin[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i16_to_i64 (const vector8i_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		int64_t conv[4] = {};
		conv[0] = (int64_t)origin[0];
		conv[1] = (int64_t)origin[1];
		conv[2] = (int64_t)origin[2];
		conv[3] = (int64_t)origin[3];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i16_to_i32 (const vectori_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		int conv[4] = {};
		conv[0] = (int)origin[0]; conv[1] = (int)origin[1];
		conv[2] = (int)origin[2]; conv[3] = (int)origin[3];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i16_to_i32 (const vector8i_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		int conv[8] = {};
		conv[0] = (int)origin[0]; conv[1] = (int)origin[1];
		conv[2] = (int)origin[2]; conv[3] = (int)origin[3];
		conv[4] = (int)origin[4]; conv[5] = (int)origin[5];
		conv[6] = (int)origin[6]; conv[7] = (int)origin[7];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i16_to_i8 (const vectori_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		char conv[16] = {};
		conv[0] = (char)origin[0]; conv[1] = (char)origin[1];
		conv[2] = (char)origin[2]; conv[3] = (char)origin[3];
		conv[4] = (char)origin[4]; conv[5] = (char)origin[5];
		conv[6] = (char)origin[6]; conv[7] = (char)origin[7];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i16_to_i8 (const vector8i_def& v) noexcept
	{
		const int16_t* origin = (const int16_t*)v.v;
		char conv[32] = {};
		conv[0] = (char)origin[0]; conv[1] = (char)origin[1];
		conv[2] = (char)origin[2]; conv[3] = (char)origin[3];
		conv[4] = (char)origin[4]; conv[5] = (char)origin[5];
		conv[6] = (char)origin[6]; conv[7] = (char)origin[7];
		conv[8] = (char)origin[8]; conv[9] = (char)origin[9];
		conv[10] = (char)origin[10]; conv[11] = (char)origin[11];
		conv[12] = (char)origin[12]; conv[13] = (char)origin[13];
		conv[14] = (char)origin[14]; conv[15] = (char)origin[15];
		return vector8i_def ((const int*)conv);
	}

	inline vectori_def FASTCALL conv_i8_to_i64 (const vectori_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		int64_t conv[2] = {};
		conv[0] = (int64_t)origin[0];
		conv[1] = (int64_t)origin[1];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i8_to_i64 (const vector8i_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		int64_t conv[4] = {};
		conv[0] = (int64_t)origin[0];
		conv[1] = (int64_t)origin[1];
		conv[2] = (int64_t)origin[2];
		conv[3] = (int64_t)origin[3];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i8_to_i32 (const vectori_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		int conv[4] = {};
		conv[0] = (int)origin[0]; conv[1] = (int)origin[1];
		conv[2] = (int)origin[2]; conv[3] = (int)origin[3];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i8_to_i32 (const vector8i_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		int conv[8] = {};
		conv[0] = (int)origin[0]; conv[1] = (int)origin[1];
		conv[2] = (int)origin[2]; conv[3] = (int)origin[3];
		conv[4] = (int)origin[4]; conv[5] = (int)origin[5];
		conv[6] = (int)origin[6]; conv[7] = (int)origin[7];
		return vector8i_def ((const int*)conv);
	}
	inline vectori_def FASTCALL conv_i8_to_i16 (const vectori_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		short conv[8] = {};
		conv[0] = (char)origin[0]; conv[1] = (char)origin[1];
		conv[2] = (char)origin[2]; conv[3] = (char)origin[3];
		conv[4] = (char)origin[4]; conv[5] = (char)origin[5];
		conv[6] = (char)origin[6]; conv[7] = (char)origin[7];
		return vectori_def ((const int*)conv);
	}
	inline vector8i_def FASTCALL conv8_i8_to_i16 (const vector8i_def& v) noexcept
	{
		const int8_t* origin = (const int8_t*)v.v;
		short conv[16] = {};
		conv[0] = (char)origin[0]; conv[1] = (char)origin[1];
		conv[2] = (char)origin[2]; conv[3] = (char)origin[3];
		conv[4] = (char)origin[4]; conv[5] = (char)origin[5];
		conv[6] = (char)origin[6]; conv[7] = (char)origin[7];
		conv[8] = (char)origin[8]; conv[9] = (char)origin[9];
		conv[10] = (char)origin[10]; conv[11] = (char)origin[11];
		conv[12] = (char)origin[12]; conv[13] = (char)origin[13];
		conv[14] = (char)origin[14]; conv[15] = (char)origin[15];
		return vector8i_def ((const int*)conv);
	}

	inline int movemaskvf (const vectorf_def& v) noexcept
	{
		auto ia = reinterpret_cast<const uint32_t*> (v.v);
		return (ia[0] >> 31) | ((ia[1] >> 30) & 2) | ((ia[2] >> 29) & 4) | ((ia[3] >> 28) & 8);
	}
	inline int movemaskv8f (const vector8f_def& v) noexcept
	{
		auto ia = reinterpret_cast<const uint32_t*> (v.v);
		return (ia[0] >> 31) | ((ia[1] >> 30) & 2) | ((ia[2] >> 29) & 4) | ((ia[3] >> 28) & 8)
			| ((ia[4] >> 27) & 16) | ((ia[5] >> 26) & 32) | ((ia[6] >> 25) & 64) | ((ia[7] >> 24) & 128);
	}
	inline int movemaskvi (const vectori_def& v) noexcept
	{
		auto ia = reinterpret_cast<const uint8_t*> (v.v);
		return ((uint32_t)ia[0] >> 31)
			| (((uint32_t)ia[1] >> 30) & 2) | (((uint32_t)ia[2] >> 29) & 4)
			| (((uint32_t)ia[3] >> 28) & 8) | (((uint32_t)ia[4] >> 27) & 16)
			| (((uint32_t)ia[5] >> 26) & 32) | (((uint32_t)ia[6] >> 25) & 64)
			| (((uint32_t)ia[7] >> 24) & 128) | (((uint32_t)ia[8] >> 23) & 256)
			| (((uint32_t)ia[9] >> 22) & 512) | (((uint32_t)ia[10] >> 21) & 1024)
			| (((uint32_t)ia[11] >> 20) & 2048) | (((uint32_t)ia[12] >> 19) & 4096)
			| (((uint32_t)ia[13] >> 18) & 8192) | (((uint32_t)ia[14] >> 17) & 16384)
			| (((uint32_t)ia[15] >> 16) & 32768);
	}
	inline int movemaskv8i (const vector8i_def& v) noexcept
	{
		auto ia = reinterpret_cast<const uint8_t*> (v.v);
		return ((uint32_t)ia[0] >> 31)
			| (((uint32_t)ia[1] >> 30) & 2) | (((uint32_t)ia[2] >> 29) & 4)
			| (((uint32_t)ia[3] >> 28) & 8) | (((uint32_t)ia[4] >> 27) & 16)
			| (((uint32_t)ia[5] >> 26) & 32) | (((uint32_t)ia[6] >> 25) & 64)
			| (((uint32_t)ia[7] >> 24) & 128) | (((uint32_t)ia[8] >> 23) & 256)
			| (((uint32_t)ia[9] >> 22) & 512) | (((uint32_t)ia[10] >> 21) & 1024)
			| (((uint32_t)ia[11] >> 20) & 2048) | (((uint32_t)ia[12] >> 19) & 4096)
			| (((uint32_t)ia[13] >> 18) & 8192) | (((uint32_t)ia[14] >> 17) & 16384)
			| (((uint32_t)ia[15] >> 16) & 32768) | (((uint32_t)ia[16] >> 15) & 65536)
			| (((uint32_t)ia[17] >> 14) & 131072) | (((uint32_t)ia[18] >> 13) & 262144)
			| (((uint32_t)ia[19] >> 12) & 524288) | (((uint32_t)ia[20] >> 11) & 1048576)
			| (((uint32_t)ia[21] >> 10) & 2097152) | (((uint32_t)ia[22] >> 9) & 4194304)
			| (((uint32_t)ia[23] >> 8) & 8388608) | (((uint32_t)ia[24] >> 7) & 16777216)
			| (((uint32_t)ia[25] >> 6) & 33554432) | (((uint32_t)ia[26] >> 5) & 67108864)
			| (((uint32_t)ia[27] >> 4) & 134217728) | (((uint32_t)ia[28] >> 3) & 268435456)
			| (((uint32_t)ia[29] >> 2) & 536870912) | (((uint32_t)ia[30] >> 1) & 1073741824)
			| ((uint32_t)ia[31] & 2147483648);
	}
}

#endif