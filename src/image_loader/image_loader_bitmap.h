/**
 * File:   bitmap.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap image loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_IMAGE_LOADER_BITMAP_H
#define LFTK_IMAGE_LOADER_BITMAP_H

#include "base/image.h"

BEGIN_C_DECLS

typedef struct _bitmap_header_t {
  uint32_t magic;
  uint16_t w;
  uint16_t h;
  uint32_t format;
  char name[NAME_LEN+1];
  uint8_t data[4];
}bitmap_header_t;

image_loader_t* image_loader_bitmap_create(uint32_t max_image_nr);

ret_t image_loader_bitmap_add(image_loader_t* loader, const uint8_t* data);

#define IMAGE_BITMAP_MAGIC 0x22223344

END_C_DECLS

#endif/*LFTK_IMAGE_LOADER_BITMAP_H*/

