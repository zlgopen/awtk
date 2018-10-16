/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  image_loader interface
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define STB_IMAGE_IMPLEMENTATION

#include "base/mem.h"

#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(char, p, s)

#include "stb/stb_image.h"
#include "image_loader/image_loader_stb.h"

static ret_t image_loader_stb_load(image_loader_t* l, const uint8_t* buff, uint32_t buff_size,
                                   bitmap_t* image) {
  int i = 0;
  int w = 0;
  int h = 0;
  int n = 0;
  int nr = 0;
  ret_t ret = RET_FAIL;
  uint8_t* data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 0);
  return_value_if_fail(data != NULL, RET_FAIL);

  if (n == 3) {
#ifdef WITH_BITMAP_BGR565
    ret = bitmap_init(image, w, h, BITMAP_FMT_BGR565, data, n);
    stbi_image_free((uint8_t*)(data));
    return ret;
#endif /*WITH_BITMAP_BGR565*/
  }

#ifdef WITH_BITMAP_BGRA
  ret = bitmap_init(image, w, h, BITMAP_FMT_BGRA8888, data, n);
#else
  ret = bitmap_init(image, w, h, BITMAP_FMT_RGBA8888, data, n);
#endif /*WITH_BITMAP_BGRA*/

  stbi_image_free((uint8_t*)(data));
  return ret;
}

static const image_loader_t stb_loader = {.load = image_loader_stb_load};

image_loader_t* image_loader_stb() {
  return (image_loader_t*)&stb_loader;
}
