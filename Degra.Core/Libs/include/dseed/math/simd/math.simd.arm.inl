#if !defined (__DSEED_MATH_SIMD_ARM_INL__) && ARCH_ARMSET
#define __DSEED_MATH_SIMD_ARM_INL__

#if ARCH_ARM
#	include <arm_neon.h>
#elif ARCH_ARM64
#	include <arm64_neon.h>
#endif

namespace dseed
{
	struct vectorf_arm;
	struct vectori_arm;

	inline vectori_arm FASTCALL conv_f32_to_i32 (const vectorf_arm& v) noexcept;
	inline vectori_arm FASTCALL reinterpret_f32_to_i32 (const vectorf_arm& v) noexcept;

	inline vectorf_arm FASTCALL conv_i32_to_f32 (const vectori_arm& v) noexcept;
	inline vectorf_arm FASTCALL reinterpret_i32_to_f32 (const vectori_arm& v) noexcept;

	inline vectori_arm FASTCALL conv_i64_to_i32 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i64_to_i16 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i64_to_i8 (const vectori_arm& v) noexcept;

	inline vectori_arm FASTCALL conv_i32_to_i64 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i32_to_i16 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i32_to_i8 (const vectori_arm& v) noexcept;

	inline vectori_arm FASTCALL conv_i16_to_i64 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i16_to_i32 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i16_to_i8 (const vectori_arm& v) noexcept;

	inline vectori_arm FASTCALL conv_i8_to_i64 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i8_to_i32 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL conv_i8_to_i16 (const vectori_arm& v) noexcept;

