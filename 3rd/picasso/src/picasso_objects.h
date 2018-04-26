/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PICASSO_OBJS_H_
#define _PICASSO_OBJS_H_

#include "common.h"
#include "math_type.h"
#include "data_vector.h"
#include "graphic_base.h"
#include "graphic_path.h"
#include "picasso_matrix.h"
#include "picasso_mask.h"
#include "picasso_font.h"
#include "picasso_gradient.h"
#include "picasso_painter.h"
#include "picasso_private.h"
#include "picasso_rendering_buffer.h"
#include "picasso_raster_adapter.h"

namespace picasso {

// pen object
enum {
    pen_style_solid    = 0,
    pen_style_image    = 1,
    pen_style_pattern  = 2,
    pen_style_gradient = 3,
    pen_style_canvas   = 4,
};

struct graphic_pen
{
    graphic_pen()
        : style(pen_style_solid)
        , width(FLT_TO_SCALAR(1.0f))
        , miter_limit(FLT_TO_SCALAR(4.0f))
        , cap(butt_cap)
        , join(miter_join)
        , inner(inner_miter)
        , color(0,0,0)
        , data(0)
        , is_dash(false)
        , dashes(0)
        , ndashes(0)
        , dstart(0)
    {
    }

    graphic_pen(const graphic_pen& o)
        : style(o.style)
        , width(o.width)
        , miter_limit(o.miter_limit)
        , cap(o.cap)
        , join(o.join)
        , inner(o.inner)
        , color(o.color)
        , data(0)
        , is_dash(o.is_dash)
        , dashes(0)
        , ndashes(0)
        , dstart(0)
    {
        if (style == pen_style_image)
            data = static_cast<void*>(ps_image_ref(static_cast<ps_image*>(o.data)));
        else if (style == pen_style_pattern)
            data = static_cast<void*>(ps_pattern_ref(static_cast<ps_pattern*>(o.data)));
        else if (style == pen_style_gradient)
            data = static_cast<void*>(ps_gradient_ref(static_cast<ps_gradient*>(o.data)));
        else if (style == pen_style_canvas)
            data = static_cast<void*>(ps_canvas_ref(static_cast<ps_canvas*>(o.data)));

        // set dashes
        if (is_dash) {
            ndashes = o.ndashes;
            dstart = o.dstart;
            dashes = new scalar[ndashes];
            if (dashes) {
                for (unsigned int i = 0; i < ndashes; i++)
                    dashes[i] = o.dashes[i];
            } else {
                //memory alloc error, ignore this dash
                //I think it is never happen.
                ndashes = 0;
                dstart = 0;
                is_dash = false;
            }
        }

    }

    graphic_pen& operator = (const graphic_pen& o)
    {
        if (this == &o)
            return *this;

        clear_dash();
        clear(); // free old data

        style = o.style;
        width = o.width;
        miter_limit = o.miter_limit;
        cap = o.cap;
        join = o.join;
        inner = o.inner;
        color = o.color;
        is_dash = o.is_dash;

        if (style == pen_style_image)
            data = static_cast<void*>(ps_image_ref(static_cast<ps_image*>(o.data)));
        else if (style == pen_style_pattern)
            data = static_cast<void*>(ps_pattern_ref(static_cast<ps_pattern*>(o.data)));
        else if (style == pen_style_gradient)
            data = static_cast<void*>(ps_gradient_ref(static_cast<ps_gradient*>(o.data)));
        else if (style == pen_style_canvas)
            data = static_cast<void*>(ps_canvas_ref(static_cast<ps_canvas*>(o.data)));

        // set dashes
        if (is_dash) {
            ndashes = o.ndashes;
            dstart = o.dstart;
            dashes = new scalar[ndashes];
            if (dashes) {
                for (unsigned int i = 0; i < ndashes; i++)
                    dashes[i] = o.dashes[i];
            } else {
                //memory alloc error, ignore this dash
                //I think it is never happen.
                ndashes = 0;
                dstart = 0;
                is_dash = false;
            }
        }

        return *this;
    }

