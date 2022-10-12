/**
 * File:   timer_widget.c
 * Author: AWTK Develop Team
 * Brief:  定时器。
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-07-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "timer_widget.h"

ret_t timer_widget_set_duration(widget_t* widget, uint32_t duration) {
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);
  return_value_if_fail(timer_widget != NULL, RET_BAD_PARAMS);

  if (timer_widget->timer_id != TK_INVALID_ID) {
    timer_modify(timer_widget->timer_id, duration);
  }

  timer_widget->duration = duration;

  return RET_OK;
}

static ret_t timer_widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);
  return_value_if_fail(timer_widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(TIMER_WIDGET_PROP_DURATION, name)) {
    value_set_uint32(v, timer_widget->duration);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t timer_widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(TIMER_WIDGET_PROP_DURATION, name)) {
    timer_widget_set_duration(widget, value_uint32(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t timer_widget_on_destroy(widget_t* widget) {
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);
  return_value_if_fail(widget != NULL && timer_widget != NULL, RET_BAD_PARAMS);

  if (timer_widget->timer_id != TK_INVALID_ID) {
    timer_remove(timer_widget->timer_id);
  }

  return RET_OK;
}

static ret_t timer_widget_on_paint_self(widget_t* widget, canvas_t* c) {
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);

  (void)timer_widget;

  return RET_OK;
}

static ret_t timer_widget_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  if (widget->enable) {
    widget_dispatch_simple_event(widget, EVT_TIMER);
  }

  return RET_REPEAT;
}

static ret_t timer_widget_on_event(widget_t* widget, event_t* e) {
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);
  return_value_if_fail(widget != NULL && timer_widget != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_OPEN) {
    timer_widget->timer_id = timer_add(timer_widget_on_timer, widget, timer_widget->duration);
  }

  return RET_OK;
}

const char* s_timer_widget_properties[] = {TIMER_WIDGET_PROP_DURATION, NULL};

TK_DECL_VTABLE(timer_widget) = {.size = sizeof(timer_widget_t),
                                .type = WIDGET_TYPE_TIMER_WIDGET,
                                .clone_properties = s_timer_widget_properties,
                                .persistent_properties = s_timer_widget_properties,
                                .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                .create = timer_widget_create,
                                .on_paint_self = timer_widget_on_paint_self,
                                .set_prop = timer_widget_set_prop,
                                .get_prop = timer_widget_get_prop,
                                .on_event = timer_widget_on_event,
                                .on_destroy = timer_widget_on_destroy};

widget_t* timer_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(timer_widget), x, y, w, h);
  timer_widget_t* timer_widget = TIMER_WIDGET(widget);
  return_value_if_fail(timer_widget != NULL, NULL);

  timer_widget->duration = 1000;

  return widget;
}

widget_t* timer_widget_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, timer_widget), NULL);

  return widget;
}
