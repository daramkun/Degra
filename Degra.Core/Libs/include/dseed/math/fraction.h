#ifndef __DSEED_MATH_FRACTION_H__
#define __DSEED_MATH_FRACTION_H__

namespace dseed
{
	struct DSEEDEXP fraction
	{
		int32_t numerator, denominator;

		fraction() = default;
		fraction(int32_t numerator) noexcept;
		fraction(int32_t numerator, int32_t denominator) noexcept;
		fraction(double fp) noexcept;

		operator double() noexcept;
	};
}

#endif