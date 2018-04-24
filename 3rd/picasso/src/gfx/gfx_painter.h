/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_PAINTER_H_
#define _GFX_PAINTER_H_

#include "common.h"
#include "interfaces.h"
#include "convert.h"

#include "picasso.h"
#include "picasso_painter.h"

#include "gfx_blur.h"
#include "gfx_gradient_adapter.h"
#include "gfx_image_filters.h"
#include "gfx_painter_helper.h"
#include "gfx_pixfmt_wrapper.h"
#include "gfx_raster_adapter.h"
#include "gfx_renderer.h"
#include "gfx_rendering_buffer.h"
#include "gfx_scanline.h"
#include "gfx_scanline_renderer.h"
#include "gfx_scanline_storage.h"
#include "gfx_span_generator.h"
#include "gfx_trans_affine.h"

namespace gfx {

template <typename Pixfmt>
class gfx_painter : public abstract_painter
{
    typedef gfx_pixfmt_wrapper<Pixfmt, mask_type> pixfmt;
    typedef typename pixfmt::color_type color_type;
    typedef gfx_renderer<pixfmt> renderer_base_type;
    typedef gfx_renderer_scanline_aa_solid<renderer_base_type> renderer_solid_type;
    typedef gfx_renderer_scanline_bin_solid<renderer_base_type> renderer_bin_type;
public:
    typedef enum {
        type_solid      = 0,
        type_image      = 1,
        type_pattern    = 2,
        type_gradient   = 3,
        type_canvas     = 4,
    } source_type;

    typedef struct {
        abstract_rendering_buffer* buffer;
        int filter;
        rect_s rect;
        rgba8 key;
        bool transparent;
        bool colorkey;
    } image_holder;

    typedef struct {
        abstract_rendering_buffer* buffer;
        int filter;
        rect_s rect;
        int xtype;
        int ytype;
        abstract_trans_affine* matrix;
        bool transparent;
    } pattern_holder;

    typedef struct {
        abstract_gradient_adapter* gradient;
    } gradient_holder;

    gfx_painter() 
        : m_fill_type(type_solid)
        , m_draw_shadow(false)
        , m_shadow_area(0,0,0,0)
        , m_shadow_buffer(0)
    {
    }

    virtual ~gfx_painter() {}

    virtual void attach(abstract_rendering_buffer*); 
    virtual pix_fmt pixel_format(void) const;

    virtual void set_alpha(scalar a);
    virtual void set_composite(comp_op op);
    virtual void set_stroke_color(const rgba& c);
    virtual void set_fill_color(const rgba& c);
    virtual void set_fill_image(const abstract_rendering_buffer* img, int filter, const rect_s& rc);
    virtual void set_fill_canvas(const abstract_rendering_buffer* img, int filter, const rect_s& rc);
    virtual void set_fill_pattern(const abstract_rendering_buffer* img, int filter, const rect_s& rc, 
                                    int xtype, int ytype, const abstract_trans_affine* mtx);
    virtual void set_fill_gradient(const abstract_gradient_adapter* g);
    virtual void set_font_fill_color(const rgba& c);

    virtual void apply_stroke(abstract_raster_adapter* raster);
    virtual void apply_fill(abstract_raster_adapter* raster);
    virtual void apply_text_fill(abstract_raster_adapter* rs, text_style style);
    virtual void apply_mono_text_fill(void * storage);
    virtual void apply_clear(const rgba& c);
    virtual void apply_clip_path(const vertex_source& v, int rule, const abstract_trans_affine* mtx);
    virtual void apply_clip_device(const rect_s& rc, scalar xoffset, scalar yoffset);
    virtual void clear_clip(void);

    virtual void apply_masking(abstract_mask_layer*);
    virtual void clear_masking(void);

    virtual void apply_blur(scalar blur);

    virtual bool begin_shadow(const rect_s& rc);
    virtual void apply_shadow(abstract_raster_adapter* rs, const rect_s& r, 
                                                const rgba& c, scalar x, scalar y, scalar b);

