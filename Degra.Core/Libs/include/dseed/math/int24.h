#ifndef __DSEED_MATH_INT24_H__
#define __DSEED_MATH_INT24_H__

namespace dseed
{
#if COMPILER_MSVC
#	pragma pack (push, 1)
#else
#	pragma pack (1)
#endif
	struct uint24_t;
	
	struct DSEEDEXP int24_t
	{
		uint8_t value[3];
		int24_t() = default;
		int24_t(int32_t v) noexcept { memcpy(value, &v, 3); }
		int24_t(uint24_t v) noexcept;
		operator int32_t () const noexcept
		{
			return ((value[2] & 0x80) << 24) >> 7
				| (value[2] << 16) | (value[1] << 8) | value[0];
		}
	};
	struct DSEEDEXP uint24_t
	{
		uint8_t value[3];
		uint24_t() = default;
		uint24_t(uint32_t v) noexcept { memcpy(value, &v, 3); }
		uint24_t(int24_t v) noexcept;
		operator uint32_t () const noexcept
		{
			return (value[2] << 16) | (value[1] << 8) | value[0];
		}
	};

	inline int24_t::int24_t(uint24_t v) noexcept : int24_t(static_cast<uint32_t>(v)) { }
	inline uint24_t::uint24_t(int24_t v) noexcept : uint24_t(static_cast<int32_t>(v)) { }
#if COMPILER_MSVC
#	pragma pack (pop)
#else
#	pragma pack ()
#endif

	inline bool operator==(int24_t i1, int24_t i2) { return static_cast<int32_t>(i1) == static_cast<int32_t>(i2); }
	inline bool operator!=(int24_t i1, int24_t i2) { return static_cast<int32_t>(i1) != static_cast<int32_t>(i2); }

	inline bool operator==(uint24_t i1, uint24_t i2) { return static_cast<uint32_t>(i1) == static_cast<uint32_t>(i2); }
	inline bool operator!=(uint24_t i1, uint24_t i2) { return static_cast<uint32_t>(i1) != static_cast<uint32_t>(i2); }

	constexpr int32_t int24_max = 8388607;
	constexpr int32_t int24_min = -8388608;
	constexpr uint32_t uint24_max = 16777215;
	constexpr uint32_t uint24_min = 0;
}

#endif