/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

ps_image* PICAPI ps_image_create(ps_color_format fmt, int w, int h)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (w <= 0 || h <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = fmt;
        img->host = 0;
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        int pitch = picasso::_byte_pre_color(fmt) * w;
        byte* buf = 0;
        if ((buf = (byte*)BufferAlloc(h * pitch))) {
            img->flage = buffer_alloc_surface;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else if ((buf = (byte*)mem_malloc(h * pitch))) {
            img->flage = buffer_alloc_malloc;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else {
            mem_free(img);
            global_status = STATUS_OUT_OF_MEMORY;
            return 0;
        }
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_image* PICAPI ps_image_create_from_data(ps_byte* data, ps_color_format fmt, int w, int h, int pitch)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!data || w <= 0 || h <= 0 || pitch <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = fmt;
        img->host = 0;
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        int pitch = picasso::_byte_pre_color(fmt) * w;
        byte* buf = 0;
        if ((buf = (byte*)BufferAlloc(h * pitch))) {
            BufferCopy(buf, data, pitch*h);
            img->flage = buffer_alloc_surface;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else if ((buf = (byte*)mem_malloc(h * pitch))) {
            mem_copy(buf, data, pitch*h);
            img->flage = buffer_alloc_malloc;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else {
            mem_free(img);
            global_status = STATUS_OUT_OF_MEMORY;
            return 0;
        }
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_image* PICAPI ps_image_create_with_data(ps_byte* data, ps_color_format fmt, int w, int h, int pitch)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!data || w <= 0 || h <= 0 || pitch <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = fmt;
        img->host = 0;
        img->flage = buffer_alloc_none;
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        img->buffer.attach(data, w, h, pitch);
        global_status = STATUS_SUCCEED;
        return img;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_image* PICAPI ps_image_create_compatible(const ps_canvas* c, int w, int h)
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

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = c->fmt;
        img->host = 0;
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        int pitch = picasso::_byte_pre_color(c->fmt) * w;
        byte* buf = 0;
        if ((buf = (byte*)BufferAlloc(h * pitch))) {
            img->flage = buffer_alloc_surface;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else if ((buf = (byte*)mem_malloc(h * pitch))) {
            img->flage = buffer_alloc_malloc;
            img->buffer.attach(buf, w, h, pitch);
            global_status = STATUS_SUCCEED;
            return img;
        } else {
            mem_free(img);
            global_status = STATUS_OUT_OF_MEMORY;
            return 0;
        }
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_image* PICAPI ps_image_create_from_image(ps_image* i, const ps_rect* r)
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
    if (!r) {
        //Note: if rect is NULL, It equal reference.
        global_status = STATUS_SUCCEED;
        return ps_image_ref(i);
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

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = i->fmt;
        img->flage = buffer_alloc_image;
        img->host = (void*)ps_image_ref(i);
        int bpp = picasso::_byte_pre_color(i->fmt);
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        img->buffer.attach(i->buffer.buffer()+_iround(rc.y*i->buffer.stride()+rc.x*bpp), 
                                       _iround(rc.w), _iround(rc.h), i->buffer.stride());
        img->buffer.set_transparent(i->buffer.is_transparent());
        img->buffer.set_color_channel(i->buffer.get_color_channel());
        global_status = STATUS_SUCCEED;
        return img;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_image* PICAPI ps_image_create_from_canvas(ps_canvas* c, const ps_rect* r)
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

    ps_image *img = (ps_image*)mem_malloc(sizeof(ps_image));
    if (img) {
        img->refcount = 1;
        img->fmt = c->fmt;
        img->flage = buffer_alloc_canvas;
        img->host = (void*)ps_canvas_ref(c);
        int bpp = picasso::_byte_pre_color(c->fmt);
        new ((void*)&(img->buffer)) picasso::rendering_buffer; 
        img->buffer.attach(c->buffer.buffer()+_iround(rc.y*c->buffer.stride()+rc.x*bpp), 
                                       _iround(rc.w), _iround(rc.h), c->buffer.stride());
        img->buffer.set_transparent(true);
        global_status = STATUS_SUCCEED;
        return img;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}


ps_image* PICAPI ps_image_ref(ps_image* img)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    img->refcount++;
    global_status = STATUS_SUCCEED;
    return img;
}

void PICAPI ps_image_unref(ps_image* img)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    img->refcount--;
    if (img->refcount <= 0) {
        if (img->flage == buffer_alloc_surface)
            BufferFree(img->buffer.buffer());
        else if (img->flage == buffer_alloc_malloc)
            mem_free(img->buffer.buffer());
        else if (img->flage == buffer_alloc_image)
            ps_image_unref(static_cast<ps_image*>(img->host));
        else if (img->flage == buffer_alloc_canvas)
            ps_canvas_unref(static_cast<ps_canvas*>(img->host));

        (&img->buffer)->picasso::rendering_buffer::~rendering_buffer();
        mem_free(img);
    }
    global_status = STATUS_SUCCEED;
}

ps_size PICAPI ps_image_get_size(const ps_image* img)
{
    ps_size size = {0 , 0};
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return size;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return size;
    }

    size.w = (float)img->buffer.width();
    size.h = (float)img->buffer.height();
    global_status = STATUS_SUCCEED;
    return size;
}

ps_color_format PICAPI ps_image_get_format(const ps_image* img)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return COLOR_FORMAT_UNKNOWN;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return COLOR_FORMAT_UNKNOWN;
    }
    global_status = STATUS_SUCCEED;
    return img->fmt;
}

void PICAPI ps_image_set_allow_transparent(ps_image* img, ps_bool a)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    img->buffer.set_transparent(a ? true : false);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_image_set_transparent_color(ps_image* img, const ps_color* c)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    
    if (!c) {
        img->buffer.clear_color_channel(); 
    } else {
        img->buffer.set_color_channel(picasso::rgba(FLT_TO_SCALAR(c->r),
                        FLT_TO_SCALAR(c->g),FLT_TO_SCALAR(c->b),FLT_TO_SCALAR(c->a)));
    }
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
