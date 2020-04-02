#include <agge/vector_rasterizer.h>

#include <agge/config.h>
#include <agge/precise_delta.h>

namespace agge
{
	namespace
	{
		typedef vector_rasterizer::cells_container::iterator cells_iterator;

		template <typename T>
		void update_min(T &value, T candidate)
		{
			if (candidate < value)
				value = candidate;
		}

		template <typename T>
		void update_max(T &value, T candidate)
		{
			if (candidate > value)
				value = candidate;
		}

		template <typename T>
		AGGE_INLINE cells_iterator resize_by(T &container, int by)
		{
			count_t n = container.size();

			container.resize(n + by);
			return container.begin() + n;
		}

		template <typename IteratorT>
		void sort_cells_x_ascending(IteratorT begin, IteratorT end)
		{
			// Yeah, bubblesort! 7th grade knowledge finally put to work! Seriously, if you append presorted cells and then
			//	sort them by Ys, you end up with almost sorted array.
			for (IteratorT i = end; i != begin; )
			{
				bool sorted = true;

				--i;
				for (IteratorT j = begin; j != i; )
				{
					IteratorT k = j++;

					if (k->x > j->x)
					{
						vector_rasterizer::cell t = *k;
						
						*k = *j;
						*j = t;
						sorted = false;
					}
				}
				if (sorted)
					break;
			}
		}

		void jump_xy(cells_iterator &current, int x, int y)
		{
			if ((current->x ^ x) | (current->y ^ y))
			{
				if (current->cover | current->area)
				{
					++current;
					current->area = current->cover = 0;
				}
				current->x = static_cast<short>(x), current->y = static_cast<short>(y);
			}
		}

		void add(cells_iterator current, int x1x2, int delta)
		{
			current->area += x1x2 * delta;
			current->cover += static_cast<short>(delta);
		}

		void seta(cells_iterator current, int x, int y, int area, int delta)
		{
			current->x = static_cast<short>(x);
			current->y = static_cast<short>(y);
			current->area = area;
			current->cover = static_cast<short>(delta);
		}

		void set(cells_iterator current, int x, int y, int x1x2, int delta)
		{
			seta(current, x, y, x1x2 * delta, delta);
		}

		void add_and_commit(cells_iterator &current, int x1x2, int delta)
		{
			int a = current->area + x1x2 * delta;
			int c = current->cover + delta;

			if (a | c)
			{
				current->area = a;
				current->cover = static_cast<short>(c);
				current++;
			}
		}
	}

	vector_rasterizer::vector_rasterizer()
	{	reset();	}

	void vector_rasterizer::reset()
	{
		const cell empty_cell = { };

		_cells.assign(1, empty_cell);
		_histogram_y.clear();
		_max_x = _max_y = -(_min_x = _min_y = 0x7FFF);
		_sorted = 0;
	}

	void vector_rasterizer::line(int x1, int y1, int x2, int y2)
	{
		const int dx = x2 - x1;
		const int ex1 = x1 >> _1_shift;
		int ey1 = y1 >> _1_shift;
		const int ex2 = x2 >> _1_shift;
		const int ey2 = y2 >> _1_shift;
		const int dy = y2 - y1;

		if (!dy) // Trivial case. Happens often.
			return;

		extend_bounds(ex1, ey1);
		extend_bounds(ex2, ey2);

		_sorted = 0;

		// Untested: we use top metric of cells required to draw the longest line given the current bounds.
		cells_container::iterator current = resize_by(_cells, 2 * agge_max(width(), height()) + 1) - 1;

		if (ey2 == ey1)
		{
			// Everything is on a single hline.

			if (ex2 == ex1)
			{
				jump_xy(current, ex1, ey1);
				add(current, (x1 & _1_mask) + (x2 & _1_mask), dy);
			}
			else
			{
				precise_delta tg_delta(dy, dx);

				hline(current, tg_delta, ey1, x1, x2, dy);
			}
		}
		else
		{
			const int fy1 = y1 & _1_mask;
			const int fy2 = y2 & _1_mask;
			const int step = dy > 0 ? +1 : -1;
			const int near_ = dy > 0 ? _1 : 0;
			const int far_ = _1 - near_;

			if (x2 == x1)
			{
				// Vertical line - we have to calculate start and end cells,
				// and then - the common values of the area and coverage for
				// all cells of the line. We know exactly there's only one 
				// cell, so, we don't have to call hline().

				const int two_fx = 2 * (x1 & _1_mask);

				jump_xy(current, ex1, ey1);
				add_and_commit(current, two_fx, near_ - fy1);
				ey1 += step;

				if (ey1 != ey2)
				{
					const int inner_delta = near_ - far_, inner_area = two_fx * inner_delta;

					do
					{
						seta(current++, ex1, ey1, inner_area, inner_delta);
						ey1 += step;
					} while (ey1 != ey2);
				}
				set(current, ex1, ey1, two_fx, fy2 - far_);
			}
			else
			{
				// Ok, we have to render several hlines.

				const int lift = near_ - fy1;
				precise_delta ctg_delta(dx, dy), tg_delta(dy, dx);

				ctg_delta.multiply(lift);

				int x_to = x1 + ctg_delta.next();

				if (lift)
					hline(current, tg_delta, ey1, x1, x_to, lift);
				ey1 += step;

				if (ey1 != ey2)
				{
					const int lift_full = near_ - far_;

					ctg_delta.multiply(lift_full);

					do
					{
						x1 = x_to;
						x_to += ctg_delta.next();
						hline(current, tg_delta, ey1, x1, x_to, lift_full);
						ey1 += step;
					} while (ey1 != ey2);
				}
				if (int dy_rest = fy2 - far_)
					hline(current, tg_delta, ey1, x_to, x2, dy_rest);
			}
		}
		_cells.set_end(++current);
	}

