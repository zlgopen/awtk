/* Picasso - a vector graphics library
 *
 * Copyright (C) 2012 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_MASK_LAYER_H_
#define _GFX_MASK_LAYER_H_

#include "common.h"
#include "interfaces.h"

#include "picasso_mask.h"

#include "gfx_rendering_buffer.h"

namespace gfx {

// alpha mask
class gfx_alpha_mask_u8
{
public:
    typedef uint8_t cover_type;
    enum {
        cover_shift = 8,
        cover_none  = 0,
        cover_full  = 255,
    };

    enum { // gray8 color
        step = 1,
        offset = 0,
    };

    gfx_alpha_mask_u8()
        : m_buffer(0)
    {
    }

    explicit gfx_alpha_mask_u8(gfx_rendering_buffer& buffer)
        : m_buffer(&buffer)
    {
    }

    void attach(gfx_rendering_buffer& buffer) { m_buffer = &buffer; }

    cover_type pixel(int x, int y) const
    {
        if (x >= 0 && y >= 0 &&
            x < (int)m_buffer->internal_width() &&
            y < (int)m_buffer->internal_height())
        {
            return (cover_type)calculate(m_buffer->row_ptr(y) + x * step + offset);
        }
        return 0;
    }

    cover_type combine_pixel(int x, int y, cover_type val) const
    {
        if (x >= 0 && y >= 0 &&
            x < (int)m_buffer->internal_width() &&
            y < (int)m_buffer->internal_height())
        {
            return (cover_type)((cover_full + val * calculate(
                                 m_buffer->row_ptr(y) + x * step + offset)) >> cover_shift);
        }
        return 0;
    }

    void fill_hspan(int x, int y, cover_type* dst, int num_pix) const
    {
        int xmax = m_buffer->internal_width() - 1;
        int ymax = m_buffer->internal_height() - 1;

        int count = num_pix;
        cover_type* covers = dst;

        if (y < 0 || y > ymax) {
            memset(dst, 0, num_pix * sizeof(cover_type));
            return;
        }

        if (x < 0) {
            count += x;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers, 0, -x * sizeof(cover_type));
            covers -= x;
            x = 0;
        }

        if (x + count > xmax) {
            int rest = x + count - xmax - 1;
            count -= rest;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers + count, 0, rest * sizeof(cover_type));
        }

        const uint8_t* mask = m_buffer->row_ptr(y) + x * step + offset;
        do {
            *covers++ = (cover_type)calculate(mask);
            mask += step;
        } while(--count);
    }

    void combine_hspan(int x, int y, cover_type* dst, int num_pix) const
    {
        int xmax = m_buffer->internal_width() - 1;
        int ymax = m_buffer->internal_height() - 1;

        int count = num_pix;
        cover_type* covers = dst;

        if (y < 0 || y > ymax) {
            memset(dst, 0, num_pix * sizeof(cover_type));
            return;
        }

        if (x < 0) {
            count += x;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers, 0, -x * sizeof(cover_type));
            covers -= x;
            x = 0;
        }

        if (x + count > xmax) {
            int rest = x + count - xmax - 1;
            count -= rest;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers + count, 0, rest * sizeof(cover_type));
        }

        const uint8_t* mask = m_buffer->row_ptr(y) + x * step + offset;
        do {
            *covers = (cover_type)((cover_full + (*covers) *
                                   calculate(mask)) >> cover_shift);
            ++covers;
            mask += step;
        } while(--count);
    }

    void fill_vspan(int x, int y, cover_type* dst, int num_pix) const
    {
        int xmax = m_buffer->internal_width() - 1;
        int ymax = m_buffer->internal_height() - 1;

        int count = num_pix;
        cover_type* covers = dst;

        if (x < 0 || x > xmax) {
            memset(dst, 0, num_pix * sizeof(cover_type));
            return;
        }

        if (y < 0) {
            count += y;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers, 0, -y * sizeof(cover_type));
            covers -= y;
            y = 0;
        }

        if (y + count > ymax) {
            int rest = y + count - ymax - 1;
            count -= rest;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers + count, 0, rest * sizeof(cover_type));
        }

        const uint8_t* mask = m_buffer->row_ptr(y) + x * step + offset;
        do {
            *covers++ = (cover_type)calculate(mask);
            mask += m_buffer->internal_stride();
        } while(--count);
    }

    void combine_vspan(int x, int y, cover_type* dst, int num_pix) const
    {
        int xmax = m_buffer->internal_width() - 1;
        int ymax = m_buffer->internal_height() - 1;

        int count = num_pix;
        cover_type* covers = dst;

        if (x < 0 || x > xmax) {
            memset(dst, 0, num_pix * sizeof(cover_type));
            return;
        }

        if (y < 0) {
            count += y;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers, 0, -y * sizeof(cover_type));
            covers -= y;
            y = 0;
        }

        if (y + count > ymax) {
            int rest = y + count - ymax - 1;
            count -= rest;
            if (count <= 0) {
                memset(dst, 0, num_pix * sizeof(cover_type));
                return;
            }
            memset(covers + count, 0, rest * sizeof(cover_type));
        }

        const uint8_t* mask = m_buffer->row_ptr(y) + x * step + offset;
        do {
            *covers = (cover_type)((cover_full + (*covers) *
                                   calculate(mask)) >> cover_shift);
            ++covers;
            mask += m_buffer->internal_stride();
        } while(--count);
    }

private:
    gfx_alpha_mask_u8(const gfx_alpha_mask_u8&);
    const gfx_alpha_mask_u8& operator=(const gfx_alpha_mask_u8&);
    unsigned int calculate(const uint8_t* p) const { return *p; }

    gfx_rendering_buffer* m_buffer;
};

// pixfmt alpha mask adaptor
template <typename PixFmt, typename AlphaMask>
class gfx_pixfmt_amask_adaptor
{
public:
    typedef PixFmt pixfmt_type;
    typedef typename pixfmt_type::color_type color_type;
    typedef typename pixfmt_type::row_data row_data;
    typedef AlphaMask amask_type;
    typedef typename amask_type::cover_type cover_type;

    enum {
        span_extra_tail = 1<<8,
    };

private:
    void realloc_span(unsigned int len)
    {
        if (len > m_span.size())
            m_span.resize(len + span_extra_tail);
    }

    void init_span(unsigned int len)
    {
        realloc_span(len);
        memset(&m_span[0], amask_type::cover_full, len * sizeof(cover_type));
    }

    void init_span(unsigned int len, const cover_type* covers)
    {
        realloc_span(len);
        mem_copy(&m_span[0], covers, len * sizeof(cover_type));
    }

public:
    gfx_pixfmt_amask_adaptor(pixfmt_type& pixfmt, const amask_type& mask)
        : m_pixfmt(&pixfmt)
        , m_mask(&mask)
    {
    }

    void attach_pixfmt(pixfmt_type& pixfmt) { m_pixfmt = &pixfmt; }
    void attach_alpha_mask(const amask_type& mask) { m_mask = &mask; }

    unsigned int width(void) const { return m_pixfmt->width();  }
    unsigned int height(void) const { return m_pixfmt->height(); }

    color_type pixel(int x, int y)
    {
        return m_pixfmt->pixel(x, y);
    }

    void copy_pixel(int x, int y, const color_type& c)
    {
        m_pixfmt->blend_pixel(x, y, c, m_mask->pixel(x, y));
    }

    void blend_pixel(int x, int y, const color_type& c, cover_type cover)
    {
        m_pixfmt->blend_pixel(x, y, c, m_mask->combine_pixel(x, y, cover));
    }

    void copy_hline(int x, int y, unsigned int len, const color_type& c)
    {
        realloc_span(len);
        m_mask->fill_hspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_hspan(x, y, len, c, &m_span[0]);
    }

    void blend_hline(int x, int y, unsigned int len,
                     const color_type& c, cover_type cover)
    {
        init_span(len);
        m_mask->combine_hspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_hspan(x, y, len, c, &m_span[0]);
    }

    void copy_vline(int x, int y, unsigned int len, const color_type& c)
    {
        realloc_span(len);
        m_mask->fill_vspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_vspan(x, y, len, c, &m_span[0]);
    }

    void blend_vline(int x, int y, unsigned int len,
                     const color_type& c, cover_type cover)
    {
        init_span(len);
        m_mask->combine_vspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_vspan(x, y, len, c, &m_span[0]);
    }

    void copy_from(const gfx_rendering_buffer& from,
                   int xdst, int ydst, int xsrc, int ysrc, unsigned int len)
    {
        m_pixfmt->copy_from(from, xdst, ydst, xsrc, ysrc, len);
    }


    void blend_solid_hspan(int x, int y, unsigned int len,
                           const color_type& c, const cover_type* covers)
    {
        init_span(len, covers);
        m_mask->combine_hspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_hspan(x, y, len, c, &m_span[0]);
    }


    void blend_solid_vspan(int x, int y, unsigned int len,
                           const color_type& c, const cover_type* covers)
    {
        init_span(len, covers);
        m_mask->combine_vspan(x, y, &m_span[0], len);
        m_pixfmt->blend_solid_vspan(x, y, len, c, &m_span[0]);
    }

    void copy_color_hspan(int x, int y, unsigned int len, const color_type* colors)
    {
        realloc_span(len);
        m_mask->fill_hspan(x, y, &m_span[0], len);
        m_pixfmt->blend_color_hspan(x, y, len, colors, &m_span[0], cover_full);
    }

    void copy_color_vspan(int x, int y, unsigned int len, const color_type* colors)
    {
        realloc_span(len);
        m_mask->fill_vspan(x, y, &m_span[0], len);
        m_pixfmt->blend_color_vspan(x, y, len, colors, &m_span[0], cover_full);
    }

    void blend_color_hspan(int x, int y, unsigned int len, const color_type* colors,
                           const cover_type* covers, cover_type cover = cover_full)
    {
        if (covers) {
            init_span(len, covers);
            m_mask->combine_hspan(x, y, &m_span[0], len);
        } else {
            realloc_span(len);
            m_mask->fill_hspan(x, y, &m_span[0], len);
        }
        m_pixfmt->blend_color_hspan(x, y, len, colors, &m_span[0], cover);
    }

    void blend_color_vspan(int x, int y, unsigned int len, const color_type* colors,
                           const cover_type* covers, cover_type cover = cover_full)
    {
        if (covers) {
            init_span(len, covers);
            m_mask->combine_vspan(x, y, &m_span[0], len);
        } else {
            realloc_span(len);
            m_mask->fill_vspan(x, y, &m_span[0], len);
        }
        m_pixfmt->blend_color_vspan(x, y, len, colors, &m_span[0], cover);
    }

private:
    pixfmt_type* m_pixfmt;
    const amask_type* m_mask;
    pod_array<cover_type> m_span;
};


// mask layer
class gfx_mask_layer : public abstract_mask_layer
{
public:
    gfx_mask_layer(byte* buf, unsigned int width, unsigned int height, int stride)
        : m_type(MASK_ALPHA)
    {
        attach(buf, width, height, stride);
    }

    virtual ~gfx_mask_layer()
    {
        m_colors.clear();
    }

    virtual void attach(byte* buf, unsigned int width, unsigned int height, int stride)
    {
        m_buffer.init(buf, width, height, stride);
    }

    virtual void set_mask_type(int t)
    {
        m_type = t;
    }

    virtual void add_filter_color(const picasso::rgba& c)
    {
        m_colors.add(rgba8(c));
    }

    virtual void clear_filter_colors(void)
    {
        m_colors.clear();
    }

    int type(void) const { return m_type; }
    gfx_rendering_buffer& buffer(void) { return m_buffer; }
    pod_bvector<rgba8>& colors(void) { return m_colors; }
private:
    int m_type;
    gfx_rendering_buffer m_buffer;
    pod_bvector<rgba8> m_colors;
};

}
#endif /*_GFX_MASK_LAYER_H_*/
