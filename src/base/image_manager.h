/**
 * File:   image_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  image manager
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_IMAGE_MANAGER_H
#define LFTK_IMAGE_MANAGER_H

#include "base/array.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

typedef struct _bitmap_header_t {
  uint16_t w;
  uint16_t h;
  uint32_t format;
  uint8_t data[4];
}bitmap_header_t;

typedef struct _image_manager_t {
  image_loader_t* loader;
}image_manager_t;

image_manager_t* default_im(void);

image_manager_t* image_manager_create(void);
image_manager_t* image_manager_init(image_manager_t* im, image_loader_t* loader);
ret_t image_manager_load(image_manager_t* im, const char* name, bitmap_t* image);
ret_t image_manager_deinit(image_manager_t* im);
ret_t image_manager_destroy(image_manager_t* im);

END_C_DECLS

#endif/*LFTK_IMAGE_MANAGER_H*/

