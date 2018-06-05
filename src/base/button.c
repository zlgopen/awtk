/**
 * File:   button.h
 * Author: AWTK Develop Team
 * Brief:  button
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/button.h"

static ret_t button_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      break;
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      evt.e = event_init(EVT_CLICK, widget);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
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

static const widget_vtable_t s_button_vtable = {.on_event = button_on_event,
                                                .on_paint_self = button_on_paint_self};

widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  button_t* button = TKMEM_ZALLOC(button_t);
  return_value_if_fail(button != NULL, NULL);

  widget = WIDGETP(button);
  widget_init(widget, parent, WIDGET_BUTTON);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_button_vtable;

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
