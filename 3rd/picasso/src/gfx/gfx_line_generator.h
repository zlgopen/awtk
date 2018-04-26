/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_LINE_GENERATOR_H_
#define _GFX_LINE_GENERATOR_H_

#include "common.h"

namespace gfx {

// dda_line_interpolator
template <int FractionShift, int YShift = 0>
class gfx_dda_line_interpolator
{
public:
    gfx_dda_line_interpolator()
        : m_y(0)
        , m_inc(0)
        , m_dy(0)
    {
    }

    gfx_dda_line_interpolator(int y1, int y2, unsigned int count)
        : m_y(y1)
        , m_inc(((y2 - y1) << FractionShift) / (int)count)
        , m_dy(0)
    {
    }

    void operator ++ ()
    {
        m_dy += m_inc;
    }

    void operator -- ()
    {
        m_dy -= m_inc;
    }

    void operator += (unsigned int n)
    {
        m_dy += m_inc * n;
    }

    void operator -= (unsigned int n)
    {
        m_dy -= m_inc * n;
    }

    int y(void) const { return m_y + (m_dy >> (FractionShift-YShift)); }
    int dy(void) const { return m_dy; }

private:
    int m_y;
    int m_inc;
    int m_dy;
};

// dda2 line interpolator
class gfx_dda2_line_interpolator
{
public:
    typedef int save_data_type;
    enum {
        save_size = 2,
    };

    gfx_dda2_line_interpolator()
        : m_cnt(0), m_lft(0), m_rem(0), m_mod(0), m_y(0)
    {
    }

    // forward-adjusted line
    gfx_dda2_line_interpolator(int y1, int y2, int count)
        : m_cnt(count <= 0 ? 1 : count)
        , m_lft((y2 - y1) / m_cnt)
        , m_rem((y2 - y1) % m_cnt)
        , m_mod(m_rem)
        , m_y(y1)
    {
        if (m_mod <= 0) {
            m_mod += count;
            m_rem += count;
            m_lft--;
        }
        m_mod -= count;
    }

    // backward-adjusted line
    gfx_dda2_line_interpolator(int y1, int y2, int count, int)
        : m_cnt(count <= 0 ? 1 : count)
        , m_lft((y2 - y1) / m_cnt)
        , m_rem((y2 - y1) % m_cnt)
        , m_mod(m_rem)
        , m_y(y1)
    {
        if (m_mod <= 0) {
            m_mod += count;
            m_rem += count;
            m_lft--;
        }
    }

    // backward-adjusted line
    gfx_dda2_line_interpolator(int y, int count)
        : m_cnt(count <= 0 ? 1 : count)
        , m_lft(y / m_cnt)
        , m_rem(y % m_cnt)
        , m_mod(m_rem)
        , m_y(0)
    {
        if (m_mod <= 0) {
            m_mod += count;
            m_rem += count;
            m_lft--;
        }
    }

    void save(save_data_type* data) const
    {
        data[0] = m_mod;
        data[1] = m_y;
    }

    void load(const save_data_type* data)
    {
        m_mod = data[0];
        m_y = data[1];
    }

    void operator++()
    {
        m_mod += m_rem;
        m_y += m_lft;
        if (m_mod > 0) {
            m_mod -= m_cnt;
            m_y++;
        }
    }

    void operator--()
    {
        if (m_mod <= m_rem) {
            m_mod += m_cnt;
            m_y--;
        }
        m_mod -= m_rem;
        m_y -= m_lft;
    }

    void adjust_forward(void)
    {
        m_mod -= m_cnt;
    }

    void adjust_backward(void)
    {
        m_mod += m_cnt;
    }

    int mod(void) const { return m_mod; }
    int rem(void) const { return m_rem; }
    int lft(void) const { return m_lft; }
    int y(void) const { return m_y; }
private:
    int m_cnt;
    int m_lft;
    int m_rem;
    int m_mod;
    int m_y;
};

}
#endif /*_GFX_LINE_GENERATOR_H_*/
