/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2012 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "graphic_path.h"
#include "geometry.h"
#include "convert.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_objects.h"
#include "picasso_painter.h"
#include "picasso_private.h"

namespace picasso {

static inline void _clip_path(context_state* state, const graphic_path& p, filling_rule r)
{
    if (!state->clip.path.total_vertices()) {
        state->clip.path = p;
    } else if (p.total_vertices()) {
        graphic_path rp;
        _path_operation(conv_clipper::clip_intersect, state->clip.path, p, rp);
        state->clip.path = rp;
    }
    state->clip.rule = r;
}

}

#define PICASSO_VERSION 21050     // version 2.1.5

#ifdef __cplusplus
extern "C" {
#endif

ps_status global_status = STATUS_SUCCEED;

int PICAPI ps_version(void)
{
    return PICASSO_VERSION;
}

ps_bool PICAPI ps_initialize(void)
{
    return (picasso::_init_system_device()
           && picasso::font_engine::initialize()
           && picasso::_init_default_font()) ? True : False;
}

void PICAPI ps_shutdown(void)
{
    picasso::_destory_default_font();
    picasso::font_engine::shutdown();
    picasso::_destroy_system_device();
}

ps_status PICAPI ps_last_status(void)
{
    return global_status;
}

ps_context* PICAPI ps_context_create(ps_canvas* canvas, ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    picasso::context_state * state = new picasso::context_state;
    if (!state) {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }

    ps_context *c = (ps_context*)mem_malloc(sizeof(ps_context));
    if (c) {
        c->refcount = 1;
        c->canvas = ps_canvas_ref(canvas);
        c->state = state;
        c->font_antialias = True;
        c->font_kerning = True;
        c->font_render_type = TEXT_TYPE_STROKE;
        if (ctx) {
            c->parent = ps_context_ref(ctx);
            c->fonts = ctx->fonts;
        } else {
            c->parent = 0;
            c->fonts = new picasso::font_engine;
        }
        new ((void*)&(c->text_matrix)) picasso::trans_affine;
        new ((void*)&(c->path)) picasso::graphic_path;
        new ((void*)&(c->raster)) picasso::raster_adapter;
        global_status = STATUS_SUCCEED;
        return c;
    } else {
        delete state;     // free state on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_context_set_canvas(ps_context* ctx, ps_canvas* canvas)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!ctx || !canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }
    ps_canvas* old = ctx->canvas; // context's canvas must more than 2
    ctx->canvas = ps_canvas_ref(canvas);
    ps_canvas_unref(old);// release context's reference
    global_status = STATUS_SUCCEED;
    return old;
}

ps_canvas* PICAPI ps_context_get_canvas(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }
    
    ps_canvas* canvas= ctx->canvas;
    global_status = STATUS_SUCCEED;
    return canvas;
}

ps_context* PICAPI ps_context_ref(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ctx->refcount++;
    global_status = STATUS_SUCCEED;
    return ctx;
}

void PICAPI ps_context_unref(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->refcount--;
    if (ctx->refcount <= 0) {
        ps_canvas_unref(ctx->canvas);
        while (ctx->state) {
            picasso::context_state * p = ctx->state;
            ctx->state = ctx->state->next;
            delete p;
        }
        if (ctx->parent) {
            ps_context_unref(ctx->parent);
        } else {
            delete ctx->fonts;
        }
        (&ctx->path)->graphic_path::~graphic_path();
        (&ctx->raster)->raster_adapter::~raster_adapter();
        (&ctx->text_matrix)->trans_affine::~trans_affine();
        mem_free(ctx);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_source_gradient(ps_context* ctx, const ps_gradient* gradient)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !gradient) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->brush.clear(); //clear source
    ctx->state->brush.set_gradient_brush(const_cast<ps_gradient*>(gradient));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_source_pattern(ps_context* ctx, const ps_pattern* pattern)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !pattern) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (pattern->img->fmt != ctx->canvas->fmt) {
        global_status = STATUS_MISMATCHING_FORMAT;
        return;
    }

    ctx->state->brush.clear(); //clear source
    ctx->state->brush.set_pattern_brush(const_cast<ps_pattern*>(pattern));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_source_image(ps_context* ctx, const ps_image* image)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !image) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (image->fmt != ctx->canvas->fmt) {
        global_status = STATUS_MISMATCHING_FORMAT;
        return;
    }

    ctx->state->brush.clear(); //clear source
    ctx->state->brush.set_image_brush(const_cast<ps_image*>(image));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_source_color(ps_context* ctx, const ps_color* color)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !color) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->state->brush.clear(); //clear source
    ctx->state->brush.color.r = FLT_TO_SCALAR(color->r);
    ctx->state->brush.color.g = FLT_TO_SCALAR(color->g);
    ctx->state->brush.color.b = FLT_TO_SCALAR(color->b);
    ctx->state->brush.color.a = FLT_TO_SCALAR(color->a);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_source_canvas(ps_context* ctx, const ps_canvas* canvas)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (canvas->fmt != ctx->canvas->fmt) {
        global_status = STATUS_MISMATCHING_FORMAT;
        return;
    }

    ctx->state->brush.clear(); //clear source
    ctx->state->brush.set_canvas_brush(const_cast<ps_canvas*>(canvas));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_stroke_color(ps_context* ctx, const ps_color* color)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !color) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->state->pen.color.r = FLT_TO_SCALAR(color->r);
    ctx->state->pen.color.g = FLT_TO_SCALAR(color->g);
    ctx->state->pen.color.b = FLT_TO_SCALAR(color->b);
    ctx->state->pen.color.a = FLT_TO_SCALAR(color->a);
    global_status = STATUS_SUCCEED;
}

