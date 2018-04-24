/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2008 Zhang Ji Peng
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

int _byte_pre_color(ps_color_format fmt)
{
    switch (fmt) 
    {
#if ENABLE(FORMAT_RGBA)
        case COLOR_FORMAT_RGBA:
            return 4;
#endif
#if ENABLE(FORMAT_ARGB)
        case COLOR_FORMAT_ARGB:
            return 4;
#endif
#if ENABLE(FORMAT_ABGR)
        case COLOR_FORMAT_ABGR:
            return 4;
#endif
#if ENABLE(FORMAT_BGRA)
        case COLOR_FORMAT_BGRA:
            return 4;
#endif
#if ENABLE(FORMAT_RGB)
        case COLOR_FORMAT_RGB:
            return 3;
#endif
#if ENABLE(FORMAT_BGR)
        case COLOR_FORMAT_BGR:
            return 3;
#endif
#if ENABLE(FORMAT_RGB565)
        case COLOR_FORMAT_RGB565:
            return 2;
#endif
#if ENABLE(FORMAT_RGB555)
        case COLOR_FORMAT_RGB555:
            return 2;
#endif
        default:
            global_status = STATUS_NOT_SUPPORT;
            return 0;
    }
}

static inline painter* get_painter_from_format(ps_color_format fmt)
{
    switch (fmt) 
    {
#if ENABLE(FORMAT_RGBA)
        case COLOR_FORMAT_RGBA:
            return new painter(pix_fmt_rgba);
#endif
#if ENABLE(FORMAT_ARGB)
        case COLOR_FORMAT_ARGB:
            return new painter(pix_fmt_argb);
#endif
#if ENABLE(FORMAT_ABGR)
        case COLOR_FORMAT_ABGR:
            return new painter(pix_fmt_abgr);
#endif
#if ENABLE(FORMAT_BGRA)
        case COLOR_FORMAT_BGRA:
            return new painter(pix_fmt_bgra);
#endif
#if ENABLE(FORMAT_RGB)
        case COLOR_FORMAT_RGB:
            return new painter(pix_fmt_rgb);
#endif
#if ENABLE(FORMAT_BGR)
        case COLOR_FORMAT_BGR:
            return new painter(pix_fmt_bgr);
#endif
#if ENABLE(FORMAT_RGB565)
        case COLOR_FORMAT_RGB565:
            return new painter(pix_fmt_rgb565);
#endif
#if ENABLE(FORMAT_RGB555)
        case COLOR_FORMAT_RGB555:
            return new painter(pix_fmt_rgb555);
#endif
        default:
            global_status = STATUS_NOT_SUPPORT;
            return 0;
    }
}

}

