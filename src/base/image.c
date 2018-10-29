/**
 * File:   image.h
 * Author: AWTK Develop Team
 * Brief:  image
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/enums.h"
#include "base/image.h"
#include "base/utils.h"
#include "base/image_manager.h"

static ret_t image_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  image_t* image = IMAGE(widget);

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      widget_invalidate(widget, NULL);
      break;
    case EVT_POINTER_UP: {
      if (image->clickable) {
        pointer_event_t evt = *(pointer_event_t*)e;
        evt.e = event_init(EVT_CLICK, widget);
        widget_dispatch(widget, (event_t*)&evt);
      }

      if (image->selectable) {
        image->selected = !image->selected;
      }

      if (image->selected) {
        widget_set_state(widget, WIDGET_STATE_SELECTED);
      } else {
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      }
      widget_invalidate(widget, NULL);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t image_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t dst;
  bitmap_t bitmap;
  image_t* image = IMAGE(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (image->image == NULL) {
    return RET_OK;
  }

  return_value_if_fail(widget_load_image(widget, image->image, &bitmap) == RET_OK, RET_BAD_PARAMS);

  if (vg != NULL) {
    bool_t need_transform = !tk_fequal(image->scale_x, 1) || !tk_fequal(image->scale_y, 1) ||
                            !tk_fequal(image->rotation, 0);

    if (need_transform) {
      float_t anchor_x = image->anchor_x * widget->w;
      float_t anchor_y = image->anchor_y * widget->h;

      vgcanvas_save(vg);
      vgcanvas_translate(vg, c->ox, c->oy);
      vgcanvas_translate(vg, anchor_x, anchor_y);

      if (!tk_fequal(image->rotation, 0)) {
        vgcanvas_rotate(vg, image->rotation);
      }

      if (!tk_fequal(image->scale_x, 1) || !tk_fequal(image->scale_y, 1)) {
        vgcanvas_scale(vg, image->scale_x, image->scale_y);
      }
      vgcanvas_translate(vg, -anchor_x, -anchor_y);
      if (bitmap.data != NULL) {
        vgcanvas_draw_icon(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, 0, 0, widget->w, widget->h);
      }
      vgcanvas_restore(vg);

      return RET_OK;
    }
  }

  if (bitmap.data != NULL) {
    color_t trans = color_init(0x00, 0x00, 0x00, 0x00);
    color_t bg = style_get_color(widget->astyle, STYLE_ID_BG_COLOR, trans);
    color_t border = style_get_color(widget->astyle, STYLE_ID_BORDER_COLOR, trans);
    if (bg.rgba.a == 0) {
#ifdef USE_FAST_MODE
      if (widget->parent != NULL) {
        bg = style_get_color(&(widget->parent->style_data), STYLE_ID_BG_COLOR, trans);
      }
#endif /*USE_FAST_MODE*/
    }

    if (bg.rgba.a) {
      canvas_set_fill_color(c, bg);
      canvas_fill_rect(c, 0, 0, widget->w, widget->h);
    }

    if (border.rgba.a) {
      canvas_set_stroke_color(c, border);
      canvas_stroke_rect(c, 0, 0, widget->w, widget->h);
    }

    dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, &bitmap, image->draw_type, &dst);
  }

  widget_paint_helper(widget, c, NULL, NULL);

  return RET_OK;
}

static ret_t image_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, image->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    value_set_int(v, image->draw_type);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_X)) {
    value_set_float(v, image->scale_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_Y)) {
    value_set_float(v, image->scale_y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    value_set_float(v, image->anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    value_set_float(v, image->anchor_y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROTATION)) {
    value_set_float(v, image->rotation);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTABLE)) {
    value_set_bool(v, image->selectable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLICKABLE)) {
    value_set_bool(v, image->clickable);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return image_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = image_draw_type_find(value_str(v));
      if (kv != NULL) {
        return image_set_draw_type(widget, (image_draw_type_t)(kv->value));
      }
    } else {
      return image_set_draw_type(widget, (image_draw_type_t)value_int(v));
    }

    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_X)) {
    image->scale_x = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_Y)) {
    image->scale_y = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    image->anchor_x = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    image->anchor_y = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROTATION)) {
    image->rotation = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTABLE)) {
    image->selectable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLICKABLE)) {
    image->clickable = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_destroy(widget_t* widget) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image->image);

  return RET_OK;
}

static const char* s_image_clone_properties[] = {WIDGET_PROP_IMAGE,      WIDGET_PROP_DRAW_TYPE,
                                                 WIDGET_PROP_SCALE_X,    WIDGET_PROP_SCALE_Y,
                                                 WIDGET_PROP_ANCHOR_X,   WIDGET_PROP_ANCHOR_Y,
                                                 WIDGET_PROP_ROTATION,   WIDGET_PROP_CLICKABLE,
                                                 WIDGET_PROP_SELECTABLE, NULL};
static const widget_vtable_t s_image_vtable = {.size = sizeof(image_t),
                                               .type = WIDGET_TYPE_IMAGE,
                                               .clone_properties = s_image_clone_properties,
                                               .create = image_create,
                                               .destroy = image_destroy,
                                               .on_event = image_on_event,
                                               .on_paint_self = image_on_paint_self,
                                               .set_prop = image_set_prop,
                                               .get_prop = image_get_prop};

widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  image_t* image = TKMEM_ZALLOC(image_t);
  widget_t* widget = WIDGET(image);
  return_value_if_fail(image != NULL, NULL);

  widget_init(widget, parent, &s_image_vtable, x, y, w, h);
  image->draw_type = IMAGE_DRAW_DEFAULT;

  image->scale_x = 1;
  image->scale_y = 1;
  image->rotation = 0;
  image->anchor_x = 0.5;
  image->anchor_y = 0.5;

  return widget;
}

ret_t image_set_image(widget_t* widget, const char* name) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image->image);
  image->image = tk_strdup(name);

  return RET_OK;
}

ret_t image_set_draw_type(widget_t* widget, image_draw_type_t draw_type) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->draw_type = draw_type;

  return RET_OK;
}

ret_t image_set_rotation(widget_t* widget, float_t rotation) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->rotation = rotation;

  return widget_invalidate(widget, NULL);
}

ret_t image_set_scale(widget_t* widget, float_t scale_x, float_t scale_y) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->scale_x = scale_x;
  image->scale_y = scale_y;

  return widget_invalidate(widget, NULL);
}

ret_t image_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->anchor_x = anchor_x;
  image->anchor_y = anchor_y;

  return widget_invalidate(widget, NULL);
}

ret_t image_set_selected(widget_t* widget, bool_t selected) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->selected = selected;

  return widget_invalidate(widget, NULL);
}

ret_t image_set_selectable(widget_t* widget, bool_t selectable) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->selectable = selectable;

  return widget_invalidate(widget, NULL);
}

ret_t image_set_clickable(widget_t* widget, bool_t clickable) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->clickable = clickable;

  return widget_invalidate(widget, NULL);
}