ps_filter PICAPI ps_set_filter(ps_context* ctx, ps_filter filter)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return FILTER_UNKNOWN;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return FILTER_UNKNOWN;
    }

    ps_filter old = ctx->state->filter;
    ctx->state->filter = filter;
       global_status = STATUS_SUCCEED;
    return old;
}


void PICAPI ps_stroke(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->canvas->p->render_shadow(ctx->state, ctx->path, false, true);
    ctx->canvas->p->render_stroke(ctx->state, ctx->raster, ctx->path);
    ctx->canvas->p->render_blur(ctx->state);
    ctx->path.free_all();
    ctx->raster.reset();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_fill(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->canvas->p->render_shadow(ctx->state, ctx->path, true, false);
    ctx->canvas->p->render_fill(ctx->state, ctx->raster, ctx->path);
    ctx->canvas->p->render_blur(ctx->state);
    ctx->path.free_all();
    ctx->raster.reset();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_paint(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->canvas->p->render_shadow(ctx->state, ctx->path, true, true);
    ctx->canvas->p->render_paint(ctx->state, ctx->raster, ctx->path);
    ctx->canvas->p->render_blur(ctx->state);
    ctx->path.free_all();
    ctx->raster.reset();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_clear(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    
    ctx->canvas->p->render_clear(ctx->state);
    global_status = STATUS_SUCCEED;
}

float PICAPI ps_set_alpha(ps_context* ctx, float a)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0.0f;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0.0f;
    }

    //alpha range 0 ~ 1
    if (a < 0.0f)
        a = 0.0f;
    else if (a > 1.0f)
        a = 1.0f;

    float rd = SCALAR_TO_FLT(ctx->state->alpha);
    ctx->state->alpha = FLT_TO_SCALAR(a);
    global_status = STATUS_SUCCEED;
    return rd;
}

float PICAPI ps_set_blur(ps_context* ctx, float b)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0.0f;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0.0f;
    }

    //blur range 0 ~ 1
    if (b < 0.0f)
        b = 0.0f;
    else if (b > 1.0f)
        b = 1.0f;

    float rd = SCALAR_TO_FLT(ctx->state->blur);
    ctx->state->blur = FLT_TO_SCALAR(b);
    global_status = STATUS_SUCCEED;
    return rd;
}

float PICAPI ps_set_gamma(ps_context* ctx, float g)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0.0f;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0.0f;
    }

    //gamma range 0.6 ~ 3.0
    if (g < 0.6f)
        g = 0.6f;
    else if (g > 3.0f)
        g = 3.0f;

    float rd = SCALAR_TO_FLT(ctx->state->gamma);
    if (rd != g) {
        ctx->state->gamma = FLT_TO_SCALAR(g);
        ctx->canvas->p->render_gamma(ctx->state, ctx->raster);
    }
    global_status = STATUS_SUCCEED;
    return rd;
}

