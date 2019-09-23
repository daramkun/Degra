#if !defined (__DSEED_MATH_SIMD_X86_INL__) && ARCH_X86SET
#define __DSEED_MATH_SIMD_X86_INL__

#	include <xmmintrin.h>									// SSE
#	include <emmintrin.h>									// SSE 2
#	include <pmmintrin.h>									// SSE 3
#	include <smmintrin.h>									// SSE 4.x
#	include <nmmintrin.h>									// SSE 4.x
#	include <immintrin.h>									// AVX

namespace dseed
{
	struct vectorf_x86;
	struct vectori_x86;
	struct vector8f_x86;
	struct vector8i_x86;

	inline vectori_x86 FASTCALL conv_f32_to_i32 (const vectorf_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_f32_to_i32 (const vector8f_x86& v) noexcept;
	inline vectori_x86 FASTCALL reinterpret_f32_to_i32 (const vectorf_x86& v) noexcept;
	inline vector8i_x86 FASTCALL reinterpret8_f32_to_i32 (const vector8f_x86& v) noexcept;

	inline vectorf_x86 FASTCALL conv_i32_to_f32 (const vectori_x86& v) noexcept;
	inline vector8f_x86 FASTCALL conv8_i32_to_f32 (const vector8i_x86& v) noexcept;
	inline vectorf_x86 FASTCALL reinterpret_i32_to_f32 (const vectori_x86& v) noexcept;
	inline vector8f_x86 FASTCALL reinterpret8_i32_to_f32 (const vector8i_x86& v) noexcept;

	inline vectori_x86 FASTCALL conv_i64_to_i32 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i64_to_i32 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i64_to_i16 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i64_to_i16 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i64_to_i8 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i64_to_i8 (const vector8i_x86& v) noexcept;

	inline vectori_x86 FASTCALL conv_i32_to_i64 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i32_to_i64 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i32_to_i16 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i32_to_i16 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i32_to_i8 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i32_to_i8 (const vector8i_x86& v) noexcept;

	inline vectori_x86 FASTCALL conv_i16_to_i64 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i16_to_i64 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i16_to_i32 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i16_to_i32 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i16_to_i8 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i16_to_i8 (const vector8i_x86& v) noexcept;

	inline vectori_x86 FASTCALL conv_i8_to_i64 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i8_to_i64 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i8_to_i32 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i8_to_i32 (const vector8i_x86& v) noexcept;
	inline vectori_x86 FASTCALL conv_i8_to_i16 (const vectori_x86& v) noexcept;
	inline vector8i_x86 FASTCALL conv8_i8_to_i16 (const vector8i_x86& v) noexcept;

	inline int movemaskvf (const vectorf_x86& v) noexcept;
	inline int movemaskv8f (const vector8f_x86& v) noexcept;
	inline int movemaskvi (const vectori_x86& v) noexcept;
	inline int movemaskv8i (const vector8i_x86& v) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectorf_x86
	{
		__m128 v;

		inline vectorf_x86 () noexcept = default;
		inline vectorf_x86 (const float* arr) noexcept { load (arr); }
		inline vectorf_x86 (float x, float y, float z, float w) noexcept : v (_mm_set_ps (w, z, y, x)) { }
		inline vectorf_x86 (float scalar) noexcept : v (_mm_set1_ps (scalar)) { }
		inline vectorf_x86 (const __m128 & v) noexcept : v (v) { }
		inline explicit vectorf_x86 (const vectorf_def& v) noexcept { load (v.v); }

		inline operator __m128& () noexcept { return v; }
		inline operator const __m128& () const noexcept { return v; }
		inline explicit operator vectorf_def () const noexcept
		{
			vectorf_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const float* arr) noexcept { v = _mm_load_ps (arr); }
		inline void FASTCALL store (float* arr) const noexcept { _mm_store_ps (arr, v); }

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		static inline vectorf_x86 FASTCALL shuffle32 (const vectorf_x86 & v1, const vectorf_x86 & v2) noexcept
		{
			return _mm_shuffle_ps (v1, v2, _MM_SHUFFLE (w, z, y, x));
		}

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		inline vectorf_x86 FASTCALL permute32 () const noexcept
		{
			return _mm_permute_ps (v, _MM_SHUFFLE (w, z, y, x));
		}
		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		inline vectorf_x86 FASTCALL permute32_legacy () const noexcept
		{
			return _mm_shuffle_ps (v, v, _MM_SHUFFLE (w, z, y, x));
		}

		inline vectorf_x86 FASTCALL splat_x () const noexcept { return permute32<0, 0, 0, 0> (); }
		inline vectorf_x86 FASTCALL splat_y () const noexcept { return permute32<1, 1, 1, 1> (); }
		inline vectorf_x86 FASTCALL splat_z () const noexcept { return permute32<2, 2, 2, 2> (); }
		inline vectorf_x86 FASTCALL splat_w () const noexcept { return permute32<3, 3, 3, 3> (); }

		inline float FASTCALL x () const noexcept { return _mm_cvtss_f32 (splat_x ()); }
		inline float FASTCALL y () const noexcept { return _mm_cvtss_f32 (splat_y ()); }
		inline float FASTCALL z () const noexcept { return _mm_cvtss_f32 (splat_z ()); }
		inline float FASTCALL w () const noexcept { return _mm_cvtss_f32 (splat_w ()); }

		static inline bool support () noexcept
		{
			auto& instInfo = x86_instruction_info::instance ();
			return instInfo.sse && instInfo.sse2 && instInfo.sse3 && instInfo.ssse3 && instInfo.sse4_1 && instInfo.fma3;
		}
		static inline bool hwvector () noexcept { return true; }
	};

	inline vectorf_x86 FASTCALL addvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL subtractvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL negatevf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL multiplyvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL multiplyvf (const vectorf_x86& v, float s) noexcept;
	inline vectorf_x86 FASTCALL multiplyvf (float s, const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL dividevf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL dividevf (const vectorf_x86& v, float s) noexcept;
	inline vectorf_x86 FASTCALL fmavf (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& av) noexcept;
	inline vectorf_x86 FASTCALL fmsvf (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& sv) noexcept;
	inline vectorf_x86 FASTCALL fnmsvf (const vectorf_x86& sv, const vectorf_x86& mv1, const vectorf_x86& mv2) noexcept;
	inline vectorf_x86 FASTCALL fmavf_legacy (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& av) noexcept;
	inline vectorf_x86 FASTCALL fmsvf_legacy (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& sv) noexcept;
	inline vectorf_x86 FASTCALL fnmsvf_legacy (const vectorf_x86& sv, const vectorf_x86& mv1, const vectorf_x86& mv2) noexcept;
	inline vectorf_x86 FASTCALL andvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL orvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL xorvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL notvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL not_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL lesservf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL lesser_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL greatervf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL greater_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;

	inline vectorf_x86 FASTCALL dotvf2d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL dotvf3d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL dotvf4d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;

	inline vectorf_x86 FASTCALL sqrtvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL rsqrtvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL rcpvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL roundvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL floorvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL ceilvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL absvf (const vectorf_x86& v) noexcept;
	inline vectorf_x86 FASTCALL minvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	inline vectorf_x86 FASTCALL maxvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept;
	
	inline vectorf_x86 FASTCALL selectvf (const vectorf_x86& v1, const vectorf_x86& v2, const vectorf_x86& controlv) noexcept;
	inline vectorf_x86 FASTCALL selectcontrolvf_x86 (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept;
	inline bool FASTCALL inboundsvf3d (const vectorf_x86& v, const vectorf_x86& bounds) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectori_x86
	{
		__m128i v;
		inline vectori_x86 () noexcept = default;
		inline vectori_x86 (const int* vector) noexcept : v (_mm_load_si128 ((const __m128i*)vector)) { }
		inline vectori_x86 (const float* vector) noexcept
			: v (_mm_cvtps_epi32 (_mm_load_ps (vector)))
		{ }
		inline vectori_x86 (int x, int y, int z, int w) noexcept : v (_mm_set_epi32 (w, z, y, x)) { }
		inline vectori_x86 (int i) noexcept : v (_mm_set1_epi32 (i)) { }
		inline vectori_x86 (const __m128i & v) noexcept : v (v) { }
		inline explicit vectori_x86 (const vectori_def& v) noexcept { load (v.v); }

		inline operator __m128i& () noexcept { return v; }
		inline operator const __m128i& () const noexcept { return v; }
		inline explicit operator vectori_def () const noexcept
		{
			vectori_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const int* vector) noexcept { v = _mm_load_si128 ((const __m128i*)vector); }
		inline void FASTCALL store (int* vector) const noexcept { _mm_store_si128 ((__m128i*)vector, v); }

		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		static inline vectori_x86 FASTCALL shuffle32 (const vectori_x86 & v1, const vectori_x86 & v2) noexcept
		{
			return _mm_castps_si128 (_mm_shuffle_ps (
				_mm_castsi128_ps (v1.v), _mm_castsi128_ps (v2.v), _MM_SHUFFLE (w, z, y, x)
			));
		}
		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		inline vectori_x86 FASTCALL permute32 () const
		{
			return shuffle32<x, y, z, w> (*this, *this);
		}
		template<uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4, uint8_t s5, uint8_t s6, uint8_t s7, uint8_t s8,
			uint8_t s9, uint8_t s10, uint8_t s11, uint8_t s12, uint8_t s13, uint8_t s14, uint8_t s15, uint8_t s16>
			inline vectori_x86 FASTCALL permute8 () const
		{
			const static __m128i shuffle = _mm_set_epi8 (s16, s15, s14, s13, s12, s11, s10, s9, s8, s7, s6, s5, s4, s3, s2, s1);
			return _mm_shuffle_epi8 (v, shuffle);
		}

		inline vectori_x86 FASTCALL splat_x () const { return permute32<0, 0, 0, 0> (); }
		inline vectori_x86 FASTCALL splat_y () const { return permute32<1, 1, 1, 1> (); }
		inline vectori_x86 FASTCALL splat_z () const { return permute32<2, 2, 2, 2> (); }
		inline vectori_x86 FASTCALL splat_w () const { return permute32<3, 3, 3, 3> (); }

		inline int FASTCALL x () const { return _mm_cvtss_si32 (_mm_castsi128_ps (splat_x ())); }
		inline int FASTCALL y () const { return _mm_cvtss_si32 (_mm_castsi128_ps (splat_y ())); }
		inline int FASTCALL z () const { return _mm_cvtss_si32 (_mm_castsi128_ps (splat_z ())); }
		inline int FASTCALL w () const { return _mm_cvtss_si32 (_mm_castsi128_ps (splat_w ())); }

		static inline bool support () noexcept
		{
			auto& instInfo = x86_instruction_info::instance ();
			return instInfo.sse2 && instInfo.sse3 && instInfo.ssse3 && instInfo.sse4_1;
		}
		static inline bool hwvector () noexcept { return true; }
	};

	inline vectori_x86 FASTCALL addvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL subtractvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL negatevi (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL multiplyvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL multiplyvi (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL multiplyvi (int s, const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL dividevi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dividevi (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL fmavi (const vectori_x86& mv1, const vectori_x86& mv2, const vectori_x86& av) noexcept;
	inline vectori_x86 FASTCALL fmsvi (const vectori_x86& mv1, const vectori_x86& mv2, const vectori_x86& sv) noexcept;
	inline vectori_x86 FASTCALL fnmsvi (const vectori_x86& sv, const vectori_x86& mv1, const vectori_x86& mv2) noexcept;
	inline vectori_x86 FASTCALL andvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL orvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL xorvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL shiftlvi (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL shiftrvi (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL notvi (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL not_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesservi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesser_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greatervi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greater_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL addvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL subtractvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL negatevi8 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL multiplyvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL multiplyvi8 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL multiplyvi8 (int s, const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL dividevi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dividevi8 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL not_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesservi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesser_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greatervi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greater_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL addvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL subtractvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL negatevi16 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL multiplyvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL multiplyvi16 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL multiplyvi16 (int s, const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL dividevi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dividevi16 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL shiftlvi16 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL shiftrvi16 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL not_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesservi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesser_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greatervi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greater_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL addvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL subtractvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL negatevi64 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL multiplyvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL multiplyvi64 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL multiplyvi64 (int s, const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL dividevi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dividevi64 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL shiftlvi64 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL shiftrvi64 (const vectori_x86& v, int s) noexcept;
	inline vectori_x86 FASTCALL equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL not_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesservi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL lesser_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greatervi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL greater_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL dotvi2d (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dotvi3d (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL dotvi4d (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL absvi (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL minvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL maxvi (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL absvi64 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL minvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL maxvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL absvi16 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL minvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL maxvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	inline vectori_x86 FASTCALL absvi8 (const vectori_x86& v) noexcept;
	inline vectori_x86 FASTCALL minvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;
	inline vectori_x86 FASTCALL maxvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 256-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(32) vector8f_x86
	{
		__m256 v;

		inline vector8f_x86 () noexcept = default;
		inline vector8f_x86 (const float* arr) noexcept : v (_mm256_load_ps (arr)) { }
		inline vector8f_x86 (float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) noexcept
			: v (_mm256_set_ps (w2, z2, y2, x2, w1, z1, y1, x1))
		{ }
		inline vector8f_x86 (float scalar) noexcept
			: v (_mm256_set1_ps (scalar))
		{ }
		inline vector8f_x86 (const vectorf_x86 & v1, const vectorf_x86 & v2) noexcept
		{
			v = _mm256_castps128_ps256 (v1);
			v = _mm256_insertf128_ps (v, v2, 1);
		}
		inline vector8f_x86 (const __m256 & v) noexcept : v (v) { }
		inline explicit vector8f_x86 (const vector8f_def& v) noexcept { load (v.v); }

		inline operator __m256 () noexcept { return v; }
		inline operator const __m256& () const noexcept { return v; }
		inline explicit operator vector8f_def () const noexcept
		{
			vector8f_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const float* arr) noexcept { v = _mm256_load_ps (arr); }
		inline void FASTCALL store (float* arr) const noexcept { _mm256_store_ps (arr, v); }

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		static inline vector8f_x86 FASTCALL shuffle32 (const vector8f_x86 & v1, const vector8f_x86 & v2) noexcept
		{
			return _mm256_shuffle_ps (v1, v2, _MM_SHUFFLE (w2, z2, y2, x2) << 8 | _MM_SHUFFLE (w1, z1, y1, x1));
		}

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		inline vector8f_x86 FASTCALL permute32 () const noexcept
		{
			return _mm256_permute_ps (v, _MM_SHUFFLE (w2, z2, y2, x2) << 8 | _MM_SHUFFLE (w1, z1, y1, x1));
		}

		inline vector8f_x86 FASTCALL splat_x1 () const noexcept { return permute32<0, 0, 0, 0, 0, 0, 0, 0> (); }
		inline vector8f_x86 FASTCALL splat_y1 () const noexcept { return permute32<1, 1, 1, 1, 1, 1, 1, 1> (); }
		inline vector8f_x86 FASTCALL splat_z1 () const noexcept { return permute32<2, 2, 2, 2, 2, 2, 2, 2> (); }
		inline vector8f_x86 FASTCALL splat_w1 () const noexcept { return permute32<3, 3, 3, 3, 3, 3, 3, 3> (); }
		inline vector8f_x86 FASTCALL splat_x2 () const noexcept { return permute32<4, 4, 4, 4, 4, 4, 4, 4> (); }
		inline vector8f_x86 FASTCALL splat_y2 () const noexcept { return permute32<5, 5, 5, 5, 5, 5, 5, 5> (); }
		inline vector8f_x86 FASTCALL splat_z2 () const noexcept { return permute32<6, 6, 6, 6, 6, 6, 6, 6> (); }
		inline vector8f_x86 FASTCALL splat_w2 () const noexcept { return permute32<7, 7, 7, 7, 7, 7, 7, 7> (); }

		inline float FASTCALL x1 () const noexcept { return _mm256_cvtss_f32 (splat_x1 ()); }
		inline float FASTCALL y1 () const noexcept { return _mm256_cvtss_f32 (splat_y1 ()); }
		inline float FASTCALL z1 () const noexcept { return _mm256_cvtss_f32 (splat_z1 ()); }
		inline float FASTCALL w1 () const noexcept { return _mm256_cvtss_f32 (splat_w1 ()); }
		inline float FASTCALL x2 () const noexcept { return _mm256_cvtss_f32 (splat_x2 ()); }
		inline float FASTCALL y2 () const noexcept { return _mm256_cvtss_f32 (splat_y2 ()); }
		inline float FASTCALL z2 () const noexcept { return _mm256_cvtss_f32 (splat_z2 ()); }
		inline float FASTCALL w2 () const noexcept { return _mm256_cvtss_f32 (splat_w2 ()); }

		inline vectorf_x86 FASTCALL vector1 () const noexcept
		{
			return _mm256_castps256_ps128 (v);
		}
		inline vectorf_x86 FASTCALL vector2 () const noexcept
		{
			return _mm256_extractf128_ps (v, 1);
		}

		static inline bool support () noexcept
		{
			auto& instInfo = x86_instruction_info::instance ();
			return instInfo.avx && instInfo.fma3;
		}
		static inline bool hwvector () noexcept { return true; }
	};

	inline vector8f_x86 FASTCALL addv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL subtractv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL negatev8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL multiplyv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL multiplyv8f (const vector8f_x86& v, float s) noexcept;
	inline vector8f_x86 FASTCALL multiplyv8f (float s, const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL dividev8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL dividev8f (const vector8f_x86& v, float s) noexcept;
	inline vector8f_x86 FASTCALL fmav8f (const vector8f_x86& mv1, const vector8f_x86& mv2, const vector8f_x86& av) noexcept;
	inline vector8f_x86 FASTCALL fmsv8f (const vector8f_x86& mv1, const vector8f_x86& mv2, const vector8f_x86& sv) noexcept;
	inline vector8f_x86 FASTCALL fnmsv8f (const vector8f_x86& sv, const vector8f_x86& mv1, const vector8f_x86& mv2) noexcept;
	inline vector8f_x86 FASTCALL andv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL orv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL xorv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL notv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL not_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL lesserv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL lesser_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL greaterv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL greater_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;

	inline vector8f_x86 FASTCALL dotv8f2d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL dotv8f3d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL dotv8f4d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;

	inline vector8f_x86 FASTCALL sqrtv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL rsqrtv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL rcpv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL roundv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL floorv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL ceilv8f (const vector8f_x86& v) noexcept;
	inline vector8f_x86 FASTCALL minv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;
	inline vector8f_x86 FASTCALL maxv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 256-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(32) vector8i_x86
	{
		__m256i v;

		inline vector8i_x86 () noexcept = default;
		inline vector8i_x86 (const int* arr) noexcept : v (_mm256_load_si256 ((const __m256i*)arr)) { }
		inline vector8i_x86 (int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2) noexcept
			: v (_mm256_set_epi32 (w2, z2, y2, x2, w1, z1, y1, x1))
		{ }
		inline vector8i_x86 (int scalar) noexcept
			: v (_mm256_set1_epi32 (scalar))
		{ }
		inline vector8i_x86 (const vectori_x86 & v1, const vectori_x86 & v2) noexcept
		{
			v = _mm256_castsi128_si256 (v1);
			v = _mm256_inserti128_si256 (v, v2, 1);
		}
		inline vector8i_x86 (const __m256i & v) noexcept : v (v) { }
		inline explicit vector8i_x86 (const vector8i_def& v) { load (v.v); }

		inline operator __m256i& () noexcept { return v; }
		inline operator const __m256i& () const noexcept { return v; }
		inline explicit operator vector8i_def () const noexcept
		{
			vector8i_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const int* arr) noexcept { v = _mm256_load_si256 ((const __m256i*)arr); }
		inline void FASTCALL store (int* arr) const noexcept { _mm256_store_si256 ((__m256i*)arr, v); }

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		static inline vector8i_x86 FASTCALL shuffle32 (const vector8i_x86 & v1, const vector8i_x86 & v2) noexcept
		{
			return _mm256_castps_si256 (
				_mm_256_shuffle_ps (
					_mm256_castsi256_ps (v1.v),
					_mm256_castsi256_ps (v2.v),
					_MM_SHUFFLE (w2, z2, y2, x2) << 8 | _MM_SHUFFLE (w1, z1, y1, x1)
				)
			);
		}

		template<uint32_t x1, uint32_t y1, uint32_t z1, uint32_t w1, uint32_t x2, uint32_t y2, uint32_t z2, uint32_t w2>
		inline vector8i_x86 FASTCALL permute32 () const noexcept
		{
			return _mm256_shuffle_epi32 (v, _MM_SHUFFLE (w2, z2, y2, x2) << 8 | _MM_SHUFFLE (w1, z1, y1, x1));
		}
		template<uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4, uint8_t s5, uint8_t s6, uint8_t s7, uint8_t s8,
			uint8_t s9, uint8_t s10, uint8_t s11, uint8_t s12, uint8_t s13, uint8_t s14, uint8_t s15, uint8_t s16,
			uint8_t s17, uint8_t s18, uint8_t s19, uint8_t s20, uint8_t s21, uint8_t s22, uint8_t s23, uint8_t s24,
			uint8_t s25, uint8_t s26, uint8_t s27, uint8_t s28, uint8_t s29, uint8_t s30, uint8_t s31, uint8_t s32>
			inline vector8i_x86 FASTCALL permute8 () const
		{
			const static __m128i shuffle = _mm256_set_epi8 (s32, s31, s30, s29, s28, s27, s26, s25, s24, s23, s22, s21, s20, s19, s18, s17, s16, s15, s14, s13, s12, s11, s10, s9, s8, s7, s6, s5, s4, s3, s2, s1);
			return _mm256_shuffle_epi8 (v, shuffle);
		}

		inline vector8i_x86 FASTCALL splat_x1 () const noexcept { return permute32<0, 0, 0, 0, 0, 0, 0, 0> (); }
		inline vector8i_x86 FASTCALL splat_y1 () const noexcept { return permute32<1, 1, 1, 1, 1, 1, 1, 1> (); }
		inline vector8i_x86 FASTCALL splat_z1 () const noexcept { return permute32<2, 2, 2, 2, 2, 2, 2, 2> (); }
		inline vector8i_x86 FASTCALL splat_w1 () const noexcept { return permute32<3, 3, 3, 3, 3, 3, 3, 3> (); }
		inline vector8i_x86 FASTCALL splat_x2 () const noexcept { return permute32<4, 4, 4, 4, 4, 4, 4, 4> (); }
		inline vector8i_x86 FASTCALL splat_y2 () const noexcept { return permute32<5, 5, 5, 5, 5, 5, 5, 5> (); }
		inline vector8i_x86 FASTCALL splat_z2 () const noexcept { return permute32<6, 6, 6, 6, 6, 6, 6, 6> (); }
		inline vector8i_x86 FASTCALL splat_w2 () const noexcept { return permute32<7, 7, 7, 7, 7, 7, 7, 7> (); }

		inline int FASTCALL x1 () const noexcept { return _mm256_cvtsi256_si32 (splat_x1 ()); }
		inline int FASTCALL y1 () const noexcept { return _mm256_cvtsi256_si32 (splat_y1 ()); }
		inline int FASTCALL z1 () const noexcept { return _mm256_cvtsi256_si32 (splat_z1 ()); }
		inline int FASTCALL w1 () const noexcept { return _mm256_cvtsi256_si32 (splat_w1 ()); }
		inline int FASTCALL x2 () const noexcept { return _mm256_cvtsi256_si32 (splat_x2 ()); }
		inline int FASTCALL y2 () const noexcept { return _mm256_cvtsi256_si32 (splat_y2 ()); }
		inline int FASTCALL z2 () const noexcept { return _mm256_cvtsi256_si32 (splat_z2 ()); }
		inline int FASTCALL w2 () const noexcept { return _mm256_cvtsi256_si32 (splat_w2 ()); }

		inline vectori_x86 FASTCALL vector1 () const noexcept
		{
			return _mm256_castsi256_si128 (v);
		}
		inline vectori_x86 FASTCALL vector2 () const noexcept
		{
			return _mm256_extracti128_si256 (v, 1);
		}

		static inline bool support () noexcept { return x86_instruction_info::instance ().avx2; }
		static inline bool hwvector () noexcept { return true; }
	};

	inline vector8i_x86 FASTCALL addv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL subtractv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL negatev8i (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i (int s, const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL dividev8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dividev8i (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL fmav8i (const vector8i_x86& mv1, const vector8i_x86& mv2, const vector8i_x86& av) noexcept;
	inline vector8i_x86 FASTCALL fmsv8i (const vector8i_x86& mv1, const vector8i_x86& mv2, const vector8i_x86& sv) noexcept;
	inline vector8i_x86 FASTCALL fnmsv8i (const vector8i_x86& sv, const vector8i_x86& mv1, const vector8i_x86& mv2) noexcept;
	inline vector8i_x86 FASTCALL andv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL orv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL xorv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL shiftlv8i (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL shiftrv8i (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL notv8i (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL not_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesserv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesser_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greaterv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greater_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL addv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL subtractv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL negatev8i8 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i8 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i8 (int s, const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL dividev8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dividev8i8 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL not_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesserv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesser_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greaterv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greater_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL addv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL subtractv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL negatev8i16 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i16 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i16 (int s, const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL dividev8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dividev8i16 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL shiftlv8i16 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL shiftrv8i16 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL not_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesserv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesser_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greaterv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greater_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL addv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL subtractv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL negatev8i64 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i64 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL multiplyv8i64 (int s, const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL dividev8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dividev8i64 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL shiftlv8i64 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL shiftrv8i64 (const vector8i_x86& v, int s) noexcept;
	inline vector8i_x86 FASTCALL equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL not_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesserv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL lesser_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greaterv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL greater_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL dotvf2d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dotvf3d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL dotvf4d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL absv8i (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL minv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL maxv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL absv8i64 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL minv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL maxv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL absv8i16 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL minv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL maxv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	inline vector8i_x86 FASTCALL absv8i8 (const vector8i_x86& v) noexcept;
	inline vector8i_x86 FASTCALL minv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;
	inline vector8i_x86 FASTCALL maxv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation vector conversions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_x86 FASTCALL conv_f32_to_i32 (const vectorf_x86& v) noexcept { return _mm_cvtps_epi32 (v); }
	inline vector8i_x86 FASTCALL conv8_f32_to_i32 (const vector8f_x86& v) noexcept { return _mm256_cvtps_epi32 (v); }
	inline vectori_x86 FASTCALL reinterpret_f32_to_i32 (const vectorf_x86& v) noexcept { return _mm_castps_si128 (v); }
	inline vector8i_x86 FASTCALL reinterpret8_f32_to_i32 (const vector8f_x86& v) noexcept { return _mm256_castps_si256 (v); }

	inline vectorf_x86 FASTCALL conv_i32_to_f32 (const vectori_x86& v) noexcept { return _mm_cvtepi32_ps (v); }
	inline vector8f_x86 FASTCALL conv8_i32_to_f32 (const vector8i_x86& v) noexcept { return _mm256_cvtepi32_ps (v); }
	inline vectorf_x86 FASTCALL reinterpret_i32_to_f32 (const vectori_x86& v) noexcept { return _mm_castsi128_ps (v); }
	inline vector8f_x86 FASTCALL reinterpret8_i32_to_f32 (const vector8i_x86& v) noexcept { return _mm256_castsi256_ps (v); }

	inline vectori_x86 FASTCALL conv_i64_to_i32 (const vectori_x86& v) noexcept { return _mm_cvtepi64_epi32 (v); }
	inline vector8i_x86 FASTCALL conv8_i64_to_i32 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi64_epi32 (v), vectori_x86 ()); }
	inline vectori_x86 FASTCALL conv_i64_to_i16 (const vectori_x86& v) noexcept { return _mm_cvtepi64_epi16 (v); }
	inline vector8i_x86 FASTCALL conv8_i64_to_i16 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi64_epi16 (v), vectori_x86 ()); }
	inline vectori_x86 FASTCALL conv_i64_to_i8 (const vectori_x86& v) noexcept { return _mm_cvtepi64_epi8 (v); }
	inline vector8i_x86 FASTCALL conv8_i64_to_i8 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi64_epi8 (v), vectori_x86 ()); }

	inline vectori_x86 FASTCALL conv_i32_to_i64 (const vectori_x86& v) noexcept { return _mm_cvtepi32_epi64 (v); }
	inline vector8i_x86 FASTCALL conv8_i32_to_i64 (const vector8i_x86& v) noexcept { return _mm256_cvtepi32_epi64 (v.vector1 ()); }
	inline vectori_x86 FASTCALL conv_i32_to_i16 (const vectori_x86& v) noexcept { return _mm_cvtepi32_epi16 (v); }
	inline vector8i_x86 FASTCALL conv8_i32_to_i16 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi32_epi16 (v), vectori_x86 ()); }
	inline vectori_x86 FASTCALL conv_i32_to_i8 (const vectori_x86& v) noexcept { return _mm_cvtepi32_epi8 (v); }
	inline vector8i_x86 FASTCALL conv8_i32_to_i8 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi32_epi8 (v), vectori_x86 ()); }

	inline vectori_x86 FASTCALL conv_i16_to_i64 (const vectori_x86& v) noexcept { return _mm_cvtepi16_epi64 (v); }
	inline vector8i_x86 FASTCALL conv8_i16_to_i64 (const vector8i_x86& v) noexcept { return _mm256_cvtepi16_epi64 (v.vector1 ()); }
	inline vectori_x86 FASTCALL conv_i16_to_i32 (const vectori_x86& v) noexcept { return _mm_cvtepi16_epi32 (v); }
	inline vector8i_x86 FASTCALL conv8_i16_to_i32 (const vector8i_x86& v) noexcept { return _mm256_cvtepi16_epi32 (v.vector1 ()); }
	inline vectori_x86 FASTCALL conv_i16_to_i8 (const vectori_x86& v) noexcept { return _mm_cvtepi16_epi8 (v); }
	inline vector8i_x86 FASTCALL conv8_i16_to_i8 (const vector8i_x86& v) noexcept { return vector8i_x86 (_mm256_cvtepi16_epi8 (v), vectori_x86 ()); }

	inline vectori_x86 FASTCALL conv_i8_to_i64 (const vectori_x86& v) noexcept { return _mm_cvtepi8_epi64 (v); }
	inline vector8i_x86 FASTCALL conv8_i8_to_i64 (const vector8i_x86& v) noexcept { return _mm256_cvtepi8_epi64 (v.vector1 ()); }
	inline vectori_x86 FASTCALL conv_i8_to_i32 (const vectori_x86& v) noexcept { return _mm_cvtepi8_epi32 (v); }
	inline vector8i_x86 FASTCALL conv8_i8_to_i32 (const vector8i_x86& v) noexcept { return _mm256_cvtepi8_epi32 (v.vector1 ()); }
	inline vectori_x86 FASTCALL conv_i8_to_i16 (const vectori_x86& v) noexcept { return _mm_cvtepi8_epi16 (v); }
	inline vector8i_x86 FASTCALL conv8_i8_to_i16 (const vector8i_x86& v) noexcept { return _mm256_cvtepi8_epi16 (v.vector1 ()); }

	inline int movemaskvf (const vectorf_x86& v) noexcept { return _mm_movemask_ps (v); }
	inline int movemaskv8f (const vector8f_x86& v) noexcept { return _mm256_movemask_ps (v); }
	inline int movemaskvi (const vectori_x86& v) noexcept { return _mm_movemask_epi8 (v); }
	inline int movemaskv8i (const vector8i_x86& v) noexcept { return _mm256_movemask_epi8 (v); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf_x86 FASTCALL addvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_add_ps (v1, v2); }
	inline vectorf_x86 FASTCALL subtractvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_sub_ps (v1, v2); }
	inline vectorf_x86 FASTCALL negatevf (const vectorf_x86& v) noexcept
	{
		static auto zero = vectorf_x86 (0, 0, 0, 0);
		return _mm_sub_ps (zero, v);
	}
	inline vectorf_x86 FASTCALL multiplyvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_mul_ps (v1, v2); }
	inline vectorf_x86 FASTCALL multiplyvf (const vectorf_x86& v, float s) noexcept { return _mm_mul_ps (v, vectorf_x86 (s)); }
	inline vectorf_x86 FASTCALL multiplyvf (float s, const vectorf_x86& v) noexcept { return multiplyvf (v, s); }
	inline vectorf_x86 FASTCALL dividevf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_div_ps (v1, v2); }
	inline vectorf_x86 FASTCALL dividevf (const vectorf_x86& v, float s) noexcept { return _mm_div_ps (v, vectorf_x86 (s)); }
	inline vectorf_x86 FASTCALL fmavf (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& av) noexcept { return _mm_fmadd_ps (mv1, mv2, av); }
	inline vectorf_x86 FASTCALL fmsvf (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& sv) noexcept { return _mm_fmsub_ps (mv1, mv2, sv); }
	inline vectorf_x86 FASTCALL fnmsvf (const vectorf_x86& sv, const vectorf_x86& mv1, const vectorf_x86& mv2) noexcept { return _mm_fnmsub_ps (mv1, mv2, sv); }
	inline vectorf_x86 FASTCALL fmavf_legacy (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& av) noexcept { return addvf (multiplyvf (mv1, mv2), av); }
	inline vectorf_x86 FASTCALL fmsvf_legacy (const vectorf_x86& mv1, const vectorf_x86& mv2, const vectorf_x86& sv) noexcept { return subtractvf (multiplyvf (mv1, mv2), sv); }
	inline vectorf_x86 FASTCALL fnmsvf_legacy (const vectorf_x86& sv, const vectorf_x86& mv1, const vectorf_x86& mv2) noexcept { return subtractvf (sv, multiplyvf (mv1, mv2)); }
	inline vectorf_x86 FASTCALL andvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return reinterpret_i32_to_f32 (andvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2))); }
	inline vectorf_x86 FASTCALL orvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return reinterpret_i32_to_f32 (orvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2))); }
	inline vectorf_x86 FASTCALL xorvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return reinterpret_i32_to_f32 (xorvi (reinterpret_f32_to_i32 (v1), reinterpret_f32_to_i32 (v2))); }
	inline vectorf_x86 FASTCALL notvf (const vectorf_x86& v) noexcept { return reinterpret_i32_to_f32 (notvi (reinterpret_f32_to_i32 (v))); }
	inline vectorf_x86 FASTCALL equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmpeq_ps (v1, v2); }
	inline vectorf_x86 FASTCALL not_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmpneq_ps (v1, v2); }
	inline vectorf_x86 FASTCALL lesservf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmplt_ps (v1, v2); }
	inline vectorf_x86 FASTCALL lesser_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmple_ps (v1, v2); }
	inline vectorf_x86 FASTCALL greatervf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmpgt_ps (v1, v2); }
	inline vectorf_x86 FASTCALL greater_equalsvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_cmpge_ps (v1, v2); }

	inline vectorf_x86 FASTCALL dotvf2d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_dp_ps (v1, v2, 0x3f); }
	inline vectorf_x86 FASTCALL dotvf3d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_dp_ps (v1, v2, 0x7f); }
	inline vectorf_x86 FASTCALL dotvf4d (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_dp_ps (v1, v2, 0xff); }

	inline vectorf_x86 FASTCALL sqrtvf (const vectorf_x86& v) noexcept { return _mm_sqrt_ps (v); }
	inline vectorf_x86 FASTCALL rsqrtvf (const vectorf_x86& v) noexcept { return _mm_rsqrt_ps (v); }
	inline vectorf_x86 FASTCALL rcpvf (const vectorf_x86& v) noexcept { return _mm_rcp_ps (v); }
	inline vectorf_x86 FASTCALL roundvf (const vectorf_x86& v) noexcept { return _mm_round_ps (v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC); }
	inline vectorf_x86 FASTCALL floorvf (const vectorf_x86& v) noexcept { return _mm_floor_ps (v); }
	inline vectorf_x86 FASTCALL ceilvf (const vectorf_x86& v) noexcept { return _mm_ceil_ps (v); }
	inline vectorf_x86 FASTCALL absvf (const vectorf_x86& v) noexcept
	{
		static vectori_x86 signing (0x7fffffff);
		return reinterpret_i32_to_f32 (andvi (reinterpret_f32_to_i32 (v), signing));
	}
	inline vectorf_x86 FASTCALL minvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_min_ps (v1, v2); }
	inline vectorf_x86 FASTCALL maxvf (const vectorf_x86& v1, const vectorf_x86& v2) noexcept { return _mm_max_ps (v1, v2); }
	
