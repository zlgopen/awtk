/**
 * File:   svg_image.h
 * Author: AWTK Develop Team
 * Brief:  svg_image
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "svg/bsvg.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "svg/bsvg_draw.h"
#include "base/widget_vtable.h"
#include "svg_image/svg_image.h"
#include "base/canvas_offline.h"

static ret_t svg_image_load_bsvg(widget_t* widget) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  return_value_if_fail(svg_image != NULL && image_base != NULL, RET_BAD_PARAMS);

  if (svg_image->bsvg_asset != NULL) {
    /* change svg image or resize widget need destory */
    if (svg_image->canvas_offline != NULL) {
      canvas_offline_destroy(svg_image->canvas_offline);
      svg_image->canvas_offline = NULL;
    }

    widget_unload_asset(widget, svg_image->bsvg_asset);
    svg_image->bsvg_asset = NULL;
  }

  svg_image->bsvg_asset =
      widget_load_asset_ex(widget, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BSVG, image_base->image);
  return_value_if_fail(svg_image->bsvg_asset != NULL, RET_NOT_FOUND);

  if (svg_image->bsvg_asset->subtype != ASSET_TYPE_IMAGE_BSVG) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    log_debug("invalid bsvg asset: %s\n", image_base->image);
    svg_image->bsvg_asset = NULL;
  }

  return RET_OK;
}

static ret_t svg_image_draw_type_ex(widget_t* widget, void* ctx) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(widget != NULL && svg_image != NULL && ctx != NULL, RET_BAD_PARAMS);

  switch (svg_image->draw_type) {
    case IMAGE_DRAW_DEFAULT: {
      svg_image->scale_x = 1.0f;
      svg_image->scale_y = 1.0f;
      break;
    }
    case IMAGE_DRAW_SCALE: {
      bsvg_t* bsvg = (bsvg_t*)ctx;
      svg_image->scale_x = (float_t)((widget->w * 1.0f) / (bsvg->header->w * 1.0f));
      svg_image->scale_y = (float_t)((widget->h * 1.0f) / (bsvg->header->h * 1.0f));
      break;
    }
    case IMAGE_DRAW_SCALE_AUTO: {
      bsvg_t* bsvg = (bsvg_t*)ctx;
      if (widget->w < widget->h) {
        svg_image->scale_x = (float_t)((widget->w * 1.0f) / (bsvg->header->w * 1.0f));
        svg_image->scale_y = svg_image->scale_x;
      } else {
        svg_image->scale_y = (float_t)((widget->h * 1.0f) / (bsvg->header->h * 1.0f));
        svg_image->scale_x = svg_image->scale_y;
      }
      break;
    }
    default: {
      log_debug("not impl svg draw type: %d\n", svg_image->draw_type);
      svg_image->draw_type = IMAGE_DRAW_DEFAULT;
      break;
    }
  }
  return RET_OK;
}

static ret_t svg_image_paint_before_adjust(widget_t* widget) {
  bsvg_t bsvg;
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(widget != NULL && svg_image != NULL, RET_BAD_PARAMS);

  const asset_info_t* asset = svg_image->bsvg_asset;
  return_value_if_fail(asset != NULL && asset->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size) != NULL,
                       RET_BAD_PARAMS);

  if (widget->w <= 0 || widget->h <= 0 || bsvg.header->w <= 0 || bsvg.header->h <= 0) {
    return RET_OK;
  }

  /* resize */
  if (svg_image->org_h != widget->h || svg_image->org_w != widget->w) {
    svg_image->org_w = widget->w;
    svg_image->org_h = widget->h;
    svg_image_draw_type_ex(widget, &bsvg);

    /* change svg image or resize widget need destory */
    if (svg_image->canvas_offline != NULL) {
      canvas_offline_destroy(svg_image->canvas_offline);
      svg_image->canvas_offline = NULL;
    }
  }
  return RET_OK;
}

