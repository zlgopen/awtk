/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GRAPHIC_HELPER_H_
#define _GRAPHIC_HELPER_H_

#include "common.h"
#include "vertex.h"
#include "graphic_base.h"

namespace picasso {

inline scalar path_length(vertex_source& vs, unsigned int path_id = 0)
{
    scalar len = 0.0;
    scalar start_x = 0.0;
    scalar start_y = 0.0;
    scalar x1 = 0.0;
    scalar y1 = 0.0;
    scalar x2 = 0.0;
    scalar y2 = 0.0;
    bool first = true;

    unsigned int cmd;
    vs.rewind(path_id);
    while (!is_stop(cmd = vs.vertex(&x2, &y2))) {
        if (is_vertex(cmd)) {
            if (first || is_move_to(cmd)) {
                start_x = x2;
                start_y = y2;
            } else {
                len += calc_distance(x1, y1, x2, y2);
            }
            x1 = x2;
            y1 = y2;
            first = false;
        } else {
            if (is_close(cmd) && !first) {
                len += calc_distance(x1, y1, start_x, start_y);
            }
        }
    }
    return len;
}

inline bool bounding_rect(vertex_source& vs, unsigned int path_id, scalar* x1, scalar* y1, scalar* x2, scalar* y2)
{
    scalar x;
    scalar y;
    bool first = true;

    *x1 = INT_TO_SCALAR(1);
    *y1 = INT_TO_SCALAR(1);
    *x2 = INT_TO_SCALAR(0);
    *y2 = INT_TO_SCALAR(0);

    vs.rewind(path_id);
    unsigned int cmd;
    while (!is_stop(cmd = vs.vertex(&x, &y))) {
        if (is_vertex(cmd)) {
            if (first) {
                *x1 = x; *y1 = y;
                *x2 = x; *y2 = y;
                first = false;
            } else {
                if (x < *x1) *x1 = x;
                if (y < *y1) *y1 = y;
                if (x > *x2) *x2 = x;
                if (y > *y2) *y2 = y;
            }
        }
    }
    return *x1 <= *x2 && *y1 <= *y2;
}

class bitset_iterator
{
public:
    bitset_iterator(const byte* bits, unsigned int offset = 0)
        : m_bits(bits + (offset >> 3))
        , m_mask(0x80 >> (offset & 7))
    {
    }

    void operator ++ ()
    {
        m_mask >>= 1;
        if (m_mask == 0) {
            ++m_bits;
            m_mask = 0x80;
        }
    }

    unsigned int bit(void) const
    {
        return (*m_bits) & m_mask;
    }

private:
    const byte* m_bits;
    byte        m_mask;
};

}
#endif /*_GRAPHIC_HELPER_H_*/

