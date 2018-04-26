/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_SCANLINE_H_
#define _GFX_SCANLINE_H_

#include "common.h"
#include "data_vector.h"

namespace gfx {

// scanline bin container
class gfx_scanline_bin
{
public:
    typedef int32_t coord_type;

    typedef struct {
        int16_t x;
        int16_t len;
    } span;

    typedef const span* const_iterator;

    gfx_scanline_bin()
        : m_last_x(0x7FFFFFF0)
        , m_y(0)
        , m_cur_span(0)
    {
    }

    void reset(int min_x, int max_x)
    {
        unsigned int max_len = max_x - min_x + 3;
        if (max_len > m_spans.size()) {
            m_spans.resize(max_len);
        }
        m_last_x = 0x7FFFFFF0;
        m_cur_span = &m_spans[0];
    }

    void add_cell(int x, unsigned int)
    {
        if (x == m_last_x+1) {
            m_cur_span->len++;
        } else {
            ++m_cur_span;
            m_cur_span->x = (int16_t)x;
            m_cur_span->len = 1;
        }
        m_last_x = x;
    }

    void add_span(int x, unsigned int len, unsigned int)
    {
        if (x == m_last_x+1) {
            m_cur_span->len = (int16_t)(m_cur_span->len + len);
        } else {
            ++m_cur_span;
            m_cur_span->x = (int16_t)x;
            m_cur_span->len = (int16_t)len;
        }
        m_last_x = x + len - 1;
    }

    void add_cells(int x, unsigned len, const void*)
    {
        add_span(x, len, 0);
    }

    void finalize(int y)
    {
        m_y = y;
    }

    void reset_spans(void)
    {
        m_last_x = 0x7FFFFFF0;
        m_cur_span = &m_spans[0];
    }

    int y(void) const { return m_y; }
    unsigned int num_spans(void) const { return (unsigned int)(m_cur_span - &m_spans[0]); }
    const_iterator begin(void) const { return &m_spans[1]; }

private:
    gfx_scanline_bin(const gfx_scanline_bin&);
    const gfx_scanline_bin operator = (const gfx_scanline_bin&);

    int m_last_x;
    int m_y;
    span* m_cur_span;
    pod_array<span> m_spans;
};


// scanline 8bit packed container
class gfx_scanline_p8
{
public:
    typedef uint8_t cover_type;
    typedef int16_t coord_type;

    typedef struct {
        coord_type x;
        coord_type len; // If negative, it's a solid span, covers is valid
        const cover_type* covers;
    } span;

    typedef span* iterator;
    typedef const span* const_iterator;

    gfx_scanline_p8()
        : m_last_x(0x7FFFFFF0)
        , m_y(0)
        , m_cover_ptr(0)
        , m_cur_span(0)
    {
    }

    void reset(int min_x, int max_x)
    {
        unsigned int max_len = max_x - min_x + 3;
        if (max_len > m_spans.size()) {
            m_spans.resize(max_len);
            m_covers.resize(max_len);
        }
        m_last_x = 0x7FFFFFF0;
        m_cover_ptr = &m_covers[0];
        m_cur_span = &m_spans[0];
        m_cur_span->len = 0;
    }

    void add_cell(int x, unsigned int cover)
    {
        *m_cover_ptr = (cover_type)cover;
        if (x == m_last_x+1 && m_cur_span->len > 0) {
            m_cur_span->len++;
        } else {
            m_cur_span++;
            m_cur_span->covers = m_cover_ptr;
            m_cur_span->x = (int16_t)x;
            m_cur_span->len = 1;
        }
        m_last_x = x;
        m_cover_ptr++;
    }

    void add_cells(int x, unsigned int len, const cover_type* covers)
    {
        mem_copy(m_cover_ptr, covers, len * sizeof(cover_type));
        if (x == m_last_x+1 && m_cur_span->len > 0) {
            m_cur_span->len += (int16_t)len;
        } else {
            m_cur_span++;
            m_cur_span->covers = m_cover_ptr;
            m_cur_span->x = (int16_t)x;
            m_cur_span->len = (int16_t)len;
        }
        m_cover_ptr += len;
        m_last_x = x + len - 1;
    }

