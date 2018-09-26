/**
 * File:   aswitch.c
 * Author: AWTK Develop Team
 * Brief:  aswitch
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-09-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "switch/switch.h"
#include "base/image_manager.h"
#include "base/widget_vtable.h"

static ret_t switch_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  switch_t* aswitch = SWITCH(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      aswitch->point_down_aborted = FALSE;
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      aswitch->point_down_aborted = TRUE;
      break;
    }
    case EVT_POINTER_UP: {
      if (!aswitch->point_down_aborted) {
        switch_set_value(widget, !(aswitch->value));
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t switch_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

ret_t switch_set_value(widget_t* widget, bool_t value) {
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (aswitch->value != value) {
    event_t e = event_init(EVT_VALUE_CHANGED, widget);
    aswitch->value = value;
    widget_dispatch(widget, &e);
  }

  widget->state = WIDGET_STATE_NORMAL;
  if (value) {
    widget_set_state(widget, WIDGET_STATE_CHECKED);
  } else {
    widget_set_state(widget, WIDGET_STATE_UNCHECKED);
  }

  return RET_OK;
}

static ret_t switch_get_prop(widget_t* widget, const char* name, value_t* v) {
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_bool(v, aswitch->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t switch_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return switch_set_value(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_switch_properties[] = {WIDGET_PROP_VALUE, NULL};
static const widget_vtable_t s_switch_vtable = {
    .size = sizeof(switch_t),
    .type = WIDGET_TYPE_SWITCH,
    .clone_properties = s_switch_properties,
    .persistent_properties = s_switch_properties,
    .create = switch_create,
    .on_event = switch_on_event,
    .on_paint_self = switch_on_paint_self,
    .get_prop = switch_get_prop,
    .set_prop = switch_set_prop,
};

widget_t* switch_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  switch_t* aswitch = TKMEM_ZALLOC(switch_t);
  widget_t* widget = WIDGET(aswitch);
  return_value_if_fail(aswitch != NULL, NULL);

  widget_init(widget, parent, &s_switch_vtable, x, y, w, h);

  return widget;
}

widget_t* switch_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && (widget->vt == &s_switch_vtable), NULL);

  return widget;
}
