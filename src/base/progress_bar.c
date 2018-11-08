/**
 * File:   progress_bar.c
 * Author: AWTK Develop Team
 * Brief:  progress_bar
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
#include "base/utils.h"
#include "base/progress_bar.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t progress_bar_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  rect_t r;
  color_t color;
  bitmap_t img;
  const char* image_name = NULL;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  image_draw_type_t draw_type = IMAGE_DRAW_CENTER;
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (progress_bar->vertical) {
    x = 0;
    y = 0;
    w = widget->w;
    h = (widget->h * (100 - progress_bar->value)) / 100;
  } else {
    x = 0;
    y = 0;
    h = widget->h;
    w = (widget->w * (100 - progress_bar->value)) / 100;
  }

  r = rect_init(x, y, w, h);
  color = style_get_color(style, STYLE_ID_BG_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }

  image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  draw_type =
      (image_draw_type_t)style_get_int(style, STYLE_ID_BG_IMAGE_DRAW_TYPE, IMAGE_DRAW_PATCH3_X);
  if (image_name && widget_load_image(widget, image_name, &img) == RET_OK) {
    if (progress_bar->vertical) {
      r.h += r.w;
    } else {
      r.w += r.h;
    }
    canvas_draw_image_ex(c, &img, draw_type, &r);
  }

  if (progress_bar->vertical) {
    x = 0;
    w = widget->w;
    h = (widget->h * progress_bar->value) / 100;
    y = widget->h - h;
  } else {
    h = widget->h;
    w = (widget->w * progress_bar->value) / 100;
    y = 0;
    x = 0;
  }

  r = rect_init(x, y, w, h);
  color = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }

  image_name = style_get_str(style, STYLE_ID_FG_IMAGE, NULL);
  draw_type =
      (image_draw_type_t)style_get_int(style, STYLE_ID_FG_IMAGE_DRAW_TYPE, IMAGE_DRAW_PATCH3_X);
  if (image_name && widget_load_image(widget, image_name, &img) == RET_OK) {
    canvas_draw_image_ex(c, &img, draw_type, &r);
  }

  color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_stroke_color(c, color);
    canvas_stroke_rect(c, 0, 0, widget->w, widget->h);
  }

  color = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  if (progress_bar->show_text && color.rgba.a) {
    uint32_t i = 0;
    uint32_t len = 0;
    char s[TK_NUM_MAX_LEN + 1];
    wchar_t str[TK_NUM_MAX_LEN + 1];
    uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);

    tk_itoa(s, sizeof(s), progress_bar->value);
    len = strlen(s);
    s[len] = '%';
    s[len + 1] = '\0';

    for (i = 0; s[i]; i++) {
      str[i] = s[i];
    }

    str[i] = 0;
    canvas_set_text_color(c, color);
    canvas_set_font(c, font_name, font_size);
    w = canvas_measure_text(c, str, i);
    x = (widget->w - w) >> 1;
    y = (widget->h - font_size) >> 1;
    canvas_draw_text(c, str, i, x, y);
  }

  return RET_OK;
}

ret_t progress_bar_set_value(widget_t* widget, uint8_t value) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL && value <= 100, RET_BAD_PARAMS);

  if (progress_bar->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    progress_bar->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

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

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_uint8(v, progress_bar->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, progress_bar->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    value_set_bool(v, progress_bar->show_text);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_bar_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return progress_bar_set_value(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return progress_bar_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    return progress_bar_set_show_text(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_progress_bar_clone_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_VERTICAL,
                                                        WIDGET_PROP_SHOW_TEXT, NULL};
static const widget_vtable_t s_progress_bar_vtable = {
    .size = sizeof(progress_bar_t),
    .type = WIDGET_TYPE_PROGRESS_BAR,
    .clone_properties = s_progress_bar_clone_properties,
    .create = progress_bar_create,
    .on_paint_self = progress_bar_on_paint_self,
    .on_paint_background = widget_on_paint_null,
    .get_prop = progress_bar_get_prop,
    .set_prop = progress_bar_set_prop};

widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  progress_bar_t* progress_bar = TKMEM_ZALLOC(progress_bar_t);
  widget_t* widget = WIDGET(progress_bar);
  return_value_if_fail(progress_bar != NULL, NULL);

  return widget_init(widget, parent, &s_progress_bar_vtable, x, y, w, h);
}

widget_t* progress_bar_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_progress_bar_vtable, NULL);

  return widget;
}