    ~graphic_pen()
    {
        if (data) {
            if (style == pen_style_image)
                ps_image_unref(static_cast<ps_image*>(data));
            else if (style == pen_style_pattern)
                ps_pattern_unref(static_cast<ps_pattern*>(data));
            else if (style == pen_style_gradient)
                ps_gradient_unref(static_cast<ps_gradient*>(data));
            else if (style == pen_style_canvas)
                ps_canvas_unref(static_cast<ps_canvas*>(data));
            //add more..
            data = 0;
        }

        if (dashes) {
            delete [] dashes;
            dashes = 0;
        }
    }

    void set_dash(float start, const float* da, unsigned int ndash)
    {
        is_dash = true;
        dstart = FLT_TO_SCALAR(start);
        ndashes = (ndash+1)&~1;
        dashes = new scalar[ndashes];
        if (dashes) {
            memset(dashes, 0, ndashes * sizeof(scalar));
            for (unsigned int i = 0; i < ndash; i++)
                dashes[i] = FLT_TO_SCALAR(da[i]);
        } else {
            //memory alloc error, ignore this dash
            //I think it is never happen.
            ndashes = 0;
            dstart = 0;
            is_dash = false;
        }
    }

    void clear_dash()
    {
        dstart = 0;
        ndashes = 0;
        is_dash = false;
        if (dashes) {
            delete [] dashes;
            dashes = 0;
        }
    }

    void clear()
    {
        if (data) {
            if (style == pen_style_image)
                ps_image_unref(static_cast<ps_image*>(data));
            else if (style == pen_style_pattern)
                ps_pattern_unref(static_cast<ps_pattern*>(data));
            else if (style == pen_style_gradient)
                ps_gradient_unref(static_cast<ps_gradient*>(data));
            else if (style == pen_style_canvas)
                ps_canvas_unref(static_cast<ps_canvas*>(data));
            //add more..
            data = 0;
        }
        style = pen_style_solid;
    }

    void set_image_pen(ps_image * p)
    {
        style = pen_style_image;
        data = ps_image_ref(p);
    }

    void set_pattern_pen(ps_pattern * p)
    {
        style = pen_style_pattern;
        data = ps_pattern_ref(p);
    }

    void set_gradient_pen(ps_gradient * p)
    {
        style = pen_style_gradient;
        data = ps_gradient_ref(p);
    }

    void set_canvas_pen(ps_canvas * p)
    {
        style = pen_style_canvas;
        data = ps_canvas_ref(p);
    }

    unsigned int style;
    scalar width;
    scalar miter_limit;
    line_cap cap;
    line_join join;
    inner_join inner;
    rgba color;
    void* data;
    //dash
    bool is_dash;
    scalar * dashes;
    unsigned int ndashes;
    scalar dstart;
};

// brush object
enum {
    brush_style_solid    = 0,
    brush_style_image    = 1,
    brush_style_pattern  = 2,
    brush_style_gradient = 3,
    brush_style_canvas   = 4,
};

struct graphic_brush {
    graphic_brush()
        : style(brush_style_solid)
        , data(0)
        , rule(fill_non_zero)
        , color(0,0,0)
    {
    }

    graphic_brush(const graphic_brush& o)
        : style(o.style)
        , data(0)
        , rule(o.rule)
        , color(o.color)
    {
        if (style == brush_style_image)
            data = static_cast<void*>(ps_image_ref(static_cast<ps_image*>(o.data)));
        else if (style == brush_style_pattern)
            data = static_cast<void*>(ps_pattern_ref(static_cast<ps_pattern*>(o.data)));
        else if (style == brush_style_gradient)
            data = static_cast<void*>(ps_gradient_ref(static_cast<ps_gradient*>(o.data)));
        else if (style == brush_style_canvas)
            data = static_cast<void*>(ps_canvas_ref(static_cast<ps_canvas*>(o.data)));
    }

