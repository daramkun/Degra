#ifndef __DSEED_GEOMETRY_H__
#define __DSEED_GEOMETRY_H__

namespace dseed
{
	struct DSEEDEXP point2i
	{
	public:
		int32_t x, y;

	public:
		point2i () = default;
		point2i (int32_t x, int32_t y);
	};

	struct DSEEDEXP point2f
	{
	public:
		float x, y;

	public:
		point2f () = default;
		point2f (float x, float y);
	};

	struct DSEEDEXP size2i
	{
	public:
		int32_t width, height;

	public:
		size2i () = default;
		size2i (int32_t width, int32_t height);
	};

	struct DSEEDEXP size2f
	{
	public:
		float width, height;

	public:
		size2f () = default;
		size2f (float width, float height);
	};

	struct DSEEDEXP point3i
	{
	public:
		int32_t x, y, z;

	public:
		point3i () = default;
		point3i (int32_t x, int32_t y, int32_t z);
	};

	struct DSEEDEXP point3f
	{
	public:
		float x, y, z;

	public:
		point3f () = default;
		point3f (float x, float y, float z);
	};

	struct DSEEDEXP size3i
	{
	public:
		int32_t width, height, depth;

	public:
		size3i () = default;
		size3i (int32_t width, int32_t height, int32_t depth);
	};

	struct DSEEDEXP size3f
	{
	public:
		float width, height, depth;

	public:
		size3f () = default;
		size3f (float width, float height, float depth);
	};

	struct DSEEDEXP rectangle
	{
	public:
		int32_t x, y, width, height;

	public:
		rectangle () = default;
		rectangle (int32_t x, int32_t y, int32_t width, int32_t height);
		rectangle (const point2i& p, const size2i& s);

	public:
		bool intersects (const rectangle& rect);
	};

	struct DSEEDEXP rectanglef
	{
	public:
		float x, y, width, height;

	public:
		rectanglef () = default;
		rectanglef (float x, float y, float width, float height);
		rectanglef (const point2f& p, const size2f& s);

	public:
		bool intersects (const rectanglef& rect);
	};

	struct DSEEDEXP circle
	{
	public:
		int32_t x, y, radius;

	public:
		circle () = default;
		circle (int32_t x, int32_t y, int32_t radius);

	public:
		bool intersects (const circle& circle);
	};

	struct DSEEDEXP circlef
	{
	public:
		float x, y, radius;

	public:
		circlef () = default;
		circlef (float x, float y, float radius);

	public:
		bool intersects (const circlef& circle);
	};
}

#endif