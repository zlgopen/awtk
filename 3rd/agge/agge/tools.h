#pragma once


namespace agge
{
	template <typename CoordT>
	struct point;

	template <typename CoordT>
	struct agge_vector;

	template <typename CoordT>
	struct rect;


	template <typename CoordT>
	inline point<CoordT> create_point(CoordT x, CoordT y)
	{
		point<CoordT> p = { x, y };
		return p;
	}

	template <typename CoordT>
	inline agge_vector<CoordT> create_vector(CoordT dx, CoordT dy)
	{
		agge_vector<CoordT> v = { dx, dy };
		return v;
	}

	template <typename CoordT>
	inline rect<CoordT> create_rect(CoordT x1, CoordT y1, CoordT x2, CoordT y2)
	{
		rect<CoordT> r = { x1, y1, x2, y2 };
		return r;
	}

	template <typename CoordT>
	inline CoordT width(const rect<CoordT> &rc)
	{	return rc.x2 - rc.x1;	}

	template <typename CoordT>
	inline CoordT height(const rect<CoordT> &rc)
	{	return rc.y2 - rc.y1;	}

	template <typename T>
	inline T agge_min(const T &lhs, const T &rhs)
	{	return lhs < rhs ? lhs : rhs;	}

	template <typename T>
	inline T agge_max(const T &lhs, const T &rhs)
	{	return lhs > rhs ? lhs : rhs;	}
}