    graphic_brush& operator = (const graphic_brush& o)
    {
        if (this == &o)
            return *this;

        clear(); // free old data

        style = o.style;

        if (style == brush_style_image)
            data = static_cast<void*>(ps_image_ref(static_cast<ps_image*>(o.data)));
        else if (style == brush_style_pattern)
            data = static_cast<void*>(ps_pattern_ref(static_cast<ps_pattern*>(o.data)));
        else if (style == brush_style_gradient)
            data = static_cast<void*>(ps_gradient_ref(static_cast<ps_gradient*>(o.data)));
        else if (style == brush_style_canvas)
            data = static_cast<void*>(ps_canvas_ref(static_cast<ps_canvas*>(o.data)));

        rule = o.rule;
        color = o.color;

        return *this;
    }

    ~graphic_brush()
    {
        if (data) {
            if (style == brush_style_image)
                ps_image_unref(static_cast<ps_image*>(data));
            else if (style == brush_style_pattern)
                ps_pattern_unref(static_cast<ps_pattern*>(data));
            else if (style == brush_style_gradient)
                ps_gradient_unref(static_cast<ps_gradient*>(data));
            else if (style == brush_style_canvas)
                ps_canvas_unref(static_cast<ps_canvas*>(data));
            //add more..
            data = 0;
        }
    }

    void clear()
    {
        if (data) {
            if (style == brush_style_image)
                ps_image_unref(static_cast<ps_image*>(data));
            else if (style == brush_style_pattern)
                ps_pattern_unref(static_cast<ps_pattern*>(data));
            else if (style == brush_style_gradient)
                ps_gradient_unref(static_cast<ps_gradient*>(data));
            else if (style == brush_style_canvas)
                ps_canvas_unref(static_cast<ps_canvas*>(data));
            //add more..
            data = 0;
        }
        style = brush_style_solid;
    }

    void set_image_brush(ps_image * p)
    {
        style = brush_style_image;
        data = ps_image_ref(p);
    }

    void set_pattern_brush(ps_pattern * p)
    {
        style = brush_style_pattern;
        data = ps_pattern_ref(p);
    }

    void set_gradient_brush(ps_gradient * p)
    {
        style = brush_style_gradient;
        data = ps_gradient_ref(p);
    }

    void set_canvas_brush(ps_canvas * p)
    {
        style = brush_style_canvas;
        data = ps_canvas_ref(p);
    }

    unsigned int style;
    void* data;
    filling_rule rule;
    rgba color;
};


//shadow object
struct shadow_state {
    shadow_state()
        : use_shadow(false)
        , x_offset(FLT_TO_SCALAR(0.0f))
        , y_offset(FLT_TO_SCALAR(0.0f))
        , blur(FLT_TO_SCALAR(0.375f)) /* 0 ~ 1 */
        , color(0,0,0,FLT_TO_SCALAR(0.33f))
    {
    }

    shadow_state(const shadow_state& o)
        : use_shadow(o.use_shadow)
        , x_offset(o.x_offset)
        , y_offset(o.y_offset)
        , blur(o.blur)
        , color(o.color)
    {
    }

    shadow_state& operator = (const shadow_state& o)
    {
        if (this == &o)
            return *this;

        use_shadow = o.use_shadow;
        x_offset = o.x_offset;
        y_offset = o.y_offset;
        blur = o.blur;
        color = o.color;
        return *this;
    }

    bool use_shadow;
    scalar x_offset;
    scalar y_offset;
    scalar blur;
    rgba color;
};

// clip area object
enum  {
    clip_none    = 0,
    clip_content = 1,
    clip_device  = 2,
};

struct clip_area {
    clip_area()
        : type(clip_none)
        , rule(fill_non_zero)
        , rect(0,0,0,0)
    {
    }

    clip_area(const clip_area& o)
        : type(o.type)
    {
        path = o.path;
        rule = o.rule;
        rect = o.rect;
    }

    clip_area& operator = (const clip_area& o)
    {
        if (this == &o)
            return *this;

        type = o.type;
        path = o.path;
        rule = o.rule;
        rect = o.rect;

        return *this;
    }

    bool is_not_same(const clip_area& o)
    {
        return (type != o.type) ||
               (path != o.path) ||
               (rule != o.rule) ||
               (rect.x1 != o.rect.x1) ||
               (rect.y1 != o.rect.y1) ||
               (rect.x2 != o.rect.x2) ||
               (rect.y2 != o.rect.y2);
    }

