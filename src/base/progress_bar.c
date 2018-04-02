/**
 * File:   progress_bar.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  progress_bar
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/progress_bar.h"
#include "base/mem.h"
#include "base/utils.h"

static ret_t progress_bar_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  style_t* style = &(widget->style);
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  color_t color = color_init(0xff, 0xff, 0xff, 0xff);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_set_text_color(c, style_get_color(style, STYLE_ID_TEXT_COLOR, color));

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_FG_COLOR, color));
  if (progress_bar->vertical) {
    h = (widget->h * progress_bar->value) / 100;
    canvas_fill_rect(c, x, y + widget->h - h, widget->w, h);
  } else {
    w = (widget->w * progress_bar->value) / 100;
    canvas_fill_rect(c, x, y, w, widget->h);
  }

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_BG_COLOR, color));
  if (progress_bar->vertical) {
    h = widget->h - h;
    canvas_fill_rect(c, x, y, widget->w, h);
  } else {
    x = w;
    w = widget->w - w;
    canvas_fill_rect(c, x, y, w, widget->h);
  }

  canvas_set_stroke_color(c, style_get_color(style, STYLE_ID_BORDER_COLOR, color));
  canvas_stroke_rect(c, 0, 0, widget->w, widget->h);

  if (progress_bar->show_text) {
    char s[32];
    uint32_t i = 0;
    wchar_t str[32];
    uint32_t len = 0;
    uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, 20);
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);

    ftk_itoa(s, sizeof(s), progress_bar->value);
    len = strlen(s);
    s[len] = '%';
    s[len + 1] = '\0';

    for (i = 0; s[i]; i++) {
      str[i] = s[i];
    }

    str[i] = 0;
    canvas_set_font(c, font_name, font_size);
    w = canvas_measure_text(c, str, i);
    x = (widget->w - w) >> 1;
    y = (widget->h - font_size) >> 1;
    canvas_draw_text(c, str, i, x, y);
  }

  return RET_OK;
}

ret_t progress_bar_set_value(widget_t* widget, uint8_t value) {
  rect_t r;
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL && value <= 100, RET_BAD_PARAMS);
  progress_bar->value = value;

  rect_init(r, 0, 0, widget->w, widget->h);
  widget_invalidate(widget, &r);

  return RET_OK;
}

ret_t progress_bar_set_vertical(widget_t* widget, bool_t vertical) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  progress_bar->vertical = vertical;

  return RET_OK;
}

ret_t progress_bar_set_show_text(widget_t* widget, bool_t show_text) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  progress_bar->show_text = show_text;

  return RET_OK;
}

static ret_t progress_bar_get_prop(widget_t* widget, const char* name, value_t* v) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "value") == 0) {
    value_set_uint8(v, progress_bar->value);
    return RET_OK;
  } else if (strcmp(name, "vertical") == 0) {
    value_set_bool(v, progress_bar->vertical);
    return RET_OK;
  } else if (strcmp(name, "show_text") == 0) {
    value_set_bool(v, progress_bar->show_text);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_bar_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "value") == 0) {
    return progress_bar_set_value(widget, value_int(v));
  } else if (strcmp(name, "vertical") == 0) {
    return progress_bar_set_vertical(widget, value_bool(v));
  } else if (strcmp(name, "show_text") == 0) {
    return progress_bar_set_show_text(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_progress_bar_vtable = {.on_paint_self = progress_bar_on_paint_self,
                                                      .get_prop = progress_bar_get_prop,
                                                      .set_prop = progress_bar_set_prop};

widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  progress_bar_t* progress_bar = MEM_ZALLOC(progress_bar_t);
  return_value_if_fail(progress_bar != NULL, NULL);

  widget = WIDGETP(progress_bar);
  widget_init(widget, parent, WIDGET_PROGRESS_BAR);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_progress_bar_vtable;

  return widget;
}
