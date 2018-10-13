#include <agge/stroke.h>

#include <agge/math.h>
#include <agge/path.h>
#include <agge/stroke_features.h>

namespace agge
{
	stroke::stroke()
		: _o(_output.end()), _cap(0), _join(0), _state(0)
	{	}

	stroke::~stroke()
	{
		delete _cap;
		delete _join;
	}

	void stroke::remove_all()
	{
		clear();
		_state = 0;
	}

	void stroke::close_polygon()
	{
		vertex_sequence::close_polygon();
		if (!empty())
			_state |= closed;
	}

	void stroke::add_vertex(real_t x, real_t y, int command)
	{	add_polyline_vertex(*this, x, y, command);	}
		
	int stroke::vertex(real_t *x, real_t *y)
	{
		for ( ; prepare(); )
		{
			if (_o != _output.end())
			{
				*x = _o->x;
				*y = _o->y;
				++_o;
				int command = _state & moveto ? path_command_move_to : path_command_line_to;
				_state &= ~moveto;
				return command;
			}

			_output.clear();

			const vertex_sequence::const_iterator first = begin();
			const vertex_sequence::const_iterator last = end() - 1;
			const vertex_sequence::const_iterator prev = _i == first ? last : _i - 1;
			const vertex_sequence::const_iterator next = _i == last ? first : _i + 1;

			switch (_state & stage_mask)
			{
			case start_cap:
				_cap->calc(_output, _width, _i->point, _i->distance, next->point);
				_i = next;
				set_state(_i == last ? end_cap : outline_forward);
				break;

			case outline_forward:
				_join->calc(_output, _width, prev->point, prev->distance, _i->point, _i->distance, next->point);
				_i = next;
				if (_i == last)
					set_state(end_cap);
				break;

			case outline_forward_closed:
				_join->calc(_output, _width, prev->point, prev->distance, _i->point, _i->distance, next->point);
				_i = next;
				if (_i == first)
					set_state(end_poly1);
				break;

			case end_poly1:
				_o = _output.end();
				set_state(outline_backward | moveto);
				return path_command_end_poly | path_flag_close;

			case end_cap:
				_cap->calc(_output, _width, _i->point, prev->distance, prev->point);
				_i = prev;
				set_state(_i == first ? end_poly : outline_backward);
				break;

			case outline_backward:
				_join->calc(_output, _width, next->point, _i->distance, _i->point, prev->distance, prev->point);
				_i = prev;
				if (_i == first)
					set_state(end_poly);
				break;

			case end_poly:
				_o = _output.end();
				set_state(stop);
				return path_command_end_poly | path_flag_close;

			case stop:
				return path_command_stop;
			}

			_o = _output.begin();
		}
		return path_command_stop;
	}
		
	void stroke::width(real_t w)
	{	_width = 0.5f * w;	}

	bool stroke::prepare()
	{
		if (_state & ready)
			return true;

		if (size() <= (_state & closed ? 2u : 1u))
			return false;

		_i = begin();
		_o = _output.end();
		set_state((_state & closed ? outline_forward_closed : start_cap) | moveto | ready);
		return true;
	}

	void stroke::set_state(int stage_and_flags)
	{	_state = (_state & ~stage_mask) | stage_and_flags;	}
}
