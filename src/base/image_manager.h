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

typedef struct _image_manager_t {
  array_t images;
  array_t loaders;
}image_manager_t;

image_manager_t* default_im(void);
image_manager_t* image_manager_create(void);
image_manager_t* image_manager_init(image_manager_t* im);

ret_t image_manager_add_loader(image_manager_t* im, image_loader_t* loader);
ret_t image_manager_load(image_manager_t* im, const char* name, bitmap_t* image);

ret_t image_manager_add_image(image_manager_t* im, const char* name, bitmap_t* image);
bitmap_t* image_manager_find_image(image_manager_t* im, const char* name);

ret_t image_manager_deinit(image_manager_t* im);
ret_t image_manager_destroy(image_manager_t* im);

END_C_DECLS

#endif/*LFTK_IMAGE_MANAGER_H*/

