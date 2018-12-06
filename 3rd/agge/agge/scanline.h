#pragma once

#include "config.h"
#include "memory.h"

namespace agge
{
	template <typename RendererT>
	class scanline_adapter : noncopyable
	{
	public:
		typedef typename RendererT::cover_type cover_type;

	public:
		scanline_adapter(RendererT &renderer_, raw_memory_object &covers_buffer, count_t max_length);

		bool begin(int y);
		void add_cell(int x, cover_type cover);
		void add_span(int x, unsigned int length, cover_type cover);
		void commit(int next_x = 0);

	private:
		RendererT &_renderer;
		cover_type *_cover;
		int _x, _start_x;
		cover_type * const _start_cover;
	};



	template <typename RendererT>
	inline scanline_adapter<RendererT>::scanline_adapter(RendererT &renderer_, raw_memory_object &covers, count_t max_length)
		: _renderer(renderer_), _x(0), _start_x(0), _start_cover(covers.get<cover_type>(max_length + 16) + 4)
	{	_cover = _start_cover;	}

	template <typename RendererT>
	inline bool scanline_adapter<RendererT>::begin(int y)
	{	return _renderer.set_y(y);	}

	template <typename RendererT>
	AGGE_INLINE void scanline_adapter<RendererT>::add_cell(int x, cover_type cover)
	{
		if (x != _x)
			commit(x);
		++_x;
		*_cover++ = cover;
	}

	template <typename RendererT>
	AGGE_INLINE void scanline_adapter<RendererT>::add_span(int x, count_t length, cover_type cover)
	{
		if (x != _x)
			commit(x);

		cover_type *p = _cover;

		_x += length;
		_cover += length;
		memset(p, cover, length);		
	}

	template <typename RendererT>
	AGGE_INLINE void scanline_adapter<RendererT>::commit(int next_x)
	{
		//*reinterpret_cast<int *>(_cover) = 0;
		*_cover = 0;
		_renderer(_start_x, _x - _start_x, _start_cover);
		_start_x = _x = next_x;
		_cover = _start_cover;
	}
}