static ret_t svg_image_paint_online_canvas(widget_t* widget, canvas_t* c) {
  bsvg_t bsvg;
  int32_t x = 0;
  int32_t y = 0;
  rect_t r = {0};
  rect_t r_save = {0};
  rect_t r_vg_save = {0};
  style_t* style = widget->astyle;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t black = color_init(0, 0, 0, 0xff);
  svg_image_t* svg_image = SVG_IMAGE(widget);
  color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, black);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
  return_value_if_fail(svg_image != NULL && widget != NULL && vg != NULL, RET_BAD_PARAMS);

  const asset_info_t* asset = svg_image->bsvg_asset;
  return_value_if_fail(asset != NULL && asset->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(bsvg_init(&bsvg, (const uint32_t*)asset->data, asset->size) != NULL,
                       RET_BAD_PARAMS);
  if (bsvg.header->w > 0 && bsvg.header->h > 0) {
    x = (widget->w - ((int32_t)bsvg.header->w * svg_image->scale_x)) / 2;
    y = (widget->h - ((int32_t)bsvg.header->h * svg_image->scale_y)) / 2;
  }

  canvas_save(c);
  canvas_get_clip_rect(c, &r_save);
  vgcanvas_save(vg);
  r_vg_save = rect_from_rectf(vgcanvas_get_clip_rect(vg));

  r = rect_init(c->ox, c->oy, widget->w, widget->h);
  r = rect_intersect(&r, &r_save);
  canvas_set_clip_rect(c, &r);
  vgcanvas_clip_rect(vg, r.x, r.y, r.w, r.h);

  image_transform(widget, c);
  vgcanvas_translate(vg, x, y);
  vgcanvas_set_fill_color(vg, bg);
  vgcanvas_set_stroke_color(vg, fg);
  vgcanvas_scale(vg, svg_image->scale_x, svg_image->scale_y);

  bsvg_draw(&bsvg, vg);

  vgcanvas_clip_rect(vg, r_vg_save.x, r_vg_save.y, r_vg_save.w, r_vg_save.h);
  vgcanvas_restore(vg);
  canvas_set_clip_rect(c, &r_save);
  canvas_restore(c);

  return RET_OK;
}

static ret_t svg_image_repaint_offline_cache(widget_t* widget, canvas_t* c) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_FAIL);

  if (svg_image->canvas_offline == NULL) {
#if defined(WITH_BITMAP_RGBA) || defined(WITH_GPU)
    svg_image->canvas_offline = canvas_offline_create_by_widget(widget, BITMAP_FMT_RGBA8888);
#else
    svg_image->canvas_offline = canvas_offline_create_by_widget(widget, BITMAP_FMT_BGRA8888);
#endif
  }

  canvas_offline_begin_draw(svg_image->canvas_offline);
  canvas_offline_clear_canvas(svg_image->canvas_offline);
  svg_image_paint_online_canvas(widget, svg_image->canvas_offline);
  canvas_offline_end_draw(svg_image->canvas_offline);

  return RET_OK;
}

static ret_t svg_image_paint_offline_canvas(widget_t* widget, canvas_t* c) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_FAIL);

  if (svg_image->canvas_offline == NULL || image_need_transform(widget)) {
    /* need repaint offline canvas cache */
    svg_image_repaint_offline_cache(widget, c);
  }

  if (svg_image->canvas_offline != NULL) {
    /* draw by bitmap */
    canvas_draw_image_at(c, canvas_offline_get_bitmap(svg_image->canvas_offline), 0, 0);
  }

  return RET_OK;
}

static ret_t svg_image_on_paint_self(widget_t* widget, canvas_t* c) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL && widget != NULL, RET_BAD_PARAMS);

  if (svg_image->bsvg_asset != NULL) {
    return_value_if_fail(svg_image_paint_before_adjust(widget) == RET_OK, RET_FAIL);

    if (svg_image->is_cache_mode == TRUE) {
      svg_image_paint_offline_canvas(widget, c);
    } else {
      svg_image_paint_online_canvas(widget, c);
    }
  }

  widget_paint_helper(widget, c, NULL, NULL);
  return RET_OK;
}

