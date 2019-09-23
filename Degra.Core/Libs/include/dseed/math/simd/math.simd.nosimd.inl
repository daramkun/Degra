#ifndef __DSEED_MATH_SIMD_NOSIMD_INL__
#define __DSEED_MATH_SIMD_NOSIMD_INL__

#include <cmath>

namespace dseed
{
	struct vectorf_def;
	struct vectori_def;
	struct vector8f_def;
	struct vector8i_def;

	inline vectori_def FASTCALL conv_f32_to_i32 (const vectorf_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_f32_to_i32 (const vector8f_def& v) noexcept;
	inline vectori_def FASTCALL reinterpret_f32_to_i32 (const vectorf_def& v) noexcept;
	inline vector8i_def FASTCALL reinterpret8_f32_to_i32 (const vector8f_def& v) noexcept;

	inline vectorf_def FASTCALL conv_i32_to_f32 (const vectori_def& v) noexcept;
	inline vector8f_def FASTCALL conv8_i32_to_f32 (const vector8i_def& v) noexcept;
	inline vectorf_def FASTCALL reinterpret_i32_to_f32 (const vectori_def& v) noexcept;
	inline vector8f_def FASTCALL reinterpret8_i32_to_f32 (const vector8i_def& v) noexcept;

	inline vectori_def FASTCALL conv_i64_to_i32 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i64_to_i32 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i64_to_i16 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i64_to_i16 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i64_to_i8 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i64_to_i8 (const vector8i_def& v) noexcept;

	inline vectori_def FASTCALL conv_i32_to_i64 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i32_to_i64 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i32_to_i16 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i32_to_i16 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i32_to_i8 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i32_to_i8 (const vector8i_def& v) noexcept;

	inline vectori_def FASTCALL conv_i16_to_i64 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i16_to_i64 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i16_to_i32 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i16_to_i32 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i16_to_i8 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i16_to_i8 (const vector8i_def& v) noexcept;

	inline vectori_def FASTCALL conv_i8_to_i64 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i8_to_i64 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i8_to_i32 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i8_to_i32 (const vector8i_def& v) noexcept;
	inline vectori_def FASTCALL conv_i8_to_i16 (const vectori_def& v) noexcept;
	inline vector8i_def FASTCALL conv8_i8_to_i16 (const vector8i_def& v) noexcept;

	inline int movemaskvf (const vectorf_def& v) noexcept;
	inline int movemaskv8f (const vector8f_def& v) noexcept;
	inline int movemaskvi (const vectori_def& v) noexcept;
	inline int movemaskv8i (const vector8i_def& v) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectorf_def
	{
		float v[4];

		inline vectorf_def () noexcept = default;
		inline vectorf_def (const float* arr) noexcept { load (arr); }
		inline vectorf_def (float x, float y, float z, float w) noexcept { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }
		inline vectorf_def (float scalar) noexcept { v[0] = v[1] = v[2] = v[3] = scalar; }

		inline operator float* () noexcept { return v; }
		inline operator const float* () const noexcept { return v; }

		inline void FASTCALL load (const float* arr) noexcept { memcpy (v, arr, sizeof (float) * 4); }
		inline void FASTCALL store (float* arr) const noexcept { memcpy (arr, v, sizeof (float) * 4); }

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		static inline vectorf_def FASTCALL shuffle32 (const vectorf_def & v1, const vectorf_def & v2) noexcept
		{
			return vectorf_def (
				(x >= 0 && x <= 3) ? v1.v[x] : v2.v[4 - x],
				(y >= 0 && y <= 3) ? v1.v[y] : v2.v[4 - y],
				(z >= 0 && z <= 3) ? v2.v[z] : v1.v[4 - z],
				(w >= 0 && w <= 3) ? v2.v[w] : v1.v[4 - w]
			);
		}

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		inline vectorf_def FASTCALL permute32 () const noexcept
		{
			return shuffle32<x, y, z, w> (*this, *this);
		}

		inline vectorf_def FASTCALL splat_x () const noexcept { return vectorf_def (v[0]); }
		inline vectorf_def FASTCALL splat_y () const noexcept { return vectorf_def (v[1]); }
		inline vectorf_def FASTCALL splat_z () const noexcept { return vectorf_def (v[2]); }
		inline vectorf_def FASTCALL splat_w () const noexcept { return vectorf_def (v[3]); }

		inline float FASTCALL x () const noexcept { return v[0]; }
		inline float FASTCALL y () const noexcept { return v[1]; }
		inline float FASTCALL z () const noexcept { return v[2]; }
		inline float FASTCALL w () const noexcept { return v[3]; }

		static inline bool support () noexcept { return true; }
		static inline bool hwvector () noexcept { return false; }
	};

	inline vectorf_def FASTCALL addvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL subtractvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL negatevf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL multiplyvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL multiplyvf (const vectorf_def& v, float s) noexcept;
	inline vectorf_def FASTCALL multiplyvf (float s, const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL dividevf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL dividevf (const vectorf_def& v, float s) noexcept;
	inline vectorf_def FASTCALL fmavf (const vectorf_def& mv1, const vectorf_def& mv2, const vectorf_def& av) noexcept;
	inline vectorf_def FASTCALL fmsvf (const vectorf_def& mv1, const vectorf_def& mv2, const vectorf_def& sv) noexcept;
	inline vectorf_def FASTCALL fnmsvf (const vectorf_def& sv, const vectorf_def& mv1, const vectorf_def& mv2) noexcept;
	inline vectorf_def FASTCALL andvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL orvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL xorvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL notvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL not_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL lesservf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL lesser_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL greatervf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL greater_equalsvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	
	inline vectorf_def FASTCALL dotvf2d (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL dotvf3d (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL dotvf4d (const vectorf_def& v1, const vectorf_def& v2) noexcept;

	inline vectorf_def FASTCALL sqrtvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL rsqrtvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL rcpvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL roundvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL floorvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL ceilvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL absvf (const vectorf_def& v) noexcept;
	inline vectorf_def FASTCALL minvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	inline vectorf_def FASTCALL maxvf (const vectorf_def& v1, const vectorf_def& v2) noexcept;
	
	inline vectorf_def FASTCALL selectvf (const vectorf_def& v1, const vectorf_def& v2, const vectorf_def& controlv) noexcept;
	inline vectorf_def FASTCALL selectcontrolvf_def (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept;
	inline bool FASTCALL inboundsvf3d (const vectorf_def& v, const vectorf_def& bounds) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectori_def
	{
		union
		{
			int32_t v[4];
			int64_t v64[2];
			int16_t v16[8];
			int8_t v8[16];
		};
		inline vectori_def () noexcept = default;
		inline vectori_def (const int* vector) noexcept { load (vector); }
		inline vectori_def (int x, int y, int z, int w) noexcept { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }
		inline vectori_def (int i) noexcept { v[0] = v[1] = v[2] = v[3] = i; }

		inline operator int* () noexcept { return v; }
		inline operator const int* () const noexcept { return v; }

		inline void FASTCALL load (const int* vector) noexcept { memcpy (v, vector, sizeof (int) * 4); }
		inline void FASTCALL store (int* vector) const noexcept { memcpy (vector, v, sizeof (int) * 4); }

		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		static inline vectori_def FASTCALL shuffle32 (const vectori_def & v1, const vectori_def & v2) noexcept
		{
			return vectori (
				(x >= 0 && x <= 3) ? v1.v[x] : v2.v[4 - x],
				(y >= 0 && y <= 3) ? v1.v[y] : v2.v[4 - y],
				(z >= 0 && z <= 3) ? v2.v[z] : v1.v[4 - z],
				(w >= 0 && w <= 3) ? v2.v[w] : v1.v[4 - w]
			);
		}
		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		inline vectori_def FASTCALL permute32 () const
		{
			return shuffle32<x, y, z, w> (*this, *this);
		}
		template<uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4, uint8_t s5, uint8_t s6, uint8_t s7, uint8_t s8,
			uint8_t s9, uint8_t s10, uint8_t s11, uint8_t s12, uint8_t s13, uint8_t s14, uint8_t s15, uint8_t s16>
			inline vectori_def FASTCALL permute8 () const
		{
			auto arr = reinterpret_cast<const char*>(v);
			vectori_def ret;
			auto destarr = reinterpret_cast<char*>(ret.v);
			destarr[0] = arr[s1]; destarr[1] = arr[s2]; destarr[2] = arr[s3]; destarr[3] = arr[s4];
			destarr[4] = arr[s5]; destarr[5] = arr[s6]; destarr[6] = arr[s7]; destarr[7] = arr[s8];
			destarr[8] = arr[s9]; destarr[9] = arr[s10]; destarr[10] = arr[s11]; destarr[11] = arr[s12];
			destarr[12] = arr[s13]; destarr[13] = arr[s14]; destarr[14] = arr[s15]; destarr[15] = arr[s16];
			return ret;
		}

		inline vectori_def FASTCALL splat_x () const { return vectori_def (v[0]); }
		inline vectori_def FASTCALL splat_y () const { return vectori_def (v[1]); }
		inline vectori_def FASTCALL splat_z () const { return vectori_def (v[2]); }
		inline vectori_def FASTCALL splat_w () const { return vectori_def (v[3]); }

		inline int FASTCALL x () const { return v[0]; }
		inline int FASTCALL y () const { return v[1]; }
		inline int FASTCALL z () const { return v[2]; }
		inline int FASTCALL w () const { return v[3]; }

		static inline bool support () noexcept { return true; }
		static inline bool hwvector () noexcept { return false; }
	};

	inline vectori_def FASTCALL addvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL subtractvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL negatevi (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL multiplyvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL multiplyvi (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL multiplyvi (int s, const vectori_def& v) noexcept;
	inline vectori_def FASTCALL dividevi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dividevi (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL fmavi (const vectori_def& mv1, const vectori_def& mv2, const vectori_def& av) noexcept;
	inline vectori_def FASTCALL fmsvi (const vectori_def& mv1, const vectori_def& mv2, const vectori_def& sv) noexcept;
	inline vectori_def FASTCALL fnmsvi (const vectori_def& sv, const vectori_def& mv1, const vectori_def& mv2) noexcept;
	inline vectori_def FASTCALL andvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL orvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL xorvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL shiftlvi (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL shiftrvi (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL notvi (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL not_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesservi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesser_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greatervi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greater_equalsvi (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL addvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL subtractvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL negatevi8 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL multiplyvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL multiplyvi8 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL multiplyvi8 (int s, const vectori_def& v) noexcept;
	inline vectori_def FASTCALL dividevi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dividevi8 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL not_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesservi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesser_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greatervi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greater_equalsvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL addvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL subtractvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL negatevi16 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL multiplyvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL multiplyvi16 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL multiplyvi16 (int s, const vectori_def& v) noexcept;
	inline vectori_def FASTCALL dividevi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dividevi16 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL shiftlvi16 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL shiftrvi16 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL not_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesservi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesser_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greatervi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greater_equalsvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL addvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL subtractvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL negatevi64 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL multiplyvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL multiplyvi64 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL multiplyvi64 (int s, const vectori_def& v) noexcept;
	inline vectori_def FASTCALL dividevi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dividevi64 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL shiftlvi64 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL shiftrvi64 (const vectori_def& v, int s) noexcept;
	inline vectori_def FASTCALL equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL not_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesservi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL lesser_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greatervi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL greater_equalsvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL dotvi2d (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dotvi3d (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL dotvi4d (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL absvi (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL minvi (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL maxvi (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL absvi64 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL minvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL maxvi64 (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL absvi16 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL minvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL maxvi16 (const vectori_def& v1, const vectori_def& v2) noexcept;

	inline vectori_def FASTCALL absvi8 (const vectori_def& v) noexcept;
	inline vectori_def FASTCALL minvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;
	inline vectori_def FASTCALL maxvi8 (const vectori_def& v1, const vectori_def& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 256-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(32) vector8f_def
	{
		float v[8];

		inline vector8f_def () noexcept = default;
		inline vector8f_def (const float* arr) noexcept { load (arr); }
		inline vector8f_def (float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) noexcept
		{
			v[0] = x1; v[1] = y1; v[2] = z1; v[3] = w1;
			v[4] = x2; v[5] = y2; v[6] = z2; v[7] = w2;
		}
		inline vector8f_def (float scalar) noexcept
		{
			v[0] = v[1] = v[2] = v[3] = v[4] = v[5] = v[6] = v[7] = scalar;
		}
		inline vector8f_def (const vectorf_def & v1, const vectorf_def & v2) noexcept
		{
			memcpy (v, v1.v, sizeof (float) * 4);
			memcpy (v + 4, v2.v, sizeof (float) * 4);
		}

		inline operator float* () noexcept { return v; }
		inline operator const float* () const noexcept { return v; }

		inline void FASTCALL load (const float* arr) noexcept { memcpy (v, arr, sizeof (float) * 8); }
		inline void FASTCALL store (float* arr) const noexcept { memcpy (arr, v, sizeof (float) * 8); }

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		static inline vector8f_def FASTCALL shuffle32 (const vector8f_def & v1, const vector8f_def & v2) noexcept
		{
			return vector8f_def (
				(x1 >= 0 && x1 <= 7) ? v1.v[x1] : v2.v[8 - x1],
				(y1 >= 0 && y1 <= 7) ? v1.v[y1] : v2.v[8 - y1],
				(z1 >= 0 && z1 <= 7) ? v2.v[z1] : v1.v[8 - z1],
				(w1 >= 0 && w1 <= 7) ? v2.v[w1] : v1.v[8 - w1],
				(x2 >= 0 && x2 <= 7) ? v1.v[x2] : v2.v[8 - x2],
				(y2 >= 0 && y2 <= 7) ? v1.v[y2] : v2.v[8 - y2],
				(z2 >= 0 && z2 <= 7) ? v2.v[z2] : v1.v[8 - z2],
				(w2 >= 0 && w2 <= 7) ? v2.v[w2] : v1.v[8 - w2]
			);
		}

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		inline vector8f_def FASTCALL permute32 () const noexcept
		{
			return shuffle32<x1, y1, z1, w1, x2, y2, z2, w2> (*this, *this);
		}

		inline vector8f_def FASTCALL splat_x1 () const noexcept { return vector8f_def (v[0]); }
		inline vector8f_def FASTCALL splat_y1 () const noexcept { return vector8f_def (v[1]); }
		inline vector8f_def FASTCALL splat_z1 () const noexcept { return vector8f_def (v[2]); }
		inline vector8f_def FASTCALL splat_w1 () const noexcept { return vector8f_def (v[3]); }
		inline vector8f_def FASTCALL splat_x2 () const noexcept { return vector8f_def (v[4]); }
		inline vector8f_def FASTCALL splat_y2 () const noexcept { return vector8f_def (v[5]); }
		inline vector8f_def FASTCALL splat_z2 () const noexcept { return vector8f_def (v[6]); }
		inline vector8f_def FASTCALL splat_w2 () const noexcept { return vector8f_def (v[7]); }

		inline float FASTCALL x1 () const noexcept { return v[0]; }
		inline float FASTCALL y1 () const noexcept { return v[1]; }
		inline float FASTCALL z1 () const noexcept { return v[2]; }
		inline float FASTCALL w1 () const noexcept { return v[3]; }
		inline float FASTCALL x2 () const noexcept { return v[4]; }
		inline float FASTCALL y2 () const noexcept { return v[5]; }
		inline float FASTCALL z2 () const noexcept { return v[6]; }
		inline float FASTCALL w2 () const noexcept { return v[7]; }

		inline vectorf_def FASTCALL vector1 () const noexcept { return vectorf_def (v); }
		inline vectorf_def FASTCALL vector2 () const noexcept { return vectorf_def (v + 4); }

		static inline bool support () noexcept { return true; }
		static inline bool hwvector () noexcept { return false; }
	};

	inline vector8f_def FASTCALL addv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL subtractv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL negatev8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL multiplyv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL multiplyv8f (const vector8f_def& v, float s) noexcept;
	inline vector8f_def FASTCALL multiplyv8f (float s, const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL dividev8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL dividev8f (const vector8f_def& v, float s) noexcept;
	inline vector8f_def FASTCALL fmav8f (const vector8f_def& mv1, const vector8f_def& mv2, const vector8f_def& av) noexcept;
	inline vector8f_def FASTCALL fmsv8f (const vector8f_def& mv1, const vector8f_def& mv2, const vector8f_def& sv) noexcept;
	inline vector8f_def FASTCALL fnmsv8f (const vector8f_def& sv, const vector8f_def& mv1, const vector8f_def& mv2) noexcept;
	inline vector8f_def FASTCALL andv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL orv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL xorv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL notv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL not_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL lesserv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL lesser_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL greaterv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL greater_equalsv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;

	inline vector8f_def FASTCALL dotv8f2d (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL dotv8f3d (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL dotv8f4d (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	
	inline vector8f_def FASTCALL sqrtv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL rsqrtv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL rcpv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL roundv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL floorv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL ceilv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL absv8f (const vector8f_def& v) noexcept;
	inline vector8f_def FASTCALL minv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;
	inline vector8f_def FASTCALL maxv8f (const vector8f_def& v1, const vector8f_def& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 256-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(32) vector8i_def
	{
		union
		{
			int32_t v[8];
			uint32_t vu[8];
			int64_t vi64[4];
			uint64_t vu64[4];
			int24_t vi24[8];
			uint24_t vu24[8];
			int16_t vi16[16];
			uint16_t vu16[16];
			int8_t vi8[32];
			uint8_t vu8[32];
		};

		inline vector8i_def () noexcept = default;
		inline vector8i_def (const int* arr) noexcept { load (arr); }
		inline vector8i_def (int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2) noexcept
		{
			v[0] = x1; v[1] = y1; v[2] = z1; v[3] = w1;
			v[4] = x2; v[5] = y2; v[6] = z2; v[7] = w2;
		}
		inline vector8i_def (int scalar) noexcept
		{
			v[0] = v[1] = v[2] = v[3] = v[4] = v[5] = v[6] = v[7] = scalar;
		}
		inline vector8i_def (const vectori_def& v1, const vectori_def& v2) noexcept
		{
			memcpy (v, v1.v, sizeof (int) * 4);
			memcpy (v + 4, v2.v, sizeof (int) * 4);
		}

		inline operator int* () noexcept { return v; }
		inline operator const int* () const noexcept { return v; }

		inline void FASTCALL load (const int* arr) noexcept { memcpy (v, arr, sizeof (int) * 8); }
		inline void FASTCALL store (int* arr) const noexcept { memcpy (arr, v, sizeof (int) * 8); }

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		static inline vector8i_def FASTCALL shuffle32 (const vector8i_def& v1, const vector8i_def& v2) noexcept
		{
			return vector8i_def (
				(x1 >= 0 && x1 <= 7) ? v1.v[x1] : v2.v[8 - x1],
				(y1 >= 0 && y1 <= 7) ? v1.v[y1] : v2.v[8 - y1],
				(z1 >= 0 && z1 <= 7) ? v2.v[z1] : v1.v[8 - z1],
				(w1 >= 0 && w1 <= 7) ? v2.v[w1] : v1.v[8 - w1],
				(x2 >= 0 && x2 <= 7) ? v1.v[x2] : v2.v[8 - x2],
				(y2 >= 0 && y2 <= 7) ? v1.v[y2] : v2.v[8 - y2],
				(z2 >= 0 && z2 <= 7) ? v2.v[z2] : v1.v[8 - z2],
				(w2 >= 0 && w2 <= 7) ? v2.v[w2] : v1.v[8 - w2]
			);
		}

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		inline vector8i_def FASTCALL permute32 () const noexcept
		{
			return shuffle32<x1, y1, z1, w1, x2, y2, z2, w2> (*this, *this);
		}
		template<uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4, uint8_t s5, uint8_t s6, uint8_t s7, uint8_t s8,
			uint8_t s9, uint8_t s10, uint8_t s11, uint8_t s12, uint8_t s13, uint8_t s14, uint8_t s15, uint8_t s16,
			uint8_t s17, uint8_t s18, uint8_t s19, uint8_t s20, uint8_t s21, uint8_t s22, uint8_t s23, uint8_t s24,
			uint8_t s25, uint8_t s26, uint8_t s27, uint8_t s28, uint8_t s29, uint8_t s30, uint8_t s31, uint8_t s32>
			inline vector8i_def FASTCALL permute8 () const
		{
			auto arr = reinterpret_cast<const char*>(v);
			vector8i_def ret;
			auto destarr = reinterpret_cast<char*>(ret.v);
			destarr[0] = arr[s1]; destarr[1] = arr[s2]; destarr[2] = arr[s3]; destarr[3] = arr[s4];
			destarr[4] = arr[s5]; destarr[5] = arr[s6]; destarr[6] = arr[s7]; destarr[7] = arr[s8];
			destarr[8] = arr[s9]; destarr[9] = arr[s10]; destarr[10] = arr[s11]; destarr[11] = arr[s12];
			destarr[12] = arr[s13]; destarr[13] = arr[s14]; destarr[14] = arr[s15]; destarr[15] = arr[s16];
			destarr[16] = arr[s17]; destarr[17] = arr[s18]; destarr[18] = arr[s19]; destarr[19] = arr[s20];
			destarr[20] = arr[s21]; destarr[21] = arr[s22]; destarr[22] = arr[s23]; destarr[23] = arr[s24];
			destarr[24] = arr[s25]; destarr[25] = arr[s26]; destarr[26] = arr[s27]; destarr[27] = arr[s28];
			destarr[28] = arr[s29]; destarr[29] = arr[s30]; destarr[30] = arr[s31]; destarr[31] = arr[s32];
			return ret;
		}

		inline vector8i_def FASTCALL splat_x1 () const noexcept { return vector8i_def (v[0]); }
		inline vector8i_def FASTCALL splat_y1 () const noexcept { return vector8i_def (v[1]); }
		inline vector8i_def FASTCALL splat_z1 () const noexcept { return vector8i_def (v[2]); }
		inline vector8i_def FASTCALL splat_w1 () const noexcept { return vector8i_def (v[3]); }
		inline vector8i_def FASTCALL splat_x2 () const noexcept { return vector8i_def (v[4]); }
		inline vector8i_def FASTCALL splat_y2 () const noexcept { return vector8i_def (v[5]); }
		inline vector8i_def FASTCALL splat_z2 () const noexcept { return vector8i_def (v[6]); }
		inline vector8i_def FASTCALL splat_w2 () const noexcept { return vector8i_def (v[7]); }

		inline int FASTCALL x1 () const noexcept { return v[0]; }
		inline int FASTCALL y1 () const noexcept { return v[1]; }
		inline int FASTCALL z1 () const noexcept { return v[2]; }
		inline int FASTCALL w1 () const noexcept { return v[3]; }
		inline int FASTCALL x2 () const noexcept { return v[4]; }
		inline int FASTCALL y2 () const noexcept { return v[5]; }
		inline int FASTCALL z2 () const noexcept { return v[6]; }
		inline int FASTCALL w2 () const noexcept { return v[7]; }

		inline vectori_def FASTCALL vector1 () const noexcept { return vectori_def (v); }
		inline vectori_def FASTCALL vector2 () const noexcept { return vectori_def (v + 4); }

		static inline bool support () noexcept { return true; }
		static inline bool hwvector () noexcept { return false; }
	};

	inline vector8i_def FASTCALL addv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL subtractv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL negatev8i (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL multiplyv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL multiplyv8i (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL multiplyv8i (int s, const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL dividev8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dividev8i (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL fmav8i (const vector8i_def& mv1, const vector8i_def& mv2, const vector8i_def& av) noexcept;
	inline vector8i_def FASTCALL fmsv8i (const vector8i_def& mv1, const vector8i_def& mv2, const vector8i_def& sv) noexcept;
	inline vector8i_def FASTCALL fnmsv8i (const vector8i_def& sv, const vector8i_def& mv1, const vector8i_def& mv2) noexcept;
	inline vector8i_def FASTCALL andv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL orv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL xorv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL shiftlv8i (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL shiftrv8i (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL notv8i (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL not_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesserv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesser_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greaterv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greater_equalsv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL addv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL subtractv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL negatev8i8 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL multiplyv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL multiplyv8i8 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL multiplyv8i8 (int s, const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL dividev8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dividev8i8 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL not_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesserv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesser_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greaterv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greater_equalsv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL addv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL subtractv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL negatev8i16 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL multiplyv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL multiplyv8i16 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL multiplyv8i16 (int s, const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL dividev8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dividev8i16 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL shiftlv8i16 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL shiftrv8i16 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL not_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesserv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesser_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greaterv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greater_equalsv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL addv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL subtractv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL negatev8i64 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL multiplyv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL multiplyv8i64 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL multiplyv8i64 (int s, const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL dividev8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dividev8i64 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL shiftlv8i64 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL shiftrv8i64 (const vector8i_def& v, int s) noexcept;
	inline vector8i_def FASTCALL equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL not_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesserv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL lesser_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greaterv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL greater_equalsv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL dotv8i2d (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dotv8i3d (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL dotv8i4d (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	
	inline vector8i_def FASTCALL absv8i (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL minv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL maxv8i (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL absv8i64 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL minv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL maxv8i64 (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL absv8i16 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL minv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL maxv8i16 (const vector8i_def& v1, const vector8i_def& v2) noexcept;

	inline vector8i_def FASTCALL absv8i8 (const vector8i_def& v) noexcept;
	inline vector8i_def FASTCALL minv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
	inline vector8i_def FASTCALL maxv8i8 (const vector8i_def& v1, const vector8i_def& v2) noexcept;
}

#include "math.simd.nosimd.conv.inl"
#include "math.simd.nosimd.vectorf.inl"
#include "math.simd.nosimd.vectori.inl"
#include "math.simd.nosimd.vector8f.inl"
#include "math.simd.nosimd.vector8i.inl"

#endif