/**
 * File:   gauge_pointer.h
 * Author: AWTK Develop Team
 * Brief:  gauge_pointer
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
 * 2018-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/matrix.h"
#include "svg/bsvg_draw.h"
#include "base/widget_vtable.h"
#include "gauge/gauge_pointer.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"

#define ANCHOR_PX_STR_LEN 2
#define DEFAULT_POINTER_SIZE 0.6f

float_t gauge_pointer_get_anchor_for_str(float_t max_size, const char* anchor);

static rect_t gauge_pointer_calc_dirty_rect(widget_t* widget, int32_t img_w, int32_t img_h) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  xy_t min_x = 0;
  xy_t min_y = 0;
  xy_t max_x = 0;
  xy_t max_y = 0;
  int32_t w = img_w;
  int32_t h = img_h;
  int32_t ox = widget->x;
  int32_t oy = widget->y;
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  float_t rotation = TK_D2R(gauge_pointer->angle);
  float_t anchor_x = gauge_pointer_get_anchor_for_str(widget->w, gauge_pointer->anchor_x);
  float_t anchor_y = gauge_pointer_get_anchor_for_str(widget->h, gauge_pointer->anchor_y);

  matrix_init(&m);
  matrix_translate(&m, ox, oy);
  matrix_translate(&m, anchor_x, anchor_y);
  matrix_rotate(&m, rotation);
  matrix_translate(&m, -anchor_x, -anchor_y);

  matrix_transform_point(&m, 0, 0, &(x), &(y));
  min_x = x;
  max_x = x;
  min_y = y;
  max_y = y;

  matrix_transform_point(&m, w, 0, &(x), &(y));
  min_x = tk_min(min_x, x);
  max_x = tk_max(max_x, x);
  min_y = tk_min(min_y, y);
  max_y = tk_max(max_y, y);

  matrix_transform_point(&m, 0, h, &(x), &(y));
  min_x = tk_min(min_x, x);
  max_x = tk_max(max_x, x);
  min_y = tk_min(min_y, y);
  max_y = tk_max(max_y, y);

  matrix_transform_point(&m, w, h, &(x), &(y));
  min_x = tk_min(min_x, x);
  max_x = tk_max(max_x, x);
  min_y = tk_min(min_y, y);
  max_y = tk_max(max_y, y);

  return rect_init(min_x, min_y, max_x - min_x, max_y - min_y);
}

static ret_t gauge_pointer_invalidate(widget_t* widget, const rect_t* rect) {
  rect_t r;
  int32_t w = 0;
  int32_t h = 0;
  widget_t* parent = widget->parent;
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);

  if (gauge_pointer->bsvg_asset != NULL || gauge_pointer->image == NULL) {
    if (gauge_pointer->bsvg_asset != NULL) {
      bsvg_t bsvg;
      const asset_info_t* asset = gauge_pointer->bsvg_asset;

      bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size);
      w = bsvg.header->w;
      h = bsvg.header->h;
    } else {
      w = widget->w;
      h = widget->h * DEFAULT_POINTER_SIZE * 1.2f;
    }
  } else {
    bitmap_t bitmap;
    if (parent != NULL && !parent->destroying &&
        widget_load_image(widget, gauge_pointer->image, &bitmap) == RET_OK) {
      w = bitmap.w;
      h = bitmap.h;
    } else {
      return RET_OK;
    }
  }

  r = gauge_pointer_calc_dirty_rect(widget, w, h);
  return widget_invalidate(parent, &r);
}

ret_t gauge_pointer_set_angle(widget_t* widget, float_t angle) {
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (gauge_pointer->angle != angle) {
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_float(&(evt.old_value), gauge_pointer->angle);
    value_set_float(&(evt.new_value), angle);

    widget_invalidate(widget, NULL);
    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      gauge_pointer->angle = angle;
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  }

  return RET_OK;
}

bool_t gauge_pointer_value_is_anchor_px(const char* value) {
  const char* tmp = NULL;
  size_t len = strlen(value);

  if (len > ANCHOR_PX_STR_LEN) {
    tmp = value + len - ANCHOR_PX_STR_LEN;
    if (tk_str_eq(tmp, "px") != 0 || tk_str_eq(tmp, "PX") != 0) {
      return TRUE;
    }
  }

  return FALSE;
}

float_t gauge_pointer_get_anchor_for_str(float_t max_size, const char* anchor) {
  float_t anchor_tmp = 0.0f;
  bool_t is_anchor_px = TRUE;

  if (anchor == NULL) {
    return anchor_tmp;
  }

  anchor_tmp = tk_atof(anchor);
  is_anchor_px = gauge_pointer_value_is_anchor_px(anchor);
  anchor_tmp = is_anchor_px ? anchor_tmp : anchor_tmp * max_size;

  return anchor_tmp;
}

ret_t gauge_pointer_set_anchor_for_str(widget_t* widget, const char* anchor, bool_t is_x) {
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(anchor != NULL, RET_BAD_PARAMS);
  return_value_if_fail(gauge_pointer != NULL, RET_BAD_PARAMS);

  if (is_x) {
    gauge_pointer->anchor_x = tk_str_copy(gauge_pointer->anchor_x, anchor);
  } else {
    gauge_pointer->anchor_y = tk_str_copy(gauge_pointer->anchor_y, anchor);
  }

  return RET_OK;
}

ret_t gauge_pointer_set_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(gauge_pointer != NULL, RET_BAD_PARAMS);

  gauge_pointer_set_anchor_for_str(widget, anchor_x, TRUE);
  gauge_pointer_set_anchor_for_str(widget, anchor_y, FALSE);

  return RET_OK;
}

static ret_t gauge_pointer_load_svg_asset(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(gauge_pointer != NULL, RET_BAD_PARAMS);

  if (gauge_pointer->image != NULL) {
    const asset_info_t* asset = widget_load_asset(widget, ASSET_TYPE_IMAGE, gauge_pointer->image);

    if (gauge_pointer->bsvg_asset != NULL) {
      widget_unload_asset(widget, gauge_pointer->bsvg_asset);
      gauge_pointer->bsvg_asset = NULL;
    }
    if (asset != NULL) {
      if (asset->subtype == ASSET_TYPE_IMAGE_BSVG) {
        gauge_pointer->bsvg_asset = asset;
      } else {
        widget_unload_asset(widget, asset);
      }
    }
  }

  return RET_REMOVE;
}

ret_t gauge_pointer_set_image(widget_t* widget, const char* image) {
  widget_t* win = widget_get_window(widget);
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image != NULL && gauge_pointer != NULL, RET_BAD_PARAMS);
  gauge_pointer->image = tk_str_copy(gauge_pointer->image, image);

  if (gauge_pointer->image != NULL) {
    if (win != NULL) {
      gauge_pointer_load_svg_asset(widget, NULL);
    } else {
      widget_on(widget, EVT_BEFORE_PAINT, gauge_pointer_load_svg_asset, widget);
    }
  }

  return RET_OK;
}

static ret_t gauge_pointer_get_prop(widget_t* widget, const char* name, value_t* v) {
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GAUGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_float(v, gauge_pointer->angle);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, gauge_pointer->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    value_set_str(v, gauge_pointer->anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    value_set_str(v, gauge_pointer->anchor_y);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t gauge_pointer_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GAUGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return gauge_pointer_set_angle(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return gauge_pointer_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    return gauge_pointer_set_anchor_for_str(widget, value_str(v), TRUE);
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    return gauge_pointer_set_anchor_for_str(widget, value_str(v), FALSE);
  }

  return RET_NOT_FOUND;
}

static ret_t gauge_pointer_on_destroy(widget_t* widget) {
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(widget != NULL && gauge_pointer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(gauge_pointer->image);
  if (gauge_pointer->bsvg_asset != NULL) {
    widget_unload_asset(widget, gauge_pointer->bsvg_asset);
  }
  TKMEM_FREE(gauge_pointer->anchor_x);
  TKMEM_FREE(gauge_pointer->anchor_y);

  return RET_OK;
}

static ret_t gauge_pointer_paint_default(widget_t* widget, vgcanvas_t* vg) {
  float_t w, h;
  float_t cx, cy;
  color_t bg, fg;
  color_t black = color_init(0, 0, 0, 0xff);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  w = widget->w;
  h = widget->h;
  cx = w * 0.5f;
  cy = h * 0.5f;
  bg = style_get_color(widget->astyle, STYLE_ID_BG_COLOR, black);
  fg = style_get_color(widget->astyle, STYLE_ID_FG_COLOR, black);

  vgcanvas_begin_path(vg);
  vgcanvas_move_to(vg, cx, 0);
  vgcanvas_line_to(vg, cx, cx + h * DEFAULT_POINTER_SIZE);
  vgcanvas_set_line_width(vg, 2);
  vgcanvas_set_stroke_color(vg, bg);
  vgcanvas_stroke(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_set_fill_color(vg, fg);
  vgcanvas_arc(vg, cx, cy, cx, 0, M_PI * 2, FALSE);
  vgcanvas_fill(vg);

  return RET_OK;
}

static ret_t gauge_pointer_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  gauge_pointer_t* gauge_pointer = GAUGE_POINTER(widget);
  return_value_if_fail(gauge_pointer != NULL && widget != NULL, RET_BAD_PARAMS);

  anchor_x = gauge_pointer_get_anchor_for_str(widget->w, gauge_pointer->anchor_x);
  anchor_y = gauge_pointer_get_anchor_for_str(widget->h, gauge_pointer->anchor_y);

  rotation = TK_D2R(gauge_pointer->angle);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, rotation);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);

  if (gauge_pointer->bsvg_asset != NULL) {
    bsvg_t bsvg;
    style_t* style = widget->astyle;
    color_t black = color_init(0, 0, 0, 0xff);
    color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, black);
    color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
    const asset_info_t* asset = gauge_pointer->bsvg_asset;

    vgcanvas_set_fill_color(vg, bg);
    vgcanvas_set_stroke_color(vg, fg);
    bsvg_draw(bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size), vg);
  } else if (gauge_pointer->image != NULL &&
             widget_load_image(widget, gauge_pointer->image, &bitmap) == RET_OK) {
    float_t w = bitmap.w;
    float_t h = bitmap.h;
    vgcanvas_draw_image(vg, &bitmap, 0, 0, w, h, 0, 0, w, h);
  } else {
    gauge_pointer_paint_default(widget, vg);
  }

  vgcanvas_restore(vg);

  return RET_OK;
}

static const char* s_gauge_pointer_properties[] = {
    GAUGE_POINTER_PROP_ANGLE, WIDGET_PROP_IMAGE, WIDGET_PROP_ANCHOR_X, WIDGET_PROP_ANCHOR_Y, NULL};

TK_DECL_VTABLE(gauge_pointer) = {.size = sizeof(gauge_pointer_t),
                                 .type = WIDGET_TYPE_GAUGE_POINTER,
                                 .clone_properties = s_gauge_pointer_properties,
                                 .persistent_properties = s_gauge_pointer_properties,
                                 .allow_draw_outside = TRUE,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = gauge_pointer_create,
                                 .on_paint_self = gauge_pointer_on_paint_self,
                                 .on_paint_background = widget_on_paint_null,
                                 .set_prop = gauge_pointer_set_prop,
                                 .get_prop = gauge_pointer_get_prop,
                                 .invalidate = gauge_pointer_invalidate,
                                 .on_destroy = gauge_pointer_on_destroy};

widget_t* gauge_pointer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  gauge_pointer_t* gauge_pointer =
      GAUGE_POINTER(widget_create(parent, TK_REF_VTABLE(gauge_pointer), x, y, w, h));
  return_value_if_fail(gauge_pointer != NULL, NULL);
  gauge_pointer->anchor_x = tk_strdup("0.5");
  gauge_pointer->anchor_y = tk_strdup("0.5");
  return (widget_t*)gauge_pointer;
}

widget_t* gauge_pointer_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, gauge_pointer), NULL);

  return widget;
}
