///////////////////////////////////////////////////////////////////////////////////////////
//
// 4D Vector Comparer
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
#if !DONT_USE_SSE
	static inline f32x4_t equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmpeq_ps(v1, v2); }
	static inline f32x4_t not_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmpneq_ps(v1, v2); }
	static inline f32x4_t lesser(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmplt_ps(v1, v2); }
	static inline f32x4_t lesser_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmple_ps(v1, v2); }
	static inline f32x4_t greater(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmpgt_ps(v1, v2); }
	static inline f32x4_t greater_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_cmpge_ps(v1, v2); }

	static inline bool equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(equals(v1, v2)) == 0xf; }
	static inline bool not_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(not_equals(v1, v2)) == 0xf; }
	static inline bool lesserb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(lesser(v1, v2)) == 0xf; }
	static inline bool lesser_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(lesser_equals(v1, v2)) == 0xf; }
	static inline bool greaterb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(greater(v1, v2)) == 0xf; }
	static inline bool greater_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return _mm_movemask_ps(greater_equals(v1, v2)) == 0xf; }

	static inline i32x4_t equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_cmpeq_epi32(v1, v2); }
	static inline i32x4_t not_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return not32(_mm_cmpeq_epi32(v1, v2)); }
	static inline i32x4_t lesser32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_cmplt_epi32(v1, v2); }
	static inline i32x4_t lesser_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return or32(lesser32(v1, v2), equals32(v1, v2)); }
	static inline i32x4_t greater32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_cmpgt_epi32(v1, v2); }
	static inline i32x4_t greater_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return or32(greater32(v1, v2), equals32(v1, v2)); }

	static inline bool equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(equals32(v1, v2)) == 0xffff; }
	static inline bool not_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(not_equals32(v1, v2)) == 0xffff; }
	static inline bool lesser32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(lesser32(v1, v2)) == 0xffff; }
	static inline bool lesser_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(lesser_equals32(v1, v2)) == 0xffff; }
	static inline bool greater32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(greater32(v1, v2)) == 0xffff; }
	static inline bool greater_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return _mm_movemask_epi8(greater_equals32(v1, v2)) == 0xffff; }