	inline vectorf_x86 FASTCALL selectvf (const vectorf_x86& v1, const vectorf_x86& v2, const vectorf_x86& controlv) noexcept
	{
		vectorf_x86 temp1 = _mm_andnot_ps (controlv, v1);
		vectorf_x86 temp2 = _mm_and_ps (v2, controlv);
		return _mm_or_ps (temp1, temp2);
	}
	inline vectorf_x86 FASTCALL selectcontrolvf_x86 (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept
	{
		static vectori_x86 zero (0, 0, 0, 0);
		return _mm_castsi128_ps (_mm_cmpgt_epi32 (_mm_set_epi32 (v3, v2, v1, v0), zero));
	}
	inline bool FASTCALL inboundsvf3d (const vectorf_x86& v, const vectorf_x86& bounds) noexcept
	{
		vectorf_x86 temp1 = lesser_equalsvf (v, bounds);
		vectorf_x86 temp2 = lesser_equalsvf (negatevf(bounds), v);
		return (((movemaskvf (andvf (temp1, temp2)) & 0x7) == 0x7) != 0);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_x86 FASTCALL addvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_add_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL subtractvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_sub_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL negatevi (const vectori_x86& v) noexcept
	{
		static auto zero = vectori_x86 (0, 0, 0, 0);
		return _mm_sub_epi32 (zero, v);
	}
	inline vectori_x86 FASTCALL multiplyvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_mul_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL multiplyvi (const vectori_x86& v, int s) noexcept { return multiplyvi (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL multiplyvi (int s, const vectori_x86& v) noexcept { return multiplyvi (v, s); }
	inline vectori_x86 FASTCALL dividevi (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		//  TODO
		//return conv_f32_to_i32 (dividevf (conv_i32_to_f32 (v1), conv_i32_to_f32 (v2)));
		return (vectori_x86)dividevi ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL dividevi (const vectori_x86& v, int s) noexcept { return dividevi (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL fmavi (const vectori_x86& mv1, const vectori_x86& mv2, const vectori_x86& av) noexcept { return addvi (multiplyvi (mv1, mv2), av); }
	inline vectori_x86 FASTCALL fmsvi (const vectori_x86& mv1, const vectori_x86& mv2, const vectori_x86& sv) noexcept { return subtractvi (multiplyvi (mv1, mv2), sv); }
	inline vectori_x86 FASTCALL fnmsvi (const vectori_x86& sv, const vectori_x86& mv1, const vectori_x86& mv2) noexcept { return subtractvi (sv, multiplyvi (mv1, mv2)); }
	inline vectori_x86 FASTCALL andvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_and_si128 (v1, v2); }
	inline vectori_x86 FASTCALL orvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_or_si128 (v1, v2); }
	inline vectori_x86 FASTCALL xorvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_xor_si128 (v1, v2); }
	inline vectori_x86 FASTCALL shiftlvi (const vectori_x86& v, int s) noexcept { return _mm_slli_epi32 (v, s); }
	inline vectori_x86 FASTCALL shiftrvi (const vectori_x86& v, int s) noexcept { return _mm_srai_epi32 (v, s); }
	inline vectori_x86 FASTCALL notvi (const vectori_x86& v) noexcept { return _mm_xor_si128 (v, _mm_cmpeq_epi32 (v, v)); }
	inline vectori_x86 FASTCALL equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpeq_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL not_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return notvi (equalsvi (v1, v2)); }
	inline vectori_x86 FASTCALL lesservi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmplt_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL lesser_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (lesservi (v1, v2), equalsvi (v1, v2)); }
	inline vectori_x86 FASTCALL greatervi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpgt_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL greater_equalsvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (greatervi (v1, v2), equalsvi (v1, v2)); }

	inline vectori_x86 FASTCALL addvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_add_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL subtractvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_sub_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL negatevi8 (const vectori_x86& v) noexcept
	{
		static auto zero = vectori_x86 (0, 0, 0, 0);
		return subtractvi8 (zero, v);
	}
	inline vectori_x86 FASTCALL multiplyvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// TODO
		return (vectori_x86)multiplyvi8 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL multiplyvi8 (const vectori_x86& v, int s) noexcept { return multiplyvi8 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL multiplyvi8 (int s, const vectori_x86& v) noexcept { return multiplyvi8 (v, s); }
	inline vectori_x86 FASTCALL dividevi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// TODO
		return (vectori_x86)dividevi8 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL dividevi8 (const vectori_x86& v, int s) noexcept { return dividevi8 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpeq_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL not_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return notvi (equalsvi8 (v1, v2)); }
	inline vectori_x86 FASTCALL lesservi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmplt_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL lesser_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (lesservi8 (v1, v2), equalsvi8 (v1, v2)); }
	inline vectori_x86 FASTCALL greatervi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpgt_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL greater_equalsvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (greatervi8 (v1, v2), equalsvi8 (v1, v2)); }

