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

#include "base/image.h"
#include "base/mem.h"
#include "base/image_manager.h"

static ret_t image_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  rect_t src;
  rect_t dst;
  bitmap_t* bitmap = NULL;
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  bitmap = &(image->bitmap);
  if (bitmap->data != NULL) {
    x = (widget->w - bitmap->w) >> 1;
    y = (widget->h - bitmap->h) >> 1;
    rect_init(src, 0, 0, bitmap->w, bitmap->h);
    rect_init(dst, x, y, bitmap->w, bitmap->h);
    canvas_draw_image(c, bitmap, &src, &dst);
  }

  return RET_OK;
}

static const widget_vtable_t s_image_vtable = {.on_paint_self = image_on_paint_self};

widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  image_t* image = MEM_ZALLOC(image_t);
  return_value_if_fail(image != NULL, NULL);

  widget = WIDGETP(image);
  widget_init(widget, parent, WIDGET_IMAGE);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_image_vtable;

  return widget;
}

ret_t image_set_image_name(widget_t* widget, const char* name) {
  bitmap_t bitmap;
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image_manager_load(default_im(), name, &bitmap) == RET_OK, RET_BAD_PARAMS);

  return image_set_image(widget, &bitmap);
}

ret_t image_set_image(widget_t* widget, bitmap_t* bitmap) {
  image_t* image = IMAGE(widget);
  return_value_if_fail(widget != NULL && bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap != NULL) {
    image->bitmap = *bitmap;
  } else {
    memset(&image->bitmap, 0x00, sizeof(bitmap_t));
  }

  return RET_OK;
}
