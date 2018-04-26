/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "interfaces.h"
#include "convert.h"
#include "graphic_path.h"
#include "graphic_helper.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_painter.h"
#include "picasso_objects.h"
#include "picasso_font.h"

namespace picasso {

painter::painter(pix_fmt fmt)
{
    m_impl = get_system_device()->create_painter(fmt);
}

painter::~painter()
{
    get_system_device()->destroy_painter(m_impl);
}

void painter::attach(rendering_buffer& buf)
{
    m_impl->attach(buf.m_impl);
}

void painter::init_raster_data(context_state* state, unsigned int methods,
                                    raster_adapter& raster, const vertex_source& p, const trans_affine& mtx)
{
    raster.set_raster_method(methods);

    if (methods & raster_stroke) {
        if (state->pen.is_dash) {
            raster.set_stroke_dashes(state->pen.dstart, state->pen.dashes, state->pen.ndashes); //dash line
        }

        raster.set_stroke_attr_val(STA_WIDTH, state->pen.width);
        raster.set_stroke_attr_val(STA_MITER_LIMIT, state->pen.miter_limit);
        raster.set_stroke_attr(STA_LINE_CAP, state->pen.cap);
        raster.set_stroke_attr(STA_LINE_JOIN, state->pen.join);
        raster.set_stroke_attr(STA_INNER_JOIN, state->pen.inner);
    }

    if (methods & raster_fill) {
        raster.set_fill_attr(FIA_FILL_RULE, state->brush.rule);
    }

    raster.set_transform(mtx);
    raster.add_shape(p, 0);
}

void painter::init_source_data(context_state* state, unsigned int methods, const graphic_path& p)
{
    m_impl->set_alpha(state->alpha);
    m_impl->set_composite(state->composite);

    if (methods & raster_stroke) {
        switch (state->pen.style) {
            case pen_style_canvas:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_canvas* canvas = static_cast<ps_canvas*>(state->pen.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_stroke_canvas(canvas->buffer.impl(), (pix_fmt)(canvas->fmt), (int)state->filter, rect);
                }
                break;
            case pen_style_pattern:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_pattern* pattern = static_cast<ps_pattern*>(state->pen.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_stroke_pattern(pattern->img->buffer.impl(), (pix_fmt)(pattern->img->fmt), (int)state->filter, rect,
                                                                            pattern->xtype, pattern->ytype, pattern->matrix.impl());
                }
                break;
            case pen_style_image:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_image* img = static_cast<ps_image*>(state->pen.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_stroke_image(img->buffer.impl(), (pix_fmt)(img->fmt),(int)state->filter, rect);
                }
                break;
            case pen_style_gradient:
                {
                    ps_gradient* gradient = static_cast<ps_gradient*>(state->pen.data);
                    m_impl->set_stroke_gradient(gradient->gradient.impl());
                }
                break;
            case pen_style_solid:
                m_impl->set_stroke_color(state->pen.color); //solid color pen.
                break;
            default:
                //make compiler happy only.
                break;
        }
    }

    if (methods & raster_fill) {
        switch (state->brush.style) {
            case brush_style_canvas:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_canvas* canvas = static_cast<ps_canvas*>(state->brush.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_fill_canvas(canvas->buffer.impl(), (pix_fmt)(canvas->fmt), (int)state->filter, rect);
                }
                break;
            case brush_style_pattern:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_pattern* pattern = static_cast<ps_pattern*>(state->brush.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_fill_pattern(pattern->img->buffer.impl(), (pix_fmt)(pattern->img->fmt), (int)state->filter, rect,
                                                                            pattern->xtype, pattern->ytype, pattern->matrix.impl());
                }
                break;
            case brush_style_image:
                {
                    scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
                    bounding_rect(const_cast<graphic_path&>(p), 0, &x1, &y1, &x2, &y2);
                    ps_image* img = static_cast<ps_image*>(state->brush.data);
                    rect_s rect(x1, y1, x2, y2);
                    m_impl->set_fill_image(img->buffer.impl(), (pix_fmt)(img->fmt),(int)state->filter, rect);
                }
                break;
            case brush_style_gradient:
                {
                    ps_gradient* gradient = static_cast<ps_gradient*>(state->brush.data);
                    m_impl->set_fill_gradient(gradient->gradient.impl());
                }
                break;
            case brush_style_solid:
                m_impl->set_fill_color(state->brush.color); //solid color brush.
                break;
            default:
                //make compiler happy only.
                break;
        }
    }
}

void painter::render_stroke(context_state* state, raster_adapter& raster, const graphic_path& p)
{
    if (raster.is_empty())
        init_raster_data(state, raster_stroke, raster, p, state->world_matrix);

    init_source_data(state, raster_stroke, p);

    raster.commit(); //calc raster data.
    m_impl->apply_stroke(raster.impl());
}

void painter::render_fill(context_state* state, raster_adapter& raster, const graphic_path& p)
{
    if (raster.is_empty())
        init_raster_data(state, raster_fill, raster, p, state->world_matrix);

    init_source_data(state, raster_fill, p);

    raster.commit(); //calc raster data.
    m_impl->apply_fill(raster.impl());
}

void painter::render_paint(context_state* state, raster_adapter& raster, const graphic_path& p)
{
    if (raster.is_empty())
        init_raster_data(state, raster_fill | raster_stroke, raster, p, state->world_matrix);

    init_source_data(state, raster_fill | raster_stroke, p);

    raster.commit(); //calc raster data.
    m_impl->apply_fill(raster.impl());
    m_impl->apply_stroke(raster.impl());
}

void painter::render_clear(context_state* state)
{
    m_impl->apply_clear(state->brush.color);
}

void painter::render_blur(context_state* state)
{
    m_impl->apply_blur(state->blur);
}

void painter::render_gamma(context_state* state, raster_adapter& raster)
{
    if (state->antialias) {
        raster.set_antialias(true);
        raster.set_gamma_power(state->gamma);
    } else {
        raster.set_antialias(false);
        raster.set_gamma_power(state->gamma);
    }
}

void painter::render_clip(context_state* state, bool clip)
{
    if (clip) {
        if (state->clip.type != clip_none) { // need clip
            m_impl->clear_clip(); // clear old clip.

            if (state->clip.type == clip_content)
                m_impl->apply_clip_path(state->clip.path, state->clip.rule, state->world_matrix.impl());
            else if (state->clip.type == clip_device)
                m_impl->apply_clip_device(state->clip.rect, 0, 0);
        }
    } else {
        m_impl->clear_clip();
    }
}

void painter::render_shadow(context_state* state, const graphic_path& p, bool fill, bool stroke)
{
    if (state->shadow.use_shadow) {

        unsigned int method = 0;
        if (fill)
            method |= raster_fill;
        if (stroke)
            method |= raster_stroke;

        scalar x1 = 1, y1 = 1, x2 = 0 ,y2 = 0;
        conv_transform tp(p, state->world_matrix);
        bounding_rect(tp, 0, &x1, &y1, &x2, &y2);

        //FIXME: it is hard code here right?
        x1 -= (state->shadow.blur*40+5);
        y1 -= (state->shadow.blur*40+5);
        x2 += (state->shadow.blur*40+5);
        y2 += (state->shadow.blur*40+5);

        rect_s rect(x1, y1, x2, y2);

        trans_affine mtx = state->world_matrix;
        mtx.translate(-x1, -y1); // translate to (0,0) of shadow layer.

        if (m_impl->begin_shadow(rect)){ //switch to shadow layer.

            raster_adapter shadow_raster;

            init_raster_data(state, method, shadow_raster, p, mtx);

            m_impl->set_alpha(state->alpha);
            m_impl->set_composite(state->composite);

            if (state->clip.type != clip_none) { // need clip
                m_impl->clear_clip(); // clear old clip.

                if (state->clip.type == clip_content)
                    m_impl->apply_clip_path(state->clip.path, state->clip.rule, mtx.impl());
                else if (state->clip.type == clip_device)
                    m_impl->apply_clip_device(state->clip.rect, -x1, -y1);
            }

            shadow_raster.commit();
            m_impl->apply_shadow(shadow_raster.impl(), rect,
                    state->shadow.color, state->shadow.x_offset, state->shadow.y_offset, state->shadow.blur);

            shadow_raster.reset();
        }
    }
}

void painter::render_mask(const mask_layer& m, bool mask)
{
    if (mask) {
        m_impl->apply_masking(m.impl());
    } else {
        m_impl->clear_masking();
    }
}

void painter::render_copy(rendering_buffer& src, const rect* r, const painter* dst, int off_x, int off_y)
{
    if (r) {
        rect rc(r->x1, r->y1, r->x2, r->y2);
        dst->m_impl->copy_rect_from(src.impl(), rc, off_x, off_y);
    } else {
        rect rc(0, 0, src.width(), src.height());
        dst->m_impl->copy_rect_from(src.impl(), rc, off_x, off_y);
    }
}

void painter::render_glyph(context_state* state, raster_adapter& raster, const font_adapter* font, int type)
{
    //FIXME: support other source !
    m_impl->set_alpha(state->alpha);
    m_impl->set_composite(state->composite);

    m_impl->set_font_fill_color(state->font_fcolor);

    if (type == glyph_type_mono) {
        mono_storage& mono = const_cast<font_adapter*>(font)->mono_adaptor();
        scalar tx = state->world_matrix.tx();
        scalar ty = state->world_matrix.ty();
        mono.translate(tx, ty);
        m_impl->apply_mono_text_fill(mono.get_storage());
    } else {
        conv_curve curve(const_cast<font_adapter*>(font)->path_adaptor());
        //FIXME: support stroke feature!
        init_raster_data(state, raster_fill, raster, curve, state->world_matrix);

        raster.commit(); //calc raster data.
    }
}

void painter::render_glyphs_raster(context_state* state, raster_adapter& raster, int style)
{
    if (!raster.is_empty()) {
        m_impl->apply_text_fill(raster.impl(), (text_style)style);
        raster.reset();
    }
}

}

