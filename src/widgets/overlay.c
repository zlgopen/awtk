/**
 * File:   overlay.h
 * Author: AWTK Develop Team
 * Brief:  overlay
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-05-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "widgets/overlay.h"

static ret_t overlay_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CLICK_THROUGH)) {
    return overlay_set_click_through(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ALWAYS_ON_TOP)) {
    return overlay_set_always_on_top(widget, value_bool(v));
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t overlay_get_prop(widget_t* widget, const char* name, value_t* v) {
  overlay_t* overlay = OVERLAY(widget);
  return_value_if_fail(overlay != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CLICK_THROUGH)) {
    value_set_bool(v, overlay->click_through);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ALWAYS_ON_TOP)) {
    value_set_bool(v, overlay->always_on_top);
    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

static bool_t overlay_is_point_in(widget_t* widget, xy_t x, xy_t y) {
  xy_t xx = 0;
  xy_t yy = 0;
  overlay_t* overlay = OVERLAY(widget);
  return_value_if_fail(overlay != NULL, RET_BAD_PARAMS);

  if (overlay->click_through) {
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

static const char* const s_overlay_properties[] = {WIDGET_PROP_CLICK_THROUGH,
                                                   WIDGET_PROP_ALWAYS_ON_TOP, NULL};

TK_DECL_VTABLE(overlay) = {.type = WIDGET_TYPE_OVERLAY,
                           .size = sizeof(overlay_t),
                           .is_window = TRUE,
                           .parent = TK_PARENT_VTABLE(window_base),
                           .create = overlay_create,
                           .clone_properties = s_overlay_properties,
                           .persistent_properties = s_overlay_properties,
                           .on_event = window_base_on_event,
                           .on_paint_self = window_base_on_paint_self,
                           .on_paint_begin = window_base_on_paint_begin,
                           .on_paint_end = window_base_on_paint_end,
                           .set_prop = overlay_set_prop,
                           .get_prop = overlay_get_prop,
                           .is_point_in = overlay_is_point_in,
                           .on_destroy = window_base_on_destroy};

widget_t* overlay_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(overlay), x, y, w, h);
}

widget_t* overlay_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, overlay), NULL);

  return widget;
}

ret_t overlay_set_click_through(widget_t* widget, bool_t click_through) {
  overlay_t* overlay = OVERLAY(widget);
  return_value_if_fail(overlay != NULL, RET_BAD_PARAMS);

  overlay->click_through = click_through;

  return RET_OK;
}

ret_t overlay_set_always_on_top(widget_t* widget, bool_t always_on_top) {
  overlay_t* overlay = OVERLAY(widget);
  return_value_if_fail(overlay != NULL, RET_BAD_PARAMS);

  overlay->always_on_top = always_on_top;

  return RET_OK;
}
