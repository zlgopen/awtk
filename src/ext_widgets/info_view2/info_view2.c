

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "info_view2.h"
#include "../gesture/gesture.h"
#include "common/log.h"
static ret_t info_view2_set_prop(widget_t *widget, const char *name, const value_t *v) {
    return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

    if (tk_str_eq(name, WIDGET_PROP_CLICK_THROUGH)) {
        return info_view2_set_click_through(widget, value_bool(v));
    } else if (tk_str_eq(name, WIDGET_PROP_ALWAYS_ON_TOP)) {
        return info_view2_set_always_on_top(widget, value_bool(v));
    }

    return window_base_set_prop(widget, name, v);
}

static ret_t info_view2_get_prop(widget_t *widget, const char *name, value_t *v) {
    info_view2_t *info_view2 = info_view2(widget);
    return_value_if_fail(info_view2 != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

    if (tk_str_eq(name, WIDGET_PROP_CLICK_THROUGH)) {
        value_set_bool(v, info_view2->click_through);
        return RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_ALWAYS_ON_TOP)) {
        value_set_bool(v, info_view2->always_on_top);
        return RET_OK;
    }

    return window_base_get_prop(widget, name, v);
}

static bool_t info_view2_is_point_in(widget_t *widget, xy_t x, xy_t y) {
    xy_t xx = 0;
    xy_t yy = 0;
    info_view2_t *info_view2 = info_view2(widget);
    return_value_if_fail(info_view2 != NULL, RET_BAD_PARAMS);

    if (info_view2->click_through) {
        WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
        xx = x - iter->x;
        yy = y - iter->y;
        if (widget_is_point_in(iter, xx, yy, TRUE)) {
            return TRUE;
        }
        WIDGET_FOR_EACH_CHILD_END();

        return FALSE;
    } else {
        return (x >= 0 && y >= 0 && x < widget->w && y < widget->h);
    }
}

static ret_t canvas_set_clip_rect_with_offset(canvas_t *c, rect_t *r, rect_t *save_r, int32_t ox,
                                              int32_t oy) {
    //LOG("into canvas_set_clip_rect_with_offset");

    rect_t rr = *r;
    vgcanvas_t *vg = canvas_get_vgcanvas(c);
    rr.x += ox;
    rr.y += oy;

    rr = rect_intersect(&rr, save_r);

    vgcanvas_clip_rect(vg, rr.x, rr.y, rr.w, rr.h);
    return canvas_set_clip_rect(c, &rr);
}

static ret_t info_view2_paint_prev_next_v_overlap(info_view2_t *info_view2, canvas_t *c,
                                                  rect_t *save_r, widget_t *prev, widget_t *next,
                                                  int32_t yoffset) {
    LOG("into info_view2_paint_prev_next_v_overlap");

    rect_t r;
    int32_t ox = c->ox;
    int32_t oy = c->oy;
    xy_t w = WIDGET(info_view2)->w;
    xy_t h = WIDGET(info_view2)->h;
    int32_t r_yoffset = h - yoffset;

    if (next != NULL && r_yoffset >= 0) {
        canvas_save(c);

        widget_paint(next, c);

        canvas_restore(c);
    }

    if (prev != NULL) {
        if (yoffset > h) {
            r_yoffset = 0;
            yoffset = h;
        }

        r = rect_init(0, 0, w, yoffset);

        if (r.h > 0) {
            canvas_save(c);
            canvas_translate(c, 0, -r_yoffset);
            canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
            widget_paint(prev, c);
            canvas_untranslate(c, 0, -r_yoffset);
            canvas_restore(c);
        }
    }
    LOG("leave info_view2_paint_prev_next_v_overlap");
    return RET_OK;
}

static ret_t info_view2_paint_v_show(info_view2_t *info_view2_temp, canvas_t *c, rect_t *save_r) {
    LOG("into info_view2_paint_v_show");

    int32_t yoffset = tk_abs(info_view2_temp->yoffset);
    widget_t *prev = WIDGET(info_view2_temp);
    widget_t *active = info_view2_temp->prev_window;

    return info_view2_paint_prev_next_v_overlap(info_view2_temp, c, save_r, prev, active, yoffset);
}

