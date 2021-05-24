#ifndef __DSEED_MATH_INT24_H__
#define __DSEED_MATH_INT24_H__

namespace dseed
{
	struct uint24_t;
	
	struct DSEEDEXP int24_t
	{
		int32_t value : 24;
		int24_t() = default;
		int24_t(int32_t v) noexcept : value(v) { }
		int24_t(uint24_t v) noexcept;
		operator int32_t () const noexcept { return value; }
	} ALIGNPACK(1);

	struct DSEEDEXP uint24_t
	{
		uint32_t value : 24;
		uint24_t() = default;
		uint24_t(uint32_t v) noexcept : value(v) { }
		uint24_t(int24_t v) noexcept : value(v.value) { }
		operator uint32_t () const noexcept { return value; }
	} ALIGNPACK(1);

	inline int24_t::int24_t(uint24_t v) noexcept { value = v.value; }

	inline bool operator==(int24_t i1, int24_t i2) { return i1.value == i2.value; }
	inline bool operator!=(int24_t i1, int24_t i2) { return i1.value != i2.value; }

	inline bool operator==(uint24_t i1, uint24_t i2) { return i1.value == i2.value; }
	inline bool operator!=(uint24_t i1, uint24_t i2) { return i1.value != i2.value; }

	constexpr int32_t int24_max = 8388607;
	constexpr int32_t int24_min = -8388608;
	constexpr uint32_t uint24_max = 16777215;
	constexpr uint32_t uint24_min = 0;
}

#endif