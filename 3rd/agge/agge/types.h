#pragma once

namespace agge
{
	typedef float real_t;

	template <typename T>
	struct point;

	template <typename T>
	struct agge_vector;

	template <typename T>
	struct rect;

	template <typename T>
	struct box;

	typedef unsigned int count_t;
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef point<real_t> point_r;
	typedef agge_vector<real_t> vector_r;
	typedef rect<int> rect_i;
	typedef box<real_t> box_r;

	template <typename T>
	struct point
	{
		T x, y;
	};

	template <typename T>
	struct agge_vector
	{
		T dx, dy;
	};

	template <typename T>
	struct rect
	{
		T x1, y1, x2, y2;
	};

	template <typename T>
	struct box
	{
		T w, h;
	};

	class noncopyable
	{
	public:
		noncopyable() throw() { }

	private:
		noncopyable(const noncopyable &other);
		const noncopyable &operator =(const noncopyable &rhs);
	};
}
