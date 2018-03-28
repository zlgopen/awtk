/**
 * File:   button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  button
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "base/button.h"
#include "base/mem.h"

static ret_t button_on_paint_self(widget_t* widget, canvas_t* c) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  return widget_paint_helper(widget, c, NULL, &(button->text));
}

ret_t button_set_text(widget_t* widget, const wchar_t* text) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(widget != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_set(&(button->text), text);
}

static ret_t button_get_prop(widget_t* widget, const char* name, value_t* v) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (name[0] == 't') {
    value_set_wstr(v, button->text.str);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (name[0] == 't') {
    return button_set_text(widget, value_wstr(v));
  }

  return RET_NOT_FOUND;
}

static ret_t button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      break;
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      evt.e.type = EVT_CLICK;
      evt.e.target = widget;
      widget_set_state(widget, WIDGET_STATE_OVER);
      widget_dispatch(widget, (event_t*)&evt);
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

static ret_t button_destroy(widget_t* widget) {
  button_t* button = BUTTON(widget);
  wstr_reset(&(button->text));

  return RET_OK;
}

static const widget_vtable_t s_button_vtable = {.on_event = button_on_event,
                                                .on_paint_self = button_on_paint_self,
                                                .get_prop = button_get_prop,
                                                .set_prop = button_set_prop,
                                                .destroy = button_destroy};

widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  button_t* button = MEM_ZALLOC(button_t);
  return_value_if_fail(button != NULL, NULL);

  widget = WIDGETP(button);
  widget_init(widget, parent, WIDGET_BUTTON);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_button_vtable;

  wstr_init(&(button->text), 0);
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
