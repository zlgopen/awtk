/**
 * File:   bitmap.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_BITMAP_H
#define LFTK_BITMAP_H

#include "base/color.h"

BEGIN_C_DECLS

struct _bitmap_t;
typedef struct _bitmap_t bitmap_t;

typedef ret_t (*bitmap_destroy_t)(bitmap_t* bitmap);

enum {
  IMAGE_FMT_NONE = 0,
  IMAGE_FMT_RGBA,
  IMAGE_FTM_1555,
  IMAGE_FTM_565
};

struct _bitmap_t {
  wh_t w;
  wh_t h;
  uint32_t format;
  const uint8_t* data;
  bitmap_destroy_t destroy;
};

struct _image_loader_t;
typedef struct _image_loader_t image_loader_t;

typedef ret_t (*image_loader_load_t)(image_loader_t* loader, const char* name, bitmap_t* bitmap);

struct _image_loader_t {
  image_loader_load_t load;
};

ret_t image_loader_load(image_loader_t* loader, const char* name, bitmap_t* bitmap);

ret_t bitmap_destroy(bitmap_t* bitmap);

END_C_DECLS

#endif/*LFTK_BITMAP_H*/