#elif !DONT_USE_NEON
	static inline f32x4_t equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vceqq_f32(v1, v2); }
	static inline f32x4_t not_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vmvnq_u32(vceqq_f32(v1, v2)); }
	static inline f32x4_t lesser(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vcltq_f32(v1, v2); }
	static inline f32x4_t lesser_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vcleq_f32(v1, v2); }
	static inline f32x4_t greater(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vcgtq_f32(v1, v2); }
	static inline f32x4_t greater_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept { return vcgeq_f32(v1, v2); }

	static inline bool equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(equals(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool not_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(not_equals(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool lesserb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(lesser(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool lesser_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(lesser_equals(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool greaterb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(greater(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool greater_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		auto result = reinterpretftoi(greater_equals(v1, v2));
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}

	static inline i32x4_t equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vceqq_s32(v1, v2); }
	static inline i32x4_t not_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vmvnq_s32(vceqq_s32(v1, v2)); }
	static inline i32x4_t lesser32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vcltq_s32(v1, v2); }
	static inline i32x4_t lesser_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vcleq_s32(v1, v2); }
	static inline i32x4_t greater32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vcgtq_s32(v1, v2); }
	static inline i32x4_t greater_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept { return vcgeq_s32(v1, v2); }

	static inline bool equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = equals32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool not_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = not_equals32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool lesser32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = lesser32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool lesser_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = lesser_equals32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool greater32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = greater32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
	static inline bool greater_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		auto result = greater_equals32(v1, v2);
		return and32(and32(and32(result.vx(), result.vy()), result.vz()), result.vw()).x() == 0xffffffff;
	}
#elif DONT_USE_SIMD
	static inline f32x4_t equals(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() == v2.x() ? 0xffffffff : 0,
			v1.y() == v2.y() ? 0xffffffff : 0,
			v1.z() == v2.z() ? 0xffffffff : 0,
			v1.w() == v2.w() ? 0xffffffff : 0
		));
	}
	static inline f32x4_t not_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() != v2.x() ? 0xffffffff : 0,
			v1.y() != v2.y() ? 0xffffffff : 0,
			v1.z() != v2.z() ? 0xffffffff : 0,
			v1.w() != v2.w() ? 0xffffffff : 0
		));
	}
	static inline f32x4_t lesser(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() < v2.x() ? 0xffffffff : 0,
			v1.y() < v2.y() ? 0xffffffff : 0,
			v1.z() < v2.z() ? 0xffffffff : 0,
			v1.w() < v2.w() ? 0xffffffff : 0
		));
	}
	static inline f32x4_t lesser_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() <= v2.x() ? 0xffffffff : 0,
			v1.y() <= v2.y() ? 0xffffffff : 0,
			v1.z() <= v2.z() ? 0xffffffff : 0,
			v1.w() <= v2.w() ? 0xffffffff : 0
		));
	}
	static inline f32x4_t greater(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() > v2.x() ? 0xffffffff : 0,
			v1.y() > v2.y() ? 0xffffffff : 0,
			v1.z() > v2.z() ? 0xffffffff : 0,
			v1.w() > v2.w() ? 0xffffffff : 0
		));
	}
	static inline f32x4_t greater_equals(const f32x4_t& v1, const f32x4_t& v2) noexcept
	{
		return reinterpretitof(i32x4_t(
			v1.x() >= v2.x() ? 0xffffffff : 0,
			v1.y() >= v2.y() ? 0xffffffff : 0,
			v1.z() >= v2.z() ? 0xffffffff : 0,
			v1.w() >= v2.w() ? 0xffffffff : 0
		));
	}

	static inline bool equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z() && v1.w() == v2.w(); }
	static inline bool not_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() != v2.x() && v1.y() != v2.y() && v1.z() != v2.z() && v1.w() != v2.w(); }
	static inline bool lesserb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() < v2.x() && v1.y() < v2.y() && v1.z() < v2.z() && v1.w() < v2.w(); }
	static inline bool lesser_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() <= v2.x() && v1.y() <= v2.y() && v1.z() <= v2.z() && v1.w() <= v2.w(); }
	static inline bool greaterb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() > v2.x() && v1.y() > v2.y() && v1.z() > v2.z() && v1.w() > v2.w(); }
	static inline bool greater_equalsb(const f32x4_t& v1, const f32x4_t& v2) noexcept { return v1.x() >= v2.x() && v1.y() >= v2.y() && v1.z() >= v2.z() && v1.w() >= v2.w(); }

	static inline i32x4_t equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() == v2.x() ? 0xffffffff : 0,
			v1.y() == v2.y() ? 0xffffffff : 0,
			v1.z() == v2.z() ? 0xffffffff : 0,
			v1.w() == v2.w() ? 0xffffffff : 0
		);
	}
	static inline i32x4_t not_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() != v2.x() ? 0xffffffff : 0,
			v1.y() != v2.y() ? 0xffffffff : 0,
			v1.z() != v2.z() ? 0xffffffff : 0,
			v1.w() != v2.w() ? 0xffffffff : 0
		);
	}
	static inline i32x4_t lesser32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() < v2.x() ? 0xffffffff : 0,
			v1.y() < v2.y() ? 0xffffffff : 0,
			v1.z() < v2.z() ? 0xffffffff : 0,
			v1.w() < v2.w() ? 0xffffffff : 0
		);
	}
	static inline i32x4_t lesser_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() <= v2.x() ? 0xffffffff : 0,
			v1.y() <= v2.y() ? 0xffffffff : 0,
			v1.z() <= v2.z() ? 0xffffffff : 0,
			v1.w() <= v2.w() ? 0xffffffff : 0
		);
	}
	static inline i32x4_t greater32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() > v2.x() ? 0xffffffff : 0,
			v1.y() > v2.y() ? 0xffffffff : 0,
			v1.z() > v2.z() ? 0xffffffff : 0,
			v1.w() > v2.w() ? 0xffffffff : 0
		);
	}
	static inline i32x4_t greater_equals32(const i32x4_t& v1, const i32x4_t& v2) noexcept
	{
		return i32x4_t(
			v1.x() >= v2.x() ? 0xffffffff : 0,
			v1.y() >= v2.y() ? 0xffffffff : 0,
			v1.z() >= v2.z() ? 0xffffffff : 0,
			v1.w() >= v2.w() ? 0xffffffff : 0
		);
	}

	static inline bool equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z() && v1.w() == v2.w(); }
	static inline bool not_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() != v2.x() && v1.y() != v2.y() && v1.z() != v2.z() && v1.w() != v2.w(); }
	static inline bool lesser32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() < v2.x() && v1.y() < v2.y() && v1.z() < v2.z() && v1.w() < v2.w(); }
	static inline bool lesser_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() <= v2.x() && v1.y() <= v2.y() && v1.z() <= v2.z() && v1.w() <= v2.w(); }
	static inline bool greater32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() > v2.x() && v1.y() > v2.y() && v1.z() > v2.z() && v1.w() > v2.w(); }
	static inline bool greater_equals32b(const i32x4_t& v1, const i32x4_t& v2) noexcept { return v1.x() >= v2.x() && v1.y() >= v2.y() && v1.z() >= v2.z() && v1.w() >= v2.w(); }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 4x4 Matrix Comparer
