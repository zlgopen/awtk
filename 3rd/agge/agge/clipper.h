#pragma once

#include "math.h"
#include "types.h"

namespace agge
{
	enum clipping_flags {
		x1_clipped_shift = 2,
		x2_clipped_shift = 0,
		y1_clipped_shift = 3,
		y2_clipped_shift = 1,

		x1_clipped = 1 << x1_clipped_shift,
		x2_clipped = 1 << x2_clipped_shift,
		y1_clipped = 1 << y1_clipped_shift,
		y2_clipped = 1 << y2_clipped_shift,
		x_clipped = x1_clipped | x2_clipped,
		y_clipped = y1_clipped | y2_clipped
	};


	template <typename T>
	class clipper
	{
	public:
		typedef T coord_type;

	public:
		clipper();

		void reset();
		void set(const rect<T> &window);

		void move_to(T x, T y);

		template <typename LinesSinkT>
		void line_to(LinesSinkT &sink, T x, T y);

	private:
		template <typename LinesSinkT>
		void line_clip_y(LinesSinkT &sink, T x1, T y1, T x2, T y2, int f1, int f2) const;

	private:
		rect<T> _window;
		T _x1, _y1;
		int _f1;
		bool _enabled;
	};


	
	template <typename T>
	inline int clipping_y(T y, const rect<T> &window)
	{	return ((y < window.y1) << y1_clipped_shift) | ((y > window.y2) << y2_clipped_shift);	}

	template <typename T>
	inline int clipping(T x, T y, const rect<T> &window)
	{	return ((x < window.x1) << x1_clipped_shift) | ((x > window.x2) << x2_clipped_shift) | clipping_y(y, window);	}


	template <typename T>
	inline clipper<T>::clipper()
		: _enabled(false)
	{	
	  this->_x1 = 0;
	  this->_y1 = 0;
	  this->_f1 = 0;

	  this->_window.x1 = 0;
	  this->_window.x2 = 0;
	  this->_window.y1 = 0;
	  this->_window.y2 = 0;
	}

	template <typename T>
	inline void clipper<T>::reset()
	{	_enabled = false;	}

	template <typename T>
	inline void clipper<T>::set(const rect<T> &window)
	{
		_window = window;
		_enabled = true;
		_f1 = clipping(_x1, _y1, _window);
	}

	template <typename T>
	inline void clipper<T>::move_to(T x, T y)
	{
		_x1 = x;
		_y1 = y;
		_f1 = clipping(x, y, _window);
	}

	template <typename T>
	template <typename LinesSinkT>
	inline void clipper<T>::line_to(LinesSinkT &sink, T x2, T y2)
	{
		if (_enabled)
		{
			const int f2 = clipping(x2, y2, _window);
			int f3, f4;
			T y3, y4;

			switch ((_f1 & x_clipped) | (f2 & x_clipped) << 1)
			{
			case 0 | 0:
				line_clip_y(sink, _x1, _y1, x2, y2, _f1, f2);
				break;

			case 0 | x2_clipped << 1:
				y3 = _y1 + muldiv(_window.x2 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				line_clip_y(sink, _x1, _y1, _window.x2, y3, _f1, f3);
				line_clip_y(sink, _window.x2, y3, _window.x2, y2, f3, f2);
				break;

			case x2_clipped | 0:
				y3 = _y1 + muldiv(_window.x2 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				line_clip_y(sink, _window.x2, _y1, _window.x2, y3, _f1, f3);
				line_clip_y(sink, _window.x2, y3, x2, y2, f3, f2);
				break;

			case x2_clipped | x2_clipped << 1:
				line_clip_y(sink, _window.x2, _y1, _window.x2, y2, _f1, f2);
				break;

			case 0 | x1_clipped << 1:
				y3 = _y1 + muldiv(_window.x1 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				line_clip_y(sink, _x1, _y1, _window.x1, y3, _f1, f3);
				line_clip_y(sink, _window.x1, y3, _window.x1, y2, f3, f2);
				break;

			case x2_clipped | x1_clipped << 1:
				y3 = _y1 + muldiv(_window.x2 - _x1, y2 - _y1, x2 - _x1);
				y4 = _y1 + muldiv(_window.x1 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				f4 = clipping_y(y4, _window);
				line_clip_y(sink, _window.x2, _y1, _window.x2, y3, _f1, f3);
				line_clip_y(sink, _window.x2, y3, _window.x1, y4, f3, f4);
				line_clip_y(sink, _window.x1, y4, _window.x1, y2, f4, f2);
				break;

			case x1_clipped | 0:
				y3 = _y1 + muldiv(_window.x1 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				line_clip_y(sink, _window.x1, _y1, _window.x1, y3, _f1, f3);
				line_clip_y(sink, _window.x1, y3, x2, y2, f3, f2);
				break;

			case x1_clipped | x2_clipped << 1:
				y3 = _y1 + muldiv(_window.x1 - _x1, y2 - _y1, x2 - _x1);
				y4 = _y1 + muldiv(_window.x2 - _x1, y2 - _y1, x2 - _x1);
				f3 = clipping_y(y3, _window);
				f4 = clipping_y(y4, _window);
				line_clip_y(sink, _window.x1, _y1, _window.x1, y3, _f1, f3);
				line_clip_y(sink, _window.x1, y3, _window.x2, y4, f3, f4);
				line_clip_y(sink, _window.x2, y4, _window.x2, y2, f4, f2);
				break;

			case x1_clipped | x1_clipped << 1:
				line_clip_y(sink, _window.x1, _y1, _window.x1, y2, _f1, f2);
				break;
			}
			_f1 = f2;
		}
		else
		{
			sink.line(_x1, _y1, x2, y2);
		}
		_x1 = x2;
		_y1 = y2;
	}

	template <typename T>
	template <typename LinesSinkT>
	inline void clipper<T>::line_clip_y(LinesSinkT &sink, T x1, T y1, T x2, T y2, int f1, int f2) const
	{
		f1 &= y_clipped;
		f2 &= y_clipped;

		if (f1 | f2)
		{
			if (f1 == f2)
				return;

			coord_type tx1 = x1;
			coord_type ty1 = y1;
			coord_type tx2 = x2;
			coord_type ty2 = y2;

			if (f1 & y1_clipped)
			{
				tx1 = x1 + muldiv(_window.y1 - y1, x2 - x1, y2 - y1);
				ty1 = _window.y1;
			}

			if (f1 & y2_clipped)
			{
				tx1 = x1 + muldiv(_window.y2 - y1, x2 - x1, y2 - y1);
				ty1 = _window.y2;
			}

			if (f2 & y1_clipped)
			{
				tx2 = x1 + muldiv(_window.y1 - y1, x2 - x1, y2 - y1);
				ty2 = _window.y1;
			}

			if (f2 & y2_clipped)
			{
				tx2 = x1 + muldiv(_window.y2 - y1, x2 - x1, y2 - y1);
				ty2 = _window.y2;
			}
			sink.line(tx1, ty1, tx2, ty2); 
		}
		else
		{
			sink.line(x1, y1, x2, y2);
		}
	}
}
