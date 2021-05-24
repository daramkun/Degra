#ifndef __DSEED_MATH_GEOMETRY_H__
#define __DSEED_MATH_GEOMETRY_H__

namespace dseed
{
	struct DSEEDEXP point2i
	{
		int x, y;
		inline point2i() noexcept = default;
		inline point2i(int x, int y) noexcept
			: x(x), y(y)
		{ }
	};
	inline bool operator== (const point2i& p1, const point2i& p2) noexcept { return p1.x == p2.x && p1.y == p2.y; }
	inline bool operator!= (const point2i& p1, const point2i& p2) noexcept { return p1.x != p2.x || p1.y != p2.y; }

	struct DSEEDEXP point2f
	{
		float x, y;
		inline point2f() noexcept = default;
		inline point2f(float x, float y) noexcept
			: x(x), y(y)
		{ }
	};
	inline bool operator== (const point2f& p1, const point2f& p2) noexcept { return p1.x == p2.x && p1.y == p2.y; }
	inline bool operator!= (const point2f& p1, const point2f& p2) noexcept { return p1.x != p2.x || p1.y != p2.y; }

	struct DSEEDEXP point3i
	{
		int x, y, z;
		inline point3i() noexcept = default;
		inline point3i(int x, int y, int z) noexcept
			: x(x), y(y), z(z)
		{ }
	};
	inline bool operator== (const point3i& p1, const point3i& p2) noexcept { return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z; }
	inline bool operator!= (const point3i& p1, const point3i& p2) noexcept { return p1.x != p2.x || p1.y != p2.y || p1.z != p2.z; }

	struct DSEEDEXP point3f
	{
		float x, y, z;
		inline point3f() noexcept = default;
		inline point3f(float x, float y, float z) noexcept
			: x(x), y(y), z(z)
		{ }
	};
	inline bool operator== (const point3f& p1, const point3f& p2) noexcept { return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z; }
	inline bool operator!= (const point3f& p1, const point3f& p2) noexcept { return p1.x != p2.x || p1.y != p2.y || p1.z != p2.z; }

	struct DSEEDEXP size2i
	{
		int width, height;
		inline size2i() noexcept = default;
		inline size2i(int width, int height) noexcept
			: width(width), height(height)
		{ }
	};
	inline bool operator== (const size2i& s1, const size2i& s2) noexcept { return s1.width == s2.width && s1.height == s2.height; }
	inline bool operator!= (const size2i& s1, const size2i& s2) noexcept { return s1.width != s2.width || s1.height != s2.height; }

	struct DSEEDEXP size2f
	{
		float width, height;
		inline size2f() noexcept = default;
		inline size2f(float width, float height) noexcept
			: width(width), height(height)
		{ }
	};
	inline bool operator== (const size2f& s1, const size2f& s2) noexcept { return s1.width == s2.width && s1.height == s2.height; }
	inline bool operator!= (const size2f& s1, const size2f& s2) noexcept { return s1.width != s2.width || s1.height != s2.height; }

	struct DSEEDEXP size3i
	{
		int width, height, depth;
		inline size3i() noexcept = default;
		inline size3i(int width, int height, int depth) noexcept
			: width(width), height(height), depth(depth)
		{ }
	};
	inline bool operator== (const size3i& s1, const size3i& s2) noexcept { return s1.width == s2.width && s1.height == s2.height && s1.depth == s2.depth; }
	inline bool operator!= (const size3i& s1, const size3i& s2) noexcept { return s1.width != s2.width || s1.height != s2.height || s1.depth != s2.depth; }

	struct DSEEDEXP size3f
	{
		float width, height, depth;
		inline size3f() noexcept = default;
		inline size3f(float width, float height, float depth) noexcept
			: width(width), height(height), depth(depth)
		{ }
	};
	inline bool operator== (const size3f& s1, const size3f& s2) noexcept { return s1.width == s2.width && s1.height == s2.height && s1.depth == s2.depth; }
	inline bool operator!= (const size3f& s1, const size3f& s2) noexcept { return s1.width != s2.width || s1.height != s2.height || s1.depth != s2.depth; }

	struct DSEEDEXP rect2i : public point2i, public size2i
	{
		inline rect2i() noexcept = default;
		inline rect2i(int x, int y, int width, int height) noexcept
			: point2i(x, y), size2i(width, height)
		{ }
		inline rect2i(const point2i& pos, const size2i& size) noexcept
			: point2i(pos), size2i(size)
		{ }
	};
	inline bool operator== (const rect2i& r1, const rect2i& r2) noexcept { return (const point2i&)r1 == (const point2i&)r2 && (const size2i&)r1 == (const size2i&)r2; }
	inline bool operator!= (const rect2i& r1, const rect2i& r2) noexcept { return (const point2i&)r1 != (const point2i&)r2 || (const size2i&)r1 != (const size2i&)r2; }

