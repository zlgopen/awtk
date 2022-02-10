/**
 * File:   color_tile.h
 * Author: AWTK Develop Team
 * Brief:  color_tile
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t color_tile_on_paint_stroke(widget_t* widget, canvas_t* c) {
  rect_t r;
  color_t bd;
  ret_t ret = RET_FAIL;
  style_t* style = NULL;
  int32_t border, border_width;
  color_tile_t* color_tile = COLOR_TILE(widget);
  uint32_t radius, radius_tl, radius_tr, radius_bl, radius_br;
  return_value_if_fail(color_tile != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  r = rect_init(0, 0, widget->w, widget->h);

  if (color_tile->border.rgba.a == 0) {
    bd = style_get_color(style, STYLE_ID_BORDER_COLOR, color_tile->border);
    if (bd.rgba.a == 0) {
      return RET_OK;
    }
  } else {
    bd = color_tile->border;
  }

  border = style_get_int(style, STYLE_ID_BORDER, BORDER_ALL);
  border_width = style_get_int(style, STYLE_ID_BORDER_WIDTH, 1);

  radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  radius_tl = style_get_int(style, STYLE_ID_ROUND_RADIUS_TOP_LEFT, radius);
  radius_tr = style_get_int(style, STYLE_ID_ROUND_RADIUS_TOP_RIGHT, radius);
  radius_bl = style_get_int(style, STYLE_ID_ROUND_RADIUS_BOTTOM_LEFT, radius);
  radius_br = style_get_int(style, STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT, radius);

  canvas_set_stroke_color(c, bd);
  if (radius_tl > 3 || radius_tr > 3 || radius_bl > 3 || radius_br > 3) {
    if (canvas_stroke_rounded_rect_ex(c, &r, NULL, &bd, radius_tl, radius_tr, radius_bl, radius_br,
                                      border_width, border) != RET_OK) {
      widget_stroke_border_rect_for_border_type(c, &r, bd, border, border_width);
    }
  } else {
    widget_stroke_border_rect_for_border_type(c, &r, bd, border, border_width);
  }

  return ret;
}

static ret_t color_tile_on_paint_fill(widget_t* widget, canvas_t* c) {
  rect_t r;
  color_t color = {0};
  gradient_t agradient;
  ret_t ret = RET_FAIL;
  style_t* style = NULL;
  gradient_t* gradient = NULL;
  color_tile_t* color_tile = COLOR_TILE(widget);
  uint32_t radius, radius_tl, radius_tr, radius_bl, radius_br;
  return_value_if_fail(color_tile != NULL && widget != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  r = rect_init(0, 0, widget->w, widget->h);
  if (color_tile->bg.rgba.a == 0) {
    gradient = style_get_gradient(style, STYLE_ID_BG_COLOR, &agradient);
    if (gradient == NULL || gradient->nr == 0) {
      return RET_OK;
    }
    if (gradient->nr == 1) {
      color = gradient_get_first_color(gradient);
      gradient = NULL;
    }
  } else {
    color = color_tile->bg;
  }
  if (gradient == NULL && color.rgba.a == 0) {
    return RET_OK;
  }

  radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  radius_tl = style_get_int(style, STYLE_ID_ROUND_RADIUS_TOP_LEFT, radius);
  radius_tr = style_get_int(style, STYLE_ID_ROUND_RADIUS_TOP_RIGHT, radius);
  radius_bl = style_get_int(style, STYLE_ID_ROUND_RADIUS_BOTTOM_LEFT, radius);
  radius_br = style_get_int(style, STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT, radius);

  canvas_set_fill_color(c, color);
  if (radius_tl > 3 || radius_tr > 3 || radius_bl > 3 || radius_br > 3) {
    if (gradient != NULL) {
      ret = canvas_fill_rounded_rect_gradient_ex(c, &r, NULL, gradient, radius_tl, radius_tr,
                                                 radius_bl, radius_br);
    } else {
      ret = canvas_fill_rounded_rect_ex(c, &r, NULL, &color, radius_tl, radius_tr, radius_bl,
                                        radius_br);
    }
  }
  if (ret == RET_FAIL) {
    ret = canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }

  return ret;
}

static ret_t color_tile_on_paint_self(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  color_tile_on_paint_fill(widget, c);
  color_tile_on_paint_stroke(widget, c);
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
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_uint32(&(evt.old_value), color_tile->bg.color);
    value_set_uint32(&(evt.new_value), color.color);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      color_tile->bg = color;
      color_hex_str(color_tile->bg, color_tile->bg_color);
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  }

  return RET_OK;
}

const char* color_tile_get_bg_color(widget_t* widget) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, NULL);

  return color_tile->bg_color;
}

const char* color_tile_get_border_color(widget_t* widget) {
  color_tile_t* color_tile = COLOR_TILE(widget);
  return_value_if_fail(color_tile != NULL, NULL);

  return color_tile->border_color;
}