	inline vectori_x86 FASTCALL addvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_add_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL subtractvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_sub_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL negatevi16 (const vectori_x86& v) noexcept
	{
		static auto zero = vectori_x86 (0, 0, 0, 0);
		return subtractvi16 (zero, v);
	}
	inline vectori_x86 FASTCALL multiplyvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// TODO
		return (vectori_x86)multiplyvi16 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL multiplyvi16 (const vectori_x86& v, int s) noexcept { return multiplyvi16 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL multiplyvi16 (int s, const vectori_x86& v) noexcept { return multiplyvi16 (v, s); }
	inline vectori_x86 FASTCALL dividevi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// TODO
		return (vectori_x86)dividevi16 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL dividevi16 (const vectori_x86& v, int s) noexcept { return dividevi16 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL shiftlvi16 (const vectori_x86& v, int s) noexcept { return _mm_slli_epi16 (v, s); }
	inline vectori_x86 FASTCALL shiftrvi16 (const vectori_x86& v, int s) noexcept { return _mm_srai_epi16 (v, s); }
	inline vectori_x86 FASTCALL equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpeq_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL not_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return notvi (equalsvi16 (v1, v2)); }
	inline vectori_x86 FASTCALL lesservi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmplt_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL lesser_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (lesservi16 (v1, v2), equalsvi16 (v1, v2)); }
	inline vectori_x86 FASTCALL greatervi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpgt_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL greater_equalsvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (greatervi16 (v1, v2), equalsvi16 (v1, v2)); }

	inline vectori_x86 FASTCALL addvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_add_epi64 (v1, v2); }
	inline vectori_x86 FASTCALL subtractvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_sub_epi64 (v1, v2); }
	inline vectori_x86 FASTCALL negatevi64 (const vectori_x86& v) noexcept
	{
		static auto zero = vectori_x86 (0, 0, 0, 0);
		return subtractvi64 (zero, v);
	}
	inline vectori_x86 FASTCALL multiplyvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// https://stackoverflow.com/questions/17863411/sse-multiplication-of-2-64-bit-integers
		__m128i ac = _mm_mul_epu32 (v1, v2);
		__m128i b = _mm_srli_epi64 (v1, 32);
		__m128i bc = _mm_mul_epu32 (b, v2);
		__m128i d = _mm_srli_epi64 (v2, 32);
		__m128i ad = _mm_mul_epu32 (v1, d);
		__m128i high = _mm_add_epi64 (bc, ad);
		high = _mm_slli_epi64 (high, 32);
		return _mm_add_epi64 (high, ac);
	}
	inline vectori_x86 FASTCALL multiplyvi64 (const vectori_x86& v, int s) noexcept { return multiplyvi64 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL multiplyvi64 (int s, const vectori_x86& v) noexcept { return multiplyvi64 (v, s); }
	inline vectori_x86 FASTCALL dividevi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		// TODO
		return (vectori_x86)dividevi64 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_x86 FASTCALL dividevi64 (const vectori_x86& v, int s) noexcept { return dividevi64 (v, vectori_x86 (s)); }
	inline vectori_x86 FASTCALL shiftlvi64 (const vectori_x86& v, int s) noexcept { return _mm_slli_epi64 (v, s); }
	inline vectori_x86 FASTCALL shiftrvi64 (const vectori_x86& v, int s) noexcept { return _mm_srai_epi64 (v, s); }
	inline vectori_x86 FASTCALL equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_cmpeq_epi64 (v1, v2); }
	inline vectori_x86 FASTCALL not_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return notvi (equalsvi64 (v1, v2)); }
	inline vectori_x86 FASTCALL lesservi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		auto temp = lesservi (v1, v2);
		return andvi (temp.permute32<0, 0, 2, 2> (), temp.permute32<1, 1, 3, 3> ());
	}
	inline vectori_x86 FASTCALL lesser_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (lesservi64 (v1, v2), equalsvi64 (v1, v2)); }
	inline vectori_x86 FASTCALL greatervi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		auto temp = greatervi (v1, v2);
		return andvi (temp.permute32<0, 0, 2, 2> (), temp.permute32<1, 1, 3, 3> ());
	}
	inline vectori_x86 FASTCALL greater_equalsvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return orvi (greatervi64 (v1, v2), equalsvi64 (v1, v2)); }

	inline vectori_x86 FASTCALL dotvi2d (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		auto temp = multiplyvi (v1, v2);
		return addvi (temp, temp.splat_y ()).splat_x ();
	}
	inline vectori_x86 FASTCALL dotvi3d (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		auto temp = multiplyvi (v1, v2);
		return addvi (temp, addvi (temp.splat_y (), temp.splat_z ())).splat_x ();
	}
	inline vectori_x86 FASTCALL dotvi4d (const vectori_x86& v1, const vectori_x86& v2) noexcept
	{
		auto temp = multiplyvi (v1, v2);
		return addvi (temp, addvi (temp.splat_y (), addvi (temp.splat_z (), temp.splat_w ()))).splat_x ();
	}

	inline vectori_x86 FASTCALL absvi (const vectori_x86& v) noexcept { return _mm_abs_epi32 (v); }
	inline vectori_x86 FASTCALL minvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_min_epi32 (v1, v2); }
	inline vectori_x86 FASTCALL maxvi (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_max_epi32 (v1, v2); }

	inline vectori_x86 FASTCALL absvi64 (const vectori_x86& v) noexcept { return _mm_abs_epi64 (v); }
	inline vectori_x86 FASTCALL minvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_min_epi64 (v1, v2); }
	inline vectori_x86 FASTCALL maxvi64 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_max_epi64 (v1, v2); }

	inline vectori_x86 FASTCALL absvi16 (const vectori_x86& v) noexcept { return _mm_abs_epi16 (v); }
	inline vectori_x86 FASTCALL minvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_min_epi16 (v1, v2); }
	inline vectori_x86 FASTCALL maxvi16 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_max_epi16 (v1, v2); }

	inline vectori_x86 FASTCALL absvi8 (const vectori_x86& v) noexcept { return _mm_abs_epi8 (v); }
	inline vectori_x86 FASTCALL minvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_min_epi8 (v1, v2); }
	inline vectori_x86 FASTCALL maxvi8 (const vectori_x86& v1, const vectori_x86& v2) noexcept { return _mm_max_epi8 (v1, v2); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 256-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vector8f_x86 FASTCALL addv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_add_ps (v1, v2); }
	inline vector8f_x86 FASTCALL subtractv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_sub_ps (v1, v2); }
	inline vector8f_x86 FASTCALL negatev8f (const vector8f_x86& v) noexcept
	{
		static vector8f_x86 zero (0, 0, 0, 0, 0, 0, 0, 0);
		return subtractv8f (zero, v);
	}
	inline vector8f_x86 FASTCALL multiplyv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_mul_ps (v1, v2); }
	inline vector8f_x86 FASTCALL multiplyv8f (const vector8f_x86& v, float s) noexcept { return _mm256_mul_ps (v, _mm256_set1_ps (s)); }
	inline vector8f_x86 FASTCALL multiplyv8f (float s, const vector8f_x86& v) noexcept { return multiplyv8f (v, s); }
	inline vector8f_x86 FASTCALL dividev8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_div_ps (v1, v2); }
	inline vector8f_x86 FASTCALL dividev8f (const vector8f_x86& v, float s) noexcept { return _mm256_div_ps (v, _mm256_set1_ps (s)); }
	inline vector8f_x86 FASTCALL fmav8f (const vector8f_x86& mv1, const vector8f_x86& mv2, const vector8f_x86& av) noexcept { return _mm256_fmadd_ps (mv1, mv2, av); }
	inline vector8f_x86 FASTCALL fmsv8f (const vector8f_x86& mv1, const vector8f_x86& mv2, const vector8f_x86& sv) noexcept { return _mm256_fmsub_ps (mv1, mv2, sv); }
	inline vector8f_x86 FASTCALL fnmsv8f (const vector8f_x86& sv, const vector8f_x86& mv1, const vector8f_x86& mv2) noexcept { return _mm256_fnmsub_ps (mv1, mv2, sv); }
	inline vector8f_x86 FASTCALL andv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return reinterpret8_i32_to_f32 (andv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2))); }
	inline vector8f_x86 FASTCALL orv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return reinterpret8_i32_to_f32 (orv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2))); }
	inline vector8f_x86 FASTCALL xorv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return reinterpret8_i32_to_f32 (xorv8i (reinterpret8_f32_to_i32 (v1), reinterpret8_f32_to_i32 (v2))); }
	inline vector8f_x86 FASTCALL notv8f (const vector8f_x86& v) noexcept { return reinterpret8_i32_to_f32 (notv8i (reinterpret8_f32_to_i32 (v))); }
	inline vector8f_x86 FASTCALL equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (equalsvf (v1.vector1 (), v2.vector1 ()), equalsvf (v1.vector2 (), v2.vector2 ())); }
	inline vector8f_x86 FASTCALL not_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (not_equalsvf (v1.vector1 (), v2.vector1 ()), not_equalsvf (v1.vector2 (), v2.vector2 ())); }
	inline vector8f_x86 FASTCALL lesserv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (lesservf (v1.vector1 (), v2.vector1 ()), lesservf (v1.vector2 (), v2.vector2 ())); }
	inline vector8f_x86 FASTCALL lesser_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (lesser_equalsvf (v1.vector1 (), v2.vector1 ()), lesser_equalsvf (v1.vector2 (), v2.vector2 ())); }
	inline vector8f_x86 FASTCALL greaterv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (greatervf (v1.vector1 (), v2.vector1 ()), greatervf (v1.vector2 (), v2.vector2 ())); }
	inline vector8f_x86 FASTCALL greater_equalsv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return vector8f_x86 (greater_equalsvf (v1.vector1 (), v2.vector1 ()), greater_equalsvf (v1.vector2 (), v2.vector2 ())); }

	inline vector8f_x86 FASTCALL dotv8f2d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_dp_ps (v1, v2, 0x3f); }
	inline vector8f_x86 FASTCALL dotv8f3d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_dp_ps (v1, v2, 0x7f); }
	inline vector8f_x86 FASTCALL dotv8f4d (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_dp_ps (v1, v2, 0xff); }

	inline vector8f_x86 FASTCALL sqrtv8f (const vector8f_x86& v) noexcept { return _mm256_sqrt_ps (v); }
	inline vector8f_x86 FASTCALL rsqrtv8f (const vector8f_x86& v) noexcept { return _mm256_rsqrt_ps (v); }
	inline vector8f_x86 FASTCALL rcpv8f (const vector8f_x86& v) noexcept { return _mm256_rcp_ps (v); }
	inline vector8f_x86 FASTCALL roundv8f (const vector8f_x86& v) noexcept { return _mm256_round_ps (v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC); }
	inline vector8f_x86 FASTCALL floorv8f (const vector8f_x86& v) noexcept { return _mm256_floor_ps (v); }
	inline vector8f_x86 FASTCALL ceilv8f (const vector8f_x86& v) noexcept { return _mm256_ceil_ps (v); }
	inline vector8f_x86 FASTCALL minv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_min_ps (v1, v2); }
	inline vector8f_x86 FASTCALL maxv8f (const vector8f_x86& v1, const vector8f_x86& v2) noexcept { return _mm256_max_ps (v1, v2); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 256-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vector8i_x86 FASTCALL addv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_add_epi32 (v1, v2); }
	inline vector8i_x86 FASTCALL subtractv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_sub_epi32 (v1, v2); }
	inline vector8i_x86 FASTCALL negatev8i (const vector8i_x86& v) noexcept
	{
		static vector8i_x86 zero (0, 0, 0, 0, 0, 0, 0, 0);
		return subtractv8i (zero, v);
	}
	inline vector8i_x86 FASTCALL multiplyv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_mul_epi32 (v1, v2); }
	inline vector8i_x86 FASTCALL multiplyv8i (const vector8i_x86& v, int s) noexcept { return _mm256_mul_epi32 (v, _mm256_set1_epi32 (s)); }
	inline vector8i_x86 FASTCALL multiplyv8i (int s, const vector8i_x86& v) noexcept { return multiplyv8i (v, s); }
	inline vector8i_x86 FASTCALL dividev8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)dividev8i ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL dividev8i (const vector8i_x86& v, int s) noexcept { return _mm256_div_epi32 (v, _mm256_set1_epi32 (s)); }
	inline vector8i_x86 FASTCALL fmav8i (const vector8i_x86& mv1, const vector8i_x86& mv2, const vector8i_x86& av) noexcept { return addv8i (multiplyv8i (mv1, mv2), av); }
	inline vector8i_x86 FASTCALL fmsv8i (const vector8i_x86& mv1, const vector8i_x86& mv2, const vector8i_x86& sv) noexcept { return subtractv8i (multiplyv8i (mv1, mv2), sv); }
	inline vector8i_x86 FASTCALL fnmsv8i (const vector8i_x86& sv, const vector8i_x86& mv1, const vector8i_x86& mv2) noexcept { return subtractv8i (sv, multiplyv8i (mv1, mv2)); }
	inline vector8i_x86 FASTCALL andv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_and_si256 (v1, v2); }
	inline vector8i_x86 FASTCALL orv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_or_si256 (v1, v2); }
	inline vector8i_x86 FASTCALL xorv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_xor_si256 (v1, v2); }
	inline vector8i_x86 FASTCALL shiftlv8i (const vector8i_x86& v, int s) noexcept { return _mm256_slli_epi32 (v, s); }
	inline vector8i_x86 FASTCALL shiftrv8i (const vector8i_x86& v, int s) noexcept { return _mm256_srai_epi32 (v, s); }
	inline vector8i_x86 FASTCALL notv8i (const vector8i_x86& v) noexcept { return _mm256_xor_si256 (v, _mm256_cmpeq_epi32 (v, v)); }
	inline vector8i_x86 FASTCALL equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_cmpeq_epi32 (v1, v2); }
	inline vector8i_x86 FASTCALL not_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return notv8i (equalsv8i (v1, v2)); }
	inline vector8i_x86 FASTCALL lesserv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesservi (v1.vector1 (), v2.vector1 ()), lesservi (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL lesser_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesser_equalsvi (v1.vector1 (), v2.vector1 ()), lesser_equalsvi (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greaterv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greatervi (v1.vector1 (), v2.vector1 ()), greatervi (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greater_equalsv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greater_equalsvi (v1.vector1 (), v2.vector1 ()), greater_equalsvi (v1.vector2 (), v2.vector2 ())); }

	inline vector8i_x86 FASTCALL addv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_add_epi8 (v1, v2); }
	inline vector8i_x86 FASTCALL subtractv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_sub_epi8 (v1, v2); }
	inline vector8i_x86 FASTCALL negatev8i8 (const vector8i_x86& v) noexcept
	{
		static vector8i_x86 zero (0, 0, 0, 0, 0, 0, 0, 0);
		return subtractv8i8 (zero, v);
	}
	inline vector8i_x86 FASTCALL multiplyv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)multiplyv8i8 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL multiplyv8i8 (const vector8i_x86& v, int s) noexcept { return multiplyv8i8 (v, _mm256_set1_epi8 (s)); }
	inline vector8i_x86 FASTCALL multiplyv8i8 (int s, const vector8i_x86& v) noexcept { return multiplyv8i8 (v, s); }
	inline vector8i_x86 FASTCALL dividev8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)dividev8i8 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL dividev8i8 (const vector8i_x86& v, int s) noexcept { return dividev8i8 (v, _mm256_set1_epi8 (s)); }
	inline vector8i_x86 FASTCALL equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_cmpeq_epi8 (v1, v2); }
	inline vector8i_x86 FASTCALL not_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return notv8i (equalsv8i8 (v1, v2)); }
	inline vector8i_x86 FASTCALL lesserv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesservi8 (v1.vector1 (), v2.vector1 ()), lesservi8 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL lesser_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesser_equalsvi8 (v1.vector1 (), v2.vector1 ()), lesser_equalsvi8 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greaterv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greatervi8 (v1.vector1 (), v2.vector1 ()), greatervi8 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greater_equalsv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greater_equalsvi8 (v1.vector1 (), v2.vector1 ()), greater_equalsvi8 (v1.vector2 (), v2.vector2 ())); }

	inline vector8i_x86 FASTCALL addv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_add_epi16 (v1, v2); }
	inline vector8i_x86 FASTCALL subtractv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_sub_epi16 (v1, v2); }
	inline vector8i_x86 FASTCALL negatev8i16 (const vector8i_x86& v) noexcept
	{
		static vector8i_x86 zero (0, 0, 0, 0, 0, 0, 0, 0);
		return subtractv8i16 (zero, v);
	}
	inline vector8i_x86 FASTCALL multiplyv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)multiplyv8i16 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL multiplyv8i16 (const vector8i_x86& v, int s) noexcept { return multiplyv8i16 (v, _mm256_set1_epi16 (s)); }
	inline vector8i_x86 FASTCALL multiplyv8i16 (int s, const vector8i_x86& v) noexcept { return multiplyv8i16 (v, s); }
	inline vector8i_x86 FASTCALL dividev8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)dividev8i16 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL dividev8i16 (const vector8i_x86& v, int s) noexcept { return dividev8i16 (v, _mm256_set1_epi16 (s)); }
	inline vector8i_x86 FASTCALL shiftlv8i16 (const vector8i_x86& v, int s) noexcept { return _mm256_slli_epi16 (v, s); }
	inline vector8i_x86 FASTCALL shiftrv8i16 (const vector8i_x86& v, int s) noexcept { return _mm256_srai_epi16 (v, s); }
	inline vector8i_x86 FASTCALL equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_cmpeq_epi16 (v1, v2); }
	inline vector8i_x86 FASTCALL not_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return notv8i (equalsv8i16 (v1, v2)); }
	inline vector8i_x86 FASTCALL lesserv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesservi16 (v1.vector1 (), v2.vector1 ()), lesservi16 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL lesser_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesser_equalsvi16 (v1.vector1 (), v2.vector1 ()), lesser_equalsvi16 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greaterv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greatervi16 (v1.vector1 (), v2.vector1 ()), greatervi16 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greater_equalsv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greater_equalsvi16 (v1.vector1 (), v2.vector1 ()), greater_equalsvi16 (v1.vector2 (), v2.vector2 ())); }

	inline vector8i_x86 FASTCALL addv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_add_epi64 (v1, v2); }
	inline vector8i_x86 FASTCALL subtractv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_sub_epi64 (v1, v2); }
	inline vector8i_x86 FASTCALL negatev8i64 (const vector8i_x86& v) noexcept
	{
		static vector8i_x86 zero (0, 0, 0, 0, 0, 0, 0, 0);
		return subtractv8i64 (zero, v);
	}
	inline vector8i_x86 FASTCALL multiplyv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)multiplyv8i64 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL multiplyv8i64 (const vector8i_x86& v, int s) noexcept { return multiplyv8i64 (v, _mm256_set1_epi64x (s)); }
	inline vector8i_x86 FASTCALL multiplyv8i64 (int s, const vector8i_x86& v) noexcept { return multiplyv8i64 (v, s); }
	inline vector8i_x86 FASTCALL dividev8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept
	{
		// TODO
		return (vector8i_x86)dividev8i64 ((const vector8i_def&)v1, (const vector8i_def&)v2);
	}
	inline vector8i_x86 FASTCALL dividev8i64 (const vector8i_x86& v, int s) noexcept { return dividev8i64 (v, _mm256_set1_epi64x (s)); }
	inline vector8i_x86 FASTCALL shiftlv8i64 (const vector8i_x86& v, int s) noexcept { return _mm256_slli_epi64 (v, s); }
	inline vector8i_x86 FASTCALL shiftrv8i64 (const vector8i_x86& v, int s) noexcept { return _mm256_srai_epi64 (v, s); }
	inline vector8i_x86 FASTCALL equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_cmpeq_epi64 (v1, v2); }
	inline vector8i_x86 FASTCALL not_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return notv8i (equalsv8i64 (v1, v2)); }
	inline vector8i_x86 FASTCALL lesserv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesservi64 (v1.vector1 (), v2.vector1 ()), lesservi64 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL lesser_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (lesser_equalsvi64 (v1.vector1 (), v2.vector1 ()), lesser_equalsvi64 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greaterv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greatervi64 (v1.vector1 (), v2.vector1 ()), greatervi64 (v1.vector2 (), v2.vector2 ())); }
	inline vector8i_x86 FASTCALL greater_equalsv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (greater_equalsvi64 (v1.vector1 (), v2.vector1 ()), greater_equalsvi64 (v1.vector2 (), v2.vector2 ())); }

	inline vector8i_x86 FASTCALL dotvf2d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (dotvi2d (v1.vector1 (), v2.vector1 ()), dotvi2d (v1.vector1 (), v2.vector1 ())); }
	inline vector8i_x86 FASTCALL dotvf3d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (dotvi3d (v1.vector1 (), v2.vector1 ()), dotvi3d (v1.vector1 (), v2.vector1 ())); }
	inline vector8i_x86 FASTCALL dotvf4d (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return vector8i_x86 (dotvi4d (v1.vector1 (), v2.vector1 ()), dotvi4d (v1.vector1 (), v2.vector1 ())); }

	inline vector8i_x86 FASTCALL absv8i (const vector8i_x86& v) noexcept { return _mm256_abs_epi32 (v); }
	inline vector8i_x86 FASTCALL minv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_min_epi32 (v1, v2); }
	inline vector8i_x86 FASTCALL maxv8i (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_max_epi32 (v1, v2); }

	inline vector8i_x86 FASTCALL absv8i64 (const vector8i_x86& v) noexcept { return _mm256_abs_epi64 (v); }
	inline vector8i_x86 FASTCALL minv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_min_epi64 (v1, v2); }
	inline vector8i_x86 FASTCALL maxv8i64 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_max_epi64 (v1, v2); }

	inline vector8i_x86 FASTCALL absv8i16 (const vector8i_x86& v) noexcept { return _mm256_abs_epi16 (v); }
	inline vector8i_x86 FASTCALL minv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_min_epi16 (v1, v2); }
	inline vector8i_x86 FASTCALL maxv8i16 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_max_epi16 (v1, v2); }

	inline vector8i_x86 FASTCALL absv8i8 (const vector8i_x86& v) noexcept { return _mm256_abs_epi8 (v); }
	inline vector8i_x86 FASTCALL minv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_min_epi8 (v1, v2); }
	inline vector8i_x86 FASTCALL maxv8i8 (const vector8i_x86& v1, const vector8i_x86& v2) noexcept { return _mm256_max_epi8 (v1, v2); }
}

#endif