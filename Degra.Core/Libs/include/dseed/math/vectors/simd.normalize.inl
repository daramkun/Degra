///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Normalize
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4_t normalize2(const f32x4_t& v) noexcept
	{
		return multiplyfv(v, rcp(length2(v)));
	}
	static inline f32x4_t normalize3(const f32x4_t& v) noexcept
	{
		return multiplyfv(v, rcp(length3(v)));
	}
	static inline f32x4_t normalize4(const f32x4_t& v) noexcept
	{
		return multiplyfv(v, rcp(length4(v)));
	}
}