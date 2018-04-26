/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_SCANLINE_RENDERER_H_
#define _GFX_SCANLINE_RENDERER_H_

#include "common.h"

namespace gfx {

// renderer scanline binary with solid color
template <typename BaseRenderer>
class gfx_renderer_scanline_bin_solid
{
public:
    typedef BaseRenderer ren_type;
    typedef typename ren_type::color_type color_type;

    gfx_renderer_scanline_bin_solid()
        : m_ren(0)
    {
    }

    explicit gfx_renderer_scanline_bin_solid(ren_type& ren)
        : m_ren(&ren)
    {
    }

    void attach(ren_type& ren)
    {
        m_ren = &ren;
    }

    void color(const color_type& c) { m_color = c; }
    const color_type& color() const { return m_color; }

    void prepare(void) { }

    template <typename Scanline>
    void render(const Scanline& sl)
    {
        unsigned int num_spans = sl.num_spans();
        typename Scanline::const_iterator span = sl.begin();
        for (;;) {
            m_ren->blend_hline(span->x, sl.y(),
                   span->x - 1 + ((span->len < 0) ? -span->len : span->len), m_color, cover_full);

            if (--num_spans == 0)
                break;

            ++span;
        }
    }

private:
    ren_type* m_ren;
    color_type m_color;
};


// renderer scanline antialias with solid color
template <typename BaseRenderer>
class gfx_renderer_scanline_aa_solid
{
public:
    typedef BaseRenderer ren_type;
    typedef typename ren_type::color_type color_type;

    gfx_renderer_scanline_aa_solid()
        : m_ren(0)
    {
    }

    explicit gfx_renderer_scanline_aa_solid(ren_type& ren)
        : m_ren(&ren)
    {
    }

    void attach(ren_type& ren)
    {
        m_ren = &ren;
    }

    void color(const color_type& c) { m_color = c; }
    const color_type& color(void) const { return m_color; }

    void prepare(void) { }

    template <typename Scanline>
    void render(const Scanline& sl)
    {
        int y = sl.y();
        unsigned int num_spans = sl.num_spans();
        typename Scanline::const_iterator span = sl.begin();

        for (;;) {
            int x = span->x;

            if (span->len > 0) {
                m_ren->blend_solid_hspan(x, y, (unsigned int)span->len, m_color, span->covers);
            } else {
                m_ren->blend_hline(x, y, (unsigned int)(x - span->len - 1), m_color, *(span->covers));
            }

            if (--num_spans == 0)
                break;

            ++span;
        }
    }

private:
    ren_type* m_ren;
    color_type m_color;
};


// render scanlines
template <typename Rasterizer, typename Scanline, typename Renderer>
void gfx_render_scanlines(Rasterizer& ras, Scanline& sl, Renderer& ren)
{
    if (ras.rewind_scanlines()) {
        sl.reset(ras.min_x(), ras.max_x());
        ren.prepare();
        while (ras.sweep_scanline(sl)) {
            ren.render(sl);
        }
    }
}


// render scanlines antialias
template <typename Rasterizer, typename Scanline, typename Renderer,
          typename SpanAllocator, typename SpanGenerator>
void gfx_render_scanlines_aa(Rasterizer& ras, Scanline& sl, Renderer& ren,
                         SpanAllocator& alloc, SpanGenerator& span_gen)
{
    if (ras.rewind_scanlines()) {
        sl.reset(ras.min_x(), ras.max_x());
        span_gen.prepare();
        while (ras.sweep_scanline(sl)) {
            int y = sl.y();

            unsigned int num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();
            for (;;) {
                int x = span->x;
                int len = span->len;
                const typename Scanline::cover_type* covers = span->covers;

                if (len < 0)
                    len = -len;

                typename Renderer::color_type* colors = alloc.allocate(len);
                span_gen.generate(colors, x, y, len);

                ren.blend_color_hspan(x, y, len, colors, (span->len < 0) ? 0 : covers, *covers);

                if (--num_spans == 0)
                    break;

                ++span;
            }
        }
    }
}

}
#endif /*_GFX_SCANLINE_RENDERER_H_*/
