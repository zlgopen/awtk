/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PIXFMT_WRAPPER_H_
#define _PIXFMT_WRAPPER_H_

#include "common.h"

#include "gfx_mask_layer.h"
#include "gfx_image_accessors.h"

namespace gfx {

static inline bool operator == (const rgba8& a, const rgba8& b)
{
    return (a.r==b.r) && (a.g==b.g) && (a.b==b.b) && (a.a==b.a); 
}

// for color key compare
static inline bool operator == (const rgba8* a, const rgba8& b)
{
    //Note : for color key only, not compare alpha value.
    return (a->r==b.r) && (a->g==b.g) && (a->b==b.b); 
}

static inline bool operator != (const rgba8* a, const rgba8& b)
{
    //Note : for color key only, not compare alpha value.
    return (a->r!=b.r) || (a->g!=b.g) || (a->b!=b.b); 
}

//pixfmt wrapper
template<typename Pixfmt, typename AlphaMask>
class gfx_pixfmt_wrapper
{
public:
    typedef Pixfmt pixfmt_type;
    typedef typename pixfmt_type::color_type color_type;
    typedef typename pixfmt_type::order_type order_type;
    typedef typename pixfmt_type::pixel_type pixel_type;
    typedef typename pixfmt_type::value_type value_type;
    typedef typename pixfmt_type::row_data row_data;

    typedef AlphaMask amask_type;
    typedef typename amask_type::cover_type cover_type;

    enum {
        base_shift = pixfmt_type::base_shift,
        base_scale = pixfmt_type::base_scale,
        base_mask  = pixfmt_type::base_mask,
        pix_width  = pixfmt_type::pix_width,
    };
public:
    gfx_pixfmt_wrapper()
        : use_mask(false), m_colorkey(0), m_fmt(), m_filter(), m_mask(m_fmt, m_filter), m_colors(0) 
    {
    }
    explicit gfx_pixfmt_wrapper(gfx_rendering_buffer& rb)
        : use_mask(false), m_colorkey(0), m_fmt(rb), m_filter(), m_mask(m_fmt, m_filter), m_colors(0) 
    {
    }

    ~gfx_pixfmt_wrapper() { clear_mask(); clear_key();}

    void attach(gfx_rendering_buffer& rb) { m_fmt.attach(rb); }

    void clear_mask(void)
    {
        use_mask = false;
        m_colors = 0;
    }

    void clear_key(void)
    {
        m_colorkey = 0;
    }

    void attach_mask(gfx_mask_layer* mask)
    {
        clear_mask(); // clear old data

        use_mask = true;
        m_filter.attach(mask->buffer());
        if (mask->type() == MASK_COLORS)
            m_colors = &mask->colors();
    }

    void set_transparent_color(rgba8 * color)
    {
        m_colorkey = color;
    }

    bool is_color_mask() const
    {
        return m_colors && m_colors->size();
    }

    bool has_color(const color_type& c) const
    {
        //Note: m_color is not NULL first!
        for (unsigned int i = 0; i < m_colors->size(); i++)
            if (c == m_colors->at(i))
                return true;
        return false;
    }

public:

    unsigned int width(void) const { return m_fmt.width();  }
    unsigned int height(void) const { return m_fmt.height(); }
    int stride(void) const { return m_fmt.stride(); }

    byte* row_ptr(int y) { return m_fmt.row_ptr(y); }
    const byte* row_ptr(int y) const { return m_fmt.row_ptr(y); }
    row_data row(int y) const { return m_fmt.row(y); }

    byte* pix_ptr(int x, int y)
    {
        return m_fmt.pix_ptr(x, y);
    }

    const byte* pix_ptr(int x, int y) const
    {
        return m_fmt.pix_ptr(x, y);
    }

    void alpha(scalar a) { m_fmt.alpha(a); }
    scalar alpha(void) const { return m_fmt.alpha(); }

    void blend_op(unsigned int op) { m_fmt.blend_op(op); }
    unsigned int blend_op(void) const { return m_fmt.blend_op(); }

    static void make_pix(byte* p, const color_type& c)
    {
        pixfmt_type::make_pix(p, c);
    }

    color_type pixel(int x, int y)
    {
        return m_fmt.pixel(x, y);
    }