    unsigned int type;
    graphic_path path;
    filling_rule rule;
    rect_s       rect;
};


// context state object
struct context_state {
    context_state()
        : next(0)
        , filter(FILTER_BILINEAR)
        , font(ps_font_ref(_default_font()))
        , antialias(true)
        , gamma(FLT_TO_SCALAR(1.0f))
        , alpha(FLT_TO_SCALAR(1.0f))
        , blur(FLT_TO_SCALAR(0.0f))
        , font_scolor(0,0,0)
        , font_fcolor(0,0,0)
        , composite(comp_op_src_over)
    {
    }

    ~context_state()
    {
        ps_font_unref(font);
    }

    context_state(const context_state& o)
        : next(0)
        , filter(o.filter)
        , font(ps_font_ref(o.font))
        , antialias(o.antialias)
        , gamma(o.gamma)
        , alpha(o.alpha)
        , blur(o.blur)
        , font_scolor(o.font_scolor)
        , font_fcolor(o.font_fcolor)
        , composite(o.composite)
        , world_matrix(o.world_matrix)
        , pen(o.pen)
        , brush(o.brush)
        , clip(o.clip)
        , shadow(o.shadow)
    {
    }

    context_state& operator = (const context_state& o)
    {
        if (this == &o)
            return *this;

        next = 0;
        filter = o.filter;
        ps_font_unref(font); //free old font
        font = ps_font_ref(o.font);//reference new one
        antialias = o.antialias;
        gamma = o.gamma;
        alpha = o.alpha;
        blur = o.blur;
        font_scolor = o.font_scolor;
        font_fcolor = o.font_fcolor;
        composite = o.composite;
        world_matrix = o.world_matrix;
        pen = o.pen;
        brush = o.brush;
        clip = o.clip;
        shadow = o.shadow;

        return *this;
    }

    struct context_state * next;

    ps_filter filter;
    ps_font * font;
    bool antialias;
    scalar gamma;
    scalar alpha;
    scalar blur;
    rgba font_scolor;
    rgba font_fcolor;
    comp_op composite;
    trans_affine world_matrix;
    graphic_pen pen;
    graphic_brush brush;
    clip_area clip;
    shadow_state shadow;
};

}

#ifdef __cplusplus
extern "C" {
#endif

struct _ps_context {
    int refcount;
    ps_canvas* canvas;
    picasso::context_state* state;
    ps_bool font_antialias;
    ps_bool font_kerning;
    ps_text_type font_render_type;
    struct _ps_context* parent;
    picasso::font_engine* fonts;
    picasso::trans_affine text_matrix;
    picasso::graphic_path path;
    picasso::raster_adapter raster;
};

enum {
    buffer_alloc_none      = 0,
    buffer_alloc_surface   = 1,
    buffer_alloc_malloc    = 2,
    buffer_alloc_image     = 3,
    buffer_alloc_canvas    = 4,
};

struct _ps_canvas {
    int refcount;
    ps_color_format fmt;
    picasso::painter *p;
    unsigned int flage;
    void      *host;
    ps_mask* mask;
    picasso::rendering_buffer buffer;
};

struct _ps_image {
    int refcount;
    ps_color_format fmt;
    unsigned int flage;
    void      *host;
    picasso::rendering_buffer buffer;
};

struct _ps_pattern {
    int refcount;
    picasso::trans_affine matrix;
    ps_wrap_type xtype;
    ps_wrap_type ytype;
    ps_image *img;
};

struct _ps_gradient {
    int refcount;
    picasso::gradient_adapter gradient;
};

struct _ps_matrix {
    int refcount;
    picasso::trans_affine matrix;
};

struct _ps_path {
    int refcount;
    picasso::graphic_path path;
};

struct _ps_mask {
    int refcount;
    picasso::mask_layer mask;
};

struct _ps_font {
    int refcount;
    picasso::font_desc desc;
};

#ifdef __cplusplus
}
#endif
#endif/*_PICASSO_OBJS_H_*/
