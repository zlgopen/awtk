/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_mask.h"
#include "picasso_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

ps_mask* PICAPI ps_mask_create_with_data(ps_byte* data, int w, int h)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!data || w <= 0 || h <= 0) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_mask *p = (ps_mask*)mem_malloc(sizeof(ps_mask));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->mask))picasso::mask_layer(data, w, h, w); //gray color format for alpha
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_mask* PICAPI ps_mask_ref(ps_mask* mask)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!mask) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    mask->refcount++;
    global_status = STATUS_SUCCEED;
    return mask;
}

void PICAPI ps_mask_unref(ps_mask* mask)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!mask) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    mask->refcount--;
    if (mask->refcount <= 0) {
        (&mask->mask)->picasso::mask_layer::~mask_layer();
        mem_free(mask);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_mask_add_color_filter(ps_mask* mask, const ps_color* c)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!mask || !c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    mask->mask.set_mask_type(MASK_COLORS);
    mask->mask.add_filter_color(picasso::rgba(FLT_TO_SCALAR(c->r),
                FLT_TO_SCALAR(c->g), FLT_TO_SCALAR(c->b), FLT_TO_SCALAR(c->a)));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_mask_clear_color_filters(ps_mask* mask)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!mask) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    mask->mask.clear_filter_colors();
    mask->mask.set_mask_type(MASK_ALPHA);
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
