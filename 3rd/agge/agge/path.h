#pragma once

#include "types.h"

namespace agge
{
	enum path_commands {
		path_command_stop = 0x00,
		path_command_move_to = 0x01,
		path_command_line_to = 0x02,
		
		path_command_end_poly = 0x10,

		path_vertex_mask = 0x07,
		path_command_mask = 0x1F
	};

	enum path_flags {
		path_flag_close = 0x20,
		path_flags_mask = 0xE0
	};


	template <typename SourceT, typename GeneratorT>
	class path_generator_adapter
	{
	public:
		path_generator_adapter(const SourceT &source, GeneratorT &generator);

		void rewind(int /*path_id*/) { /*not implemented*/ }
		int vertex(real_t *x, real_t *y);

	private:
		enum state { initial = 0, accumulate = 1, generate = 2, stage_mask = 3, complete = 4 };

	private:
		const path_generator_adapter &operator =(const path_generator_adapter &rhs);

		void set_stage(state stage, bool force_complete = false);

	private:
		SourceT _source;
		GeneratorT &_generator;
		real_t _start_x, _start_y;
		int _state;
	};


	template <typename PathIterator1T, typename PathIterator2T>
	class joined_path
	{
	public:
		joined_path(const PathIterator1T &path1, const PathIterator2T &path2);

		void rewind(unsigned id);
		int vertex(real_t *x, real_t *y);

	private:
		enum state { first_initial, first, second };

	private:
		PathIterator1T _path1;
		PathIterator2T _path2;
		state _state;
	};



	template <typename SourceT, typename GeneratorT>
	path_generator_adapter<SourceT, GeneratorT> assist(const SourceT &source, GeneratorT &generator)
	{	return path_generator_adapter<SourceT, GeneratorT>(source, generator);	}

	template <typename PathIterator1T, typename PathIterator2T>
	joined_path<PathIterator1T, PathIterator2T> join(const PathIterator1T &path1, const PathIterator2T &path2)
	{	return joined_path<PathIterator1T, PathIterator2T>(path1, path2);	}

	inline bool is_vertex(int c)
	{	return 0 != (path_vertex_mask & c);	}

	inline bool is_end_poly(int c)
	{	return path_command_end_poly == (path_command_mask & c);	}

	inline bool is_close(int c)
	{	return 0 != (path_flag_close & c);	}

	template <typename AcceptorT>
	inline void add_polyline_vertex(AcceptorT &acceptor, real_t x, real_t y, int command)
	{
		if (path_command_move_to == (path_vertex_mask & command))
			acceptor.move_to(x, y);
		else if (path_command_line_to == (path_vertex_mask & command))
			acceptor.line_to(x, y);
		
		if (is_close(command))
			acceptor.close_polygon();
	}

	template <typename SinkT, typename PathIteratorT>
	inline void add_path(SinkT &sink, PathIteratorT path)
	{
		real_t x, y;

		path.rewind(0);
		for (int command; command = path.vertex(&x, &y), path_command_stop != command; )
			add_polyline_vertex(sink, x, y, command);
	}


	template <typename SourceT, typename GeneratorT>
	inline path_generator_adapter<SourceT, GeneratorT>::path_generator_adapter(const SourceT &source, GeneratorT &generator)
		: _source(source), _generator(generator), _state(initial)
	{	}

	template <typename SourceT, typename GeneratorT>
	inline int path_generator_adapter<SourceT, GeneratorT>::vertex(real_t *x, real_t *y)
	{
		int command;

		for (;;)
			switch (_state & stage_mask)
			{
			case initial:
				command = _source.vertex(&_start_x, &_start_y);
				set_stage(accumulate, path_command_stop == command);

			case accumulate:
				if (_state & complete)
					return path_command_stop;

				_generator.remove_all();
				_generator.add_vertex(_start_x, _start_y, path_command_move_to);

				for (;;)
				{
					real_t xx, yy;

					command = _source.vertex(&xx, &yy);
					if (path_command_move_to == command)
					{
						_start_x = xx;
						_start_y = yy;
					}
					else if (path_command_stop != command)
					{
						_generator.add_vertex(xx, yy, command);
						continue;
					}
					break;
				}
				set_stage(generate, path_command_stop == command);

			case generate:
				command = _generator.vertex(x, y);
				if (path_command_stop != command)
					return command;
				set_stage(accumulate);
			}
	}

	template <typename SourceT, typename GeneratorT>
	inline void path_generator_adapter<SourceT, GeneratorT>::set_stage(state stage, bool force_complete)
	{	_state = (stage & stage_mask) | (force_complete ? complete : (_state & complete));	}


	template <typename PathIterator1T, typename PathIterator2T>
	inline joined_path<PathIterator1T, PathIterator2T>::joined_path(const PathIterator1T &path1, const PathIterator2T &path2)
		: _path1(path1), _path2(path2), _state(first_initial)
	{	}

	template <typename PathIterator1T, typename PathIterator2T>
	inline void joined_path<PathIterator1T, PathIterator2T>::rewind(unsigned /*id*/)
	{
		_state = first_initial;
		_path1.rewind(0);
		_path2.rewind(0);
	}

	template <typename PathIterator1T, typename PathIterator2T>
	inline int joined_path<PathIterator1T, PathIterator2T>::vertex(real_t *x, real_t *y)
	{
		int command;

		switch (_state)
		{
		case first_initial:
			command = _path1.vertex(x, y);
			if (command == path_command_stop)
				_state = second;
			else
				return _state = first, command;

		case second:
			return _path2.vertex(x, y);

		case first:
			command = _path1.vertex(x, y);
			if (command != path_command_stop)
				return command;
			_state = second;
			command = _path2.vertex(x, y);
			return command == path_command_move_to ? path_command_line_to : command;
		}
		return path_command_stop;
	}
}