    virtual void copy_rect_from(abstract_rendering_buffer* src, const rect& rc, int x, int y);
private:
    pattern_wrapper<pixfmt>* pattern_wrap(int xtype, int ytype, pixfmt& fmt)
    {
        pattern_wrapper<pixfmt>* p = 0;
        if ((xtype == WRAP_TYPE_REPEAT) && (ytype == WRAP_TYPE_REPEAT))
            p = new pattern_wrapper_adaptor<pixfmt, wrap_mode_repeat, wrap_mode_repeat>(fmt);
        else if ((xtype == WRAP_TYPE_REPEAT) && (ytype == WRAP_TYPE_REFLECT))
            p = new pattern_wrapper_adaptor<pixfmt, wrap_mode_repeat, wrap_mode_reflect>(fmt);
        else if ((xtype == WRAP_TYPE_REFLECT) && (ytype == WRAP_TYPE_REPEAT))
            p = new pattern_wrapper_adaptor<pixfmt, wrap_mode_reflect, wrap_mode_repeat>(fmt);
        else if ((xtype == WRAP_TYPE_REFLECT) && (ytype == WRAP_TYPE_REFLECT))
            p = new pattern_wrapper_adaptor<pixfmt, wrap_mode_reflect, wrap_mode_reflect>(fmt);

        return p;
    }
    //fill
    source_type        m_fill_type; 
    rgba               m_fill_color;
    image_holder       m_image_source;
    pattern_holder     m_pattern_source;
    gradient_holder    m_gradient_source;
    //stroke
    rgba               m_stroke_color; //FIXME: need stroke type feature.
    rgba               m_font_fill_color; //FIXME: need stroke type feature.
    pixfmt             m_fmt;
    renderer_base_type m_rb;
    //shadow
    bool               m_draw_shadow;
    rect_s             m_shadow_area;
    byte*              m_shadow_buffer;
    gfx_rendering_buffer m_shadow_rb;
    pixfmt_rgba32        m_shadow_fmt;
    gfx_renderer<pixfmt_rgba32> m_shadow_base;
    //scanline storage
    gfx_scanline_p8 m_scanline_p;
    gfx_scanline_u8 m_scanline_u;
    gfx_scanline_bin m_scanline_bin;
    //span allocater
    gfx_span_allocator<color_type> m_spans;
};

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::attach(abstract_rendering_buffer* buffer)
{
    if (buffer) {   
        m_fmt.attach(*static_cast<gfx_rendering_buffer*>(buffer));
        m_rb.attach(m_fmt);
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_alpha(scalar a)
{
    m_fmt.alpha(a);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_composite(comp_op op)
{
    m_fmt.blend_op(op);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_fill_color(const rgba& c)
{
    m_fill_type = type_solid;
    m_fill_color = c;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_font_fill_color(const rgba& c)
{
    m_font_fill_color = c;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_stroke_color(const rgba& c)
{
    m_stroke_color = c;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_fill_image(const abstract_rendering_buffer* img, int filter, const rect_s& rc)
{
    m_fill_type = type_image;
    m_image_source.buffer = const_cast<abstract_rendering_buffer*>(img);
    m_image_source.filter = filter;
    m_image_source.rect = rc;
    m_image_source.key = rgba8(img->get_color_channel());
    m_image_source.transparent = img->is_transparent();
    m_image_source.colorkey = img->has_color_channel();
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_fill_canvas(const abstract_rendering_buffer* img, int filter, const rect_s& rc)
{
    m_fill_type = type_canvas;
    m_image_source.buffer = const_cast<abstract_rendering_buffer*>(img);
    m_image_source.filter = filter;
    m_image_source.rect = rc;
    m_image_source.key = rgba8(0,0,0,0);
    m_image_source.transparent = true; // canvas default transpaent.
    m_image_source.colorkey = false;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_fill_pattern(const abstract_rendering_buffer* img, int filter, const rect_s& rc,
                                        int xtype, int ytype, const abstract_trans_affine* mtx)
{
    m_fill_type = type_pattern;
    m_pattern_source.buffer = const_cast<abstract_rendering_buffer*>(img);
    m_pattern_source.filter = filter;
    m_pattern_source.rect = rc;
    m_pattern_source.xtype = xtype;
    m_pattern_source.ytype = ytype;
    m_pattern_source.matrix = const_cast<abstract_trans_affine*>(mtx);
    m_pattern_source.transparent = img->is_transparent();
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::set_fill_gradient(const abstract_gradient_adapter* g)
{
    m_fill_type = type_gradient;
    m_gradient_source.gradient = const_cast<abstract_gradient_adapter*>(g);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_stroke(abstract_raster_adapter* raster)
{
    if (raster) {
        renderer_solid_type ren(m_rb);
        ren.color(m_stroke_color);
        gfx_render_scanlines(static_cast<gfx_raster_adapter*>(raster)->stroke_impl(), m_scanline_p, ren);
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_mono_text_fill(void * storage)
{
    gfx_serialized_scanlines_adaptor_bin* storage_bin = (gfx_serialized_scanlines_adaptor_bin*)storage;
    renderer_bin_type ren_solid(m_rb);
    ren_solid.color(m_font_fill_color);
    gfx_serialized_scanlines_adaptor_bin::embedded_scanline sl;
    gfx_render_scanlines(*storage_bin, sl, ren_solid);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_text_fill(abstract_raster_adapter* raster, text_style render_type)
{
    if (raster) {
        switch (render_type) {
            case text_smooth:
                {
                    renderer_solid_type ren_solid(m_rb);
                    ren_solid.color(m_font_fill_color);
                    gfx_render_scanlines(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), m_scanline_u, ren_solid);
                }
                break;
            case text_mono:
                {
                    renderer_bin_type ren_solid(m_rb);
                    ren_solid.color(m_font_fill_color);
                    gfx_render_scanlines(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), m_scanline_bin, ren_solid);
                }
                break;
            case text_stroke:
                {
                    renderer_solid_type ren_solid(m_rb);
                    ren_solid.color(m_font_fill_color);
                    gfx_render_scanlines(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), m_scanline_p, ren_solid);
                }
                break;
        }
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_fill(abstract_raster_adapter* raster)
{
    if (raster) {
        switch (m_fill_type) {
        case type_canvas:
            {
                pixfmt canvas_fmt(*static_cast<gfx_rendering_buffer*>(m_image_source.buffer));

                rect_s dr = m_image_source.rect;
                gfx_trans_affine mtx;
                mtx *= gfx_trans_affine_translation(sround(dr.x()), sround(dr.y()));
                mtx *= stable_matrix(static_cast<gfx_raster_adapter*>(raster)->transformation());
                mtx.invert();

                gfx_span_interpolator_linear interpolator(mtx);

                typename painter_raster<Pixfmt>::source_type img_src(canvas_fmt);

                if (m_image_source.filter) {
                    image_filter_adapter* filter = create_image_filter(m_image_source.filter);

                    typename painter_raster<Pixfmt>::span_canvas_filter_type
                        sg(img_src, interpolator, *(filter));
                    gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(),
                                            m_scanline_u, m_rb, m_spans, sg);

                    if (filter) delete filter;
                } else {
                    typename painter_raster<Pixfmt>::span_canvas_filter_type_nn
                        sg(img_src, interpolator);
                    gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(),
                                            m_scanline_u, m_rb, m_spans, sg);
                }
            }
            break;
        case type_image:
            {
                pixfmt img_fmt(*static_cast<gfx_rendering_buffer*>(m_image_source.buffer));

                if (m_image_source.colorkey)
                    m_fmt.set_transparent_color(&m_image_source.key);

                rect_s dr = m_image_source.rect;
                bool transparent = m_image_source.transparent;

                scalar xs = (scalar)dr.width() / m_image_source.buffer->width();
                scalar ys = (scalar)dr.height() / m_image_source.buffer->height();

                gfx_trans_affine mtx; 
                mtx *= gfx_trans_affine_scaling(xs, ys);
                mtx *= gfx_trans_affine_translation(sround(dr.x()), sround(dr.y()));
                mtx *= stable_matrix(static_cast<gfx_raster_adapter*>(raster)->transformation());
                mtx.invert();

                gfx_span_interpolator_linear interpolator(mtx);

                typename painter_raster<Pixfmt>::source_type img_src(img_fmt);

                if (m_image_source.filter) {
                    image_filter_adapter* filter = create_image_filter(m_image_source.filter);

                    if (transparent) {
                        typename painter_raster<Pixfmt>::span_canvas_filter_type
                                                sg(img_src, interpolator, *(filter));
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(),
                                                m_scanline_u, m_rb, m_spans, sg);
                    } else {
                        typename painter_raster<Pixfmt>::span_image_filter_type
                                                sg(img_src, interpolator, *(filter));
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(),
                                                m_scanline_u, m_rb, m_spans, sg);
                    }

                    if (filter) delete filter;
                } else {
                    if (transparent) {
                        typename painter_raster<Pixfmt>::span_canvas_filter_type_nn
                                                sg(img_src, interpolator);
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(),
                                                m_scanline_u, m_rb, m_spans, sg);
                    } else {
                        typename painter_raster<Pixfmt>::span_image_filter_type_nn
                                                sg(img_src, interpolator);
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                                m_scanline_u, m_rb, m_spans, sg);
                    }
                }

                m_fmt.clear_key();
            }
            break;
        case type_pattern:
            {
                pixfmt pattern_fmt(*static_cast<gfx_rendering_buffer*>(m_pattern_source.buffer));

                rect_s dr = m_pattern_source.rect;
                bool transparent = m_pattern_source.transparent;
                gfx_trans_affine mtx;
                mtx = *static_cast<gfx_trans_affine*>(m_pattern_source.matrix);
                mtx *= gfx_trans_affine_translation(sround(dr.x()), sround(dr.y()));
                mtx *= stable_matrix(static_cast<gfx_raster_adapter*>(raster)->transformation());
                mtx.invert();

                gfx_span_interpolator_linear interpolator(mtx);

                pattern_wrapper<pixfmt>* pattern = 
                            pattern_wrap(m_pattern_source.xtype, m_pattern_source.ytype, pattern_fmt);

                if (m_pattern_source.filter) {
                    image_filter_adapter* filter = create_image_filter(m_pattern_source.filter);

                    if (transparent) {
                        typename painter_raster<Pixfmt>::span_canvas_pattern_type 
                                                sg(*pattern, interpolator, *(filter));
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                                m_scanline_u, m_rb, m_spans, sg);
                    } else {
                        typename painter_raster<Pixfmt>::span_image_pattern_type 
                                                sg(*pattern, interpolator, *(filter));
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                                m_scanline_u, m_rb, m_spans, sg);
                    }

                    if (filter) delete filter;
                } else {
                    if (transparent) {
                        typename painter_raster<Pixfmt>::span_canvas_pattern_type_nn 
                                                sg(*pattern, interpolator);
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                                m_scanline_u, m_rb, m_spans, sg);
                    } else {
                        typename painter_raster<Pixfmt>::span_image_pattern_type_nn 
                                                sg(*pattern, interpolator);
                        gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                                m_scanline_u, m_rb, m_spans, sg);
                    }
                }

                delete pattern;
            }
            break;
        case type_gradient:
            {
                gfx_gradient_adapter* gradient = static_cast<gfx_gradient_adapter*>(m_gradient_source.gradient);
                gradient->build();

                gfx_trans_affine mtx;
                mtx = gradient->matrix();
                mtx *= stable_matrix(static_cast<gfx_raster_adapter*>(raster)->transformation());
                mtx.invert();

                gfx_span_interpolator_linear inter(mtx);

                gfx_gradient_wrapper* pwr = gradient->wrapper();
                scalar len = gradient->length();
                scalar st = gradient->start();

                gfx_span_gradient<color_type> sg(inter, *pwr, gradient->colors(), st, len);
                gfx_render_scanlines_aa(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), 
                                        m_scanline_u, m_rb, m_spans, sg);
            }
            break;
        case type_solid: // solid fill default.
        default:
            {   
                renderer_solid_type ren(m_rb);
                ren.color(m_fill_color);
                gfx_render_scanlines(static_cast<gfx_raster_adapter*>(raster)->fill_impl(), m_scanline_p, ren);
            }
        }
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_clip_path(const vertex_source& v, int rule, const abstract_trans_affine* mtx)
{
    abstract_trans_affine* cm = const_cast<abstract_trans_affine*>(mtx); 
    gfx_trans_affine* m = static_cast<gfx_trans_affine*>(cm);
    conv_transform p(const_cast<vertex_source&>(v), m);

    if (m_draw_shadow) { //in shadow draw context.
        m_shadow_base.add_clipping(p, (picasso::filling_rule)rule);
    } else {
        m_rb.add_clipping(p, (picasso::filling_rule)rule);
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_clip_device(const rect_s& rc, scalar xoffset, scalar yoffset)
{
    rect r(iround(rc.x1+xoffset), iround(rc.y1+yoffset), 
           iround(rc.x2+xoffset), iround(rc.y2+yoffset));

    if (m_draw_shadow) { //in shadow draw context.
        m_shadow_base.clip_rect(r.x1, r.y1, r.x2, r.y2);    
    } else {
        m_rb.clip_rect(r.x1, r.y1, r.x2, r.y2);        
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_clear(const rgba& c)
{
    m_rb.clear(c);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_blur(scalar blur)
{
    if (blur > 0) {
        m_fmt.alpha(FLT_TO_SCALAR(1.0f));
        m_fmt.blend_op(comp_op_src_over);
        stack_blur<rgba8> b;
        b.blur(m_fmt, uround(blur * FLT_TO_SCALAR(40.0f)));
    }
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::clear_clip(void)
{
    if (m_draw_shadow) //in shadow draw context.
        m_shadow_base.reset_clipping(true);
    else
        m_rb.reset_clipping(true);
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_masking(abstract_mask_layer* m)
{
    m_fmt.attach_mask(static_cast<gfx_mask_layer*>(m));
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::clear_masking(void)
{
    m_fmt.clear_mask();
}

template<typename Pixfmt> 
inline bool gfx_painter<Pixfmt>::begin_shadow(const rect_s& rc)
{
    m_draw_shadow = true;
    m_shadow_area = rc;

    unsigned int w = uround(rc.x2 - rc.x1);
    unsigned int h = uround(rc.y2 - rc.y1);
    m_shadow_buffer = (byte*)mem_calloc(1, h * w * 4);

    if (!m_shadow_buffer) {
        m_draw_shadow = false;
        return false;
    }

    m_shadow_rb.init(m_shadow_buffer, w, h, w * 4);
    m_shadow_fmt.attach(m_shadow_rb);
    m_shadow_base.attach(m_shadow_fmt);

    return true;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::apply_shadow(abstract_raster_adapter* rs, 
                                            const rect_s& r, const rgba& c, scalar x, scalar y, scalar blur)
{
    gfx_raster_adapter* ras = static_cast<gfx_raster_adapter*>(rs);

    gfx_renderer_scanline_aa_solid<gfx_renderer<pixfmt_rgba32> > ren(m_shadow_base);
    ren.color(c);

    if (ras->raster_method() & raster_fill) {
        gfx_render_scanlines(ras->fill_impl(), m_scanline_p, ren);
    }

    if (ras->raster_method() & raster_stroke) {
        gfx_render_scanlines(ras->stroke_impl(), m_scanline_p, ren);
    }

    if (blur > FLT_TO_SCALAR(0.0f)) {
        stack_blur<rgba8> b;
        b.set_shading(rgba8(c));
        b.blur(m_shadow_fmt, uround(blur * FLT_TO_SCALAR(40.0f)));
    }

    //Note: shadow need a no clip render base.
    renderer_base_type rb(m_fmt); 
    //blend shadow layer to base.
    rb.blend_from(m_shadow_fmt, 0, iround(x+r.x1), iround(y+r.y1));

    if (m_shadow_buffer) {
        mem_free(m_shadow_buffer);
        m_shadow_buffer = 0;
    }
    m_draw_shadow = false;
}

template<typename Pixfmt> 
inline void gfx_painter<Pixfmt>::copy_rect_from(abstract_rendering_buffer* src, const rect& rc, int x, int y)
{
    m_rb.copy_absolute_from(*static_cast<gfx_rendering_buffer*>(src), &rc, x, y);
}

#if ENABLE(FORMAT_RGBA)
template<> 
inline pix_fmt gfx_painter<pixfmt_rgba32>::pixel_format(void) const
{
    return pix_fmt_rgba;
}
#endif

#if ENABLE(FORMAT_ARGB)
template<> 
inline pix_fmt gfx_painter<pixfmt_argb32>::pixel_format(void) const
{
    return pix_fmt_argb;
}
#endif

#if ENABLE(FORMAT_ABGR)
template<> 
inline pix_fmt gfx_painter<pixfmt_abgr32>::pixel_format(void) const
{
    return pix_fmt_abgr;
}
#endif

#if ENABLE(FORMAT_BGRA)
template<> 
inline pix_fmt gfx_painter<pixfmt_bgra32>::pixel_format(void) const
{
    return pix_fmt_bgra;
}
#endif

#if ENABLE(FORMAT_RGB)
template<> 
inline pix_fmt gfx_painter<pixfmt_rgb24>::pixel_format(void) const
{
    return pix_fmt_rgb;
}
#endif

#if ENABLE(FORMAT_BGR)
template<> 
inline pix_fmt gfx_painter<pixfmt_bgr24>::pixel_format(void) const
{
    return pix_fmt_bgr;
}
#endif

#if ENABLE(FORMAT_RGB565)
template<> 
inline pix_fmt gfx_painter<pixfmt_rgb565>::pixel_format(void) const
{
    return pix_fmt_rgb565;
}
#endif

#if ENABLE(FORMAT_RGB555)
template<> 
inline pix_fmt gfx_painter<pixfmt_rgb555>::pixel_format(void) const
{
    return pix_fmt_rgb555;
}
#endif

}
#endif /*_GFX_PAINTER_H_*/

