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

#include "base/mem.h"
#include "stb/stb_image.h"
#include "image_loader/image_loader_stb.h"

static ret_t image_stb_destroy_free(bitmap_t* image) {
  MEM_FREE((uint8_t*)(image->data));
  image->data = NULL;

  return RET_OK;
}

static ret_t image_stb_destroy(bitmap_t* image) {
  stbi_image_free((uint8_t*)(image->data));
  image->data = NULL;

  return RET_OK;
}

static ret_t image_loader_stb_load(image_loader_t* l, const uint8_t* buff, uint32_t buff_size,
                                   bitmap_t* image) {
  int i = 0;
  int w = 0;
  int h = 0;
  int n = 0;
  uint8_t* data4 = NULL;
  uint8_t* data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 4);
  return_value_if_fail(data != NULL, RET_FAIL);

  image->w = w;
  image->h = h;
  image->format = BITMAP_FMT_RGBA;
  image->flags = BITMAP_FLAG_IMMUTABLE;

  if(n != 4) {
    data4 = MEM_ALLOC(w*h*4);
    if(data4 == NULL) {
      stbi_image_free((uint8_t*)(data));
      return RET_FAIL;
    }

    image->data = data4;
    image->destroy = image_stb_destroy_free;
    image->flags |= BITMAP_FLAG_OPAQUE;

    for(i = 0; i < w*h; i++) {
      data4[0] = data[0];     
      data4[1] = data[1];     
      data4[2] = data[2];     
      data4[3] = 0xff;     
      data4 += 4;
      data +=3;
    }
  } else {
    image->data = data;
    image->destroy = image_stb_destroy;

    for(i = 0; i < w*h; i+=n) {
      if(data[3] != 0xff) {
        return RET_OK;  
      }
      data += 4;
    }
    image->flags |= BITMAP_FLAG_OPAQUE;
  }

  return RET_OK;
}

image_loader_t* image_loader_stb() {
  static image_loader_t stb_loader;

  stb_loader.load = image_loader_stb_load;

  return &stb_loader;
}