void PICAPI ps_set_antialias(ps_context* ctx, ps_bool anti)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ps_bool old = ctx->state->antialias ? True : False;
    if (old != anti) {
        ctx->state->antialias = anti ? true : false;
        ctx->canvas->p->render_gamma(ctx->state, ctx->raster);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_shadow(ps_context* ctx, float x, float y, float b)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    //blur range 0 ~ 1
    if (b < 0.0f)
        b = 0.0f;
    else if (b > 1.0f)
        b = 1.0f;

    ctx->state->shadow.use_shadow = true;
    ctx->state->shadow.x_offset = FLT_TO_SCALAR(x);
    ctx->state->shadow.y_offset = FLT_TO_SCALAR(y);
    ctx->state->shadow.blur = FLT_TO_SCALAR(b);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_shadow_color(ps_context* ctx, const ps_color* c)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->state->shadow.color = 
        picasso::rgba(FLT_TO_SCALAR(c->r), FLT_TO_SCALAR(c->g), FLT_TO_SCALAR(c->b), FLT_TO_SCALAR(c->a));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_reset_shadow(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->state->shadow = picasso::shadow_state();
    global_status = STATUS_SUCCEED;
}

ps_fill_rule PICAPI ps_set_fill_rule(ps_context* ctx, ps_fill_rule rule)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return FILL_RULE_ERROR;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return FILL_RULE_ERROR;
    }

    ps_fill_rule rl = (ps_fill_rule)(ctx->state->brush.rule);
    switch (rule) 
    {
        case FILL_RULE_WINDING:
            ctx->state->brush.rule = picasso::fill_non_zero;
            break;
        case FILL_RULE_EVEN_ODD:
            ctx->state->brush.rule = picasso::fill_even_odd;
            break;
        default:
            global_status = STATUS_UNKNOWN_ERROR;
            return FILL_RULE_ERROR;
    }
    global_status = STATUS_SUCCEED;
    return rl;
}

void PICAPI ps_set_line_cap(ps_context* ctx, ps_line_cap line_cap)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    switch (line_cap) 
    {
        case LINE_CAP_BUTT:
            ctx->state->pen.cap = picasso::butt_cap;
            break;
        case LINE_CAP_SQUARE:
            ctx->state->pen.cap = picasso::square_cap;
            break;
        case LINE_CAP_ROUND:
            ctx->state->pen.cap = picasso::round_cap;
            break;
        default:
            global_status = STATUS_UNKNOWN_ERROR;
            return;
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_line_inner_join(ps_context* ctx, ps_line_inner_join line_inner_join)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    switch (line_inner_join) 
    {
        case LINE_INNER_MITER:
            ctx->state->pen.inner = picasso::inner_miter;
            break;
        case LINE_INNER_ROUND:
            ctx->state->pen.inner = picasso::inner_round;
            break;
        case LINE_INNER_BEVEL:
            ctx->state->pen.inner = picasso::inner_bevel;
            break;
        case LINE_INNER_JAG:
            ctx->state->pen.inner = picasso::inner_jag;
            break;
        default:
            global_status = STATUS_UNKNOWN_ERROR;
            return;
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_line_join(ps_context* ctx, ps_line_join line_join)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    switch (line_join) 
    {
        case LINE_JOIN_MITER:
            ctx->state->pen.join = picasso::miter_join;
            break;
        case LINE_JOIN_MITER_REVERT:
            ctx->state->pen.join = picasso::miter_join_revert;
            break;
        case LINE_JOIN_MITER_ROUND:
            ctx->state->pen.join = picasso::miter_join_round;
            break;
        case LINE_JOIN_ROUND:
            ctx->state->pen.join = picasso::round_join;
            break;
        case LINE_JOIN_BEVEL:
            ctx->state->pen.join = picasso::bevel_join;
            break;
        default:
            global_status = STATUS_UNKNOWN_ERROR;
            return;
    }
    global_status = STATUS_SUCCEED;
}

float PICAPI ps_set_line_width(ps_context* ctx, float width)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0.0f;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0.0f;
    }

    if (width < 0.0f)
        width = 0.0f;

    float rw = SCALAR_TO_FLT(ctx->state->pen.width);
    ctx->state->pen.width = FLT_TO_SCALAR(width);
    global_status = STATUS_SUCCEED;
    return rw;
}

