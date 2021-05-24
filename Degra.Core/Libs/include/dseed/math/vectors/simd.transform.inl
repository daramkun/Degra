///////////////////////////////////////////////////////////////////////////////////////////
//
// 4x4 Matrix Transform
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4_t transform2(const f32x4_t& v, const f32x4x4_t& m) noexcept
	{
		return fmaf(v.vx(), m.column1(), fmaf(v.vy(), m.column2(), m.column4()));
	}
	static inline f32x4_t transform3(const f32x4_t& v, const f32x4x4_t& m) noexcept
	{
		return fmaf(v.vx(), m.column1(), fmaf(v.vy(), m.column2(), fmaf(v.vz(), m.column3(), m.column4())));
	}
	static inline f32x4_t transform4(const f32x4_t& v, const f32x4x4_t& m) noexcept
	{
		return fmaf(v.vx(), m.column1(), fmaf(v.vy(), m.column2(), fmaf(v.vz(), m.column3(), multiplyfv(v.vw(), m.column4()))));
	}

	static inline f32x4_t transform_normal2(const f32x4_t& v, const f32x4x4_t& m) noexcept
	{
		return fmaf(v.vx(), m.column1(), multiplyfv(v.vy(), m.column2()));
	}
	static inline f32x4_t transform_normal3(const f32x4_t& v, const f32x4x4_t& m) noexcept
	{
		return fmaf(v.vx(), m.column1(), fmaf(v.vy(), m.column2(), multiplyfv(v.vz(), m.column3())));
	}
}