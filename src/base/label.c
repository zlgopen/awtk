/**
 * File:   label.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/label.h"

static ret_t label_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  uint8_t text_align_h = 0;
  uint8_t text_align_v = 0;
  style_t* style = &(widget->style);
  label_t* label = LABEL(widget);
  color_t color = color_init(0xff, 0xff, 0xff, 0xff);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_set_font(c, NULL, 20);
  canvas_set_fill_color(c, style_get_color(style, E_BG_COLOR, color));
  canvas_set_text_color(c, style_get_color(style, E_TEXT_COLOR, color));

  canvas_fill_rect(c, 0, 0, widget->w, widget->h);
  w = canvas_measure_text(c, label->text.str, label->text.size);

  text_align_h = style_get_int(style, E_TEXT_ALIGN_H, label->text_align_h);
  text_align_v = style_get_int(style, E_TEXT_ALIGN_V, label->text_align_v);

  switch (text_align_v) {
    case ALIGN_V_TOP:
      y = label->border;
      break;
    case ALIGN_V_BOTTOM:
      y = widget->h - label->border;
      break;
    default:
      y = (widget->h) >> 1;
      break;
  }

  switch (text_align_h) {
    case ALIGN_H_LEFT:
      x = label->border;
      break;
    case ALIGN_H_RIGHT:
      x = widget->w - label->border - w;
      break;
    default:
      x = (widget->w - w) >> 1;
      break;
  }

  canvas_draw_text(c, label->text.str, label->text.size, x, y);

  return RET_OK;
}

ret_t label_set_text(widget_t* widget, const wchar_t* text) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_set(&(label->text), text);
}

ret_t label_set_border(widget_t* widget, uint8_t border) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  label->border = border;

  return RET_OK;
}

ret_t label_set_text_align_h(widget_t* widget, uint8_t text_align_h) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  label->text_align_h = text_align_h;

  return RET_OK;
}

ret_t label_set_text_align_v(widget_t* widget, uint8_t text_align_v) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  label->text_align_v = text_align_v;

  return RET_OK;
}

static ret_t label_get_prop(widget_t* widget, const char* name, value_t* v) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (name[0] == 't') {
    if (strcmp(name, "text") == 0) {
      value_set_wstr(v, label->text.str);
      return RET_OK;
    } else if (strcmp(name, "text-align-v") == 0) {
      value_set_uint8(v, label->text_align_v);
      return RET_OK;
    } else if (strcmp(name, "text-align-h") == 0) {
      value_set_uint8(v, label->text_align_h);
      return RET_OK;
    }
  } else if (name[0] == 'b') {
    value_set_uint8(v, label->border);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t label_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (name[0] == 't') {
    if (strcmp(name, "text") == 0) {
      return label_set_text(widget, value_wstr(*v));
    } else if (strcmp(name, "text-align-v") == 0) {
      return label_set_text_align_v(widget, value_int(*v));
    } else if (strcmp(name, "text-align-h") == 0) {
      return label_set_text_align_h(widget, value_int(*v));
    }
  } else if (name[0] == 'b') {
    return label_set_border(widget, value_int(*v));
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_label_vtable = {
    .on_paint_self = label_on_paint_self, .get_prop = label_get_prop, .set_prop = label_set_prop};

widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  label_t* label = MEM_ZALLOC(label_t);
  return_value_if_fail(label != NULL, NULL);

  widget = WIDGETP(label);
  widget_init(widget, parent, WIDGET_LABEL);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_label_vtable;

  label->border = 3;
  wstr_init(&(label->text), 0);

  return widget;
}
