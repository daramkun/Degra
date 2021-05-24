#ifndef __DSEED_MATHES_H__
#define __DSEED_MATHES_H__

#include <cstdint>
#include <math.h>

namespace dseed
{
	constexpr int8_t int8_max = CHAR_MAX;
	constexpr int8_t int8_min = CHAR_MIN;
	constexpr uint8_t uint8_max = UCHAR_MAX;
	constexpr uint8_t uint8_min = 0;

	constexpr int16_t int16_max = SHRT_MAX;
	constexpr int16_t int16_min = SHRT_MIN;
	constexpr uint16_t uint16_max = USHRT_MAX;
	constexpr uint16_t uint16_min = 0;

	constexpr int32_t int32_max = INT_MAX;
	constexpr int32_t int32_min = INT_MIN;
	constexpr uint32_t uint32_max = UINT_MAX;
	constexpr uint32_t uint32_min = 0;

	constexpr int64_t int64_max = LLONG_MAX;
	constexpr int64_t int64_min = LLONG_MIN;
	constexpr uint64_t uint64_max = ULLONG_MAX;
	constexpr uint64_t uint64_min = 0;
}

namespace dseed
{
	using single = float;

	constexpr single single_positive_max = FLT_MAX;
	constexpr single single_positive_min = FLT_MIN;
	constexpr single single_negative_max = -FLT_MAX;
	constexpr single single_negative_min = -FLT_MIN;
	constexpr single single_positive_infinity = INFINITY;
	constexpr single single_negative_infinity = -INFINITY;
	constexpr single single_epsilon = FLT_EPSILON;
	constexpr single single_nan = NAN;

	constexpr double double_positive_max = DBL_MAX;
	constexpr double double_positive_min = DBL_MIN;
	constexpr double double_negative_max = -DBL_MAX;
	constexpr double double_negative_min = -DBL_MIN;
	constexpr double double_positive_infinity = HUGE_VAL;
	constexpr double double_negative_infinity = -HUGE_VAL;
	constexpr double double_epsilon = DBL_EPSILON;
#if COMPILER_MSVC
	constexpr double double_nan = HUGE_VAL * 0.0;
#else
	constexpr double double_nam = NAN;
#endif
}

namespace dseed
{
	constexpr single pi = 3.14159265358979323846f;
	constexpr single pi2 = pi * 2;
	constexpr single piover2 = pi / 2;
	constexpr single piover4 = pi / 4;
	constexpr single e = 2.7182818285f;
	constexpr single log10e = 0.4342944819f;
	constexpr single log2e = 1.4426950409f;
}

namespace dseed
{
	constexpr bool equals(single s1, single s2) noexcept { return (s1 - s2) < single_epsilon; }
	constexpr bool equals(double d1, double d2) noexcept { return (d1 - d2) < double_epsilon; }
	template<typename T>
	constexpr T minimum(const T& v1, const T& v2) noexcept { return (v1 < v2) ? v1 : v2; }
	template<typename T>
	constexpr T maximum(const T& v1, const T& v2) noexcept { return (v1 < v2) ? v2 : v1; }

	template<class T>
	constexpr T clamp(T v, T max) noexcept { return minimum(max, maximum((T)0, v)); }
	template<class T>
	constexpr T wrap(T v, T max) noexcept { return v % max; }
	template<class T>
	constexpr T mirror(T v, T max) noexcept { while (v < max || v > 0) { v = max - v; if (v < 0) v *= -1; } return v; }

	constexpr int gcd(int a, int b) { return (b == 0) ? a : gcd(b, a % b); }

	constexpr single to_degree(single radian) noexcept { return radian * 180 / pi; }
	constexpr single to_radian(single degree) noexcept { return degree * pi / 180; }
}

#include "half.h"
#include "int24.h"
#include "fraction.h"

#include "vectors/simd.h"

#include "vectors/wrapped.h"

#include "geometry.h"

namespace dseed
{
	enum class binary_operator
	{
		add,
		subtract,
		multiply,
		divide,
		andop,
		orop,
		xorop,
	};

	enum class unary_operator
	{
		negate,
		notop,
		invert,
	};

	enum class comparison_operator
	{
		equal,
		not_equal,

		less,
		less_equal,

		greater,
		greater_equal,

		never,
		always,
	};
}

#endif