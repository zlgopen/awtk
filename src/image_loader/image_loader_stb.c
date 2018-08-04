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

static ret_t image_stb_destroy_free(bitmap_t* image) {
  TKMEM_FREE(image->data);
  image->data = NULL;

  return RET_OK;
}

static ret_t image_stb_destroy(bitmap_t* image) {
  stbi_image_free((uint8_t*)(image->data));
  image->data = NULL;

  return RET_OK;
}

static ret_t normalize_image(bitmap_t* image) {
#ifdef WITH_BITMAP_RGB565
  if (image->flags & BITMAP_FLAG_OPAQUE) {
    uint32_t size = image->w * image->h * 2;
    uint16_t* data = (uint16_t*)TKMEM_ALLOC(size);
    if (data != NULL) {
      bitmap_rgba_to_rgb565(image, data);
      stbi_image_free((uint8_t*)(image->data));
      image->data = (uint8_t*)data;
      image->destroy = image_stb_destroy_free;
    } else {
      bitmap_rgba_to_rgb565(image, (uint16_t*)(image->data));
    }
    image->format = BITMAP_FMT_RGB565;

    return RET_OK;
  }
#endif /*WITH_BITMAP_RGB565*/

#ifdef WITH_BITMAP_BGRA
  bitmap_rgba_to_bgra(image);
#else
  (void)image;
#endif /*WITH_BITMAP_BGRA*/

  return RET_OK;
}

static ret_t image_loader_stb_load(image_loader_t* l, const uint8_t* buff, uint32_t buff_size,
                                   bitmap_t* image) {
  int i = 0;
  int w = 0;
  int h = 0;
  int n = 0;
  int nr = 0;
  uint8_t* data4 = NULL;
  uint8_t* data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 0);
  return_value_if_fail(data != NULL, RET_FAIL);

  image->w = w;
  image->h = h;
  image->format = BITMAP_FMT_RGBA;
  image->flags = BITMAP_FLAG_IMMUTABLE;

  nr = w * h;
  if (n != 4) {
    void* org_data = data;

    data4 = TKMEM_ALLOC(w * h * 4);
    if (data4 == NULL) {
      stbi_image_free((uint8_t*)(data));
      return RET_FAIL;
    }

    image->data = data4;
    image->destroy = image_stb_destroy_free;
    image->flags |= BITMAP_FLAG_OPAQUE;

    for (i = 0; i < nr; i++) {
      data4[0] = data[0];
      data4[1] = data[1];
      data4[2] = data[2];
      data4[3] = 0xff;
      data4 += 4;
      data += 3;
    }
    stbi_image_free((uint8_t*)(org_data));
  } else {
    image->data = data;
    image->destroy = image_stb_destroy;

    for (i = 0; i < nr; i += n) {
      if (data[3] != 0xff) {
        return normalize_image(image);
      }
      data += 4;
    }
    image->flags |= BITMAP_FLAG_OPAQUE;
  }

  return normalize_image(image);
}

static const image_loader_t stb_loader = {.load = image_loader_stb_load};

image_loader_t* image_loader_stb() {
  return (image_loader_t*)&stb_loader;
}
