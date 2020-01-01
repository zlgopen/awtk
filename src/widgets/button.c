/**
 * File:   button.h
 * Author: AWTK Develop Team
 * Brief:  button
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "widgets/button.h"
#include "base/widget_vtable.h"

static ret_t button_remove_timer(widget_t* widget) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_REMOVE);

  if (button->timer_id != TK_INVALID_ID) {
    timer_remove(button->timer_id);
    button->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

static ret_t button_on_repeat(const timer_info_t* info) {
  pointer_event_t evt;
  button_t* button = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  button = BUTTON(info->ctx);
  widget = WIDGET(info->ctx);
  return_value_if_fail(button != NULL && widget != NULL, RET_REMOVE);

  evt.x = 0;
  evt.y = 0;
  evt.e = event_init(EVT_CLICK, widget);
  button->repeat_nr++;
  widget_dispatch(widget, (event_t*)&evt);

  return RET_REPEAT;
}

static ret_t button_pointer_up_cleanup(widget_t* widget) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && widget != NULL, RET_BAD_PARAMS);

  button->pressed = FALSE;
  button_remove_timer(widget);
  widget_ungrab(widget->parent, widget);
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return RET_OK;
}

static ret_t button_on_long_press(const timer_info_t* info) {
  pointer_event_t evt;
  widget_t* widget = WIDGET(info->ctx);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  evt.x = 0;
  evt.y = 0;
  evt.e = event_init(EVT_LONG_PRESS, widget);

  button_pointer_up_cleanup(widget);
  widget_dispatch(widget, (event_t*)&evt);

  return RET_REMOVE;
}

static ret_t button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && widget != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      button->repeat_nr = 0;
      button->pressed = TRUE;
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      button_remove_timer(widget);
      if (button->repeat > 0) {
        button->timer_id = timer_add(button_on_repeat, widget, button->repeat);
      } else if (button->enable_long_press) {
        button->timer_id = timer_add(button_on_long_press, widget, TK_LONG_PRESS_TIME);
      }

      widget_grab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      button_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (button->pressed && widget_is_point_in(widget, evt.x, evt.y, FALSE)) {
        evt.e = event_init(EVT_CLICK, widget);
        button_pointer_up_cleanup(widget);
        widget_dispatch(widget, (event_t*)&evt);
      } else {
        button_pointer_up_cleanup(widget);
      }

      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (button->pressed || !evt->pressed) {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t button_set_repeat(widget_t* widget, int32_t repeat) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->repeat = repeat;

  return RET_OK;
}

ret_t button_set_enable_long_press(widget_t* widget, bool_t enable_long_press) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->enable_long_press = enable_long_press;

  return RET_OK;
}

static ret_t button_get_prop(widget_t* widget, const char* name, value_t* v) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    value_set_int(v, button->repeat);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    value_set_bool(v, button->enable_long_press);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t button_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    value_set_bool(v, FALSE);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    return button_set_repeat(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    return button_set_enable_long_press(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static ret_t button_on_destroy(widget_t* widget) {
  return button_remove_timer(widget);
}

static const char* const s_button_properties[] = {WIDGET_PROP_REPEAT, NULL};

TK_DECL_VTABLE(button) = {.size = sizeof(button_t),
                          .type = WIDGET_TYPE_BUTTON,
                          .space_key_to_activate = TRUE,
                          .return_key_to_activate = TRUE,
                          .parent = TK_PARENT_VTABLE(widget),
                          .create = button_create,
                          .clone_properties = s_button_properties,
                          .persistent_properties = s_button_properties,
                          .on_event = button_on_event,
                          .set_prop = button_set_prop,
                          .get_prop = button_get_prop,
                          .get_prop_default_value = button_get_prop_default_value,
                          .on_destroy = button_on_destroy,
                          .on_paint_self = widget_on_paint_self_default};

widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(button), x, y, w, h);
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, NULL);

  button->repeat = 0;
  button->repeat_nr = 0;
  button->pressed = FALSE;
  button->enable_long_press = FALSE;
  button->timer_id = TK_INVALID_ID;

  return widget;
}

widget_t* button_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, button), NULL);

  return widget;
}