	void vector_rasterizer::append(const vector_rasterizer &source, int dx, int dy)
	{
		if (source.empty())
			return;

		const cell &last = *(_cells.end() - 1); // Relying on a guarantee that we had at least one cell prior this call.
		const int shift_back = !(last.area | last.cover);
		count_t n = source._cells.size();
		const_cells_iterator src = source._cells.begin();
		cells_container::iterator dst = resize_by(_cells, n - shift_back) - shift_back;

		_sorted = 0;

		while (n--)
		{
			dst->x = static_cast<short>(src->x + dx);
			dst->y = static_cast<short>(src->y + dy);
			dst->area = src->area;
			dst++->cover = src++->cover;
		}

		update_min(_min_x, source._min_x + dx);
		update_min(_min_y, source._min_y + dy);
		update_max(_max_x, source._max_x + dx);
		update_max(_max_y, source._max_y + dy);
	}

	void vector_rasterizer::sort(bool was_presorted)
	{
		if (_sorted || empty())
			return;

		_x_sorted_cells.resize(_cells.size());
		_histogram_y.assign(height() + 2, 0);

		count_t *phistogram_y = &_histogram_y[0] - _min_y + 2;

		if (was_presorted)
		{
			for (const_cells_iterator i = _cells.begin(), e = _cells.end(); i != e; ++i)
				phistogram_y[i->y]++;
			_cells.swap(_x_sorted_cells);
		}
		else
		{
			_histogram_x.assign(width() + 1, 0);

			count_t *phistogram_x = &_histogram_x[0] - _min_x + 1;

			for (const_cells_iterator i = _cells.begin(), e = _cells.end(); i != e; ++i)
			{
				phistogram_x[i->x]++;
				phistogram_y[i->y]++;
			}
			for (histogram::iterator i = _histogram_x.begin() + 1; i != _histogram_x.end(); ++i)
				*i += *(i - 1);
			phistogram_x--;
			for (cells_container::iterator i = _cells.begin(), j = _x_sorted_cells.begin(), e = _cells.end(); i != e; ++i)
				*(j + phistogram_x[i->x]++) = *i;
		}

		for (histogram::iterator i = _histogram_y.begin() + 2; i != _histogram_y.end(); ++i)
			*i += *(i - 1);
		phistogram_y--;
		for (cells_container::iterator i = _x_sorted_cells.begin(), j = _cells.begin(), e = _x_sorted_cells.end(); i != e; ++i)
			*(j + phistogram_y[i->y]++) = *i;

		if (was_presorted)
		{
			for (histogram::iterator i = _histogram_y.begin(); i != _histogram_y.end() - 1; ++i)
				sort_cells_x_ascending(_cells.begin() + *i, _cells.begin() + *(i + 1));
		}

		_sorted = 1;
	}

	AGGE_INLINE void vector_rasterizer::hline(cells_container::iterator &current, precise_delta &tg_delta, int ey, int x1, int x2, int dy)
	{
		int ex1 = x1 >> _1_shift;
		const int ex2 = x2 >> _1_shift;
		const int fx1 = x1 & _1_mask;
		const int fx2 = x2 & _1_mask;

		jump_xy(current, ex1, ey);

		if (ex1 == ex2)
		{
			// Everything is located in a single cell. That is easy!

			add(current, fx1 + fx2, dy);
		}
		else
		{
			// Ok, we'll have to render a run of adjacent cells on the same hline...

			const int step = x2 > x1 ? +1 : -1;
			const int near_ = x2 > x1 ? _1 : 0;
			const int far_ = _1 - near_;

			tg_delta.multiply(near_ - fx1);

			int y_to = tg_delta.next();

			add_and_commit(current, fx1 + near_, y_to);
			ex1 += step;

			if (ex1 != ex2)
			{
				tg_delta.multiply(near_ - far_);

				do
				{
					int d = tg_delta.next();

					y_to += d;
					if (d)
						set(current++, ex1, ey, _1, d);
					ex1 += step;
				} while (ex1 != ex2);
			}
			set(current, ex1, ey, fx2 + far_, dy - y_to);
		}
	}

	void vector_rasterizer::extend_bounds(int x, int y)
	{
		update_min(_min_x, x);
		update_min(_min_y, y);
		update_max(_max_x, x);
		update_max(_max_y, y);
	}
}