    void copy_pixel(int x, int y, const color_type& c)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) { 
                if (has_color(c))
                    m_mask.copy_pixel(x, y, c);
                else
                    m_fmt.copy_pixel(x, y, c);
            } else {
                m_mask.copy_pixel(x, y, c);
            }
        } else {
            m_fmt.copy_pixel(x, y, c);
        }
    }

    void blend_pixel(int x, int y, const color_type& c, cover_type cover)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.blend_pixel(x, y, c, cover);
                else
                    m_fmt.blend_pixel(x, y, c, cover);
            } else {
                m_mask.blend_pixel(x, y, c, cover);
            }
        } else {
            m_fmt.blend_pixel(x, y, c, cover);
        }
    }

    void copy_hline(int x, int y, unsigned int len, const color_type& c)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.copy_hline(x, y, len, c);
                else
                    m_fmt.copy_hline(x, y, len, c);
            } else {
                m_mask.copy_hline(x, y, len, c);
            }
        } else {
            m_fmt.copy_hline(x, y, len, c);
        }
    }

    void blend_hline(int x, int y, unsigned int len, const color_type& c, cover_type cover)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.blend_hline(x, y, len, c, cover);
                else
                    m_fmt.blend_hline(x, y, len, c, cover);
            } else {
                m_mask.blend_hline(x, y, len, c, cover);
            }
        } else {
            m_fmt.blend_hline(x, y, len, c, cover);
        }
    }

    void copy_vline(int x, int y, unsigned int len, const color_type& c)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.copy_vline(x, y, len, c);
                else
                    m_fmt.copy_vline(x, y, len, c);
            } else {
                m_mask.copy_vline(x, y, len, c);
            }
        } else {
            m_fmt.copy_vline(x, y, len, c);
        }
    }

    void blend_vline(int x, int y, unsigned int len, const color_type& c, cover_type cover)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.blend_vline(x, y, len, c, cover);
                else
                    m_fmt.blend_vline(x, y, len, c, cover);
            } else {
                m_mask.blend_vline(x, y, len, c, cover);
            }
        } else {
            m_fmt.blend_vline(x, y, len, c, cover);
        }
    }

    void blend_solid_hspan(int x, int y, unsigned int len, 
                               const color_type& c, const cover_type* covers)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.blend_solid_hspan(x, y, len, c, covers);
                else
                    m_fmt.blend_solid_hspan(x, y, len, c, covers);
            } else {
                m_mask.blend_solid_hspan(x, y, len, c, covers);
            }
        } else {
            m_fmt.blend_solid_hspan(x, y, len, c, covers);
        }
    }

    void blend_solid_vspan(int x, int y, unsigned int len, 
                               const color_type& c, const cover_type* covers)
    {
        if (m_colorkey && m_colorkey == c)
            return;

        if (unlikely(use_mask)) {
            if (is_color_mask()) {
                if (has_color(c))
                    m_mask.blend_solid_vspan(x, y, len, c, covers);
                else
                    m_fmt.blend_solid_vspan(x, y, len, c, covers);
            } else {
                m_mask.blend_solid_vspan(x, y, len, c, covers);
            }
        } else {
            m_fmt.blend_solid_vspan(x, y, len, c, covers);
        }
    }

    void copy_color_hspan(int x, int y, unsigned int len, const color_type* colors)
    {
        if (m_colorkey) {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) {
                            if (has_color(colors[i]))
                                m_mask.copy_pixel(x+i, y, colors[i]);
                            else
                                m_fmt.copy_pixel(x+i, y, colors[i]);
                        }
                    }
                } else {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) 
                            m_mask.copy_pixel(x+i, y, colors[i]);
                    }
                }
            } else {
                for (unsigned int i = 0; i < len; i++) {
                    if (m_colorkey != colors[i]) 
                        m_fmt.copy_pixel(x+i, y, colors[i]);
                }
            }
        } else {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (has_color(colors[i]))
                            m_mask.copy_pixel(x+i, y, colors[i]);
                        else
                            m_fmt.copy_pixel(x+i, y, colors[i]);
                    }
                } else {
                    m_mask.copy_color_hspan(x, y, len, colors);
                }
            } else {
                m_fmt.copy_color_hspan(x, y, len, colors);
            }
        }
    }

    void copy_color_vspan(int x, int y, unsigned int len, const color_type* colors)
    {
        if (m_colorkey) {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) {
                            if (has_color(colors[i]))
                                m_mask.copy_pixel(x, y+i, colors[i]);
                            else
                                m_fmt.copy_pixel(x, y+i, colors[i]);
                        }
                    }
                } else {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i])
                            m_mask.copy_pixel(x, y+i, colors[i]);
                    }
                }
            } else {
                for (unsigned int i = 0; i < len; i++) {
                    if (m_colorkey != colors[i])
                        m_fmt.copy_pixel(x, y+i, colors[i]);
                }
            }
        } else {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (has_color(colors[i]))
                            m_mask.copy_pixel(x, y+i, colors[i]);
                        else
                            m_fmt.copy_pixel(x, y+i, colors[i]);
                    }
                } else {
                    m_mask.copy_color_vspan(x, y, len, colors);
                }
            } else {
                m_fmt.copy_color_vspan(x, y, len, colors);
            }
        }
    }

    void blend_color_hspan(int x, int y, unsigned int len, const color_type* colors,
            const cover_type* covers, cover_type cover)
    {
        if (m_colorkey) {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) {
                            if (has_color(colors[i]))
                                m_mask.blend_pixel(x+i, y, colors[i], covers[i]);
                            else
                                m_fmt.blend_pixel(x+i, y, colors[i], covers[i]);
                        }
                    }
                } else {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) 
                            m_mask.blend_pixel(x+i, y, colors[i], covers[i]);
                    }
                }
            } else {
                for (unsigned int i = 0; i < len; i++) {
                    if (m_colorkey != colors[i]) 
                        m_fmt.blend_pixel(x+i, y, colors[i], covers[i]);
                }
            }
        } else {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (has_color(colors[i]))
                            m_mask.blend_pixel(x+i, y, colors[i], covers[i]);
                        else
                            m_fmt.blend_pixel(x+i, y, colors[i], covers[i]);
                    }
                } else {
                    m_mask.blend_color_hspan(x, y, len, colors, covers, cover);
                }
            } else {
                m_fmt.blend_color_hspan(x, y, len, colors, covers, cover);
            }
        }
    }

    void blend_color_vspan(int x, int y, unsigned int len, const color_type* colors,
            const cover_type* covers, cover_type cover)
    {
        if (m_colorkey) {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) {
                            if (has_color(colors[i]))
                                m_mask.blend_pixel(x, y+i, colors[i], covers[i]);
                            else
                                m_fmt.blend_pixel(x, y+i, colors[i], covers[i]);
                        }
                    }
                } else {
                    for (unsigned int i = 0; i < len; i++) {
                        if (m_colorkey != colors[i]) 
                            m_mask.blend_pixel(x, y+i, colors[i], covers[i]);
                    }
                }
            } else {
                for (unsigned int i = 0; i < len; i++) {
                    if (m_colorkey != colors[i]) 
                        m_fmt.blend_pixel(x, y+i, colors[i], covers[i]);
                }
            }
        } else {
            if (unlikely(use_mask)) {
                if (is_color_mask()) {
                    for (unsigned int i = 0; i < len; i++) {
                        if (has_color(colors[i]))
                            m_mask.blend_pixel(x, y+i, colors[i], covers[i]);
                        else
                            m_fmt.blend_pixel(x, y+i, colors[i], covers[i]);
                    }
                } else {
                    m_mask.blend_color_vspan(x, y, len, colors, covers, cover);
                }
            } else {
                m_fmt.blend_color_vspan(x, y, len, colors, covers, cover);
            }
        }
    }

    template<class RenBuf2> void copy_from(const RenBuf2& from, 
            int xdst, int ydst,
            int xsrc, int ysrc,
            unsigned int len)
    {
        m_fmt.copy_from(from, xdst, ydst, xsrc, ysrc, len);
    }

    template<class RenBuf2>
        void copy_point_from(const RenBuf2& from, 
                int xdst, int ydst,
                int xsrc, int ysrc)
    {
        m_fmt.copy_point_from(from, xdst, ydst, xsrc, ysrc);
    }

    template<class SrcPixelFormatRenderer> 
        void blend_from(const SrcPixelFormatRenderer& from, 
                int xdst, int ydst,
                int xsrc, int ysrc,
                unsigned int len,
                cover_type cover)
    {
        m_fmt.blend_from(from, xdst, ydst, xsrc, ysrc, len, cover);
    }

    template<class SrcPixelFormatRenderer> 
        void blend_point_from(const SrcPixelFormatRenderer& from, 
                int xdst, int ydst,
                int xsrc, int ysrc,
                cover_type cover)
    {
        m_fmt.blend_point_from(from, xdst, ydst, xsrc, ysrc, cover);
    }

