#pragma once

#include "math.h"
#include "pod_vector.h"

namespace agge
{
	struct vertex
	{
		point_r point;
		real_t distance;
	};

	class vertex_sequence : pod_vector<vertex>
	{
	public:
		using pod_vector<vertex>::const_iterator;
		using pod_vector<vertex>::iterator;

	public:
		void move_to(real_t x, real_t y);
		void line_to(real_t x, real_t y);
		void close_polygon();

		using pod_vector<vertex>::clear;
		using pod_vector<vertex>::size;
		using pod_vector<vertex>::begin;
		using pod_vector<vertex>::end;
		using pod_vector<vertex>::empty;

	private:
		static bool set_distance(vertex &v, const point_r &next);
	};

	inline void vertex_sequence::move_to(real_t x, real_t y)
	{
		vertex v = { { x, y } };

		push_back(v);
	}

	inline void vertex_sequence::line_to(real_t x, real_t y)
	{
		vertex v = { { x, y } };

		if (empty())
		{
			push_back(v);
		}
		else
		{
			vertex &last = *(end() - 1);

			if (set_distance(last, v.point))
				push_back(v);
		}
	}

	inline void vertex_sequence::close_polygon()
	{
		if (!empty() && !set_distance(*(end() - 1), begin()->point))
			pop_back();
	}

	inline bool vertex_sequence::set_distance(vertex &v, const point_r &next)
	{
		v.distance = distance(v.point, next);
		return v.distance > distance_epsilon;
	}
}
