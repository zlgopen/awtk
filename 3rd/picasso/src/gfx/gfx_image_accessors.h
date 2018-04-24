/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_IMAGE_ACCESSORS_H_
#define _GFX_IMAGE_ACCESSORS_H_

#include "common.h"

namespace gfx {

// image_accessor
template <typename PixFmt> class image_accessor
{
public:
    typedef typename PixFmt::color_type color_type;
    typedef typename PixFmt::order_type order_type;
    typedef typename PixFmt::value_type value_type;
    typedef typename PixFmt::pixel_type pixel_type;

    enum { 
        pix_width = PixFmt::pix_width
    };

    explicit image_accessor(const PixFmt& pixf)
        : m_x(0), m_y(0), m_tx(0)
        , m_pixf(&pixf) 
        , m_pix_ptr(0)
    {
    }

public:
    const byte* span(int x, int y, unsigned int len)
    {
        m_x = m_tx = x;
        m_y = y;

        if ((y >= 0) && (y < (int)m_pixf->height()) &&
            (x >= 0) && ((x+(int)len) <= (int)m_pixf->width()))
        {
            return m_pix_ptr = m_pixf->pix_ptr(x, y);
        }

        m_pix_ptr = 0;
        return pixel();
    }

    const byte* next_x(void)
    {
        if (m_pix_ptr)
            return m_pix_ptr += pix_width;

        ++m_x;
        return pixel();
    }

    const byte* next_y(void)
    {
        ++m_y;
        m_x = m_tx;

        if (m_pix_ptr && (m_y >= 0) && (m_y < (int)m_pixf->height())) {
            return m_pix_ptr = m_pixf->pix_ptr(m_x, m_y);
        }

        m_pix_ptr = 0;
        return pixel();
    }

private:
    image_accessor(const image_accessor&);
    image_accessor& operator=(const image_accessor&);

    const byte* pixel(void) const
    {
        register int x = m_x;
        register int y = m_y;

        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (x >= (int)m_pixf->width())
            x = m_pixf->width() - 1;
        if (y >= (int)m_pixf->height())
            y = m_pixf->height() - 1;

        return m_pixf->pix_ptr(x, y);
    }

private:
    int m_x;
    int m_y;
    int m_tx;
    const PixFmt* m_pixf;
    const byte* m_pix_ptr;
};


// image_accessor_wrap
template<class PixFmt, class WrapX, class WrapY> class image_accessor_wrap
{
public:
    typedef typename PixFmt::color_type color_type;
    typedef typename PixFmt::order_type order_type;
    typedef typename PixFmt::value_type value_type;
    typedef typename PixFmt::pixel_type pixel_type;

    enum { 
        pix_width = PixFmt::pix_width 
    };

    explicit image_accessor_wrap(const PixFmt& pixf)
        : m_pixf(&pixf)
        , m_row_ptr(0) 
        , m_x(0)
        , m_wrap_x(pixf.width())
        , m_wrap_y(pixf.height())
    {
    }

public:
    const byte* span(int x, int y, unsigned int)
    {
        m_x = x;
        m_row_ptr = m_pixf->row_ptr(m_wrap_y(y));
        return m_row_ptr + m_wrap_x(x) * pix_width;
    }

    const byte* next_x(void)
    {
        int x = ++m_wrap_x;
        return m_row_ptr + x * pix_width;
    }

    const byte* next_y(void)
    {
        m_row_ptr = m_pixf->row_ptr(++m_wrap_y);
        return m_row_ptr + m_wrap_x(m_x) * pix_width;
    }
private:
    image_accessor_wrap(const image_accessor_wrap&);
    image_accessor_wrap& operator=(const image_accessor_wrap&);

private:
    const PixFmt* m_pixf;
    const byte* m_row_ptr;
    int m_x;
    WrapX m_wrap_x;
    WrapY m_wrap_y;
};


// wrap_mode_repeat
class wrap_mode_repeat
{
public:
    wrap_mode_repeat(unsigned int size)
        : m_size(size)
        , m_add(size * (0x3FFFFFFF / size))
        , m_value(0)
    {
    }

    unsigned int operator()(int v)
    { 
        return m_value = ((unsigned int)v + m_add) % m_size; 
    }

    unsigned int operator++()
    {
        ++m_value;
        if (m_value >= m_size)
            m_value = 0;
        return m_value;
    }
private:
    unsigned int m_size;
    unsigned int m_add;
    unsigned int m_value;
};


// wrap_mode_reflect
class wrap_mode_reflect
{
public:
    wrap_mode_reflect(unsigned int size)
        : m_size(size)
        , m_size2(size << 1)
        , m_add(m_size2 * (0x3FFFFFFF / m_size2))
        , m_value(0)
    {
    }

    unsigned int operator()(int v)
    { 
        m_value = ((unsigned int)v + m_add) % m_size2;
        if (m_value >= m_size)
            return m_size2 - m_value - 1;
        return m_value;
    }

    unsigned int operator++()
    {
        ++m_value;
        if (m_value >= m_size2)
            m_value = 0;
        if (m_value >= m_size)
            return m_size2 - m_value - 1;
        return m_value;
    }
private:
    unsigned int m_size;
    unsigned int m_size2;
    unsigned int m_add;
    unsigned int m_value;
};

}
#endif /*_GFX_IMAGE_ACCESSORS_H_*/
