#pragma once

#include "vertex_sequence.h"

namespace agge
{
	typedef pod_vector<point_r> points;

	class stroke : vertex_sequence, noncopyable
	{
	public:
		struct cap;
		struct join;

	public:
		stroke();
		~stroke();

		// Vertex population
		void remove_all();
		using vertex_sequence::move_to;
		using vertex_sequence::line_to;
		void close_polygon();
		void add_vertex(real_t x, real_t y, int command);
		
		// Vertex access
		int vertex(real_t *x, real_t *y);
		
		// Setup
		void width(real_t w);

		template <typename CapT>
		void set_cap(const CapT &c);

		template <typename JoinT>
		void set_join(const JoinT &j);

	private:
		enum state {
			// Stages
			start_cap = 0x00,
			outline_forward = 0x01,
			outline_forward_closed = 0x02,
			end_poly1 = 0x03,
			end_cap = 0x04,
			outline_backward = 0x05,
			end_poly = 0x06,
			stop = 0x07,
			stage_mask = 0x07,

			// Flags
			closed = 0x10,
			moveto = 0x20,
			ready = 0x40
		};

	private:
		bool prepare();
		void set_state(int stage_and_flags);

	private:
		points _output;
		vertex_sequence::const_iterator _i;
		points::const_iterator _o;
		const cap *_cap;
		const join *_join;
		real_t _width;
		int _state;
	};

	struct stroke::cap
	{
		virtual ~cap() { }
		virtual void calc(points &output, real_t w, const point_r &v0, real_t d, const point_r &v1) const = 0;
	};

	struct stroke::join
	{
		virtual ~join() { }
		virtual void calc(points &output, real_t w, const point_r &v0, real_t d01, const point_r &v1, real_t d12, const point_r &v2) const = 0;
	};



	template <typename CapT>
	inline void stroke::set_cap(const CapT &c)
	{
		const CapT *replacement = new CapT(c);
		
		delete _cap;
		_cap = replacement;
	}

	template <typename JoinT>
	inline void stroke::set_join(const JoinT &j)
	{
		const JoinT *replacement = new JoinT(j);
		
		delete _join;
		_join = replacement;
	}
}
