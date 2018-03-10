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
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  style_t* style = &(widget->style);
  button_t* button = BUTTON(widget);
  color_t color = color_init(0xff, 0xff, 0xff, 0xff);

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_BG_COLOR, color));
  canvas_set_text_color(c, style_get_color(style, STYLE_ID_TEXT_COLOR, color));
  canvas_set_stroke_color(c, style_get_color(style, STYLE_ID_BORDER_COLOR, color));

  canvas_fill_rect(c, 0, 0, widget->w, widget->h);
  canvas_stroke_rect(c, 0, 0, widget->w, widget->h);

  if (button->text.size > 0) {
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
    uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, 20);

    canvas_set_font(c, font_name, font_size);
    w = canvas_measure_text(c, button->text.str, button->text.size);
    x = (widget->w - w) >> 1;
    y = (widget->h) >> 1;
    canvas_draw_text(c, button->text.str, button->text.size, x, y);
  }

  return RET_OK;
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
