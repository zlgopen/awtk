/**
 * File:   stb.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stb image loader
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

#ifndef LFTK_IMAGE_LOADER_STB_H
#define LFTK_IMAGE_LOADER_STB_H

#include "base/image.h"

BEGIN_C_DECLS

typedef struct _stb_header_t {
  uint32_t magic;
  const char name[32];
  uint32_t size;
  uint8_t data[1];
}stb_header_t;

image_loader_t* image_loader_stb_file_create();
image_loader_t* image_loader_stb_mem_create(uint32_t max_image_nr);

ret_t image_loader_stb_add(image_loader_t* loader, const stb_header_t* data);

#define IMAGE_STB_MAGIC 0x33223344

END_C_DECLS

#endif/*LFTK_IMAGE_LOADER_STB_H*/

