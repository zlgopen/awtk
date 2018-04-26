/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_gradient.h"
#include "picasso_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

ps_gradient* PICAPI ps_gradient_create_linear(ps_gradient_spread sp, const ps_point* s, const ps_point* e)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!s || !e) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    int spread = SPREAD_PAD;
    switch (sp) {
    case GRADIENT_SPREAD_PAD:
        spread = SPREAD_PAD;
        break;
    case GRADIENT_SPREAD_REPEAT:
        spread = SPREAD_REPEAT;
        break;
    case GRADIENT_SPREAD_REFLECT:
        spread = SPREAD_REFLECT;
        break;
    };

    scalar x1 = FLT_TO_SCALAR(s->x);
    scalar y1 = FLT_TO_SCALAR(s->y);
    scalar x2 = FLT_TO_SCALAR(e->x);
    scalar y2 = FLT_TO_SCALAR(e->y);

    ps_gradient *p = (ps_gradient*)mem_malloc(sizeof(ps_gradient));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->gradient))picasso::gradient_adapter;
        p->gradient.init_linear(spread, x1, y1, x2, y2);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_gradient* PICAPI ps_gradient_create_radial(ps_gradient_spread sp, const ps_point* s, float sr,
                                                                            const ps_point* e, float er)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!s || !e) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    int spread = SPREAD_PAD;
    switch (sp) {
    case GRADIENT_SPREAD_PAD:
        spread = SPREAD_PAD;
        break;
    case GRADIENT_SPREAD_REPEAT:
        spread = SPREAD_REPEAT;
        break;
    case GRADIENT_SPREAD_REFLECT:
        spread = SPREAD_REFLECT;
        break;
    };

    scalar x1 = FLT_TO_SCALAR(s->x);
    scalar y1 = FLT_TO_SCALAR(s->y);
    scalar r1 = FLT_TO_SCALAR(sr);
    scalar x2 = FLT_TO_SCALAR(e->x);
    scalar y2 = FLT_TO_SCALAR(e->y);
    scalar r2 = FLT_TO_SCALAR(er);

    ps_gradient *p = (ps_gradient*)mem_malloc(sizeof(ps_gradient));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->gradient))picasso::gradient_adapter;;
        p->gradient.init_radial(spread, x1, y1, r1, x2, y2, r2);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_gradient* PICAPI ps_gradient_create_conic(ps_gradient_spread sp, const ps_point* o, float a)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!o) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    int spread = SPREAD_PAD;
    switch (sp) {
    case GRADIENT_SPREAD_PAD:
        spread = SPREAD_PAD;
        break;
    case GRADIENT_SPREAD_REPEAT:
        spread = SPREAD_REPEAT;
        break;
    case GRADIENT_SPREAD_REFLECT:
        spread = SPREAD_REFLECT;
        break;
    };

    scalar x = FLT_TO_SCALAR(o->x);
    scalar y = FLT_TO_SCALAR(o->y);
    scalar ca = FLT_TO_SCALAR(a);

    ps_gradient *p = (ps_gradient*)mem_malloc(sizeof(ps_gradient));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->gradient))picasso::gradient_adapter;
        p->gradient.init_conic(spread, x, y, ca);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_gradient* PICAPI ps_gradient_ref(ps_gradient* g)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!g) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    g->refcount++;
    global_status = STATUS_SUCCEED;
    return g;
}

void PICAPI ps_gradient_unref(ps_gradient* g)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!g) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    g->refcount--;
    if (g->refcount <= 0) {
        (&g->gradient)->picasso::gradient_adapter::~gradient_adapter();
        mem_free(g);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_gradient_clear_color_stops(ps_gradient* g)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!g) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    g->gradient.clear_stops();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_gradient_add_color_stop(ps_gradient* g, float off, const ps_color* c)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!g || !c) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    if (off < 0)
        off = 0.0f;
    else if (off > 1)
        off = 1.0f;

    g->gradient.add_color_stop(FLT_TO_SCALAR(off),
                picasso::rgba(FLT_TO_SCALAR(c->r), FLT_TO_SCALAR(c->g), FLT_TO_SCALAR(c->b), FLT_TO_SCALAR(c->a)));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_gradient_transform(ps_gradient* g, const ps_matrix* m)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!g || !m) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    g->gradient.transform(m->matrix);
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
