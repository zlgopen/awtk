/**
 * File:   image_loader.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  image_loader interface
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

#define STB_IMAGE_IMPLEMENTATION

#include "image_loader/image_loader_stb.h"
#include "base/mem.h"
#include "stb/stb_image.h"

static ret_t image_stb_destroy(bitmap_t* image) {
  stbi_image_free((uint8_t*)(image->data));

  return RET_OK;
}

static bool_t image_loader_stb_match(image_loader_t* l, uint16_t type) {
  return_value_if_fail(l != NULL, FALSE);
  (void)type;
  return TRUE;
}

static ret_t image_loader_stb_load(image_loader_t* l, const uint8_t* buff, uint32_t buff_size, bitmap_t* image) {
  int w = 0;
  int h = 0;
  int n = 0;
  uint8_t* data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 4);
  return_value_if_fail(data != NULL, RET_FAIL);

  image->w = w;
  image->h = h;
  image->data = data;
  image->format = IMAGE_FMT_RGBA;
  image->destroy = image_stb_destroy;

  return RET_OK;
}

image_loader_t* image_loader_stb() {
  static image_loader_t stb_loader;

  stb_loader.match = image_loader_stb_match;
  stb_loader.load = image_loader_stb_load;

  return &stb_loader;
}