float PICAPI ps_set_miter_limit(ps_context* ctx, float limit)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0.0f;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0.0f;
    }

    if (limit < 0.0f)
        limit = 0.0f;

    float rd = SCALAR_TO_FLT(ctx->state->pen.miter_limit);
    ctx->state->pen.miter_limit = FLT_TO_SCALAR(limit);
    global_status = STATUS_SUCCEED;
    return rd;
}

void PICAPI ps_set_line_dash(ps_context* ctx, float start, float* dashes, unsigned int num_dashes)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !dashes || !num_dashes) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (start < 0.0f)
        start = 0.0f;

    ctx->state->pen.clear_dash();
    ctx->state->pen.set_dash(start, dashes, num_dashes);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_reset_line_dash(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->state->pen.clear_dash();
    global_status = STATUS_SUCCEED;
}

//path function
void PICAPI ps_new_path(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.free_all();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_new_sub_path(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.end_poly();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_rectangle(ps_context* ctx, const ps_rect* pr)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !pr) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.set_shape(picasso::graphic_path::shape_rectangle); //It is because boder edge.
    ctx->path.move_to(FLT_TO_SCALAR(floor(pr->x)), FLT_TO_SCALAR(floor(pr->y)));
    ctx->path.hline_rel(FLT_TO_SCALAR(floor(pr->w)));
    ctx->path.vline_rel(FLT_TO_SCALAR(floor(pr->h)));
    ctx->path.hline_rel(-FLT_TO_SCALAR(floor(pr->w)));
    ctx->path.vline_rel(-FLT_TO_SCALAR(floor(pr->h)));
    ctx->path.end_poly();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_rounded_rect(ps_context* ctx, const ps_rect* r, float ltx, float lty, float rtx, float rty,
                                                                    float lbx, float lby, float rbx, float rby)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::rounded_rect rr;
    rr.rect(FLT_TO_SCALAR(floor(r->x)), FLT_TO_SCALAR(floor(r->y)), 
                    FLT_TO_SCALAR(floor(r->x)+r->w), FLT_TO_SCALAR(floor(r->y)+r->h));
    rr.radius(FLT_TO_SCALAR(ltx), FLT_TO_SCALAR(lty), FLT_TO_SCALAR(rtx), FLT_TO_SCALAR(rty), 
                    FLT_TO_SCALAR(rbx), FLT_TO_SCALAR(rby), FLT_TO_SCALAR(lbx), FLT_TO_SCALAR(lby));
    rr.normalize_radius();
    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(rr, 0);
    else
        ctx->path.join_path(rr, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_ellipse(ps_context* ctx, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::ellipse e(FLT_TO_SCALAR(floor(r->x)+r->w/2), FLT_TO_SCALAR(floor(r->y)+r->h/2), 
                                                FLT_TO_SCALAR(r->w/2), FLT_TO_SCALAR(r->h/2));
    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(e, 0);
    else
        ctx->path.join_path(e, 0);
    global_status = STATUS_SUCCEED;
}


void PICAPI ps_close_path(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.close_polygon();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_move_to(ps_context* ctx, const ps_point* pt)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !pt) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.move_to(FLT_TO_SCALAR(floor(pt->x)), FLT_TO_SCALAR(floor(pt->y)));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_line_to(ps_context* ctx, const ps_point* pt)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !pt) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path.line_to(FLT_TO_SCALAR(floor(pt->x)), FLT_TO_SCALAR(floor(pt->y)));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_bezier_curve_to(ps_context* ctx, const ps_point* fcp, 
                                            const ps_point* scp, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !fcp || !scp || !ep) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::curve4 c(FLT_TO_SCALAR(floor(ctx->path.last_x())), FLT_TO_SCALAR(floor(ctx->path.last_y())), 
            FLT_TO_SCALAR(floor(fcp->x)), FLT_TO_SCALAR(floor(fcp->y)), FLT_TO_SCALAR(floor(scp->x)), 
            FLT_TO_SCALAR(floor(scp->y)), FLT_TO_SCALAR(floor(ep->x)), FLT_TO_SCALAR(floor(ep->y)));

    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(c, 0);
    else
        ctx->path.join_path(c, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_quad_curve_to(ps_context* ctx, const ps_point* cp, const ps_point* ep)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !cp || !ep) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::curve3 c(FLT_TO_SCALAR(floor(ctx->path.last_x())), FLT_TO_SCALAR(floor(ctx->path.last_y())), 
                                        FLT_TO_SCALAR(floor(cp->x)), FLT_TO_SCALAR(floor(cp->y)), 
                                        FLT_TO_SCALAR(floor(ep->x)), FLT_TO_SCALAR(floor(ep->y)));

    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(c, 0);
    else
        ctx->path.join_path(c, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_arc(ps_context* ctx, const ps_point* cp, float r, 
                                            float sa, float ea, ps_bool clockwise)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !cp || r <=0.0f ) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::arc a(FLT_TO_SCALAR(floor(cp->x)), FLT_TO_SCALAR(floor(cp->y)), 
            FLT_TO_SCALAR(r), FLT_TO_SCALAR(r), FLT_TO_SCALAR(sa), FLT_TO_SCALAR(ea), (clockwise ? true : false));

    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(a, 0);
    else
        ctx->path.join_path(a, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_tangent_arc(ps_context* ctx, const ps_rect* r, float sa, float sw)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    scalar xr = FLT_TO_SCALAR(r->w / 2);
    scalar yr = FLT_TO_SCALAR(r->h / 2);
    scalar cx = FLT_TO_SCALAR(r->x + xr);
    scalar cy = FLT_TO_SCALAR(r->y + yr);

    picasso::bezier_arc ba(Floor(cx), Floor(cy), xr, yr, FLT_TO_SCALAR(sa), FLT_TO_SCALAR(sw));
    picasso::conv_curve cr(ba);

    if (picasso::_is_closed_path(ctx->path))
        ctx->path.concat_path(cr, 0);
    else
        ctx->path.join_path(cr, 0);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_path(ps_context* ctx, const ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->path = path->path;
    global_status = STATUS_SUCCEED;
}

// transform world
void PICAPI ps_translate(ps_context* ctx, float tx, float ty)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix.translate(FLT_TO_SCALAR(tx), FLT_TO_SCALAR(ty));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_scale(ps_context* ctx, float sx, float sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix.scale(FLT_TO_SCALAR(sx), FLT_TO_SCALAR(sy));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_shear(ps_context* ctx, float sx, float sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix.shear(FLT_TO_SCALAR(sx), FLT_TO_SCALAR(sy));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_rotate(ps_context* ctx, float angle)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix.rotate(FLT_TO_SCALAR(angle));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_identity(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix.reset();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_transform(ps_context* ctx, const ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix *= matrix->matrix;
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_set_matrix(ps_context* ctx, const ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    ctx->state->world_matrix = matrix->matrix;
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_get_matrix(ps_context* ctx, ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!ctx || !matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }
    matrix->matrix = ctx->state->world_matrix;
    global_status = STATUS_SUCCEED;
    return True;
}

void PICAPI ps_world_to_viewport(ps_context* ctx, ps_point* point)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx ||!point) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    scalar x = FLT_TO_SCALAR(point->x);
    scalar y = FLT_TO_SCALAR(point->y);
    ctx->state->world_matrix.transform(&x, &y);
    point->x = SCALAR_TO_FLT(x);
    point->y = SCALAR_TO_FLT(y);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_viewport_to_world(ps_context* ctx, ps_point* point)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx ||!point) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    scalar x = FLT_TO_SCALAR(point->x);
    scalar y = FLT_TO_SCALAR(point->y);
    ctx->state->world_matrix.inverse_transform(&x, &y);
    point->x = SCALAR_TO_FLT(x);
    point->y = SCALAR_TO_FLT(y);
    global_status = STATUS_SUCCEED;
}

ps_composite PICAPI ps_set_composite_operator(ps_context* ctx, ps_composite composite)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return COMPOSITE_ERROR;
    }

    if (!ctx) { 
        global_status = STATUS_INVALID_ARGUMENT;
        return COMPOSITE_ERROR;
    }

    ps_composite op = (ps_composite)(ctx->state->composite);

    switch (composite)
    {
        case COMPOSITE_CLEAR:
            ctx->state->composite = picasso::comp_op_clear;
            break;
        case COMPOSITE_SRC:
            ctx->state->composite = picasso::comp_op_src;
            break;
        case COMPOSITE_SRC_OVER:
            ctx->state->composite = picasso::comp_op_src_over;
            break;
        case COMPOSITE_SRC_IN:
            ctx->state->composite = picasso::comp_op_src_in;
            break;
        case COMPOSITE_SRC_OUT:
            ctx->state->composite = picasso::comp_op_src_out;
            break;
        case COMPOSITE_SRC_ATOP:
            ctx->state->composite = picasso::comp_op_src_atop;
            break;
        case COMPOSITE_DST:
            ctx->state->composite = picasso::comp_op_dst;
            break;
        case COMPOSITE_DST_OVER:
            ctx->state->composite = picasso::comp_op_dst_over;
            break;
        case COMPOSITE_DST_IN:
            ctx->state->composite = picasso::comp_op_dst_in;
            break;
        case COMPOSITE_DST_OUT:
            ctx->state->composite = picasso::comp_op_dst_out;
            break;
        case COMPOSITE_DST_ATOP:
            ctx->state->composite = picasso::comp_op_dst_atop;
            break;
        case COMPOSITE_XOR:
            ctx->state->composite = picasso::comp_op_xor;
            break;
        case COMPOSITE_DARKEN:
            ctx->state->composite = picasso::comp_op_darken;
            break;
        case COMPOSITE_LIGHTEN:
            ctx->state->composite = picasso::comp_op_lighten;
            break;
        case COMPOSITE_OVERLAY:
            ctx->state->composite = picasso::comp_op_overlay;
            break;
        case COMPOSITE_SCREEN:
            ctx->state->composite = picasso::comp_op_screen;
            break;
        case COMPOSITE_MULTIPLY:
            ctx->state->composite = picasso::comp_op_multiply;
            break;
        case COMPOSITE_PLUS:
            ctx->state->composite = picasso::comp_op_plus;
            break;
        case COMPOSITE_MINUS:
            ctx->state->composite = picasso::comp_op_minus;
            break;
        case COMPOSITE_EXCLUSION:
            ctx->state->composite = picasso::comp_op_exclusion;
            break;
        case COMPOSITE_DIFFERENCE:
            ctx->state->composite = picasso::comp_op_difference;
            break;
        case COMPOSITE_SOFTLIGHT:
            ctx->state->composite = picasso::comp_op_soft_light;
            break;
        case COMPOSITE_HARDLIGHT:
            ctx->state->composite = picasso::comp_op_hard_light;
            break;
        case COMPOSITE_BURN:
            ctx->state->composite = picasso::comp_op_color_burn;
            break;
        case COMPOSITE_DODGE:
            ctx->state->composite = picasso::comp_op_color_dodge;
            break;
        case COMPOSITE_CONTRAST:
            ctx->state->composite = picasso::comp_op_contrast;
            break;
        case COMPOSITE_INVERT:
            ctx->state->composite = picasso::comp_op_invert;
            break;
        case COMPOSITE_INVERT_BLEND:
            ctx->state->composite = picasso::comp_op_invert_rgb;
            break;
        default:
            global_status = STATUS_UNKNOWN_ERROR;
            return op;
    }
    global_status = STATUS_SUCCEED;
    return op;
}

void PICAPI ps_clip(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (!ctx->path.total_vertices()) {
        global_status = STATUS_SUCCEED;
        return;
    }

    ctx->state->clip.type = picasso::clip_content;
    picasso::_clip_path(ctx->state, ctx->path, ctx->state->brush.rule);
    ctx->canvas->p->render_clip(ctx->state, true);
    ctx->path.free_all();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_clip_path(ps_context* ctx, const ps_path* p, ps_fill_rule r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !p) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }    

    ctx->state->clip.type = picasso::clip_content;
    picasso::_clip_path(ctx->state, p->path, (picasso::filling_rule)r);
    ctx->canvas->p->render_clip(ctx->state, true);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_clip_device_rect(ps_context* ctx, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::rect_s tr(FLT_TO_SCALAR(r->x),
            FLT_TO_SCALAR(r->y), 
            FLT_TO_SCALAR(r->x+r->w), 
            FLT_TO_SCALAR(r->y+r->h));

    picasso::trans_affine mtx = ctx->state->world_matrix;
    scalar rotate = mtx.rotation();
    mtx.rotate(-rotate); //not support rotation
    mtx.transform(&(tr.x1), &(tr.y1));
    mtx.transform(&(tr.x2), &(tr.y2));

    if (ctx->state->clip.type == picasso::clip_device) { //clip device rect.
        picasso::rect_s cr = ctx->state->clip.rect;
        if (cr.clip(tr)) {
            ctx->state->clip.rect = cr;
        }
    } else {
        ctx->state->clip.rect = tr;
    }
    ctx->state->clip.type = picasso::clip_device;
    ctx->canvas->p->render_clip(ctx->state, true);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_clip_rect(ps_context* ctx, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !r) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::graphic_path path;
    path.move_to(FLT_TO_SCALAR(r->x), FLT_TO_SCALAR(r->y));
    path.hline_rel(FLT_TO_SCALAR(r->w));
    path.vline_rel(FLT_TO_SCALAR(r->h));
    path.hline_rel(-FLT_TO_SCALAR(r->w));
    path.end_poly();

    ctx->state->clip.type = picasso::clip_content;
    picasso::_clip_path(ctx->state, path, picasso::fill_non_zero);
    ctx->canvas->p->render_clip(ctx->state, true);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_clip_rects(ps_context* ctx, const ps_rect* rs, unsigned int num_rs)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !rs || !num_rs) {
        global_status = STATUS_INVALID_ARGUMENT;
         return;
    }

    picasso::graphic_path path;
    for (unsigned int i = 0; i < num_rs; i++) {
        path.move_to(FLT_TO_SCALAR(rs[i].x), FLT_TO_SCALAR(rs[i].y));
        path.hline_rel(FLT_TO_SCALAR(rs[i].w));
        path.vline_rel(FLT_TO_SCALAR(rs[i].h));
        path.hline_rel(-FLT_TO_SCALAR(rs[i].w));
        path.end_poly();
    }
    ctx->state->clip.type = picasso::clip_content;
    picasso::_clip_path(ctx->state, path, picasso::fill_non_zero);
    ctx->canvas->p->render_clip(ctx->state, true);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_reset_clip(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    ctx->canvas->p->render_clip(ctx->state, false);
    ctx->state->clip.rule = picasso::fill_non_zero;
    ctx->state->clip.path.free_all();
    ctx->state->clip.rect = picasso::rect_s(0,0,0,0);
    ctx->state->clip.type = picasso::clip_none;
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_save(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !ctx->state) {
        global_status = STATUS_INVALID_ARGUMENT;
         return;
    }

    picasso::context_state * new_state = new picasso::context_state(*ctx->state); //copy constructor
    if (!new_state) {
        global_status = STATUS_OUT_OF_MEMORY;
        return;
    }

    new_state->next = ctx->state;
    ctx->state = new_state;
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_restore(ps_context* ctx)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!ctx || !ctx->state->next) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::context_state * old_state = ctx->state;

    ctx->state = ctx->state->next;

    if (old_state->clip.is_not_same(ctx->state->clip)) {
        ctx->canvas->p->render_clip(ctx->state, false);
        ctx->canvas->p->render_clip(ctx->state, true);
    }

    if ((old_state->gamma != ctx->state->gamma) 
        || (old_state->antialias != ctx->state->antialias)) {
        ctx->canvas->p->render_gamma(ctx->state, ctx->raster);
    }

    delete old_state;
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
