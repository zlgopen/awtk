#pragma once

#include "pod_vector.h"
#include "types.h"

namespace agge
{
	class precise_delta;

	class vector_rasterizer
	{
	public:
		enum
		{
			_1_shift = 8,

			_1 = 1 << _1_shift,
			_1_mask = _1 - 1,
		};

#pragma pack(push, 1)
		struct cell
		{
			short x, y;
			int area;
			short cover;
		};
#pragma pack(pop)

		struct scanline_cells;

		typedef pod_vector<cell> cells_container;
		typedef cells_container::const_iterator const_cells_iterator;

	public:
		vector_rasterizer();

		void reset();
		void clear_cache();

		void line(int x1, int y1, int x2, int y2);
		void append(const vector_rasterizer &source, int dx, int dy);
		const cells_container &cells() const;
		void sort(bool was_presorted = false);
		bool empty() const;
		bool sorted() const;

		scanline_cells operator [](int y) const;
		int width() const;
		int min_y() const;
		int height() const;

	private:
		typedef pod_vector<count_t> histogram;

	private:
		void hline(cells_container::iterator &current, precise_delta &tg_delta, int ey, int x1, int x2, int dy);
		void extend_bounds(int x, int y);

	private:
		cells_container _cells;
		histogram _histogram_y, _histogram_x;
		cells_container _x_sorted_cells;
		int _min_y, _min_x, _max_x, _max_y, _sorted;
	};

	struct vector_rasterizer::scanline_cells
	{
		vector_rasterizer::const_cells_iterator first;
		vector_rasterizer::const_cells_iterator second;
	};



	inline const vector_rasterizer::cells_container &vector_rasterizer::cells() const
	{	return _cells;	}

	inline bool vector_rasterizer::empty() const
	{	return _min_y > _max_y;	}

	inline void vector_rasterizer::clear_cache()
	{	_cells.clear_cache();	}

	inline bool vector_rasterizer::sorted() const
	{	return !!_sorted;	}

	inline vector_rasterizer::scanline_cells vector_rasterizer::operator [](int y) const
	{
		histogram::const_iterator offset = _histogram_y.begin() + y - _min_y;
		const const_cells_iterator start = _cells.begin();
		const scanline_cells sc = { start + *offset++, start + *offset };

		return sc;
	}

	inline int vector_rasterizer::width() const
	{	return empty() ? 0 : _max_x - _min_x + 1;	}

	inline int vector_rasterizer::min_y() const
	{	return _min_y;	}

	inline int vector_rasterizer::height() const
	{	return empty() ? 0 : _max_y - _min_y + 1;	}
}