static ret_t info_view2_paint_v_hide(info_view2_t *info_view2_temp, canvas_t *c, rect_t *save_r) {
    LOG("into info_view2_paint_v_hide");

    widget_t *next = info_view2_temp->prev_window;
    int32_t yoffset = WIDGET(info_view2_temp)->h - tk_abs(info_view2_temp->yoffset);
    widget_t *active = WIDGET(info_view2_temp);

    return info_view2_paint_prev_next_v_overlap(info_view2_temp, c, save_r, active, next, yoffset);
}

static ret_t info_view2_on_paint_self(widget_t *widget, canvas_t *c) {
    rect_t save_r;
    widget_t *active = NULL;
    uint8_t save_a = c->lcd->global_alpha;
    vgcanvas_t *vg = canvas_get_vgcanvas(c);
    info_view2_t *info_view2_temp = info_view2(widget);
    return_value_if_fail(widget != NULL && info_view2_temp != NULL,
                         RET_BAD_PARAMS);

    active = WIDGET(get_info_view2_no_widget());

    return_value_if_fail(active != NULL, RET_BAD_PARAMS);

    canvas_get_clip_rect(c, &save_r);

    LOG("into info_view2_on_paint_self ");
    if (info_view2_temp->yoffset > 0 && info_view2_temp->info_view2_show == FALSE) {
        info_view2_paint_v_show(info_view2_temp, c, &save_r);
    } else if (info_view2_temp->yoffset < 0 && info_view2_temp->info_view2_show == TRUE) {
        info_view2_paint_v_hide(info_view2_temp, c, &save_r);
    } else {
        widget_paint(active, c);
    }

    vgcanvas_clip_rect(vg, save_r.x, save_r.y, save_r.w, save_r.h);
    canvas_set_clip_rect(c, &save_r);
    canvas_set_global_alpha(c, save_a);

    LOG("leave info_view2_on_paint_children");
    return RET_OK;
}

static const char *const s_info_view2_properties[] = {WIDGET_PROP_CLICK_THROUGH,
                                                      WIDGET_PROP_ALWAYS_ON_TOP, NULL};

TK_DECL_VTABLE(info_view2) = {.type = WIDGET_TYPE_info_view2,
                              .size = sizeof(info_view2_t),
                              .is_window = TRUE,
                              .parent = TK_PARENT_VTABLE(window_base),
                              .create = info_view2_create,
                              .clone_properties = s_info_view2_properties,
                              .persistent_properties = s_info_view2_properties,
                              .on_event = window_base_on_event,
                              .on_paint_self = info_view2_on_paint_self,
                              .on_paint_begin = window_base_on_paint_begin,
                              .on_paint_end = window_base_on_paint_end,
                              .set_prop = info_view2_set_prop,
                              .get_prop = info_view2_get_prop,
                              .is_point_in = info_view2_is_point_in,
                              .on_destroy = window_base_on_destroy};

widget_t *info_view2_create(widget_t *parent, xy_t x, xy_t y, wh_t w, wh_t h) {
    LOG("into info_view2_create");
    widget_t *widget = window_base_create(parent, TK_REF_VTABLE(info_view2), x, y, w, h);
    info_view2_t *info_view2_temp = info_view2(widget);
    return_value_if_fail(info_view2_temp != NULL, NULL);
    info_view2_temp->yoffset = 0;
    info_view2_temp->always_on_top = TRUE;
    info_view2_temp->click_through = FALSE;
    info_view2_temp->info_view2_show = FALSE;
    info_view2_temp->prev_window = NULL;
    return widget;
}

widget_t *info_view2_cast(widget_t *widget) {
    return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, info_view2), NULL);

    return widget;
}

ret_t info_view2_set_click_through(widget_t *widget, bool_t click_through) {
    info_view2_t *info_view2 = info_view2(widget);
    return_value_if_fail(info_view2 != NULL, RET_BAD_PARAMS);

    info_view2->click_through = click_through;

    return RET_OK;
}

ret_t info_view2_set_always_on_top(widget_t *widget, bool_t always_on_top) {
    info_view2_t *info_view2 = info_view2(widget);
    return_value_if_fail(info_view2 != NULL, RET_BAD_PARAMS);

    info_view2->always_on_top = always_on_top;

    return RET_OK;
}
