/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _VERTEX_DIST_H_
#define _VERTEX_DIST_H_

#include "math_type.h"
#include "graphic_base.h"
#include "data_vector.h"

namespace picasso {

struct vertex_dist
{
    scalar x;
    scalar y;
    scalar d;

    vertex_dist() : x(0), y(0), d(FLT_TO_SCALAR(0.0f)) { }
    vertex_dist(scalar _x, scalar _y) : x(_x), y(_y), d(FLT_TO_SCALAR(0.0f)) { }

    bool operator() (const vertex_dist& t)
    {
        bool ret = (d = calc_distance(x, y, t.x, t.y)) > vertex_dist_epsilon;
        if (!ret)
            d = FLT_TO_SCALAR(1.0f) / vertex_dist_epsilon;
        return ret;
    }
};

// vertex_sequence
template <typename T, unsigned int S = 6>
class vertex_sequence : public pod_bvector<T, S>
{
public:
    typedef pod_bvector<T, S> base_type;

    void add(const T& val)
    {
        if (base_type::size() > 1) {
            if (!(*this)[base_type::size() - 2]((*this)[base_type::size() - 1])) {
                base_type::remove_last();
            }
        }
        base_type::add(val);
    }

    void modify_last(const T& val)
    {
        base_type::remove_last();
        add(val);
    }

    void close(bool closed)
    {
        while(base_type::size() > 1) {
            if ((*this)[base_type::size() - 2]((*this)[base_type::size() - 1]))
                break;

            T t = (*this)[base_type::size() - 1];
            base_type::remove_last();
            modify_last(t);
        }

        if (closed) {
            while(base_type::size() > 1) {
                if ((*this)[base_type::size() - 1]((*this)[0]))
                    break;

                base_type::remove_last();
            }
        }
    }
};

// shorten_path
template<class VertexSequence>
void shorten_path(VertexSequence& vs, scalar s, unsigned int closed = 0)
{
    typedef typename VertexSequence::value_type vertex_type;

    if ((s > FLT_TO_SCALAR(0.0f)) && (vs.size() > 1)) {
        scalar d;
        int n = (int)(vs.size() - 2);

        while (n) {
            d = vs[n].d;
            if (d > s)
                break;

            vs.remove_last();
            s -= d;
            --n;
        }

        if (vs.size() < 2) {
            vs.clear();
        } else {
            n = vs.size() - 1;
            vertex_type& prev = vs[n-1];
            vertex_type& last = vs[n];
            d = (prev.d - s) / prev.d;
            scalar x = prev.x + (last.x - prev.x) * d;
            scalar y = prev.y + (last.y - prev.y) * d;
            last.x = x;
            last.y = y;

            if (!prev(last))
                vs.remove_last();

            vs.close(closed != 0);
        }
    }
}

}
#endif /*_VERTEX_DIST_H_*/
