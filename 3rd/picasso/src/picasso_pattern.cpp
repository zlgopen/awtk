/* Picasso - a vector graphics library
 *
 * Copyright (C) 2012 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "picasso.h"
#include "picasso_global.h"
#include "picasso_matrix.h"
#include "picasso_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

ps_pattern* PICAPI ps_pattern_create_image(const ps_image* img, ps_wrap_type xp, ps_wrap_type yp, const ps_matrix* m)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!img) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_pattern *p = (ps_pattern*)mem_malloc(sizeof(ps_pattern));
    if (p) {
        p->refcount = 1;
        if (m) {
            new ((void*)&(p->matrix))picasso::trans_affine(m->matrix);
        } else {
            new ((void*)&(p->matrix))picasso::trans_affine;
        }
        p->xtype = xp;
        p->ytype = yp;
        p->img = ps_image_ref(const_cast<ps_image*>(img));
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_pattern* PICAPI ps_pattern_ref(ps_pattern* pattern)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!pattern) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    pattern->refcount++;
    global_status = STATUS_SUCCEED;
    return pattern;
}

void PICAPI ps_pattern_unref(ps_pattern* pattern)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!pattern) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    pattern->refcount--;
    if (pattern->refcount <= 0) {
        ps_image_unref(pattern->img);
        mem_free(pattern);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_pattern_transform(ps_pattern* p, const ps_matrix* m)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!p || !m) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    p->matrix *= m->matrix;
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
