/**
 * File:   color_tile.h
 * Author: AWTK Develop Team
 * Brief:  color_tile
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
 * 2018-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/color_tile.h"
#include "tkc/color_parser.h"

static ret_t color_tile_on_paint_self(widget_t* widget, canvas_t* c) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, RET_BAD_PARAMS);

  canvas_set_fill_color(c, color_tile->bg);
  canvas_fill_rect(c, 0, 0, widget->w, widget->h);

  canvas_set_stroke_color(c, color_tile->border);
  canvas_stroke_rect(c, 0, 0, widget->w, widget->h);

  return RET_OK;
}

static ret_t color_tile_get_prop(widget_t* widget, const char* name, value_t* v) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_BG_COLOR)) {
    value_set_str(v, color_tile->bg_color);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_uint32(v, color_tile->bg.color);
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
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    color_t c;
    c.color = value_int(v);
    return color_tile_set_value(widget, c);
  } else if (tk_str_eq(name, WIDGET_PROP_BORDER_COLOR)) {
    return color_tile_set_border_color(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static const char* const s_color_tile_properties[] = {WIDGET_PROP_BG_COLOR,
                                                      WIDGET_PROP_BORDER_COLOR, NULL};
TK_DECL_VTABLE(color_tile) = {.size = sizeof(color_tile_t),
                              .type = WIDGET_TYPE_COLOR_TILE,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = color_tile_create,
                              .clone_properties = s_color_tile_properties,
                              .persistent_properties = s_color_tile_properties,
                              .set_prop = color_tile_set_prop,
                              .get_prop = color_tile_get_prop,
                              .on_paint_self = color_tile_on_paint_self};

widget_t* color_tile_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(color_tile), x, y, w, h);
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, NULL);

  color_tile_set_bg_color(widget, "#ffffff");
  color_tile_set_border_color(widget, "#000000");

  return widget;
}

widget_t* color_tile_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, color_tile), NULL);

  return widget;
}

ret_t color_tile_set_bg_color(widget_t* widget, const char* color) {
  return_value_if_fail(widget != NULL && color != NULL, RET_BAD_PARAMS);

  return color_tile_set_value(widget, color_parse(color));
}

ret_t color_tile_set_border_color(widget_t* widget, const char* color) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL && color != NULL, RET_BAD_PARAMS);

  color_tile->border = color_parse(color);
  color_hex_str(color_tile->border, color_tile->border_color);

  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t color_tile_set_value(widget_t* widget, color_t color) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, RET_BAD_PARAMS);

  if (color_tile->bg.color != color.color) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);

    color_tile->bg = color;
    color_hex_str(color_tile->bg, color_tile->bg_color);

    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);

    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}