//
///////////////////////////////////////////////////////////////////////////////////////////
namespace dseed
{
	static inline f32x4x4_t equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			equals(m1.column1(), m2.column1()),
			equals(m1.column2(), m2.column2()),
			equals(m1.column3(), m2.column3()),
			equals(m1.column4(), m2.column4())
		);
	}
	static inline f32x4x4_t not_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			not_equals(m1.column1(), m2.column1()),
			not_equals(m1.column2(), m2.column2()),
			not_equals(m1.column3(), m2.column3()),
			not_equals(m1.column4(), m2.column4())
		);
	}
	static inline f32x4x4_t lesser(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			lesser(m1.column1(), m2.column1()),
			lesser(m1.column2(), m2.column2()),
			lesser(m1.column3(), m2.column3()),
			lesser(m1.column4(), m2.column4())
		);
	}
	static inline f32x4x4_t lesser_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			lesser_equals(m1.column1(), m2.column1()),
			lesser_equals(m1.column2(), m2.column2()),
			lesser_equals(m1.column3(), m2.column3()),
			lesser_equals(m1.column4(), m2.column4())
		);
	}
	static inline f32x4x4_t greater(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			greater(m1.column1(), m2.column1()),
			greater(m1.column2(), m2.column2()),
			greater(m1.column3(), m2.column3()),
			greater(m1.column4(), m2.column4())
		);
	}
	static inline f32x4x4_t greater_equals(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return f32x4x4_t(
			greater_equals(m1.column1(), m2.column1()),
			greater_equals(m1.column2(), m2.column2()),
			greater_equals(m1.column3(), m2.column3()),
			greater_equals(m1.column4(), m2.column4())
		);
	}

	static inline bool equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return equalsb(m1.column1(), m2.column1()) &&
			equalsb(m1.column2(), m2.column2()) &&
			equalsb(m1.column3(), m2.column3()) &&
			equalsb(m1.column4(), m2.column4());
	}
	static inline bool not_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return not_equalsb(m1.column1(), m2.column1()) &&
			not_equalsb(m1.column2(), m2.column2()) &&
			not_equalsb(m1.column3(), m2.column3()) &&
			not_equalsb(m1.column4(), m2.column4());
	}
	static inline bool lesserb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return lesserb(m1.column1(), m2.column1()) &&
			lesserb(m1.column2(), m2.column2()) &&
			lesserb(m1.column3(), m2.column3()) &&
			lesserb(m1.column4(), m2.column4());
	}
	static inline bool lesser_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return lesser_equalsb(m1.column1(), m2.column1()) &&
			lesser_equalsb(m1.column2(), m2.column2()) &&
			lesser_equalsb(m1.column3(), m2.column3()) &&
			lesser_equalsb(m1.column4(), m2.column4());
	}
	static inline bool greaterb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return greaterb(m1.column1(), m2.column1()) &&
			greaterb(m1.column2(), m2.column2()) &&
			greaterb(m1.column3(), m2.column3()) &&
			greaterb(m1.column4(), m2.column4());
	}
	static inline bool greater_equalsb(const f32x4x4_t& m1, const f32x4x4_t& m2) noexcept
	{
		return greater_equalsb(m1.column1(), m2.column1()) &&
			greater_equalsb(m1.column2(), m2.column2()) &&
			greater_equalsb(m1.column3(), m2.column3()) &&
			greater_equalsb(m1.column4(), m2.column4());
	}
}