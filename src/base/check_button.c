/**
 * File:   check_button.c
 * Author: AWTK Develop Team
 * Brief:  check_button
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/check_button.h"
#include "base/image_manager.h"
#include "base/widget_vtable.h"

static ret_t check_button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  check_button_t* check_button = CHECK_BUTTON(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      check_button->point_down_aborted = FALSE;
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      check_button->point_down_aborted = TRUE;
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_UP: {
      if (!check_button->point_down_aborted) {
        if (check_button->radio) {
          check_button_set_value(widget, TRUE);
        } else {
          check_button_set_value(widget, !(check_button->value));
        }
      }
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, WIDGET_STATE_OVER);
      break;
    default:
      break;
  }

  return RET_OK;
}

static ret_t check_button_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t check_button_set_value_only(widget_t* widget, bool_t value) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (check_button->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    check_button->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);

    widget_update_style(widget);
    widget_invalidate_force(widget);
  }

  return RET_OK;
}

ret_t check_button_set_value(widget_t* widget, bool_t value) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  check_button_set_value_only(widget, value);

  if (check_button->radio && widget->parent != NULL) {
    widget_t* parent = widget->parent;

    WIDGET_FOR_EACH_CHILD_BEGIN(parent, iter, i)
    if (iter != widget && iter->vt == widget->vt) {
      check_button_set_value_only(iter, !value);
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return RET_OK;
}

static ret_t check_button_get_prop(widget_t* widget, const char* name, value_t* v) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_bool(v, check_button->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STATE_FOR_STYLE)) {
    value_set_int(v, widget_get_state_for_style(widget, check_button->value));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t check_button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return check_button_set_value(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_check_button_properties[] = {WIDGET_PROP_VALUE, NULL};
static const widget_vtable_t s_check_button_vtable = {
    .size = sizeof(check_button_t),
    .type = WIDGET_TYPE_CHECK_BUTTON,
    .clone_properties = s_check_button_properties,
    .persistent_properties = s_check_button_properties,
    .create = check_button_create,
    .on_event = check_button_on_event,
    .on_paint_self = check_button_on_paint_self,
    .get_prop = check_button_get_prop,
    .set_prop = check_button_set_prop,
};

static const widget_vtable_t s_radio_button_vtable = {
    .size = sizeof(check_button_t),
    .type = WIDGET_TYPE_RADIO_BUTTON,
    .clone_properties = s_check_button_properties,
    .create = check_button_create_radio,
    .on_event = check_button_on_event,
    .on_paint_self = check_button_on_paint_self,
    .get_prop = check_button_get_prop,
    .set_prop = check_button_set_prop,
};

widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  check_button_t* check_button = TKMEM_ZALLOC(check_button_t);
  widget_t* widget = WIDGET(check_button);
  return_value_if_fail(check_button != NULL, NULL);

  widget_init(widget, parent, &s_check_button_vtable, x, y, w, h);
  check_button_set_value_only(widget, FALSE);

  return widget;
}

widget_t* check_button_create_radio(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  check_button_t* check_button = TKMEM_ZALLOC(check_button_t);
  widget_t* widget = WIDGET(check_button);
  return_value_if_fail(check_button != NULL, NULL);

  widget_init(widget, parent, &s_radio_button_vtable, x, y, w, h);

  check_button->radio = TRUE;
  widget->state = WIDGET_STATE_NORMAL;
  check_button_set_value_only(widget, FALSE);

  return widget;
}

widget_t* check_button_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && (widget->vt == &s_check_button_vtable ||
                                          widget->vt == &s_radio_button_vtable),
                       NULL);

  return widget;
}
