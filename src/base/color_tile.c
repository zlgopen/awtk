/**
 * File:   color_tile.h
 * Author: AWTK Develop Team
 * Brief:  color_tile
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
 * 2018-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/color_tile.h"
#include "base/color_parser.h"

static ret_t color_tile_on_paint_self(widget_t* widget, canvas_t* c) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  canvas_set_fill_color(c, color_tile->bg);
  canvas_fill_rect(c, 0, 0, widget->w, widget->h);

  canvas_set_fill_color(c, color_tile->border);
  canvas_stroke_rect(c, 0, 0, widget->w, widget->h);

  return RET_OK;
}

static ret_t color_tile_get_prop(widget_t* widget, const char* name, value_t* v) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_BG_COLOR)) {
    value_set_str(v, color_tile->bg_color);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BORDER_COLOR)) {
    value_set_str(v, color_tile->border_color);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t color_tile_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_BG_COLOR)) {
    return color_tile_set_bg_color(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_BORDER_COLOR)) {
    return color_tile_set_border_color(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_color_tile_properties[] = {WIDGET_PROP_BG_COLOR, WIDGET_PROP_BORDER_COLOR,
                                                NULL};
static const widget_vtable_t s_color_tile_vtable = {
    .size = sizeof(color_tile_t),
    .type = WIDGET_TYPE_COLOR_TILE,
    .create = color_tile_create,
    .clone_properties = s_color_tile_properties,
    .persistent_properties = s_color_tile_properties,
    .set_prop = color_tile_set_prop,
    .get_prop = color_tile_get_prop,
    .on_paint_self = color_tile_on_paint_self};

widget_t* color_tile_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  color_tile_t* color_tile = TKMEM_ZALLOC(color_tile_t);
  widget_t* widget = WIDGET(color_tile);
  return_value_if_fail(color_tile != NULL, NULL);

  widget_init(widget, parent, &s_color_tile_vtable, x, y, w, h);
  color_tile_set_bg_color(widget, "#ffffff");
  color_tile_set_border_color(widget, "#000000");

  return widget;
}

widget_t* color_tile_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_color_tile_vtable, NULL);

  return widget;
}

ret_t color_tile_set_bg_color(widget_t* widget, const char* color) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL && color != NULL, RET_BAD_PARAMS);

  color_tile->bg = color_parse_simple(color);
  color_hex_str(color_tile->bg, color_tile->bg_color);

  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t color_tile_set_border_color(widget_t* widget, const char* color) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL && color != NULL, RET_BAD_PARAMS);

  color_tile->border = color_parse_simple(color);
  color_hex_str(color_tile->border, color_tile->border_color);

  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t color_tile_set_value(widget_t* widget, color_t color) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, RET_BAD_PARAMS);

  color_tile->bg = color;
  color_hex_str(color_tile->bg, color_tile->bg_color);

  widget_invalidate(widget, NULL);

  return RET_OK;
}