private:
    bool use_mask;
    rgba8 *m_colorkey;
    pixfmt_type m_fmt;
    amask_type m_filter;
    gfx_pixfmt_amask_adaptor<pixfmt_type, amask_type> m_mask;
    pod_bvector<rgba8> *m_colors;
};

//pattern wrapper
template<typename Pixfmt>
class pattern_wrapper
{
public:
    typedef Pixfmt   pixfmt_type;
    typedef typename pixfmt_type::color_type color_type;
    typedef typename pixfmt_type::order_type order_type;
    typedef typename pixfmt_type::value_type value_type;
    typedef typename pixfmt_type::pixel_type pixel_type;
    enum { pix_width = pixfmt_type::pix_width };

    pattern_wrapper() {}
    virtual ~pattern_wrapper() {}
    virtual const byte* span(int x, int y, unsigned) = 0;
    virtual const byte* next_x() = 0;
    virtual const byte* next_y() = 0;
};

template<typename Pixfmt, typename Wrap_X, typename Wrap_Y>
class pattern_wrapper_adaptor : public pattern_wrapper<Pixfmt>
{
public:
    explicit pattern_wrapper_adaptor(const Pixfmt& fmt)
        : m_wrap(fmt)
    {
    }

    virtual const byte* span(int x, int y, unsigned int i)
    {
        return m_wrap.span(x, y, i);
    }

    virtual const byte* next_x()
    {
        return m_wrap.next_x();
    }

    virtual const byte* next_y()
    {
        return m_wrap.next_y();
    }
private:
    image_accessor_wrap<Pixfmt, Wrap_X, Wrap_Y> m_wrap;
};


}
#endif /*_PIXFMT_WRAPPER_H_*/
