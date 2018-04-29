/**
 * File:   image.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  image
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/enums.h"
#include "base/image.h"
#include "base/image_manager.h"

static ret_t image_set_image(widget_t* widget, bitmap_t* image);

static ret_t image_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t dst;
  bitmap_t* bitmap = NULL;
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  bitmap = &(image->bitmap);
  if (bitmap->data != NULL) {
    color_t trans = color_init(0x00, 0x00, 0x00, 0x00);
    color_t bg = style_get_color(&(widget->style), STYLE_ID_BG_COLOR, trans);
    color_t border = style_get_color(&(widget->style), STYLE_ID_BORDER_COLOR, trans);
    if (bg.rgba.a == 0) {
#ifdef FAST_MODE
      if (widget->parent != NULL) {
        bg = style_get_color(&(widget->parent->style), STYLE_ID_BG_COLOR, trans);
      }
#endif /*FAST_MODE*/
    }

    if (bg.rgba.a) {
      canvas_set_fill_color(c, bg);
      canvas_fill_rect(c, 0, 0, widget->w, widget->h);
    }

    if (border.rgba.a) {
      canvas_set_stroke_color(c, border);
      canvas_stroke_rect(c, 0, 0, widget->w, widget->h);
    }

    rect_init(dst, 0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, bitmap, image->draw_type, &dst);
  }

  return RET_OK;
}

static ret_t image_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "image") == 0) {
    value_set_pointer(v, &(image->bitmap));
    return RET_OK;
  } else if (strcmp(name, "draw_type") == 0) {
    value_set_int(v, image->draw_type);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "image") == 0) {
    if (v->type == VALUE_TYPE_STRING) {
      return image_set_image_name(widget, value_str(v));
    } else if (v->type == VALUE_TYPE_POINTER) {
      return image_set_image(widget, (bitmap_t*)value_pointer(v));
    }
  } else if (strcmp(name, "draw_type") == 0) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = image_draw_type_find(value_str(v));
      if (kv != NULL) {
        return image_set_draw_type(widget, (image_draw_type_t)(kv->value));
      }
    } else {
      return image_set_draw_type(widget, (image_draw_type_t)value_int(v));
    }

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_image_vtable = {
    .on_paint_self = image_on_paint_self, .set_prop = image_set_prop, .get_prop = image_get_prop};

widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  image_t* image = TKMEM_ZALLOC(image_t);
  return_value_if_fail(image != NULL, NULL);

  widget = WIDGETP(image);
  widget_init(widget, parent, WIDGET_IMAGE);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_image_vtable;
  image->draw_type = IMAGE_DRAW_CENTER;

  return widget;
}

ret_t image_set_image_name(widget_t* widget, const char* name) {
  bitmap_t bitmap;
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image_manager_load(image_manager(), name, &bitmap) == RET_OK,
                       RET_BAD_PARAMS);

  return image_set_image(widget, &bitmap);
}

static ret_t image_set_image(widget_t* widget, bitmap_t* bitmap) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap != NULL) {
    image->bitmap = *bitmap;
  } else {
    memset(&image->bitmap, 0x00, sizeof(bitmap_t));
  }

  return RET_OK;
}

ret_t image_set_draw_type(widget_t* widget, image_draw_type_t draw_type) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->draw_type = draw_type;

  return RET_OK;
}