#ifdef __cplusplus
extern "C" {
#endif

ps_canvas* PICAPI ps_canvas_create(ps_color_format fmt, int w, int h)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (w <= 0 || h <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    picasso::painter* pa = picasso::get_painter_from_format(fmt);
    if (!pa){
        return 0;
    }

    ps_canvas *p = (ps_canvas*)mem_malloc(sizeof(ps_canvas));
    if (p) {
        p->refcount = 1;
        p->fmt = fmt;
        p->p = pa;
        p->host = 0;
        p->mask = 0;
        new ((void*)&(p->buffer)) picasso::rendering_buffer; 
        int pitch = picasso::_byte_pre_color(fmt) * w;
        byte* buf = 0;
        if ((buf = (byte*)BufferAlloc(h * pitch))) {
            p->flage = buffer_alloc_surface;
            p->buffer.attach(buf, w, h, pitch);
            p->p->attach(p->buffer);
            global_status = STATUS_SUCCEED;
            return p;
        } else if ((buf = (byte*)mem_malloc(h * pitch))) {
            p->flage = buffer_alloc_malloc;
            p->buffer.attach(buf, w, h, pitch);
            p->p->attach(p->buffer);
            global_status = STATUS_SUCCEED;
            return p;
        } else {
            delete pa;
            mem_free(p);
            global_status = STATUS_OUT_OF_MEMORY;
            return 0;
        }
    } else {
        delete pa; //mem_free painter on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_canvas_create_compatible(const ps_canvas* c, int w, int h)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    if (w <= 0)
        w = c->buffer.width();

    if (h <= 0)
        h = c->buffer.height();

    picasso::painter* pa = picasso::get_painter_from_format(c->fmt);
    if (!pa){
        return 0;
    }

    ps_canvas *p = (ps_canvas*)mem_malloc(sizeof(ps_canvas));
    if (p) {
        p->refcount = 1;
        p->fmt = c->fmt;
        p->p = pa;
        p->host = 0;
        p->mask = 0;
        new ((void*)&(p->buffer)) picasso::rendering_buffer; 
        int pitch = picasso::_byte_pre_color(c->fmt) * w;
        byte* buf = 0;
        if ((buf = (byte*)BufferAlloc(h * pitch))) {
            p->flage = buffer_alloc_surface;
            p->buffer.attach(buf, w, h, pitch);
            p->p->attach(p->buffer);
            global_status = STATUS_SUCCEED;
            return p;
        } else if ((buf = (byte*)mem_malloc(h * pitch))) {
            p->flage = buffer_alloc_malloc;
            p->buffer.attach(buf, w, h, pitch);
            p->p->attach(p->buffer);
            global_status = STATUS_SUCCEED;
            return p;
        } else {
            delete pa;
            mem_free(p);
            global_status = STATUS_OUT_OF_MEMORY;
            return 0;
        }
    } else {
        delete pa; //mem_free painter on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_canvas_create_from_canvas(ps_canvas* c, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_rect rc = {0, 0, (float)c->buffer.width(), (float)c->buffer.height()}; 
    if (!r) {
        //Note: if rect is NULL, It equal reference.
        global_status = STATUS_SUCCEED;
        return ps_canvas_ref(c);
    } else {
        if (r->x > 0)
            rc.x = r->x;
        if (r->y > 0)
            rc.y = r->y;
        if (r->w > 0)
            rc.w = r->w;
        if (r->h > 0)
            rc.h = r->h;
    }

    picasso::painter* pa = picasso::get_painter_from_format(c->fmt);
    if (!pa){
        return 0;
    }

    ps_canvas *p = (ps_canvas*)mem_malloc(sizeof(ps_canvas));
    if (p) {
        p->refcount = 1;
        p->fmt = c->fmt;
        p->p = pa;
        p->flage = buffer_alloc_image;
        p->host = (void*)ps_canvas_ref(c);
        p->mask = 0;
        int bpp = picasso::_byte_pre_color(c->fmt);
        new ((void*)&(p->buffer)) picasso::rendering_buffer; 
        p->buffer.attach(c->buffer.buffer()+_iround(rc.y*c->buffer.stride()+rc.x*bpp), 
                                       _iround(rc.w), _iround(rc.h), c->buffer.stride());
        p->p->attach(p->buffer);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        delete pa; //mem_free painter on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_canvas_create_from_image(ps_image* i, const ps_rect* r)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!i) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_rect rc = {0, 0, (float)i->buffer.width(), (float)i->buffer.height()}; 
    if (r) {
        if (r->x > 0)
            rc.x = r->x;
        if (r->y > 0)
            rc.y = r->y;
        if (r->w > 0)
            rc.w = r->w;
        if (r->h > 0)
            rc.h = r->h;
    }

    picasso::painter* pa = picasso::get_painter_from_format(i->fmt);
    if (!pa){
        return 0;
    }

    ps_canvas *p = (ps_canvas*)mem_malloc(sizeof(ps_canvas));
    if (p) {
        p->refcount = 1;
        p->fmt = i->fmt;
        p->p = pa;
        p->flage = buffer_alloc_image;
        p->host = (void*)ps_image_ref(i);
        p->mask = 0;
        int bpp = picasso::_byte_pre_color(i->fmt);
        new ((void*)&(p->buffer)) picasso::rendering_buffer; 
        p->buffer.attach(i->buffer.buffer()+_iround(rc.y*i->buffer.stride()+rc.x*bpp), 
                                       _iround(rc.w), _iround(rc.h), i->buffer.stride());
        p->p->attach(p->buffer);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        delete pa; //mem_free painter on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_canvas_create_with_data(ps_byte * addr, ps_color_format fmt, int w, int h, int pitch)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!addr || w <= 0 || h <= 0 || pitch <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    picasso::painter* pa = picasso::get_painter_from_format(fmt);
    if (!pa){
        return 0;
    }

    ps_canvas *p = (ps_canvas*)mem_malloc(sizeof(ps_canvas));
    if (p) {
        p->refcount = 1;
        p->fmt = fmt;
        p->p = pa;
        p->flage = buffer_alloc_none;
        p->host = 0;
        p->mask = 0;
        new ((void*)&(p->buffer)) picasso::rendering_buffer; 
        p->buffer.attach(addr, w, h, pitch);
        p->p->attach(p->buffer);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        delete pa; //mem_free painter on error
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_canvas* PICAPI ps_canvas_replace_data(ps_canvas* canvas, ps_byte* addr,
                                            ps_color_format fmt, int w, int h, int pitch)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!canvas || !addr || w <= 0 || h <= 0 || pitch <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    if (canvas->flage != buffer_alloc_none) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    if (canvas->fmt != fmt) {
        global_status = STATUS_MISMATCHING_FORMAT;
        return 0;
    }

    // replace target buffer address.
    canvas->buffer.replace(addr, w, h, pitch);
    global_status = STATUS_SUCCEED;
    return canvas;
}

ps_canvas* PICAPI ps_canvas_ref(ps_canvas* canvas)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    canvas->refcount++;
    global_status = STATUS_SUCCEED;
    return canvas;
}

void PICAPI ps_canvas_unref(ps_canvas* canvas)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    canvas->refcount--;
    if (canvas->refcount <= 0) {
        delete canvas->p; //mem_free painter
        if (canvas->flage == buffer_alloc_surface)
            BufferFree(canvas->buffer.buffer());
        else if (canvas->flage == buffer_alloc_malloc)
            mem_free(canvas->buffer.buffer());
        else if (canvas->flage == buffer_alloc_image)
            ps_image_unref(static_cast<ps_image*>(canvas->host));
        else if (canvas->flage == buffer_alloc_canvas)
            ps_canvas_unref(static_cast<ps_canvas*>(canvas->host));

        if (canvas->mask)
            ps_mask_unref(canvas->mask);

        (&canvas->buffer)->picasso::rendering_buffer::~rendering_buffer();
        mem_free(canvas);
    }
    global_status = STATUS_SUCCEED;
}

ps_size PICAPI ps_canvas_get_size(const ps_canvas* canvas)
{
    ps_size size = {0 , 0};
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return size;
    }

    if (!canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return size;
    }

    size.w = (float)canvas->buffer.width();
    size.h = (float)canvas->buffer.height();
    global_status = STATUS_SUCCEED;
    return size;
}

ps_color_format PICAPI ps_canvas_get_format(const ps_canvas* canvas)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return COLOR_FORMAT_UNKNOWN;
    }

    if (!canvas) {
        global_status = STATUS_INVALID_ARGUMENT;
        return COLOR_FORMAT_UNKNOWN;
    }
    global_status = STATUS_SUCCEED;
    return canvas->fmt;
}

void PICAPI ps_canvas_set_mask(ps_canvas* c, const ps_mask* m)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!c || !m) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (c->mask)
        ps_canvas_reset_mask(c);

    c->mask = ps_mask_ref(const_cast<ps_mask*>(m));
    c->p->render_mask(c->mask->mask, true);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_canvas_reset_mask(ps_canvas* c)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (c->mask) {
        c->p->render_mask(c->mask->mask, false);
        ps_mask_unref(c->mask);
        c->mask = 0;
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_canvas_bitblt(ps_canvas* src, const ps_rect* r, ps_canvas* dst, const ps_point* l)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!src || !dst) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    int x = 0, y = 0;
    if (l) {
        x = _iround(l->x);
        y = _iround(l->y);
    }

    if (r) {
        picasso::rect rc(_iround(r->x), _iround(r->y), 
                         _iround(r->x+r->w), _iround(r->y+r->h));
        src->p->render_copy(src->buffer, &rc, dst->p, x, y);
    } else {
        src->p->render_copy(src->buffer, 0, dst->p, x, y);
    }

    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
