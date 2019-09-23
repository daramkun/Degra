#ifndef __DSEED_MATH_COLLISION_INL__
#define __DSEED_MATH_COLLISION_INL__

// Most of Sources from DirectXMath's Collision

namespace dseed
{
	struct plane;
	struct ray;

	struct bounding_box;
	struct bounding_sphere;
	struct bounding_frustum;

	enum intersect_t { intersect_disjoint = 0, intersect_intersects, intersect_contains };
	enum plane_intersect_t { plane_intersect_front, plane_intersect_back, plane_intersect_intersecting };

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Bounding shape Type definitions
	//
	////////////////////////////////////////////////////////////////////////////////////////
	struct plane
	{
	public:
		float x, y, z, w;

	public:
		plane () = default;
		inline plane (const float3& p1, const float3& p2, const float3& p3);
		inline plane (const float3& p, const float3& n);
		inline plane (float x, float y, float z, float w) : x (x), y (y), z (z), w (w) { }
		inline plane (const float4& v) : plane (v.x, v.y, v.z, v.w) { }

	public:
		inline operator float4 () const noexcept { return float4 (x, y, z, w); }

	public:
		inline vectorf normalize ();

	public:
		inline bool intersects (const plane& p);
		inline void intersects (const plane& p, float3* ip1, float3* ip2);

		inline bool intersects (const float3& lp1, const float3& lp2);
		inline void intersects (const float3& lp1, const float3& lp2, float3* ip);
	};

	struct ray
	{
	public:
		float3 position, direction;

	public:
		ray () = default;
		inline ray (const float3& position, const float3& direction)
			: position (position), direction (direction)
		{ }
	};

	struct bounding_box
	{
	public:
		float3 center, extents;
		quaternion orientation;

	public:
		bounding_box () = default;
		inline bounding_box (const float3& center, const float3& extents,
			const quaternion& orientation)
			: center (center), extents (extents), orientation (orientation)
		{ }

	public:
		plane_intersect_t intersects (const plane& p) const;
		intersect_t intersects (const bounding_box& bb) const;
		intersect_t intersects (const bounding_sphere& bs) const;
		intersect_t intersects (const bounding_frustum& bf) const;
	};

	struct bounding_sphere
	{
	public:
		float3 center;
		float radius;

	public:
		bounding_sphere () = default;
		inline bounding_sphere (const float3& center, float radius)
			: center (center), radius (radius)
		{ }

	public:
		plane_intersect_t intersects (const plane& p) const;
		intersect_t intersects (const bounding_box& bb) const;
		intersect_t intersects (const bounding_sphere& bs) const;
		intersect_t intersects (const bounding_frustum& bf) const;
	};

	struct bounding_frustum
	{
	public:
		float3 position;
		quaternion orientation;

		float right, left, top, bottom;
		float znear, zfar;

	public:
		bounding_frustum () = default;
		inline bounding_frustum (const float3& position, const quaternion& orientation,
			float left, float top, float bottom, float right, float znear, float zfar)
			: position (position), orientation (orientation)
			, left (left), top (top), bottom (bottom), right (right)
			, znear (znear), zfar (zfar)
		{}
		inline bounding_frustum (const float4x4& projection);

	public:
		void generate_planes (plane* n, plane* f, plane* l, plane* r, plane* t, plane* b) const noexcept;

	public:
		plane_intersect_t intersects (const plane& p) const;
		intersect_t intersects (const bounding_box& bb) const;
		intersect_t intersects (const bounding_sphere& bs) const;
		intersect_t intersects (const bounding_frustum& bf) const;
	};

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Bounding Shape Type Operators
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline vectorf dot_plane_normal (const vectorf& p, const vectorf& n) noexcept
	{
		return dotvf3d (p, n);
	}
	inline vectorf dot_plane_coord (const vectorf& p, const vectorf& v) noexcept
	{
		return dot_plane_normal (p, v) + p.splat_w ();
	}