	struct DSEEDEXP rect2f : public point2f, public size2f
	{
		inline rect2f() noexcept = default;
		inline rect2f(float x, float y, float width, float height) noexcept
			: point2f(x, y), size2f(width, height)
		{ }
		inline rect2f(const point2f& pos, const size2f& size) noexcept
			: point2f(pos), size2f(size)
		{ }
	};
	inline bool operator== (const rect2f& r1, const rect2f& r2) noexcept { return (const point2f&)r1 == (const point2f&)r2 && (const size2f&)r1 == (const size2f&)r2; }
	inline bool operator!= (const rect2f& r1, const rect2f& r2) noexcept { return (const point2f&)r1 != (const point2f&)r2 || (const size2f&)r1 != (const size2f&)r2; }

	struct DSEEDEXP box3i : public point3i, public size3i
	{
		inline box3i() noexcept = default;
		inline box3i(int x, int y, int z, int width, int height, int depth) noexcept
			: point3i(x, y, z), size3i(width, height, depth)
		{ }
		inline box3i(const point3i& pos, const size3i& size) noexcept
			: point3i(pos), size3i(size)
		{ }
	};
	inline bool operator== (const box3i& b1, const box3i& b2) noexcept { return (const point3i&)b1 == (const point3i&)b2 && (const size3i&)b1 == (const size3i&)b2; }
	inline bool operator!= (const box3i& b1, const box3i& b2) noexcept { return (const point3i&)b1 != (const point3i&)b2 || (const size3i&)b1 != (const size3i&)b2; }

	struct DSEEDEXP box3f : public point3f, public size3f
	{
		inline box3f() noexcept = default;
		inline box3f(float x, float y, float z, float width, float height, float depth) noexcept
			: point3f(x, y, z), size3f(width, height, depth)
		{ }
		inline box3f(const point3f& pos, const size3f& size) noexcept
			: point3f(pos), size3f(size)
		{ }
	};
	inline bool operator== (const box3f& b1, const box3f& b2) noexcept { return (const point3f&)b1 == (const point3f&)b2 && (const size3f&)b1 == (const size3f&)b2; }
	inline bool operator!= (const box3f& b1, const box3f& b2) noexcept { return (const point3f&)b1 != (const point3f&)b2 || (const size3f&)b1 != (const size3f&)b2; }

	struct DSEEDEXP circle2i : public point2i
	{
		int radius;
		inline circle2i() noexcept = default;
		inline circle2i(int x, int y, int radius) noexcept : point2i(x, y), radius(radius) { }
		inline circle2i(const point2i& pos, int radius) noexcept : point2i(pos), radius(radius) { }
	};
	inline bool operator== (const circle2i& c1, const circle2i& c2) noexcept { return (const point2i&)c1 == (const point2i&)c2 && c1.radius == c2.radius; }
	inline bool operator!= (const circle2i& c1, const circle2i& c2) noexcept { return (const point2i&)c1 != (const point2i&)c2 || c1.radius != c2.radius; }

	struct DSEEDEXP circle2f : public point2f
	{
		float radius;
		inline circle2f() noexcept = default;
		inline circle2f(float x, float y, float radius) noexcept : point2f(x, y), radius(radius) { }
		inline circle2f(const point2f& pos, float radius) noexcept : point2f(pos), radius(radius) { }
	};
	inline bool operator== (const circle2f& c1, const circle2f& c2) noexcept { return (const point2f&)c1 == (const point2f&)c2 && c1.radius == c2.radius; }
	inline bool operator!= (const circle2f& c1, const circle2f& c2) noexcept { return (const point2f&)c1 != (const point2f&)c2 || c1.radius != c2.radius; }

	struct DSEEDEXP sphere3i : public point3i
	{
		int radius;
		inline sphere3i() noexcept = default;
		inline sphere3i(int x, int y, int z, int radius) noexcept : point3i(x, y, z), radius(radius) { }
		inline sphere3i(const point3i& pos, int radius) noexcept : point3i(pos), radius(radius) { }
	};
	inline bool operator== (const sphere3i& s1, const sphere3i& s2) noexcept { return (const point3i&)s1 == (const point3i&)s2 && s1.radius == s2.radius; }
	inline bool operator!= (const sphere3i& s1, const sphere3i& s2) noexcept { return (const point3i&)s1 != (const point3i&)s2 || s1.radius != s2.radius; }

	struct DSEEDEXP sphere3f : public point3f
	{
		float radius;
		inline sphere3f() noexcept = default;
		inline sphere3f(float x, float y, float z, float radius) noexcept : point3f(x, y, z), radius(radius) { }
		inline sphere3f(const point3f& pos, float radius) noexcept : point3f(pos), radius(radius) { }
	};
	inline bool operator== (const sphere3f& s1, const sphere3f& s2) noexcept { return (const point3f&)s1 == (const point3f&)s2 && s1.radius == s2.radius; }
	inline bool operator!= (const sphere3f& s1, const sphere3f& s2) noexcept { return (const point3f&)s1 != (const point3f&)s2 || s1.radius != s2.radius; }
}

#endif