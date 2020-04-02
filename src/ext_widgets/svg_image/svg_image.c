/**
 * File:   svg_image.h
 * Author: AWTK Develop Team
 * Brief:  svg_image
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
 * 2018-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "svg/bsvg.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "svg/bsvg_draw.h"
#include "base/widget_vtable.h"
#include "svg_image/svg_image.h"

static ret_t svg_image_load_bsvg(widget_t* widget) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  return_value_if_fail(svg_image != NULL && image_base != NULL, RET_BAD_PARAMS);

  if (svg_image->bsvg_asset != NULL) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    svg_image->bsvg_asset = NULL;
  }

  svg_image->bsvg_asset = widget_load_asset(widget, ASSET_TYPE_IMAGE, image_base->image);
  return_value_if_fail(svg_image->bsvg_asset != NULL, RET_NOT_FOUND);

  if (svg_image->bsvg_asset->subtype != ASSET_TYPE_IMAGE_BSVG) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    log_debug("invalid bsvg asset: %s\n", image_base->image);
    svg_image->bsvg_asset = NULL;
  }

  return RET_OK;
}

static ret_t svg_image_on_paint_self(widget_t* widget, canvas_t* c) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  image_base_t* image_base = IMAGE_BASE(widget);
  return_value_if_fail(svg_image != NULL && image_base != NULL && widget != NULL, RET_BAD_PARAMS);

  if (image_base->image == NULL || image_base->image[0] == '\0') {
    widget_paint_helper(widget, c, NULL, NULL);
    return RET_OK;
  }

  if (svg_image_load_bsvg(widget) == RET_OK) {
    bsvg_t bsvg;
    int32_t x = 0;
    int32_t y = 0;
    style_t* style = widget->astyle;
    color_t black = color_init(0, 0, 0, 0xff);
    const asset_info_t* asset = svg_image->bsvg_asset;
    color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, black);
    color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
    return_value_if_fail(asset != NULL && asset->data != NULL, RET_FAIL);

    return_value_if_fail(bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size) != NULL,
                         RET_BAD_PARAMS);
    if (bsvg.header->w && bsvg.header->h) {
      x = (widget->w - (int32_t)bsvg.header->w) / 2;
      y = (widget->h - (int32_t)bsvg.header->h) / 2;
    } else if (bsvg.header->viewport.w && bsvg.header->viewport.h) {
      x = (widget->w - (int32_t)bsvg.header->viewport.w) / 2;
      y = (widget->h - (int32_t)bsvg.header->viewport.h) / 2;
    }

    vgcanvas_save(vg);

    image_transform(widget, c);
    vgcanvas_translate(vg, x, y);
    vgcanvas_set_fill_color(vg, bg);
    vgcanvas_set_stroke_color(vg, fg);

    bsvg_draw(&bsvg, vg);

    vgcanvas_restore(vg);
  }

  widget_paint_helper(widget, c, NULL, NULL);

  return RET_OK;
}

static ret_t svg_image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    svg_image_set_image(widget, value_str(v));
    return RET_OK;
  } else {
    return image_base_set_prop(widget, name, v);
  }
}

static ret_t svg_image_on_destroy(widget_t* widget) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_BAD_PARAMS);

  if (svg_image->bsvg_asset != NULL) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    svg_image->bsvg_asset = NULL;
  }

  return image_base_on_destroy(widget);
}

ret_t svg_image_set_image(widget_t* widget, const char* name) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL && name != NULL, RET_BAD_PARAMS);

  svg_image->image_base.image = tk_str_copy(svg_image->image_base.image, name);
  if (svg_image->bsvg_asset != NULL) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    svg_image->bsvg_asset = NULL;
  }

  return widget_invalidate(widget, NULL);
}

static const char* s_svg_image_properties[] = {
    WIDGET_PROP_IMAGE,     WIDGET_PROP_SCALE_X,    WIDGET_PROP_SCALE_Y,
    WIDGET_PROP_ANCHOR_X,  WIDGET_PROP_ANCHOR_Y,   WIDGET_PROP_ROTATION,
    WIDGET_PROP_CLICKABLE, WIDGET_PROP_SELECTABLE, NULL};

TK_DECL_VTABLE(svg_image) = {.size = sizeof(svg_image_t),
                             .type = WIDGET_TYPE_SVG_IMAGE,
                             .clone_properties = s_svg_image_properties,
                             .persistent_properties = s_svg_image_properties,
                             .parent = TK_PARENT_VTABLE(image_base),
                             .create = svg_image_create,
                             .on_destroy = svg_image_on_destroy,
                             .on_event = image_base_on_event,
                             .on_paint_self = svg_image_on_paint_self,
                             .on_paint_background = widget_on_paint_null,
                             .on_copy = image_base_on_copy,
                             .set_prop = svg_image_set_prop,
                             .get_prop = image_base_get_prop};

widget_t* svg_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(svg_image), x, y, w, h);
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, NULL);

  image_base_init(widget);

  return widget;
}

widget_t* svg_image_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, svg_image), NULL);

  return widget;
}