    void add_span(int x, unsigned int len, unsigned int cover)
    {
        if (x == m_last_x+1 && m_cur_span->len < 0
            && cover == *m_cur_span->covers)
        {
            m_cur_span->len -= (int16_t)len;
        } else {
            *m_cover_ptr = (cover_type)cover;
            m_cur_span++;
            m_cur_span->covers = m_cover_ptr++;
            m_cur_span->x = (int16_t)x;
            m_cur_span->len = (int16_t)(-int(len));
        }
        m_last_x = x + len - 1;
    }

    void finalize(int y)
    {
        m_y = y;
    }

    void reset_spans(void)
    {
        m_last_x = 0x7FFFFFF0;
        m_cover_ptr = &m_covers[0];
        m_cur_span = &m_spans[0];
        m_cur_span->len = 0;
    }

    int y(void) const { return m_y; }
    unsigned int num_spans(void) const { return (unsigned int)(m_cur_span - &m_spans[0]); }
    const_iterator begin(void) const { return &m_spans[1]; }

private:
    gfx_scanline_p8(const gfx_scanline_p8&);
    const gfx_scanline_p8& operator = (const gfx_scanline_p8&);

    int m_last_x;
    int m_y;
    cover_type* m_cover_ptr;
    pod_array<cover_type> m_covers;
    span* m_cur_span;
    pod_array<span> m_spans;
};


// scanline unpacked container
class gfx_scanline_u8
{
public:
    typedef uint8_t cover_type;
    typedef int16_t coord_type;

    typedef struct {
        coord_type x;
        coord_type len;
        cover_type* covers;
    } span;

    typedef span* iterator;
    typedef const span* const_iterator;

    gfx_scanline_u8()
        : m_min_x(0)
        , m_last_x(0x7FFFFFF0)
        , m_y(0)
        , m_cur_span(0)
    {
    }

    void reset(int min_x, int max_x)
    {
        unsigned int max_len = max_x - min_x + 2;
        if (max_len > m_spans.size()) {
            m_spans.resize(max_len);
            m_covers.resize(max_len);
        }
        m_last_x = 0x7FFFFFF0;
        m_min_x = min_x;
        m_cur_span = &m_spans[0];
    }

    void add_cell(int x, unsigned int cover)
    {
        x -= m_min_x;
        m_covers[x] = (cover_type)cover;
        if (x == m_last_x+1) {
            m_cur_span->len++;
        } else {
            m_cur_span++;
            m_cur_span->x = (coord_type)(x + m_min_x);
            m_cur_span->len = 1;
            m_cur_span->covers = &m_covers[x];
        }
        m_last_x = x;
    }

    void add_cells(int x, unsigned int len, const cover_type* covers)
    {
        x -= m_min_x;
        mem_copy(&m_covers[x], covers, len * sizeof(cover_type));
        if (x == m_last_x+1) {
            m_cur_span->len += (coord_type)len;
        } else {
            m_cur_span++;
            m_cur_span->x = (coord_type)(x + m_min_x);
            m_cur_span->len = (coord_type)len;
            m_cur_span->covers = &m_covers[x];
        }
        m_last_x = x + len - 1;
    }

    void add_span(int x, unsigned int len, unsigned int cover)
    {
        x -= m_min_x;
        memset(&m_covers[x], cover, len);
        if (x == m_last_x+1) {
            m_cur_span->len += (coord_type)len;
        } else {
            m_cur_span++;
            m_cur_span->x = (coord_type)(x + m_min_x);
            m_cur_span->len = (coord_type)len;
            m_cur_span->covers = &m_covers[x];
        }
        m_last_x = x + len - 1;
    }

    void finalize(int y)
    {
        m_y = y;
    }

    void reset_spans(void)
    {
        m_last_x = 0x7FFFFFF0;
        m_cur_span = &m_spans[0];
    }

    int y(void) const { return m_y; }
    unsigned int num_spans(void) const { return (unsigned int)(m_cur_span - &m_spans[0]); }
    const_iterator begin(void) const { return &m_spans[1]; }
    iterator begin(void) { return &m_spans[1]; }

private:
    gfx_scanline_u8(const gfx_scanline_u8&);
    const gfx_scanline_u8& operator = (const gfx_scanline_u8&);

    int m_min_x;
    int m_last_x;
    int m_y;
    pod_array<cover_type> m_covers;
    span* m_cur_span;
    pod_array<span> m_spans;
};

}
#endif /*_GFX_SCANLINE_H_*/
