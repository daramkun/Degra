namespace dseed
{
	static inline f32x4x4_t transpose(const f32x4x4_t& m) noexcept
	{
		auto temp0 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::x2, shuffle2::y2>(m.column1(), m.column2());
		auto temp2 = f32x4_t::shuffle<shuffle1::z1, shuffle1::w1, shuffle2::z2, shuffle2::w2>(m.column1(), m.column2());
		auto temp1 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::x2, shuffle2::y2>(m.column3(), m.column4());
		auto temp3 = f32x4_t::shuffle<shuffle1::z1, shuffle1::w1, shuffle2::z2, shuffle2::w2>(m.column3(), m.column4());

		f32x4x4_t ret;
		ret._c1 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::x2, shuffle2::z2>(temp0, temp1);
		ret._c2 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::y2, shuffle2::w2>(temp0, temp1);
		ret._c3 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::x2, shuffle2::z2>(temp2, temp3);
		ret._c4 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::y2, shuffle2::w2>(temp2, temp3);

		return ret;
	}
	static inline float determinant(const f32x4x4_t& m) noexcept
	{
		const auto m11 = m.column1().x(), m12 = m.column1().y(), m13 = m.column1().z(), m14 = m.column1().w()
			, m21 = m.column2().x(), m22 = m.column2().y(), m23 = m.column2().z(), m24 = m.column2().w();
		const auto m31 = m.column3().x(), m32 = m.column3().y(), m33 = m.column3().z(), m34 = m.column3().w()
			, m41 = m.column4().x(), m42 = m.column4().y(), m43 = m.column4().z(), m44 = m.column4().w();

		const auto n18 = (m33 * m44) - (m34 * m43), n17 = (m32 * m44) - (m34 * m42);
		const auto n16 = (m32 * m43) - (m33 * m42), n15 = (m31 * m44) - (m34 * m41);
		const auto n14 = (m31 * m43) - (m33 * m41), n13 = (m31 * m42) - (m32 * m41);

		return (((m11 * (((m22 * n18) - (m23 * n17)) + (m24 * n16))) -
			(m12 * (((m21 * n18) - (m23 * n15)) + (m24 * n14)))) +
			(m13 * (((m21 * n17) - (m22 * n15)) + (m24 * n13)))) -
			(m14 * (((m21 * n16) - (m22 * n14)) + (m23 * n13)));
	}
	static inline f32x4x4_t invert(const f32x4x4_t& m, float* determinant) noexcept
	{
		static auto one = f32x4_t(1);

		auto transposed = transpose(m);
		auto v00 = f32x4_t::shuffle<permute::x, permute::x, permute::y, permute::y>(transposed.column3());
		auto v10 = f32x4_t::shuffle<permute::z, permute::w, permute::z, permute::w>(transposed.column4());
		auto v01 = f32x4_t::shuffle<permute::x, permute::x, permute::y, permute::y>(transposed.column1());
		auto v11 = f32x4_t::shuffle<permute::z, permute::w, permute::z, permute::w>(transposed.column2());
		auto v02 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::x2, shuffle2::z2>(transposed.column3(), transposed.column1());
		auto v12 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::y2, shuffle2::w2>(transposed.column4(), transposed.column2());

		auto d0 = v00 * v10;
		auto d1 = v01 * v11;
		auto d2 = v02 * v12;

		v00 = f32x4_t::shuffle<permute::z, permute::w, permute::z, permute::w>(transposed.column3());
		v10 = f32x4_t::shuffle<permute::x, permute::x, permute::y, permute::y>(transposed.column4());
		v01 = f32x4_t::shuffle<permute::z, permute::w, permute::z, permute::w>(transposed.column1());
		v11 = f32x4_t::shuffle<permute::x, permute::x, permute::y, permute::y>(transposed.column2());
		v02 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::y2, shuffle2::w2>(transposed.column3(), transposed.column1());
		v12 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::x2, shuffle2::z2>(transposed.column4(), transposed.column2());

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;

		d0 = d0 - v00;
		d1 = d1 - v01;
		d2 = d2 - v02;

		v11 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::y2, shuffle2::y2>(d0, d2);
		v00 = f32x4_t::shuffle<permute::y, permute::z, permute::x, permute::y>(transposed.column2());
		v10 = f32x4_t::shuffle<shuffle1::z1, shuffle1::x1, shuffle2::w2, shuffle2::x2>(v11, d0);
		v01 = f32x4_t::shuffle<permute::z, permute::x, permute::y, permute::x>(transposed.column1());
		v11 = f32x4_t::shuffle<shuffle1::y1, shuffle1::z1, shuffle2::y2, shuffle2::z2>(v11, d0);

		auto v13 = f32x4_t::shuffle<shuffle1::y1, shuffle1::w1, shuffle2::w2, shuffle2::w2>(d1, d2);
		v02 = f32x4_t::shuffle<permute::y, permute::z, permute::x, permute::y>(transposed.column4());
		v12 = f32x4_t::shuffle<shuffle1::z1, shuffle1::x1, shuffle2::w2, shuffle2::x2>(v13, d1);
		auto v03 = f32x4_t::shuffle<permute::z, permute::x, permute::y, permute::x>(transposed.column3());
		v13 = f32x4_t::shuffle<shuffle1::y1, shuffle1::z1, shuffle2::y2, shuffle2::z2>(v13, d1);

		auto c0 = v00 * v10;
		auto c2 = v01 * v11;
		auto c4 = v02 * v12;
		auto c6 = v03 * v13;

		v11 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::x2, shuffle2::x2>(d0, d2);
		v00 = f32x4_t::shuffle<permute::z, permute::w, permute::y, permute::z>(transposed.column2());
		v10 = f32x4_t::shuffle<shuffle1::w1, shuffle1::x1, shuffle2::y2, shuffle2::z2>(d0, v11);
		v01 = f32x4_t::shuffle<permute::w, permute::z, permute::w, permute::y>(transposed.column1());
		v11 = f32x4_t::shuffle<shuffle1::z1, shuffle1::y1, shuffle2::z2, shuffle2::x2>(d0, v11);

		v13 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::z2, shuffle2::z2>(d1, d2);
		v02 = f32x4_t::shuffle<permute::z, permute::w, permute::y, permute::z>(transposed.column4());
		v12 = f32x4_t::shuffle<shuffle1::w1, shuffle1::x1, shuffle2::y2, shuffle2::z2>(d1, v13);
		v03 = f32x4_t::shuffle<permute::w, permute::z, permute::w, permute::y>(transposed.column3());
		v13 = f32x4_t::shuffle<shuffle1::z1, shuffle1::y1, shuffle2::z2, shuffle2::x2>(d1, v13);

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;
		v03 = v03 * v13;
		c0 = c0 - v00;
		c2 = c2 - v01;
		c4 = c4 - v02;
		c6 = c6 - v03;

		v00 = f32x4_t::shuffle<permute::w, permute::x, permute::w, permute::x>(transposed.column2());
		v10 = f32x4_t::shuffle<shuffle1::z1, shuffle1::z1, shuffle2::x2, shuffle2::y2>(d0, d2);
		v10 = f32x4_t::shuffle<permute::x, permute::w, permute::z, permute::x>(v10);
		v01 = f32x4_t::shuffle<permute::y, permute::w, permute::x, permute::z>(transposed.column1());
		v11 = f32x4_t::shuffle<shuffle1::x1, shuffle1::w1, shuffle2::x2, shuffle2::y2>(d0, d2);
		v11 = f32x4_t::shuffle<permute::w, permute::x, permute::y, permute::z>(v11);
		v02 = f32x4_t::shuffle<permute::w, permute::x, permute::w, permute::x>(transposed.column4());
		v12 = f32x4_t::shuffle<shuffle1::z1, shuffle1::z1, shuffle2::z2, shuffle2::w2>(d1, d2);
		v12 = f32x4_t::shuffle<permute::x, permute::w, permute::z, permute::x>(v12);
		v03 = f32x4_t::shuffle<permute::y, permute::w, permute::x, permute::z>(transposed.column3());
		v13 = f32x4_t::shuffle<shuffle1::x1, shuffle1::w1, shuffle2::z2, shuffle2::w2>(d1, d2);
		v13 = f32x4_t::shuffle<permute::w, permute::x, permute::y, permute::z>(v13);

		v00 = v00 * v10;
		v01 = v01 * v11;
		v02 = v02 * v12;
		v03 = v03 * v13;
		auto c1 = c0 - v00;
		c0 = c0 + v00;
		auto c3 = c2 + v01;
		c2 = c2 - v01;
		auto c5 = c4 - v02;
		c4 = c4 + v02;
		auto c7 = c6 + v03;
		c6 = c6 - v03;

		c0 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::y2, shuffle2::w2>(c0, c1);
		c2 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::y2, shuffle2::w2>(c2, c3);
		c4 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::y2, shuffle2::w2>(c4, c5);
		c6 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::y2, shuffle2::w2>(c6, c7);
		c0 = f32x4_t::shuffle<permute::x, permute::z, permute::y, permute::w>(c0);
		c2 = f32x4_t::shuffle<permute::x, permute::z, permute::y, permute::w>(c2);
		c4 = f32x4_t::shuffle<permute::x, permute::z, permute::y, permute::w>(c4);
		c6 = f32x4_t::shuffle<permute::x, permute::z, permute::y, permute::w>(c6);

		auto temp = dot4(c0, transposed.column1());
		if (determinant)
			*determinant = temp.x();
		temp = one / temp;

		f32x4x4_t ret;
		ret._c1 = c0 * temp;
		ret._c2 = c2 * temp;
		ret._c3 = c4 * temp;
		ret._c4 = c6 * temp;

		return ret;
	}
}