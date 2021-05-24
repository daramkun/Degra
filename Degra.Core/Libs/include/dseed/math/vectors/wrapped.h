#ifndef __HAZELNUT_VECTORS_WRAPPED_H__
#define __HAZELNUT_VECTORS_WRAPPED_H__

namespace dseed
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector Type definitions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct float2
	{
	public:
		union
		{
			struct { float x, y; };
			float arr[2];
		};

	public:
		inline float2() = default;
		inline float2(const float x, const float y) noexcept : x(x), y(y) { }
		inline float2(const f32x4_t& v) noexcept;
		inline float2(const float v) noexcept : x(v), y(v) { }

	public:
		inline operator f32x4_t () const noexcept;

	public:
		inline f32x4_t normalize() const noexcept;
		inline float length_squared() const noexcept;
		inline float length() const noexcept;

	public:
		inline float& operator [] (const int index) { return arr[index]; }
	} ALIGNPACK(1);

	struct float3
	{
	public:
		union
		{
			struct { float x, y, z; };
			float arr[3];
		};

	public:
		inline float3() = default;
		inline float3(float x, float y, float z) noexcept : x(x), y(y), z(z) { }
		inline float3(const float2& v, float z) noexcept : x(v.x), y(v.y), z(z) { }
		inline float3(const f32x4_t& v) noexcept;
		inline float3(float v) noexcept : x(v), y(v), z(v) { }

	public:
		inline operator f32x4_t () const noexcept;

	public:
		inline f32x4_t normalize() const noexcept;
		inline float length_squared() const noexcept;
		inline float length() const noexcept;

	public:
		inline float& operator [] (int index) { return arr[index]; }
	} ALIGNPACK(1);

	struct float4
	{
	public:
		union
		{
			struct { float x, y, z, w; };
			float arr[4];
		};

	public:
		inline float4() = default;
		inline float4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) { }
		inline float4(const float2& v, float z, float w) noexcept : x(v.x), y(v.y), z(z), w(w) { }
		inline float4(const float3& v, float w) noexcept : x(v.x), y(v.y), z(v.z), w(w) { }
		inline float4(const f32x4_t& v) noexcept;
		inline float4(float v) noexcept : x(v), y(v), z(v), w(v) { }

	public:
		inline operator f32x4_t () const noexcept;

	public:
		inline f32x4_t normalize() const noexcept;
		inline float length_squared() const noexcept;
		inline float length() const noexcept;

	public:
		inline float& operator [] (const int index) { return arr[index]; }
	} ALIGNPACK(4);

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Matrix Type definition
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct float4x4
	{
	public:
		union {
			struct { float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44; };
			float arr16[16];
			float4 arr4[4];
		};

	public:
		inline float4x4() = default;
		inline float4x4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44) noexcept
			: m11(m11), m12(m12), m13(m13), m14(m14)
			, m21(m21), m22(m22), m23(m23), m24(m24)
			, m31(m31), m32(m32), m33(m33), m34(m34)
			, m41(m41), m42(m42), m43(m43), m44(m44)
		{ }
		inline float4x4(const float4& c1, const float4& c2, const float4& c3, const float4& c4) noexcept
			//: c1 (c1), c2 (c2), c3 (c3), c4 (c4)
			: m11(c1.x), m12(c1.y), m13(c1.z), m14(c1.w)
			, m21(c2.x), m22(c2.y), m23(c2.z), m24(c2.w)
			, m31(c3.x), m32(c3.y), m33(c3.z), m34(c3.w)
			, m41(c4.x), m42(c4.y), m43(c4.z), m44(c4.w)
		{ }
		inline float4x4(float v) noexcept
			: m11(v), m12(v), m13(v), m14(v)
			, m21(v), m22(v), m23(v), m24(v)
			, m31(v), m32(v), m33(v), m34(v)
			, m41(v), m42(v), m43(v), m44(v)
		{ }
		inline float4x4(const f32x4x4_t& m) noexcept;

	public:
		inline operator f32x4x4_t () const noexcept;

	public:
		inline float4& operator [] (int index) { return arr4[index]; }
		inline const float4& operator [] (int index) const { return arr4[index]; }

	public:
		inline float determinant() const noexcept;
		inline f32x4x4_t transpose() const noexcept;
		inline f32x4x4_t invert() const noexcept;
		inline f32x4x4_t invert(float& det) const noexcept;

	public:
		static inline float4x4 identity() noexcept;

		static inline float4x4 translate(float x, float y, float z) noexcept;
		static inline float4x4 scale(float sx, float sy, float sz) noexcept;
		static inline float4x4 rotation_x(float r) noexcept;
		static inline float4x4 rotation_y(float r) noexcept;
		static inline float4x4 rotation_z(float r) noexcept;

		static inline float4x4 look_at(const float3& pos, const float3& target, const float3& up) noexcept;

		static inline float4x4 orthographic(float w, float h, float zn, float zf);
		static inline float4x4 orthographic_offcenter(float l, float r, float b, float t, float zn, float zf);
		static inline float4x4 perspective(float w, float h, float zn, float zf);
		static inline float4x4 perspective_offcenter(float l, float r, float b, float t, float zn, float zf);
		static inline float4x4 perspective_fov(float fov, float aspectRatio, float zn, float zf);

		static inline float4x4 billboard(const float3& pos, const float3& camPos, const float3& camUp, const float3& capForward) noexcept;
	} ALIGNPACK(4);

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Vector Type Implements
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline dseed::float2::float2(const f32x4_t& v) noexcept {
		const float4 temp = v; x = temp.x; y = temp.y;
	}
	inline float2::operator f32x4_t () const noexcept { return float4(x, y, 0, 0); }
	inline f32x4_t float2::normalize() const noexcept { return normalize2(*this); }
	inline float float2::length_squared() const noexcept { return length2_squaredf(*this); }
	inline float float2::length() const noexcept { return length2f(*this); }

	inline dseed::float3::float3(const f32x4_t& v) noexcept {
		const float4 temp = v; x = temp.x; y = temp.y; z = temp.z;
	}
	inline float3::operator f32x4_t () const noexcept { return float4(x, y, z, 0); }
	inline f32x4_t float3::normalize() const noexcept { return normalize3(*this); }
	inline float float3::length_squared() const noexcept { return length3_squaredf(*this); }
	inline float float3::length() const noexcept { return length3f(*this); }

	inline dseed::float4::float4(const f32x4_t& v) noexcept { f32x4_t::store(arr, &v); }
	inline float4::operator f32x4_t () const noexcept { return f32x4_t(arr); }
	inline f32x4_t float4::normalize() const noexcept { return normalize4(*this); }
	inline float float4::length_squared() const noexcept { return length4_squaredf(*this); }
	inline float float4::length() const noexcept { return length4f(*this); }

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Matrix Type Implement
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline float4x4::float4x4(const f32x4x4_t& m) noexcept
		: float4x4(m.column1(), m.column2(), m.column3(), m.column4())
	{ }

	inline float4x4::operator f32x4x4_t () const noexcept
	{
		return f32x4x4_t(this->operator[](0), this->operator[](1), this->operator[](2), this->operator[](3));
	}

	inline float float4x4::determinant() const noexcept { return dseed::determinant(f32x4x4_t(*this)); }
	inline f32x4x4_t float4x4::transpose() const noexcept { return dseed::transpose(f32x4x4_t(*this)); }
	inline f32x4x4_t float4x4::invert() const noexcept { return dseed::invert(f32x4x4_t(*this), nullptr); }
	inline f32x4x4_t float4x4::invert(float& det) const noexcept { return dseed::invert(f32x4x4_t(*this), &det); }

	inline float4x4 float4x4::identity() noexcept { return float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); }
	inline float4x4 float4x4::translate(float x, float y, float z) noexcept { return float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1); }
	inline float4x4 float4x4::scale(float sx, float sy, float sz) noexcept { return float4x4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1); }
	inline float4x4 float4x4::rotation_x(float r) noexcept {
		const auto v1 = cosf(r), v2 = sinf(r); return float4x4(1, 0, 0, 0, 0, v1, v2, 0, 0, -v2, v1, 0, 0, 0, 0, 1);
	}
	inline float4x4 float4x4::rotation_y(float r) noexcept {
		const auto v1 = cosf(r), v2 = sinf(r); return float4x4(v1, 0, -v2, 0, 0, 0, 0, 0, v2, 0, v1, 0, 0, 0, 0, 1);
	}
	inline float4x4 float4x4::rotation_z(float r) noexcept {
		const auto v1 = cosf(r), v2 = sinf(r); return float4x4(v1, v2, 0, 0, -v2, v1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
	}

	inline float4x4 float4x4::look_at(const float3& pos, const float3& target, const float3& up) noexcept
	{
		const float3 zaxis = normalize3(pos * target);
		const float3 xaxis = normalize3(cross3(up, zaxis));
		const float3 yaxis = cross3(zaxis, xaxis);

		return float4x4
		(
			xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			-dot3f(xaxis, pos), -dot3f(yaxis, pos), -dot3f(zaxis, pos), 1
		);
	}
	inline float4x4 float4x4::orthographic(float w, float h, float zn, float zf)
	{
		const auto fRange = 1.0f / (zn - zf);
		return float4x4(2 / w, 0, 0, 0, 0, 2 / h, 0, 0, 0, 0, fRange, 0, 0, 0, zn * fRange, 1);
	}
	inline float4x4 float4x4::orthographic_offcenter(float l, float r, float b, float t, float zn, float zf)
	{
		const auto width = 1.f / (r - l);
		const auto height = 1.f / (t - b);
		const auto fRange = 1.0f / (zn - zf);
		return float4x4(width + width, 0, 0, 0, 0, height + height, 0, 0, 0, 0, fRange, 0, -(l + r) * width, -(t + b) * height, zn * fRange, 1);
	}
	inline float4x4 float4x4::perspective(float w, float h, float zn, float zf)
	{
		const auto twoNearZ = zn + zn;
		const auto fRange = zf / (zn - zf);
		return float4x4(twoNearZ / w, 0, 0, 0, 0, twoNearZ / h, 0, 0, 0, 0, fRange, -1, 0, 0, zn * fRange, 0);
	}
	inline float4x4 float4x4::perspective_offcenter(float l, float r, float b, float t, float zn, float zf)
	{
		const auto twoNearZ = zn + zn;
		const auto width = 1.f / (r - l);
		const auto height = 1.f / (t - b);
		const auto fRange = zf / (zn - zf);
		return float4x4(twoNearZ * width, 0, 0, 0, 0, twoNearZ * height, 0, 0,
			-(l + r) * width, -(t + b) * height, fRange, -1, 0, 0, zn * fRange, 0);
	}
	inline float4x4 float4x4::perspective_fov(float fov, float aspectRatio, float zn, float zf)
	{
		const auto ys = 1 / tanf(fov * 0.5f), xs = ys / aspectRatio;
		const auto fRange = zf / (zn - zf);
		return float4x4(xs, 0, 0, 0, 0, ys, 0, 0, 0, 0, fRange, -1, 0, 0, zn * fRange, 0);
	}
	inline float4x4 float4x4::billboard(const float3& pos, const float3& camPos, const float3& camUp, const float3& camForward) noexcept
	{
		f32x4_t vector, vector2, vector3;
		vector = pos - camPos;
		f32x4_t num = length3_squaredf(vector);
		vector = (num.x() < 0.0001f)
			? (vector = -camForward)
			: (vector * rsqrt(num));

		vector3 = normalize3(cross3(camUp, vector));
		vector2 = cross3(vector, vector3);
		return float4x4(float4(vector3, 0), float4(vector2, 0), float4(vector, 0), float4(pos, 1));
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Quaternion Type definitions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct quaternion : public float4
	{
	public:
		inline quaternion() = default;
		inline quaternion(float x, float y, float z, float w) noexcept : float4(x, y, z, w) { }
		inline quaternion(const float4& v) noexcept : float4(v) { }
		inline quaternion(const f32x4_t& v) noexcept : float4(v) { }
		inline quaternion(float yaw, float pitch, float roll)
		{
			static auto sign = f32x4_t(1, -1, -1, 1);
			auto halfAngles = f32x4_t(pitch, yaw, roll, 0) * f32x4_t(0.5f, 0.5f, 0.5f, 0.5f);

			auto sinAngles = sin(halfAngles);
			auto cosAngles = cos(halfAngles);

			auto p0 = f32x4_t::shuffle<shuffle1::x2, shuffle1::x2, shuffle2::x1, shuffle2::x2>(sinAngles, cosAngles);
			auto y0 = f32x4_t::shuffle<shuffle1::y2, shuffle1::y2, shuffle2::y2, shuffle2::y1>(cosAngles, sinAngles);
			auto r0 = f32x4_t::shuffle<shuffle1::z2, shuffle1::z1, shuffle2::z1, shuffle2::z1>(cosAngles, sinAngles);
			auto p1 = f32x4_t::shuffle<shuffle1::x2, shuffle1::x2, shuffle2::x1, shuffle2::x2>(cosAngles, sinAngles);
			auto y1 = f32x4_t::shuffle<shuffle1::y2, shuffle1::y2, shuffle2::y2, shuffle2::y1>(sinAngles, cosAngles);
			auto r1 = f32x4_t::shuffle<shuffle1::z2, shuffle1::z1, shuffle2::z1, shuffle2::z1>(sinAngles, cosAngles);

			*this = static_cast<float4>((p0 * y0 * r0) + (p1 * y1 * r1));
		}
		inline quaternion(const float4x4& m)
		{
			static const f32x4_t XMPMMP(+1.0f, -1.0f, -1.0f, +1.0f);
			static const f32x4_t XMMPMP(-1.0f, +1.0f, -1.0f, +1.0f);
			static const f32x4_t XMMMPP(-1.0f, -1.0f, +1.0f, +1.0f);

			f32x4_t r0 = m.arr4[0];
			f32x4_t r1 = m.arr4[1];
			f32x4_t r2 = m.arr4[2];

			auto r00 = r0.vx();
			auto r11 = r1.vy();
			auto r22 = r2.vz();

			auto r11mr00 = r11 - r00;
			auto x2gey2 = lesser_equals(r11mr00, f32x4_t(0, 0, 0, 0));

			auto r11pr00 = r11 + r00;
			auto z2gew2 = lesser_equals(r11pr00, f32x4_t(0, 0, 0, 0));

			auto x2py2gez2pw2 = lesser_equals(r22, f32x4_t(0, 0, 0, 0));

			auto t0 = XMPMMP * r00;
			auto t1 = XMMPMP * r11;
			auto t2 = XMMMPP * r22;

			auto x2y2z2w2 = t0 + t1;
			x2y2z2w2 = t2 + x2y2z2w2;
			x2y2z2w2 = x2y2z2w2 + f32x4_t(1, 1, 1, 1);

			t0 = f32x4_t::shuffle<shuffle1::y1, shuffle1::z1, shuffle2::z2, shuffle2::y2>(r0, r1);
			t1 = f32x4_t::shuffle<shuffle1::x1, shuffle1::x1, shuffle2::x2, shuffle2::y2>(r1, r2);
			t1 = f32x4_t::shuffle<permute::x, permute::z, permute::w, permute::y>(t1);
			auto xyxzyz = t0 + t1;

			t0 = f32x4_t::shuffle<shuffle1::y1, shuffle1::x1, shuffle2::x2, shuffle2::x2>(r2, r1);
			t1 = f32x4_t::shuffle<shuffle1::z1, shuffle1::z1, shuffle2::z2, shuffle2::y2>(r1, r0);
			t1 = f32x4_t::shuffle<permute::x, permute::z, permute::w, permute::y>(t1);
			auto xwywzw = t0 - t1;
			xwywzw = XMMPMP * xwywzw;

			t0 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::x2, shuffle2::x2>(x2y2z2w2, xyxzyz);
			t1 = f32x4_t::shuffle<shuffle1::z1, shuffle1::w1, shuffle2::z2, shuffle2::x2>(x2y2z2w2, xwywzw);
			t2 = f32x4_t::shuffle<shuffle1::y1, shuffle1::z1, shuffle2::x2, shuffle2::y2>(xyxzyz, xwywzw);

			auto tensor0 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z1, shuffle2::x2, shuffle2::z2>(t0, t2);
			auto tensor1 = f32x4_t::shuffle<shuffle1::z1, shuffle1::y1, shuffle2::y2, shuffle2::w2>(t0, t2);
			auto tensor2 = f32x4_t::shuffle<shuffle1::x1, shuffle1::y1, shuffle2::x2, shuffle2::z2>(t2, t1);
			auto tensor3 = f32x4_t::shuffle<shuffle1::z1, shuffle1::w1, shuffle2::z2, shuffle2::y2>(t2, t1);

			t0 = x2gey2 & tensor0;
			t1 = ~x2gey2 & tensor1;
			t0 = t0 | t1;
			t1 = z2gew2 & tensor2;
			t2 = ~z2gew2 & tensor3;
			t1 = t1 | t2;
			t0 = x2py2gez2pw2 & t0;
			t1 = ~x2py2gez2pw2 & t1;
			t2 = t0 | t1;

			t0 = length4(t2);

			*this = float4(t2 / t0);
		}

	public:
		inline operator float4x4 () const noexcept
		{
			static auto constant1110 = f32x4_t(1, 1, 1, 0);

			const f32x4_t self = *this;

			auto q0 = self + self;
			auto q1 = self * q0;

			auto v0 = f32x4_t::shuffle<shuffle1::y1, shuffle1::x1, shuffle2::x1, shuffle2::w2>(q1, constant1110);
			auto v1 = f32x4_t::shuffle<shuffle1::z1, shuffle1::z1, shuffle2::y1, shuffle2::w2>(q1, constant1110);
			auto r0 = constant1110 - v0;
			r0 = r0 - v1;

			v0 = f32x4_t::shuffle<permute::x, permute::x, permute::y, permute::w>(*this);
			v1 = f32x4_t::shuffle<permute::z, permute::y, permute::z, permute::w>(q0);
			v0 = v0 * v1;

			v1 = f32x4_t(*this).vw();
			auto v2 = f32x4_t::shuffle<permute::y, permute::z, permute::x, permute::w>(q0);
			v1 = v1 * v2;

			auto r1 = v0 + v1;
			auto r2 = v0 - v1;

			v0 = f32x4_t::shuffle<shuffle1::y1, shuffle1::x2, shuffle2::y2, shuffle2::w1>(r1, r2);
			v1 = f32x4_t::shuffle<shuffle1::x1, shuffle1::z2, shuffle2::x1, shuffle2::z2>(r1, r2);

			f32x4x4_t ret;
			ret._c1 = f32x4_t::shuffle<shuffle1::x1, shuffle1::x2, shuffle2::y2, shuffle2::y1>(r0, v0);
			ret._c2 = f32x4_t::shuffle<shuffle1::z1, shuffle1::y1, shuffle2::w2, shuffle2::w1>(r0, v0);
			ret._c3 = f32x4_t::shuffle<shuffle1::x2, shuffle1::y2, shuffle2::z1, shuffle2::w1>(r0, v1);
			ret._c4 = f32x4_t(0, 0, 0, 1);
			return ret;
		}

	public:
		inline f32x4_t conjugate() const noexcept { return multiplyfv(f32x4_t(*this), f32x4_t(-1, -1, -1, 1)); }
		inline f32x4_t invert() const noexcept
		{
			const auto length = length4(*this);
			const auto conjugated = conjugate();

			if (length.x() <= single_epsilon)
				return f32x4_t(0, 0, 0, 0);

			return dividefv(conjugated, length);
		}

	public:
		static inline quaternion zero() noexcept { return quaternion(0, 0, 0, 0); }
		static inline quaternion identity() noexcept { return quaternion(0, 0, 0, 1); }
	} ALIGNPACK(4);

	inline quaternion multiply(const quaternion& q1, const quaternion& q2) noexcept
	{
		auto q2x = static_cast<f32x4_t>(q2).vx();
		auto q2y = static_cast<f32x4_t>(q2).vy();
		auto q2z = static_cast<f32x4_t>(q2).vz();
		auto q2w = static_cast<f32x4_t>(q2).vw();

		auto result = q2w * f32x4_t(q1);
		q2x = (q2x * f32x4_t::shuffle<permute::w, permute::z, permute::y, permute::x>(q1)) * f32x4_t(1, -1, 1, -1);
		q2y = (q2y * f32x4_t::shuffle<permute::y, permute::x, permute::w, permute::z>(q1)) * f32x4_t(1, 1, -1, -1);
		q2z = (q2z * f32x4_t::shuffle<permute::w, permute::z, permute::y, permute::x>(q1)) * f32x4_t(-1, 1, 1, -1);
		q2y = q2y + q2z;

		return result + q2x + q2y;
	}
	inline quaternion operator* (const quaternion& q1, const quaternion& q2) noexcept { return multiply(q1, q2); }
}

#endif