static ret_t svg_image_on_destroy(widget_t* widget) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_BAD_PARAMS);

  if (svg_image->bsvg_asset != NULL) {
    widget_unload_asset(widget, svg_image->bsvg_asset);
    svg_image->bsvg_asset = NULL;
  }

  if (svg_image->canvas_offline != NULL) {
    canvas_offline_destroy(svg_image->canvas_offline);
    svg_image->canvas_offline = NULL;
  }

  return image_base_on_destroy(widget);
}

ret_t svg_image_set_image(widget_t* widget, const char* name) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL && name != NULL, RET_BAD_PARAMS);

  image_base_set_image(widget, name);
  svg_image_load_bsvg(widget);

  return widget_invalidate(widget, NULL);
}

ret_t svg_image_set_cache_mode(widget_t* widget, bool_t is_cache_mode) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_BAD_PARAMS);

  svg_image->is_cache_mode = is_cache_mode;

  return widget_invalidate(widget, NULL);
}

ret_t svg_image_set_draw_type(widget_t* widget, image_draw_type_t draw_type) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_BAD_PARAMS);

  svg_image->draw_type = draw_type;

  return widget_invalidate(widget, NULL);
}

static ret_t svg_image_get_prop(widget_t* widget, const char* name, value_t* v) {
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    value_set_int(v, svg_image->draw_type);
    return RET_OK;
  } else {
    return image_base_get_prop(widget, name, v);
  }
}

static ret_t svg_image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    svg_image_set_image(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, SVG_IMAGE_CACHE_MODE)) {
    svg_image_set_cache_mode(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = image_draw_type_find(value_str(v));
      if (kv != NULL) {
        return svg_image_set_draw_type(widget, (image_draw_type_t)(kv->value));
      }
    } else {
      return svg_image_set_draw_type(widget, (image_draw_type_t)value_int(v));
    }
    return RET_OK;
  } else {
    return image_base_set_prop(widget, name, v);
  }
}

static const char* s_svg_image_properties[] = {WIDGET_PROP_IMAGE,     WIDGET_PROP_SCALE_X,
                                               WIDGET_PROP_SCALE_Y,   WIDGET_PROP_ANCHOR_X,
                                               WIDGET_PROP_ANCHOR_Y,  WIDGET_PROP_ROTATION,
                                               WIDGET_PROP_CLICKABLE, WIDGET_PROP_SELECTABLE,
                                               WIDGET_PROP_DRAW_TYPE, NULL};

TK_DECL_VTABLE(svg_image) = {.size = sizeof(svg_image_t),
                             .type = WIDGET_TYPE_SVG_IMAGE,
                             .clone_properties = s_svg_image_properties,
                             .persistent_properties = s_svg_image_properties,
                             .get_parent_vt = TK_GET_PARENT_VTABLE(image_base),
                             .create = svg_image_create,
                             .on_destroy = svg_image_on_destroy,
                             .on_event = image_base_on_event,
                             .on_paint_self = svg_image_on_paint_self,
                             .on_copy = image_base_on_copy,
                             .set_prop = svg_image_set_prop,
                             .get_prop = svg_image_get_prop};

widget_t* svg_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(svg_image), x, y, w, h);
  svg_image_t* svg_image = SVG_IMAGE(widget);
  return_value_if_fail(svg_image != NULL, NULL);

  svg_image->is_cache_mode = FALSE;
  svg_image->canvas_offline = NULL;
  svg_image->draw_type = IMAGE_DRAW_DEFAULT;

  image_base_init(widget);

  return widget;
}

widget_t* svg_image_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, svg_image), NULL);

  return widget;
}