	inline void intersects_sphere_plane (const vectorf& center, const vectorf& radius,
		const vectorf& plane, vectorf* outside, vectorf* inside) noexcept
	{
		vectorf dist = dotvf4d (center, plane);
		*outside = greatervf (dist, radius);
		*inside = lesservf (dist, negatevf (radius));
	}
	inline void intersects_box_plane (const vectorf& center, const vectorf& extents,
		const vectorf& axis0, const vectorf& axis1, const vectorf& axis2,
		const vectorf& plane, vectorf* outside, vectorf* inside) noexcept
	{
		vectorf dist = dotvf4d (center, plane);

		vectorf radius = dotvf3d (plane, axis0);
		radius = insertvf<0, 0, 1, 0, 0> (radius, dotvf3d (plane, axis1));
		radius = insertvf<0, 0, 0, 1, 0> (radius, dotvf3d (plane, axis2));
		radius = dotvf3d (extents, absvf (radius));

		*outside = greatervf (dist, radius);
		*inside = lesservf (dist, negatevf (radius));
	}
	inline void intersects_frustum_plane (
		const vectorf& p1, const vectorf& p2, const vectorf& p3, const vectorf& p4,
		const vectorf& p5, const vectorf& p6, const vectorf& p7, const vectorf& p8,
		const vectorf& plane, vectorf* outside, vectorf* inside) noexcept
	{
		vectorf min, max, dist;

		min = max = dotvf3d (plane, p1);

		dist = dotvf3d (plane, p2);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p3);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p4);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p5);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p6);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p7);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		dist = dotvf3d (plane, p8);
		min = minvf (min, dist);
		max = maxvf (max, dist);

		vectorf planeDist = negatevf (plane.splat_w ());

		*outside = greatervf (min, planeDist);
		*inside = lesservf (max, planeDist);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Plane Type Implement
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline plane::plane (const float3& p1, const float3& p2, const float3& p3)
	: plane (p1, normalizevf3d (crossvf3d ((vectorf)p2 - p1, (vectorf)p3 - p1)))
	{ }
	inline plane::plane (const float3& p, const float3& n)
		: plane (n.x, n.y, n.z, -dotvf3d (vectorf(p), vectorf(n)).x ())
	{ }
	inline vectorf plane::normalize () { return normalize_plane ((float4)* this); }

	inline bool plane::intersects (const plane& p)
	{
		float3 ip1, ip2;
		intersects (p, &ip1, &ip2);
		return !(ip1 == ip2 && ip1 == float3 (dseed::single_nan));
	}

	inline void plane::intersects (const plane& p, float3* ip1, float3* ip2)
	{
		if (ip1 == nullptr || ip2 == nullptr) return;

		vectorf v1 = crossvf3d ((float4)p, (float4)* this);
		vectorf lengthsq = length_squaredvf3d (v1);

		vectorf v2 = crossvf3d ((float4)p, v1);
		vectorf point = v1 * float4 (w, w, w, w);

		point = fmavf (crossvf3d (v1, (float4)* this), float4 (p.w, p.w, p.w, p.w), point);

		if (equalsvf (lengthsq, vectorf (0, 0, 0, 0)) == 0xF)
			* ip2 = (*ip1 = point / lengthsq) + v1;
		else
			*ip1 = *ip2 = float3 (dseed::single_nan);
	}

	inline bool plane::intersects (const float3& lp1, const float3& lp2)
	{
		float3 ip;
		intersects (lp1, lp2, &ip);
		return ip != float3 (dseed::single_nan);
	}
	inline void plane::intersects (const float3& lp1, const float3& lp2, float3* ip)
	{
		if (ip == nullptr)
			return;

		vectorf d = dotvf3d ((vectorf)((float4)* this), lp1) - dotvf3d ((vectorf)((float4)* this), lp2);
		*ip = (equalsvf (d, vectorf (0, 0, 0, 0)) == 0xF)
			? float3 (fmavf (lp2 - lp1, float3 (dot_plane_coord ((float4)* this, lp1) / d), lp1))
			: float3 (dseed::single_nan);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Bounding Box Type Implement
	//
	////////////////////////////////////////////////////////////////////////////////////////
	const static vectorf ____bounding_box_polygon[] = {
		float3 (-1.0f, -1.0f, +1.0f),
		float3 (+1.0f, -1.0f, +1.0f),
		float3 (+1.0f, +1.0f, +1.0f),
		float3 (-1.0f, +1.0f, +1.0f),
		float3 (-1.0f, -1.0f, -1.0f),
		float3 (+1.0f, -1.0f, -1.0f),
		float3 (+1.0f, +1.0f, -1.0f),
		float3 (-1.0f, +1.0f, -1.0f),
	};

	inline plane_intersect_t bounding_box::intersects (const plane& p) const
	{
		vectorf vCenter = center;
		vectorf vExtents = extents;
		vectorf BoxOrientation = orientation;

		vCenter = insertvf<0, 0, 0, 0, 1> (vCenter, vectorf (1));

		matrixf R = to_matrixq (BoxOrientation);

		vectorf outside, inside;
		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3,
			(vectorf)(float4)p, &outside, &inside);

		if (outside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_front;

		if (inside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_back;

		return plane_intersect_intersecting;
	}
	inline intersect_t bounding_box::intersects (const bounding_box& bb) const
	{
		vectorf A_quat = orientation;
		vectorf B_quat = bb.orientation;

		vectorf Q = quaternion_multiply (A_quat, conjugateq (B_quat));
		matrixf R = to_matrixq (Q);

		vectorf A_cent = center;
		vectorf B_cent = bb.center;
		vectorf t = inverse_rotatevf3d ((B_cent - A_cent), A_quat);

		vectorf h_A = extents;
		vectorf h_B = bb.extents;

		vectorf R0X = R.column1;
		vectorf R1X = R.column2;
		vectorf R2X = R.column3;

		R = transposemf (R);

		vectorf RX0 = R.column1;
		vectorf RX1 = R.column2;
		vectorf RX2 = R.column3;

		vectorf AR0X = absvf (R0X);
		vectorf AR1X = absvf (R1X);
		vectorf AR2X = absvf (R2X);

		vectorf ARX0 = absvf (RX0);
		vectorf ARX1 = absvf (RX1);
		vectorf ARX2 = absvf (RX2);

		vectorf d, d_A, d_B;

		d = t.splat_x ();
		d_A = h_A.splat_x ();
		d_B = dotvf3d (h_B, AR0X);
		vectorf NoIntersection = greatervf (absvf (d), (d_A + d_B));

		d = t.splat_y ();
		d_A = h_A.splat_y ();
		d_B = dotvf3d (h_B, AR1X);
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = t.splat_z ();
		d_A = h_A.splat_z ();
		d_B = dotvf3d (h_B, AR2X);
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, RX0);
		d_A = dotvf3d (h_A, ARX0);
		d_B = h_B.splat_x();
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, RX1);
		d_A = dotvf3d (h_A, ARX1);
		d_B = h_B.splat_y ();
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, RX2);
		d_A = dotvf3d (h_A, ARX2);
		d_B = h_B.splat_z ();
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_aw1, shuffle_az2, shuffle_by1, shuffle_bx1> (RX0, negatevf (RX0)));
		d_A = dotvf3d (h_A, ARX0.permute32<3, 2, 1, 0> ());
		d_B = dotvf3d (h_B, AR0X.permute32<3, 2, 1, 0> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_aw1, shuffle_az2, shuffle_by1, shuffle_bx1> (RX1, negatevf (RX1)));
		d_A = dotvf3d (h_A, ARX1.permute32<3, 2, 1, 0> ());
		d_B = dotvf3d (h_B, AR0X.permute32<2, 3, 0, 1> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_aw1, shuffle_az2, shuffle_by1, shuffle_bx1> (RX2, negatevf (RX2)));
		d_A = dotvf3d (h_A, ARX2.permute32<3, 2, 1, 0> ());
		d_B = dotvf3d (h_B, AR0X.permute32<1, 0, 3, 2> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_az1, shuffle_aw1, shuffle_bx2, shuffle_by1> (RX0, negatevf (RX0)));
		d_A = dotvf3d (h_A, ARX0.permute32<2, 3, 0, 1> ());
		d_B = dotvf3d (h_B, AR1X.permute32<3, 2, 1, 0> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_az1, shuffle_aw1, shuffle_bx2, shuffle_by1> (RX1, negatevf (RX1)));
		d_A = dotvf3d (h_A, ARX1.permute32<2, 3, 0, 1> ());
		d_B = dotvf3d (h_B, AR1X.permute32<2, 3, 0, 1> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_az1, shuffle_aw1, shuffle_bx2, shuffle_by1> (RX2, negatevf (RX2)));
		d_A = dotvf3d (h_A, ARX2.permute32<2, 3, 0, 1> ());
		d_B = dotvf3d (h_B, AR1X.permute32<1, 0, 3, 2> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_ay2, shuffle_ax1, shuffle_bw1, shuffle_bz1> (RX0, negatevf (RX0)));
		d_A = dotvf3d (h_A, ARX0.permute32<1, 0, 3, 2> ());
		d_B = dotvf3d (h_B, AR2X.permute32<3, 2, 1, 0> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_ay2, shuffle_ax1, shuffle_bw1, shuffle_bz1> (RX1, negatevf (RX1)));
		d_A = dotvf3d (h_A, ARX1.permute32<1, 0, 3, 2> ());
		d_B = dotvf3d (h_B, AR2X.permute32<2, 3, 0, 1> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		d = dotvf3d (t, vectorf::shuffle32<shuffle_ay2, shuffle_ax1, shuffle_bw1, shuffle_bz1> (RX2, negatevf (RX2)));
		d_A = dotvf3d (h_A, ARX2.permute32<1, 0, 3, 2> ());
		d_B = dotvf3d (h_B, AR2X.permute32<1, 0, 3, 2> ());
		NoIntersection = orvf (NoIntersection, greatervf (absvf (d), (d_A + d_B)));

		if (reinterpret_f32_to_i32 (NoIntersection) == vectori (-1))
			return dseed::intersect_disjoint;

		vectorf aCenter = center;
		vectorf aExtents = extents;
		vectorf aOrientation = orientation;

		vectorf bCenter = bb.center;
		vectorf bExtents = bb.extents;
		vectorf bOrientation = bb.orientation;

		vectorf offset = bCenter - aCenter;

		for (size_t i = 0; i < 8; ++i)
		{
			vectorf C = rotatevf3d (bExtents * ____bounding_box_polygon[i], bOrientation) + offset;
			C = inverse_rotatevf3d (C, aOrientation);

			if (!inboundsvf3d (C, aExtents))
				return dseed::intersect_intersects;
		}

		return dseed::intersect_contains;
	}
	inline intersect_t bounding_box::intersects (const bounding_sphere& bs) const
	{
		vectorf sphereCenter = bs.center;
		vectorf sphereRadius = bs.radius;

		vectorf boxCenter = center;
		vectorf boxExtents = extents;
		vectorf boxOrient = orientation;

		sphereCenter = inverse_rotatevf3d ((sphereCenter - boxCenter), boxOrient);

		vectorf lesserThanMin = lesservf (sphereCenter, negatevf (boxExtents));
		vectorf greaterThanMax = greatervf (sphereCenter, boxExtents);

		vectorf minDelta = sphereCenter + boxExtents;
		vectorf maxDelta = sphereCenter - boxExtents;

		vectorf d = selectvf (selectvf (vectorf (0.000f), minDelta, lesserThanMin), maxDelta, greaterThanMax);
		vectorf d2 = dotvf3d (d, d);

		vectorf sphereRadiusSquared = sphereRadius * sphereRadius;

		if (d2 > sphereRadiusSquared)
			return dseed::intersect_disjoint;

		vectorf min = sphereCenter - sphereRadius;
		vectorf max = sphereCenter + sphereRadius;

		return (inboundsvf3d (min, boxExtents) && inboundsvf3d (max, boxExtents))
			? dseed::intersect_contains
			: dseed::intersect_intersects;
	}
	inline intersect_t bounding_box::intersects (const bounding_frustum& bf) const
	{
		return bf.intersects (*this);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Bounding Sphere Type Implement
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline plane_intersect_t bounding_sphere::intersects (const plane& p) const
	{
		vectorf vCenter = center;
		vectorf vRadius = radius;

		vCenter = insertvf<0, 0, 0, 0, 1> (vCenter, vectorf (1));

		vectorf outside, inside;
		intersects_sphere_plane (vCenter, vRadius,
			(vectorf)(float4)p, &outside, &inside);

		if (outside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_front;

		if (inside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_back;

		return plane_intersect_intersecting;
	}
	inline intersect_t bounding_sphere::intersects (const bounding_box& bb) const
	{
		return bb.intersects (*this);
	}
	inline intersect_t bounding_sphere::intersects (const bounding_sphere& bs) const
	{
		vectorf center1 = center;
		vectorf center2 = bs.center;

		vectorf v = center2 - center1;
		float distance = lengthvf3d (v).x ();

		return (radius + bs.radius >= distance)
			? (radius - bs.radius >= distance ? intersect_contains : intersect_intersects)
			: intersect_disjoint;
	}
	inline intersect_t bounding_sphere::intersects (const bounding_frustum& bf) const
	{
		return bf.intersects (*this);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// Bounding Frustum Type Implement
	//
	////////////////////////////////////////////////////////////////////////////////////////
	inline bounding_frustum::bounding_frustum (const float4x4& projection)
	{
		static vectorf homogenousPoints[6] =
		{
			vectorf (+1.0f, +0.0f, +1.0f, +1.0f),
			vectorf (-1.0f, +0.0f, +1.0f, +1.0f),
			vectorf (+0.0f, +1.0f, +1.0f, +1.0f),
			vectorf (+0.0f, -1.0f, +1.0f, +1.0f),

			vectorf (+0.0f, +0.0f, +0.0f, +1.0f),
			vectorf (+0.0f, +0.0f, +1.0f, +1.0f)
		};

		matrixf matInverse = projection.invertmf ();
		vectorf points[6] = {
			transformvf4d (homogenousPoints[0], matInverse),
			transformvf4d (homogenousPoints[1], matInverse),
			transformvf4d (homogenousPoints[2], matInverse),
			transformvf4d (homogenousPoints[3], matInverse),

			transformvf4d (homogenousPoints[4], matInverse),
			transformvf4d (homogenousPoints[5], matInverse),
		};

		position = float3 (0.0f, 0.0f, 0.0f);
		orientation = float4 (0.0f, 0.0f, 0.0f, 1.0f);

		points[0] = points[0] * (vectorf (1) / points[0].splat_z ());
		points[1] = points[1] * (vectorf (1) / points[1].splat_z ());
		points[2] = points[2] * (vectorf (1) / points[2].splat_z ());
		points[3] = points[3] * (vectorf (1) / points[3].splat_z ());

		right = points[0].x ();
		left = points[1].x ();
		top = points[2].y ();
		bottom = points[3].y ();

		points[4] = points[4] * (vectorf (1) / points[4].splat_w ());
		points[5] = points[5] * (vectorf (1) / points[5].splat_w ());

		znear = points[4].z ();
		zfar = points[5].z ();
	}
	inline void bounding_frustum::generate_planes (plane* n, plane* f, plane* l, plane* r, plane* t, plane* b) const noexcept
	{
		vectorf vOrigin = position;
		vectorf vOrientation = orientation;

		*n = (float4)normalize_plane (transform_plane (vectorf (0.0f, 0.0f, -1.0f, znear), vOrientation, vOrigin));
		*f = (float4)normalize_plane (transform_plane (vectorf (0.0f, 0.0f, 1.0f, -zfar), vOrientation, vOrigin));
		*l = (float4)normalize_plane (transform_plane (vectorf (-1.0f, 0.0f, left, 0.0f), vOrientation, vOrigin));
		*r = (float4)normalize_plane (transform_plane (vectorf (1.0f, 0.0f, -right, 0.0f), vOrientation, vOrigin));
		*t = (float4)normalize_plane (transform_plane (vectorf (0.0f, 1.0f, -top, 0.0f), vOrientation, vOrigin));
		*b = (float4)normalize_plane (transform_plane (vectorf (0.0f, -1.0f, bottom, 0.0f), vOrientation, vOrigin));
	}
	inline plane_intersect_t bounding_frustum::intersects (const plane& p) const
	{
		vectorf vOrigin = position;
		vectorf vOrientation = orientation;

		vOrigin = insertvf<0, 0, 0, 0, 1> (vOrigin, vectorf (1));

		vectorf RightTop (right, top, 1.0f, 0.0f);
		vectorf RightBottom (right, bottom, 1.0f, 0.0f);
		vectorf LeftTop (left, top, 1.0f, 0.0f);
		vectorf LeftBottom (left, bottom, 1.0f, 0.0f);
		vectorf vNear = znear;
		vectorf vFar = zfar;

		RightTop = rotatevf3d (RightTop, vOrientation);
		RightBottom = rotatevf3d (RightBottom, vOrientation);
		LeftTop = rotatevf3d (LeftTop, vOrientation);
		LeftBottom = rotatevf3d (LeftBottom, vOrientation);

		vectorf Corners0 = fmavf (RightTop, vNear, vOrigin);
		vectorf Corners1 = fmavf (RightBottom, vNear, vOrigin);
		vectorf Corners2 = fmavf (LeftTop, vNear, vOrigin);
		vectorf Corners3 = fmavf (LeftBottom, vNear, vOrigin);
		vectorf Corners4 = fmavf (RightTop, vFar, vOrigin);
		vectorf Corners5 = fmavf (RightBottom, vFar, vOrigin);
		vectorf Corners6 = fmavf (LeftTop, vFar, vOrigin);
		vectorf Corners7 = fmavf (LeftBottom, vFar, vOrigin);

		vectorf outside, inside;
		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)p, &outside, &inside);

		if (outside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_front;

		if (inside == reinterpret_i32_to_f32 (vectori (-1)))
			return plane_intersect_back;

		return plane_intersect_intersecting;
	}
	inline intersect_t bounding_frustum::intersects (const bounding_box& bb) const
	{
		// TODO
		plane n, f, l, r, t, b;
		generate_planes (&n, &f, &l, &r, &t, &b);

		vectorf vCenter = bb.center;
		vectorf vExtents = bb.extents;
		vectorf BoxOrientation = bb.orientation;

		vCenter = insertvf<0, 0, 0, 0, 1> (vCenter, vectorf (1));

		matrixf R = to_matrixq (BoxOrientation);

		vectorf outside, inside;
		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)n, &outside, &inside);

		vectorf AnyOutside = outside;
		vectorf AllInside = inside;

		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)f, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)l, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)r, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)t, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_box_plane (vCenter, vExtents, R.column1, R.column2, R.column3, (vectorf)(float4)b, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		if (AnyOutside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_disjoint;

		if (AllInside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_contains;

		return intersect_intersects;
	}
	inline intersect_t bounding_frustum::intersects (const bounding_sphere& bs) const
	{
		plane n, f, l, r, t, b;
		generate_planes (&n, &f, &l, &r, &t, &b);

		vectorf vCenter = bs.center;
		vectorf vRadius = bs.radius;

		vCenter = insertvf<0, 0, 0, 0, 1> (vCenter, vectorf (1));

		vectorf outside, inside;
		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)n, &outside, &inside);

		vectorf AnyOutside = outside;
		vectorf AllInside = inside;

		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)f, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)l, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)r, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)t, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_sphere_plane (vCenter, vRadius, (vectorf)(float4)b, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		if (AnyOutside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_disjoint;

		if (AllInside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_contains;

		return intersect_intersects;
	}
	inline intersect_t bounding_frustum::intersects (const bounding_frustum& bf) const
	{
		plane n, f, l, r, t, b;
		generate_planes (&n, &f, &l, &r, &t, &b);

		vectorf vOrigin = bf.position;
		vectorf vOrientation = bf.orientation;

		vOrigin = insertvf<0, 0, 0, 0, 1> (vOrigin, vectorf (1));

		vectorf RightTop (right, top, 1.0f, 0.0f);
		vectorf RightBottom (right, bottom, 1.0f, 0.0f);
		vectorf LeftTop (left, top, 1.0f, 0.0f);
		vectorf LeftBottom (left, bottom, 1.0f, 0.0f);
		vectorf vNear = znear;
		vectorf vFar = zfar;

		RightTop = rotatevf3d (RightTop, vOrientation);
		RightBottom = rotatevf3d (RightBottom, vOrientation);
		LeftTop = rotatevf3d (LeftTop, vOrientation);
		LeftBottom = rotatevf3d (LeftBottom, vOrientation);

		vectorf Corners0 = fmavf (RightTop, vNear, vOrigin);
		vectorf Corners1 = fmavf (RightBottom, vNear, vOrigin);
		vectorf Corners2 = fmavf (LeftTop, vNear, vOrigin);
		vectorf Corners3 = fmavf (LeftBottom, vNear, vOrigin);
		vectorf Corners4 = fmavf (RightTop, vFar, vOrigin);
		vectorf Corners5 = fmavf (RightBottom, vFar, vOrigin);
		vectorf Corners6 = fmavf (LeftTop, vFar, vOrigin);
		vectorf Corners7 = fmavf (LeftBottom, vFar, vOrigin);

		vectorf outside, inside;
		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)n, &outside, &inside);

		vectorf AnyOutside = outside;
		vectorf AllInside = inside;

		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)f, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)l, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)r, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)t, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		intersects_frustum_plane (Corners0, Corners1, Corners2, Corners3,
			Corners4, Corners5, Corners6, Corners7,
			(vectorf)(float4)b, &outside, &inside);
		AnyOutside = orvf (AnyOutside, outside);
		AllInside = andvf (AllInside, inside);

		if (AnyOutside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_disjoint;

		if (AllInside == reinterpret_i32_to_f32 (vectori (-1)))
			return intersect_contains;

		return intersect_intersects;
	}
}

#endif