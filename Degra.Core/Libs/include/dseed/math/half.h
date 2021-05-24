#ifndef __DSEED_MATH_HALF_H__
#define __DSEED_MATH_HALF_H__

namespace dseed
{
	/*#if ARCH_ARMSET && (COMPILER_GCC || COMPILER_CLANG)
	#	include <arm_fp16.h>
		using half = __fp16;
	#else
	#	if COMPILER_MSVC
	#		pragma pack (push, 1)
	#	else
	#		pragma pack (1)
	#	endif*/
	struct DSEEDEXP half
	{
		uint16_t word;

		half() = default;
		half(float v);
		operator float() const;

		inline half& operator+= (const half& h) noexcept { return *this = half(((float)*this) + ((float)h)); }
		inline half& operator-= (const half& h) noexcept { return *this = half(((float)*this) - ((float)h)); }
		inline half& operator*= (const half& h) noexcept { return *this = half(((float)*this) * ((float)h)); }
		inline half& operator/= (const half& h) noexcept { return *this = half(((float)*this) / ((float)h)); }
	};

	inline half operator+ (const half& a, const half& b) noexcept { return half(((float)a) + ((float)b)); }
	inline half operator- (const half& a, const half& b) noexcept { return half(((float)a) - ((float)b)); }
	inline half operator- (const half& a) noexcept { half ret = a; ret.word ^= 0x8000; return ret; }
	inline half operator* (const half& a, const half& b) noexcept { return half(((float)a) * ((float)b)); }
	inline half operator/ (const half& a, const half& b) noexcept { return half(((float)a) / ((float)b)); }
	inline bool operator== (const half& a, const half& b) noexcept { return a.word == b.word; }
	inline bool operator!= (const half& a, const half& b) noexcept { return a.word != b.word; }
	/*#	if COMPILER_MSVC
	#		pragma pack (pop)
	#	else
	#		pragma pack ()
	#	endif
	#endif*/

	extern const half half_positive_max;
	extern const half half_positive_min;
	extern const half half_negative_max;
	extern const half half_negative_min;
	extern const half half_positive_infinity;
	extern const half half_negative_infinity;
	extern const half half_epsilon;
	extern const half half_nan;
}

#endif