#pragma once

#include "types.h"

namespace agge
{
	extern const real_t distance_epsilon;
	extern const real_t pi;

	template <typename T>
	struct limits
	{
		static T resolution();
	};

	real_t sqrt(real_t x);
	real_t sin(real_t a);
	real_t cos(real_t a);

	inline int iround(real_t v)
	{	return static_cast<int>(v > real_t() ? v + static_cast<real_t>(0.5) : v - static_cast<real_t>(0.5));	}

	inline int muldiv(int a, int b, int c)
	{	return static_cast<int>(static_cast<long long>(a) * b / c);	}

	inline real_t muldiv(real_t a, real_t b, real_t c)
	{	return a * b / c;	}

	inline real_t distance(real_t ax, real_t ay, real_t bx, real_t by)
	{
		bx -= ax;
		by -= ay;
		return sqrt(bx * bx + by * by);
	}

	inline real_t distance(const point_r &lhs, const point_r &rhs)
	{	return distance(lhs.x, lhs.y, rhs.x, rhs.y);	}

	template <typename CoordT>
	inline CoordT distance(const point<CoordT> &a, const point<CoordT> &b)
	{	return distance(a.x, a.y, b.x, b.y);	}

	template <typename CoordT>
	inline point<CoordT> operator +(const point<CoordT> &lhs, const agge_vector<CoordT> &rhs)
	{
		const point<CoordT> result = { lhs.x + rhs.dx, lhs.y + rhs.dy };
		return result;
	}

	template <typename CoordT>
	inline agge_vector<CoordT> operator -(const point<CoordT> &lhs, const point<CoordT> &rhs)
	{
		const agge_vector<CoordT> result = { lhs.x - rhs.x, lhs.y - rhs.y };
		return result;
	}

	template <typename CoordT>
	inline agge_vector<CoordT> operator *(CoordT lhs, const agge_vector<CoordT> &rhs)
	{
		const agge_vector<CoordT> result = { lhs * rhs.dx, lhs * rhs.dy };
		return result;
	}

	template <typename CoordT>
	inline agge_vector<CoordT> operator *(const agge_vector<CoordT> &lhs, CoordT rhs)
	{
		const agge_vector<CoordT> result = { rhs * lhs.dx, rhs * lhs.dy };
		return result;
	}
}
