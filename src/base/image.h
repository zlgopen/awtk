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
 * 2018-02-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_IMAGE_H
#define LFTK_IMAGE_H

#include "base/wstr.h"
#include "base/image.h"
#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _image_t {
  widget_t widget;
  bitmap_t bitmap;
}image_t;

widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
ret_t image_set_image_name(widget_t* widget, const char* name);
ret_t image_set_image(widget_t* widget, bitmap_t* image);

#define IMAGE(widget) (image_t*)(widget)

END_C_DECLS

#endif/*LFTK_IMAGE_H*/

