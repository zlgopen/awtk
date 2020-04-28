/**
 * File:   guage_pointer.h
 * Author: AWTK Develop Team
 * Brief:  guage_pointer
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
 * 2018-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/matrix.h"
#include "svg/bsvg_draw.h"
#include "base/widget_vtable.h"
#include "guage/guage_pointer.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"

#define ANCHOR_PX_STR_LEN 2

ret_t guage_pointer_set_angle(widget_t* widget, int32_t angle) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (guage_pointer->angle != angle) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    guage_pointer->angle = angle;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget->parent, NULL);
  }

  return RET_OK;
}

bool_t guage_pointer_value_is_anchor_px(const char* value) {
  const char* tmp = NULL;
  size_t len = strlen(value);
  return_value_if_fail(len > ANCHOR_PX_STR_LEN, FALSE);

  tmp = value + len - ANCHOR_PX_STR_LEN;
  if (tk_str_eq(tmp, "px") != 0 || tk_str_eq(tmp, "PX") != 0) {
    return TRUE;
  }
  return FALSE;
}

float_t guage_pointer_get_anchor_for_str(float_t max_size, const char* anchor) {
  float_t anchor_tmp = 0.0f;
  bool_t is_anchor_px = TRUE;

  return_value_if_fail(anchor != NULL, 0);

  anchor_tmp = tk_atof(anchor);
  is_anchor_px = guage_pointer_value_is_anchor_px(anchor);
  anchor_tmp = is_anchor_px ? anchor_tmp : anchor_tmp * max_size;

  return anchor_tmp;
}

ret_t guage_pointer_set_anchor_for_str(widget_t* widget, const char* anchor, bool_t is_x) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(anchor != NULL, RET_BAD_PARAMS);
  return_value_if_fail(guage_pointer != NULL, RET_BAD_PARAMS);

  if (is_x) {
    guage_pointer->anchor_x = tk_str_copy(guage_pointer->anchor_x, anchor);
  } else {
    guage_pointer->anchor_y = tk_str_copy(guage_pointer->anchor_y, anchor);
  }

  return RET_OK;
}

ret_t guage_pointer_set_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(guage_pointer != NULL, RET_BAD_PARAMS);

  guage_pointer_set_anchor_for_str(widget, anchor_x, TRUE);
  guage_pointer_set_anchor_for_str(widget, anchor_y, FALSE);

  return RET_OK;
}

static ret_t guage_pointer_load_svg_asset(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);

  if (guage_pointer->image != NULL) {
    const asset_info_t* asset = widget_load_asset(widget, ASSET_TYPE_IMAGE, guage_pointer->image);

    if (guage_pointer->bsvg_asset != NULL) {
      widget_unload_asset(widget, guage_pointer->bsvg_asset);
      guage_pointer->bsvg_asset = NULL;
    }
    if (asset != NULL) {
      if (asset->subtype == ASSET_TYPE_IMAGE_BSVG) {
        guage_pointer->bsvg_asset = asset;
      } else {
        widget_unload_asset(widget, asset);
      }
    }
  }

  return RET_REMOVE;
}

ret_t guage_pointer_set_image(widget_t* widget, const char* image) {
  widget_t* win = widget_get_window(widget);
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);
  guage_pointer->image = tk_str_copy(guage_pointer->image, image);

  if (guage_pointer->image != NULL) {
    if (win != NULL) {
      guage_pointer_load_svg_asset(widget, NULL);
    } else {
      widget_on(widget, EVT_BEFORE_PAINT, guage_pointer_load_svg_asset, widget);
    }
  }

  return RET_OK;
}

static ret_t guage_pointer_get_prop(widget_t* widget, const char* name, value_t* v) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, guage_pointer->angle);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, guage_pointer->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    value_set_str(v, guage_pointer->anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    value_set_str(v, guage_pointer->anchor_y);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t guage_pointer_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return guage_pointer_set_angle(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return guage_pointer_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    return guage_pointer_set_anchor_for_str(widget, value_str(v), TRUE);
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    return guage_pointer_set_anchor_for_str(widget, value_str(v), FALSE);
  }

  return RET_NOT_FOUND;
}

static ret_t guage_pointer_on_destroy(widget_t* widget) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL && guage_pointer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(guage_pointer->image);
  if (guage_pointer->bsvg_asset != NULL) {
    widget_unload_asset(widget, guage_pointer->bsvg_asset);
  }
  TKMEM_FREE(guage_pointer->anchor_x);
  TKMEM_FREE(guage_pointer->anchor_y);

  return RET_OK;
}

static ret_t guage_pointer_paint_default(widget_t* widget, vgcanvas_t* vg) {
  float_t w = widget->w;
  float_t h = widget->h;
  float_t cx = w * 0.5f;
  float_t cy = h * 0.5f;
  style_t* style = widget->astyle;
  color_t black = color_init(0, 0, 0, 0xff);
  color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, black);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);

  vgcanvas_begin_path(vg);
  vgcanvas_move_to(vg, cx, 0);
  vgcanvas_line_to(vg, cx, cx + h * 0.6f);
  vgcanvas_set_line_width(vg, 2);
  vgcanvas_set_stroke_color(vg, bg);
  vgcanvas_stroke(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_set_fill_color(vg, fg);
  vgcanvas_arc(vg, cx, cy, cx, 0, M_PI * 2, FALSE);
  vgcanvas_fill(vg);

  return RET_OK;
}

static ret_t guage_pointer_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);

  anchor_x = guage_pointer_get_anchor_for_str(widget->w, guage_pointer->anchor_x);
  anchor_y = guage_pointer_get_anchor_for_str(widget->h, guage_pointer->anchor_y);

  rotation = TK_D2R(guage_pointer->angle);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, rotation);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);

  if (guage_pointer->bsvg_asset != NULL) {
    bsvg_t bsvg;
    style_t* style = widget->astyle;
    color_t black = color_init(0, 0, 0, 0xff);
    color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, black);
    color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
    const asset_info_t* asset = guage_pointer->bsvg_asset;

    vgcanvas_set_fill_color(vg, bg);
    vgcanvas_set_stroke_color(vg, fg);
    bsvg_draw(bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size), vg);
  } else if (guage_pointer->image != NULL &&
             widget_load_image(widget, guage_pointer->image, &bitmap) == RET_OK) {
    float_t w = bitmap.w;
    float_t h = bitmap.h;
    vgcanvas_draw_image(vg, &bitmap, 0, 0, w, h, 0, 0, w, h);
  } else {
    guage_pointer_paint_default(widget, vg);
  }

  vgcanvas_restore(vg);

  return RET_OK;
}

static const char* s_guage_pointer_properties[] = {
    GUAGE_POINTER_PROP_ANGLE, WIDGET_PROP_IMAGE, WIDGET_PROP_ANCHOR_X, WIDGET_PROP_ANCHOR_Y, NULL};

TK_DECL_VTABLE(guage_pointer) = {.size = sizeof(guage_pointer_t),
                                 .type = WIDGET_TYPE_GUAGE_POINTER,
                                 .clone_properties = s_guage_pointer_properties,
                                 .persistent_properties = s_guage_pointer_properties,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = guage_pointer_create,
                                 .on_paint_self = guage_pointer_on_paint_self,
                                 .on_paint_background = widget_on_paint_null,
                                 .set_prop = guage_pointer_set_prop,
                                 .get_prop = guage_pointer_get_prop,
                                 .on_destroy = guage_pointer_on_destroy};

widget_t* guage_pointer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  guage_pointer_t* guage_pointer =
      GUAGE_POINTER(widget_create(parent, TK_REF_VTABLE(guage_pointer), x, y, w, h));
  guage_pointer->anchor_x = tk_strdup("0.5");
  guage_pointer->anchor_y = tk_strdup("0.5");
  return (widget_t*)guage_pointer;
}

widget_t* guage_pointer_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, guage_pointer), NULL);

  return widget;
}