	inline int movemaskvf (const vectorf_arm& v) noexcept;
	inline int movemaskvi (const vectori_arm& v) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectorf_arm
	{
		float32x4_t v;

		inline vectorf_arm () noexcept = default;
		inline vectorf_arm (const float* arr) noexcept { load (arr); }
		inline vectorf_arm (float x, float y, float z, float w) noexcept { float temp[] = { x,y,z,w }; load (temp); }
		inline vectorf_arm (float scalar) noexcept : v (vmovq_n_f32 (scalar)) { }
		inline vectorf_arm (const float32x4_t& v) noexcept : v (v) { }
		inline explicit vectorf_arm (const vectorf_def& v) noexcept { load (v.v); }

		inline operator float32x4_t& () noexcept { return v; }
		inline operator const float32x4_t& () const noexcept { return v; }
		inline explicit operator vectorf_def () const noexcept
		{
			vectorf_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const float* arr) noexcept { v = vld1q_f32 (arr); }
		inline void FASTCALL store (float* arr) const noexcept { vst1q_f32 (arr, v); }

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		static inline vectorf_arm FASTCALL shuffle32 (const vectorf_arm & v1, const vectorf_arm & v2) noexcept
		{
			float32x2_t a = v1.__getpart<x, y> (v2.v);
			float32x2_t b = v2.__getpart<z, w> (v1.v);
			return vcombine_f32 (a, b);
		}

		template<uint32_t x, uint32_t y, uint32_t z, uint32_t w>
		inline vectorf_arm FASTCALL permute32 () const noexcept
		{
			return shuffle32<x, y, z, w> (*this, *this);
		}

		inline vectorf_arm FASTCALL splat_x () const noexcept { return vdupq_lane_f32 (vget_low_f32 (v), 0); }
		inline vectorf_arm FASTCALL splat_y () const noexcept { return vdupq_lane_f32 (vget_low_f32 (v), 1); }
		inline vectorf_arm FASTCALL splat_z () const noexcept { return vdupq_lane_f32 (vget_high_f32 (v), 0); }
		inline vectorf_arm FASTCALL splat_w () const noexcept { return vdupq_lane_f32 (vget_high_f32 (v), 1); }

		inline float FASTCALL x () const noexcept { return vgetq_lane_f32 (v, 0); }
		inline float FASTCALL y () const noexcept { return vgetq_lane_f32 (v, 1); }
		inline float FASTCALL z () const noexcept { return vgetq_lane_f32 (v, 2); }
		inline float FASTCALL w () const noexcept { return vgetq_lane_f32 (v, 3); }

		static inline bool support () noexcept
		{
			auto& instInfo = arm_instruction_info::instance ();
			return instInfo.neon;
		}
		static inline bool hwvector () noexcept { return true; }

	private:
		template<uint8_t x, uint8_t y>
		inline float32x2_t __getpart (const float32x4_t& v2) const
		{
			float32x2_t ret;
			if (x <= 0 && x >= 3) ret = vmov_n_f32 (vgetq_lane_f32 (v, x));
			else if (x <= 4 && x >= 7) ret = vmov_n_f32 (vgetq_lane_f32 (v2, 4 - x));
			else assert (true);

			if (y <= 0 && y >= 3) ret = vset_lane_f32 (vgetq_lane_f32 (v2, y), ret, 1);
			else if (y <= 4 && y >= 7) ret = vset_lane_f32 (vgetq_lane_f32 (v, 4 - y), ret, 1);
			else assert (true);

			return ret;
		}
		template<> inline float32x2_t __getpart<0, 1> (const float32x4_t & v2) const { return vget_low_f32 (v); }
		template<> inline float32x2_t __getpart<2, 3> (const float32x4_t & v2) const { return vget_high_f32 (v); }
		template<> inline float32x2_t __getpart<4, 5> (const float32x4_t & v2) const { return vget_low_f32 (v2); }
		template<> inline float32x2_t __getpart<6, 7> (const float32x4_t & v2) const { return vget_high_f32 (v2); }
		template<> inline float32x2_t __getpart<1, 0> (const float32x4_t & v2) const { return vrev64_f32 (vget_low_f32 (v)); }
		template<> inline float32x2_t __getpart<3, 2> (const float32x4_t & v2) const { return vrev64_f32 (vget_high_f32 (v)); }
		template<> inline float32x2_t __getpart<5, 4> (const float32x4_t & v2) const { return vrev64_f32 (vget_low_f32 (v2)); }
		template<> inline float32x2_t __getpart<7, 6> (const float32x4_t & v2) const { return vrev64_f32 (vget_high_f32 (v2)); }
		template<> inline float32x2_t __getpart<0, 0> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_low_f32 (v), 0); }
		template<> inline float32x2_t __getpart<1, 1> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_low_f32 (v), 1); }
		template<> inline float32x2_t __getpart<2, 2> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_high_f32 (v), 0); }
		template<> inline float32x2_t __getpart<3, 3> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_high_f32 (v), 1); }
		template<> inline float32x2_t __getpart<4, 4> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_low_f32 (v2), 0); }
		template<> inline float32x2_t __getpart<5, 5> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_low_f32 (v2), 1); }
		template<> inline float32x2_t __getpart<6, 6> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_high_f32 (v2), 0); }
		template<> inline float32x2_t __getpart<7, 7> (const float32x4_t & v2) const { return vdup_lane_f32 (vget_high_f32 (v2), 1); }
	};

	inline vectorf_arm FASTCALL addvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL subtractvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL negatevf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL multiplyvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL multiplyvf (const vectorf_arm& v, float s) noexcept;
	inline vectorf_arm FASTCALL multiplyvf (float s, const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL dividevf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL dividevf (const vectorf_arm& v, float s) noexcept;
	inline vectorf_arm FASTCALL fmavf (const vectorf_arm& mv1, const vectorf_arm& mv2, const vectorf_arm& av) noexcept;
	inline vectorf_arm FASTCALL fmsvf (const vectorf_arm& mv1, const vectorf_arm& mv2, const vectorf_arm& sv) noexcept;
	inline vectorf_arm FASTCALL fnmsvf (const vectorf_arm& sv, const vectorf_arm& mv1, const vectorf_arm& mv2) noexcept;
	inline vectorf_arm FASTCALL andvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL orvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL xorvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL notvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL not_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL lesservf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL lesser_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL greatervf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL greater_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;

	inline vectorf_arm FASTCALL dotvf2d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL dotvf3d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL dotvf4d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;

	inline vectorf_arm FASTCALL sqrtvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL rsqrtvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL rcpvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL roundvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL floorvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL ceilvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL absvf (const vectorf_arm& v) noexcept;
	inline vectorf_arm FASTCALL minvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	inline vectorf_arm FASTCALL maxvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept;
	
	inline vectorf_arm FASTCALL selectvf (const vectorf_arm& v1, const vectorf_arm& v2, const vectorf_arm& controlv) noexcept;
	inline vectorf_arm FASTCALL selectcontrolvf_arm (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept;
	inline bool FASTCALL inboundsvf3d (const vectorf_arm& v, const vectorf_arm& bounds) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(16) vectori_arm
	{
		int32x4_t v;
		inline vectori_arm () noexcept = default;
		inline vectori_arm (const int* vector) noexcept : v (vld1q_s32 (vector)) { }
		inline vectori_arm (int x, int y, int z, int w) noexcept { int temp[] = { x,y,z,w }; load (temp); }
		inline vectori_arm (int i) noexcept : v (vmovq_n_s32 (i)) { }
		inline vectori_arm (const int32x4_t& v) noexcept : v (v) { }
		inline explicit vectori_arm (const vectori_def& v) noexcept { load (v.v); }

		inline operator int32x4_t& () noexcept { return v; }
		inline operator const int32x4_t& () const noexcept { return v; }
		inline explicit operator vectori_def() const noexcept
		{
			vectori_def ret;
			store (ret.v);
			return ret;
		}

		inline void FASTCALL load (const int* vector) noexcept { v = vld1q_s32 (vector); }
		inline void FASTCALL store (int* vector) const noexcept { vst1q_s32 (vector, v); }

		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		static inline vectori_arm FASTCALL shuffle32 (const vectori_arm & v1, const vectori_arm & v2) noexcept
		{
			int32x2_t a = v1.__getpart<x, y> (v2.v);
			int32x2_t b = v2.__getpart<z, w> (v1.v);
			return vcombine_s32 (a, b);
		}
		template<uint8_t x, uint8_t y, uint8_t z, uint8_t w>
		inline vectori_arm FASTCALL permute32 () const
		{
			return shuffle32<x, y, z, w> (*this, *this);
		}
		template<uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4, uint8_t s5, uint8_t s6, uint8_t s7, uint8_t s8,
			uint8_t s9, uint8_t s10, uint8_t s11, uint8_t s12, uint8_t s13, uint8_t s14, uint8_t s15, uint8_t s16>
			inline vectori_arm FASTCALL permute8 () const
		{
			int8_t* arr = reinterpret_cast<int8_t*>(&v);
			vectori ret;
			int8_t* destarr = reinterpret_cast<int8_t*>(&ret.v);
			destarr[0] = arr[s1]; destarr[1] = arr[s2]; destarr[2] = arr[s3]; destarr[3] = arr[s4];
			destarr[4] = arr[s5]; destarr[5] = arr[s6]; destarr[6] = arr[s7]; destarr[7] = arr[s8];
			destarr[8] = arr[s9]; destarr[9] = arr[s10]; destarr[10] = arr[s11]; destarr[11] = arr[s12];
			destarr[12] = arr[s13]; destarr[13] = arr[s14]; destarr[14] = arr[s15]; destarr[15] = arr[s16];
			return ret;
		}

		inline vectori_arm FASTCALL splat_x () const { return vdupq_lane_s32 (vget_low_s32 (v), 0); }
		inline vectori_arm FASTCALL splat_y () const { return vdupq_lane_s32 (vget_low_s32 (v), 1); }
		inline vectori_arm FASTCALL splat_z () const { return vdupq_lane_s32 (vget_high_s32 (v), 0); }
		inline vectori_arm FASTCALL splat_w () const { return vdupq_lane_s32 (vget_high_s32 (v), 1); }

		inline int FASTCALL x () const { return vgetq_lane_s32 (v, 0); }
		inline int FASTCALL y () const { return vgetq_lane_s32 (v, 1); }
		inline int FASTCALL z () const { return vgetq_lane_s32 (v, 2); }
		inline int FASTCALL w () const { return vgetq_lane_s32 (v, 3); }

		static inline bool support () noexcept
		{
			auto& instInfo = arm_instruction_info::instance ();
			return instInfo.neon;
		}
		static inline bool hwvector () noexcept { return true; }
	
	private:
		template<uint8_t x, uint8_t y>
		inline int32x2_t __getpart (const float32x4_t& v2) const
		{
			int32x2_t ret;
			if (x <= 0 && x >= 3) ret = vmov_n_s32 (vgetq_lane_s32 (v, x));
			else if (x <= 4 && x >= 7) ret = vmov_n_s32 (vgetq_lane_s32 (v2, 4 - x));
			else assert (true);

			if (y <= 0 && y >= 3) ret = vset_lane_s32 (vgetq_lane_s32 (v2, y), ret, 1);
			else if (y <= 4 && y >= 7) ret = vset_lane_s32 (vgetq_lane_s32 (v, 4 - y), ret, 1);
			else assert (true);

			return ret;
		}
		template<> inline int32x2_t __getpart<0, 1> (const int32x4_t & v2) const { return vget_low_s32 (v); }
		template<> inline int32x2_t __getpart<2, 3> (const int32x4_t & v2) const { return vget_high_s32 (v); }
		template<> inline int32x2_t __getpart<4, 5> (const int32x4_t & v2) const { return vget_low_s32 (v2); }
		template<> inline int32x2_t __getpart<6, 7> (const int32x4_t & v2) const { return vget_high_s32 (v2); }
		template<> inline int32x2_t __getpart<1, 0> (const int32x4_t & v2) const { return vrev64_s32 (vget_low_s32 (v)); }
		template<> inline int32x2_t __getpart<3, 2> (const int32x4_t & v2) const { return vrev64_s32 (vget_high_s32 (v)); }
		template<> inline int32x2_t __getpart<5, 4> (const int32x4_t & v2) const { return vrev64_s32 (vget_low_s32 (v2)); }
		template<> inline int32x2_t __getpart<7, 6> (const int32x4_t & v2) const { return vrev64_s32 (vget_high_s32 (v2)); }
		template<> inline int32x2_t __getpart<0, 0> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_low_s32 (v), 0); }
		template<> inline int32x2_t __getpart<1, 1> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_low_s32 (v), 1); }
		template<> inline int32x2_t __getpart<2, 2> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_high_s32 (v), 0); }
		template<> inline int32x2_t __getpart<3, 3> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_high_s32 (v), 1); }
		template<> inline int32x2_t __getpart<4, 4> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_low_s32 (v2), 0); }
		template<> inline int32x2_t __getpart<5, 5> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_low_s32 (v2), 1); }
		template<> inline int32x2_t __getpart<6, 6> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_high_s32 (v2), 0); }
		template<> inline int32x2_t __getpart<7, 7> (const int32x4_t & v2) const { return vdup_lane_s32 (vget_high_s32 (v2), 1); }
	};

	inline vectori_arm FASTCALL addvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL subtractvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL negatevi (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL multiplyvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL multiplyvi (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL multiplyvi (int s, const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL dividevi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dividevi (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL fmavi (const vectori_arm& mv1, const vectori_arm& mv2, const vectori_arm& av) noexcept;
	inline vectori_arm FASTCALL fmsvi (const vectori_arm& mv1, const vectori_arm& mv2, const vectori_arm& sv) noexcept;
	inline vectori_arm FASTCALL fnmsvi (const vectori_arm& sv, const vectori_arm& mv1, const vectori_arm& mv2) noexcept;
	inline vectori_arm FASTCALL andvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL orvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL xorvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL shiftlvi (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL shiftrvi (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL notvi (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL not_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesservi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesser_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greatervi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greater_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL addvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL subtractvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL negatevi8 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL multiplyvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL multiplyvi8 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL multiplyvi8 (int s, const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL dividevi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dividevi8 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL not_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesservi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesser_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greatervi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greater_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL addvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL subtractvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL negatevi16 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL multiplyvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL multiplyvi16 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL multiplyvi16 (int s, const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL dividevi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dividevi16 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL shiftlvi16 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL shiftrvi16 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL not_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesservi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesser_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greatervi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greater_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL addvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL subtractvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL negatevi64 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL multiplyvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL multiplyvi64 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL multiplyvi64 (int s, const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL dividevi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dividevi64 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL shiftlvi64 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL shiftrvi64 (const vectori_arm& v, int s) noexcept;
	inline vectori_arm FASTCALL equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL not_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesservi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL lesser_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greatervi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL greater_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL dotvi2d (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dotvi3d (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL dotvi4d (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL absvi (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL minvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL maxvi (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL absvi64 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL minvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL maxvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL absvi16 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL minvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL maxvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	inline vectori_arm FASTCALL absvi8 (const vectori_arm& v) noexcept;
	inline vectori_arm FASTCALL minvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;
	inline vectori_arm FASTCALL maxvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation vector conversions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_arm FASTCALL conv_f32_to_i32 (const vectorf_arm& v) noexcept { return vcvtq_s32_f32 (v); }
	inline vectori_arm FASTCALL reinterpret_f32_to_i32 (const vectorf_arm& v) noexcept { return vreinterpretq_s32_f32 (v.v); }

	inline vectorf_arm FASTCALL conv_i32_to_f32 (const vectori_arm& v) noexcept { return vcvtq_f32_s32 (v); }
	inline vectorf_arm FASTCALL reinterpret_i32_to_f32 (const vectori_arm& v) noexcept { return vreinterpretq_f32_s32 (v.v); }

	inline vectori_arm FASTCALL conv_i64_to_i32 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s64 (v), vdup_n_s32 (0)); }
	inline vectori_arm FASTCALL conv_i64_to_i16 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s32 (conv_i64_to_i32 (v)), vdup_n_s16 (0)); }
	inline vectori_arm FASTCALL conv_i64_to_i8 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s16 (conv_i64_to_i16 (v)), vdup_n_s8 (0)); }

	inline vectori_arm FASTCALL conv_i32_to_i64 (const vectori_arm& v) noexcept { return vmovl_s32 (vget_low_s32 (v)); }
	inline vectori_arm FASTCALL conv_i32_to_i16 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s32 (v), vdup_n_s16 (0)); }
	inline vectori_arm FASTCALL conv_i32_to_i8 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s16 (conv_i32_to_i16 (v)), vdup_n_s8 (0)); }

	inline vectori_arm FASTCALL conv_i16_to_i64 (const vectori_arm& v) noexcept { return vmovl_s32(vget_low_s32(vmovl_s16 (vget_low_s32 (v)))); }
	inline vectori_arm FASTCALL conv_i16_to_i32 (const vectori_arm& v) noexcept { return vmovl_s16 (vget_low_s32 (v)); }
	inline vectori_arm FASTCALL conv_i16_to_i8 (const vectori_arm& v) noexcept { return vcombine_s32 (vmovn_s16 (v), vdup_n_s8 (0)); }

	inline vectori_arm FASTCALL conv_i8_to_i64 (const vectori_arm& v) noexcept { return vmovl_s32 (vget_low_s32 (conv_i8_to_i32 (v))); }
	inline vectori_arm FASTCALL conv_i8_to_i32 (const vectori_arm& v) noexcept { return vmovl_s16 (vget_low_s32 (conv_i8_to_i16 (v))); }
	inline vectori_arm FASTCALL conv_i8_to_i16 (const vectori_arm& v) noexcept { return vmovl_s8 (vget_low_s32(v)); }

	inline int movemaskvf (const vectorf_arm& v) noexcept
	{
		auto ia = reinterpret_cast<const uint32_t*> (&v);
		return (ia[0] >> 31) | ((ia[1] >> 30) & 2) | ((ia[2] >> 29) & 4) | ((ia[3] >> 28) & 8);
	}
	inline int movemaskvi (const vectori_arm& v) noexcept
	{
		uint8x16_t input = v;
		static const int8_t xr[8] = { -7, -6, -5, -4, -3, -2, -1, 0 };
		uint8x8_t mask_and = vdup_n_u8 (0x80);
		int8x8_t mask_shift = vld1_s8 (xr);

		uint8x8_t lo = vget_low_u8 (input);
		uint8x8_t hi = vget_high_u8 (input);

		lo = vand_u8 (lo, mask_and);
		lo = vshl_u8 (lo, mask_shift);

		hi = vand_u8 (hi, mask_and);
		hi = vshl_u8 (hi, mask_shift);

		lo = vpadd_u8 (lo, lo);
		lo = vpadd_u8 (lo, lo);
		lo = vpadd_u8 (lo, lo);

		hi = vpadd_u8 (hi, hi);
		hi = vpadd_u8 (hi, hi);
		hi = vpadd_u8 (hi, hi);

		return ((vget_lane_s32 (hi, 0) << 8) | (vget_lane_s32 (lo, 0) & 0xFF));
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Floating-point Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf_arm FASTCALL addvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vaddq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL subtractvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vsubq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL negatevf (const vectorf_arm& v) noexcept { return vnegq_f32 (v); }
	inline vectorf_arm FASTCALL multiplyvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vmulq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL multiplyvf (const vectorf_arm& v, float s) noexcept { return multiplyvf (v, vmovq_n_f32 (s)); }
	inline vectorf_arm FASTCALL multiplyvf (float s, const vectorf_arm& v) noexcept { return multiplyvf (v, s); }
	inline vectorf_arm FASTCALL dividevf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept
	{
		float32x4_t reciprocal = vrecpeq_f32 (v2);
		reciprocal = vmulq_f32 (vrecpsq_f32 (v2, reciprocal), reciprocal);
		reciprocal = vmulq_f32 (vrecpsq_f32 (v2, reciprocal), reciprocal);
		return vmulq_f32 (v1, reciprocal);
	}
	inline vectorf_arm FASTCALL dividevf (const vectorf_arm& v, float s) noexcept { return dividevf (v, vmovq_n_f32 (s)); }
	inline vectorf_arm FASTCALL fmavf (const vectorf_arm& mv1, const vectorf_arm& mv2, const vectorf_arm& av) noexcept
	{
#if ARCH_ARM
		return vmlaq_f32 (av, mv1, mv2);
#elif ARCH_ARM64
		return vfmaq_f32 (av, mv1, mv2);
#endif
	}
	inline vectorf_arm FASTCALL fmsvf (const vectorf_arm& mv1, const vectorf_arm& mv2, const vectorf_arm& sv) noexcept
	{
		return subtractvf (multiplyvf (mv1, mv2), sv);
	}
	inline vectorf_arm FASTCALL fnmsvf (const vectorf_arm& sv, const vectorf_arm& mv1, const vectorf_arm& mv2) noexcept
	{
#if ARCH_ARM
		return vmlsq_f32 (sv, mv1, mv2);
#elif ARCH_ARM64
		return vfmsq_f32 (sv, mv1, mv2);
#endif
	}
	inline vectorf_arm FASTCALL andvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vandq_s32 (v1, v2); }
	inline vectorf_arm FASTCALL orvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vorrq_s32 (v1, v2); }
	inline vectorf_arm FASTCALL xorvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return veorq_s32 (v1, v2); }
	inline vectorf_arm FASTCALL notvf (const vectorf_arm& v) noexcept { return vmvnq_s32 (v); }
	inline vectorf_arm FASTCALL equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vceqq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL not_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vmvnq_u32 (vceqq_f32 (v1, v2)); }
	inline vectorf_arm FASTCALL lesservf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vcltq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL lesser_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vcleq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL greatervf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vcgtq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL greater_equalsvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vcgeq_f32 (v1, v2); }

	inline vectorf_arm FASTCALL dotvf2d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept
	{
		float32x2_t temp = vmul_f32 (vget_low_f32 (v1), vget_low_f32 (v2));
		temp = vadd_f32 (temp, temp);
		return vcombine_f32 (temp, temp);
	}
	inline vectorf_arm FASTCALL dotvf3d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept
	{
		float32x4_t temp = vmulq_f32 (v1, v2);
		float32x2_t tv1 = vpadd_f32 (vget_low_f32 (temp), vget_low_f32 (temp));
		float32x2_t tv2 = vdup_lane_f32 (vget_high_f32 (temp), 0);
		tv1 = vadd_f32 (tv1, tv2);
		return vcombine_f32 (tv1, tv1);
	}
	inline vectorf_arm FASTCALL dotvf4d (const vectorf_arm& v1, const vectorf_arm& v2) noexcept
	{
		float32x4_t temp = vmulq_f32 (v1, v2);
		float32x2_t tv1 = vget_low_f32 (temp);
		float32x2_t tv2 = vget_high_f32 (temp);
		tv1 = vadd_f32 (tv1, tv2);
		tv1 = vpadd_f32 (tv1, tv1);
		return vcombine_f32 (tv1, tv1);
	}

	inline vectorf_arm FASTCALL sqrtvf (const vectorf_arm& v) noexcept { return vrecpeq_f32 (vrsqrteq_f32 (v)); }
	inline vectorf_arm FASTCALL rsqrtvf (const vectorf_arm& v) noexcept { return vrsqrteq_f32 (v); }
	inline vectorf_arm FASTCALL rcpvf (const vectorf_arm& v) noexcept { return vrecpeq_f32 (v); }
	inline vectorf_arm FASTCALL roundvf (const vectorf_arm& v) noexcept
	{
		static vectorf_arm half (0.5f);
		return floorvf (addvf (v, half));
	}
	inline vectorf_arm FASTCALL floorvf (const vectorf_arm& v) noexcept
	{
		return conv_i32_to_f32 (conv_f32_to_i32 (v));
	}
	inline vectorf_arm FASTCALL ceilvf (const vectorf_arm& v) noexcept
	{
		static vectorf_arm add_factor (0.99999999f);
		return floorvf (addvf (v, add_factor));
	}
	inline vectorf_arm FASTCALL absvf (const vectorf_arm& v) noexcept { return vabsq_f32 (v); }
	inline vectorf_arm FASTCALL minvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vminq_f32 (v1, v2); }
	inline vectorf_arm FASTCALL maxvf (const vectorf_arm& v1, const vectorf_arm& v2) noexcept { return vmaxq_f32 (v1, v2); }
	
	inline vectorf_arm FASTCALL selectvf (const vectorf_arm& v1, const vectorf_arm& v2, const vectorf_arm& controlv) noexcept
	{
		return vbslq_f32 (controlv, v2, v1);
	}
	inline vectorf_arm FASTCALL selectcontrolvf_arm (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) noexcept
	{
		static vectori_arm zero (0, 0, 0, 0);
		int32x4_t temp = vcombine_s32 (
			vcreate_s32 (static_cast<uint64_t>(v0) | (static_cast<uint64_t>(v1) << 32)),
			vcreate_s32 (static_cast<uint64_t>(v2) | (static_cast<uint64_t>(v3) << 32))
		);
		return reinterpret_i32_to_f32 (vcgtq_s32 (temp, zero));
	}
	inline bool FASTCALL inboundsvf3d (const vectorf_arm& v, const vectorf_arm& bounds) noexcept
	{
		vectorf_arm temp1 = lesser_equalsvf (v, bounds);
		vectorf_arm temp2 = lesser_equalsvf (negatevf (bounds), v);
		return (((movemaskvf (andvf (temp1, temp2)) & 0x7) == 0x7) != 0);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Implementation operators for 128-bit Signed Integer Vector
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectori_arm FASTCALL addvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vaddq_s32 (v1, v2); }
	inline vectori_arm FASTCALL subtractvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vsubq_s32 (v1, v2); }
	inline vectori_arm FASTCALL negatevi (const vectori_arm& v) noexcept { return vnegq_s32 (v); }
	inline vectori_arm FASTCALL multiplyvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmulq_s32 (v1, v2); }
	inline vectori_arm FASTCALL multiplyvi (const vectori_arm& v, int s) noexcept { return vmulq_s32 (v, vmovq_n_s32 (s)); }
	inline vectori_arm FASTCALL multiplyvi (int s, const vectori_arm& v) noexcept { return multiplyvi (v, s); }
	inline vectori_arm FASTCALL dividevi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return conv_f32_to_i32 (dividevf (conv_i32_to_f32 (v1), conv_i32_to_f32 (v2))); }
	inline vectori_arm FASTCALL dividevi (const vectori_arm& v, int s) noexcept { return dividevi (v, vmovq_n_s32 (s)); }
	inline vectori_arm FASTCALL fmavi (const vectori_arm& mv1, const vectori_arm& mv2, const vectori_arm& av) noexcept { return vmlaq_s32 (mv1, mv2, av); }
	inline vectori_arm FASTCALL fmsvi (const vectori_arm& mv1, const vectori_arm& mv2, const vectori_arm& sv) noexcept { return subtractvi (multiplyvi (mv1, mv2), sv); }
	inline vectori_arm FASTCALL fnmsvi (const vectori_arm& sv, const vectori_arm& mv1, const vectori_arm& mv2) noexcept { return vmlsq_s32 (sv, mv1, mv2); }
	inline vectori_arm FASTCALL andvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vandq_s32 (v1, v2); }
	inline vectori_arm FASTCALL orvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vorrq_s32 (v1, v2); }
	inline vectori_arm FASTCALL xorvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return veorq_s32 (v1, v2); }
	inline vectori_arm FASTCALL shiftlvi (const vectori_arm& v, int s) noexcept { return vshlq_n_s32 (v, s); }
	inline vectori_arm FASTCALL shiftrvi (const vectori_arm& v, int s) noexcept { return vshrq_n_s32 (v, s); }
	inline vectori_arm FASTCALL notvi (const vectori_arm& v) noexcept { return vmvnq_s32 (v); }
	inline vectori_arm FASTCALL equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vceqq_s32 (v1, v2); }
	inline vectori_arm FASTCALL not_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmvnq_s32 (equalsvi (v1, v2)); }
	inline vectori_arm FASTCALL lesservi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcltq_s32 (v1, v2); }
	inline vectori_arm FASTCALL lesser_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcleq_s32 (v1, v2); }
	inline vectori_arm FASTCALL greatervi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgtq_s32 (v1, v2); }
	inline vectori_arm FASTCALL greater_equalsvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgeq_s32 (v1, v2); }

	inline vectori_arm FASTCALL addvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vaddq_s8 (v1, v2); }
	inline vectori_arm FASTCALL subtractvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vsubq_s8 (v1, v2); }
	inline vectori_arm FASTCALL negatevi8 (const vectori_arm& v) noexcept { return vnegq_s8 (v); }
	inline vectori_arm FASTCALL multiplyvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmulq_s8 (v1, v2); }
	inline vectori_arm FASTCALL multiplyvi8 (const vectori_arm& v, int s) noexcept { return vmulq_s8 (v, vmovq_n_s8 (s)); }
	inline vectori_arm FASTCALL multiplyvi8 (int s, const vectori_arm& v) noexcept { return multiplyvi8 (v, s); }
	inline vectori_arm FASTCALL dividevi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept
	{
		// TODO
		return (vectori_arm)dividevi8 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_arm FASTCALL dividevi8 (const vectori_arm& v, int s) noexcept { return dividevi8 (v, vmovq_n_s8 (s)); }
	inline vectori_arm FASTCALL equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vceqq_s8 (v1, v2); }
	inline vectori_arm FASTCALL not_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmvnq_s8 (equalsvi8 (v1, v2)); }
	inline vectori_arm FASTCALL lesservi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcltq_s8 (v1, v2); }
	inline vectori_arm FASTCALL lesser_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcleq_s8 (v1, v2); }
	inline vectori_arm FASTCALL greatervi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgtq_s8 (v1, v2); }
	inline vectori_arm FASTCALL greater_equalsvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgeq_s8 (v1, v2); }

	inline vectori_arm FASTCALL addvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vaddq_s16 (v1, v2); }
	inline vectori_arm FASTCALL subtractvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vsubq_s16 (v1, v2); }
	inline vectori_arm FASTCALL negatevi16 (const vectori_arm& v) noexcept { return vnegq_s16 (v); }
	inline vectori_arm FASTCALL multiplyvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmulq_s16 (v1, v2); }
	inline vectori_arm FASTCALL multiplyvi16 (const vectori_arm& v, int s) noexcept { return vmulq_s16 (v, vmovq_n_s16 (s)); }
	inline vectori_arm FASTCALL multiplyvi16 (int s, const vectori_arm& v) noexcept { return multiplyvi16 (v, s); }
	inline vectori_arm FASTCALL dividevi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept
	{
		// TODO
		return (vectori_arm)dividevi16 ((const vectori_def&)v1, (const vectori_def&)v2);
	}
	inline vectori_arm FASTCALL dividevi16 (const vectori_arm& v, int s) noexcept { return dividevi16 (v, vmovq_n_s16 (s)); }
	inline vectori_arm FASTCALL shiftlvi16 (const vectori_arm& v, int s) noexcept { return vshlq_n_s16 (v, s); }
	inline vectori_arm FASTCALL shiftrvi16 (const vectori_arm& v, int s) noexcept { return vshrq_n_s16 (v, s); }
	inline vectori_arm FASTCALL equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vceqq_s16 (v1, v2); }
	inline vectori_arm FASTCALL not_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmvnq_s16 (equalsvi16(v1, v2)); }
	inline vectori_arm FASTCALL lesservi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcltq_s16 (v1, v2); }
	inline vectori_arm FASTCALL lesser_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcleq_s16 (v1, v2); }
	inline vectori_arm FASTCALL greatervi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgtq_s16 (v1, v2); }
	inline vectori_arm FASTCALL greater_equalsvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vcgeq_s16 (v1, v2); }

	inline vectori_arm FASTCALL addvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)addvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL subtractvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)subtractvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL negatevi64 (const vectori_arm& v) noexcept { return (vectori_arm)negatevi64 ((vectori_def)v); }
	inline vectori_arm FASTCALL multiplyvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)multiplyvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL multiplyvi64 (const vectori_arm& v, int s) noexcept { return (vectori_arm)multiplyvi64 ((vectori_def)v, s); }
	inline vectori_arm FASTCALL multiplyvi64 (int s, const vectori_arm& v) noexcept { return multiplyvi64 (v, s); }
	inline vectori_arm FASTCALL dividevi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)dividevi64 ((const vectori_def&)v1, (const vectori_def&)v2); }
	inline vectori_arm FASTCALL dividevi64 (const vectori_arm& v, int s) noexcept { return dividevi64 (v, vmovq_n_s64 (s)); }
	inline vectori_arm FASTCALL shiftlvi64 (const vectori_arm& v, int s) noexcept { return (vectori_arm)shiftlvi64 ((vectori_def)v, s); }
	inline vectori_arm FASTCALL shiftrvi64 (const vectori_arm& v, int s) noexcept { return (vectori_arm)shiftrvi64 ((vectori_def)v, s); }
	inline vectori_arm FASTCALL equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)equalsvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL not_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)not_equalsvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL lesservi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)lesservi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL lesser_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)lesser_equalsvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL greatervi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)greatervi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL greater_equalsvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)greater_equalsvi64 ((vectori_def)v1, (vectori_def)v2); }

	inline vectori_arm FASTCALL dotvi2d (const vectori_arm& v1, const vectori_arm& v2) noexcept
	{
		int32x2_t temp = vmul_s32 (vget_low_s32 (v1), vget_low_s32 (v2));
		temp = vadd_s32 (temp, temp);
		return vcombine_s32 (temp, temp);
	}
	inline vectori_arm FASTCALL dotvi3d (const vectori_arm& v1, const vectori_arm& v2) noexcept
	{
		int32x4_t temp = vmulq_s32 (v1, v2);
		int32x2_t tv1 = vpadd_s32 (vget_low_s32 (temp), vget_low_s32 (temp));
		int32x2_t tv2 = vdup_lane_s32 (vget_high_s32 (temp), 0);
		tv1 = vadd_s32 (tv1, tv2);
		return vcombine_s32 (tv1, tv1);
	}
	inline vectori_arm FASTCALL dotvi4d (const vectori_arm& v1, const vectori_arm& v2) noexcept
	{
		int32x4_t temp = vmulq_s32 (v1, v2);
		int32x2_t tv1 = vget_low_s32 (temp);
		int32x2_t tv2 = vget_high_s32 (temp);
		tv1 = vadd_s32 (tv1, tv2);
		tv1 = vpadd_s32 (tv1, tv1);
		return vcombine_s32 (tv1, tv1);
	}

	inline vectori_arm FASTCALL absvi (const vectori_arm& v) noexcept { return vabsq_s32 (v); }
	inline vectori_arm FASTCALL minvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vminq_s32 (v1, v2); }
	inline vectori_arm FASTCALL maxvi (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmaxq_s32 (v1, v2); }

	inline vectori_arm FASTCALL absvi64 (const vectori_arm& v) noexcept { return (vectori_arm)absvi64 ((vectori_def)v); }
	inline vectori_arm FASTCALL minvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)minvi64 ((vectori_def)v1, (vectori_def)v2); }
	inline vectori_arm FASTCALL maxvi64 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return (vectori_arm)maxvi64 ((vectori_def)v1, (vectori_def)v2); }

	inline vectori_arm FASTCALL absvi16 (const vectori_arm& v) noexcept { return vabsq_s16 (v); }
	inline vectori_arm FASTCALL minvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vminq_s16 (v1, v2); }
	inline vectori_arm FASTCALL maxvi16 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmaxq_s16 (v1, v2); }

	inline vectori_arm FASTCALL absvi8 (const vectori_arm& v) noexcept { return vabsq_s8 (v); }
	inline vectori_arm FASTCALL minvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vminq_s8 (v1, v2); }
	inline vectori_arm FASTCALL maxvi8 (const vectori_arm& v1, const vectori_arm& v2) noexcept { return vmaxq_s8 (v1, v2); }
}

#endif