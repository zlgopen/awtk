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

typedef struct _image_loader_stb_t {
  image_loader_t loader;
  uint16_t size;
  uint16_t capacity;
  const stb_header_t* images[1];
} image_loader_stb_t;

static ret_t image_stb_destroy(bitmap_t* image) {
  stbi_image_free((uint8_t*)(image->data));

  return RET_OK;
}

static ret_t image_loader_load_img(image_loader_t* l, const char* name, bitmap_t* image) {
  uint16_t i = 0;
  image_loader_stb_t* loader = (image_loader_stb_t*)l;

  for (i = 0; i < loader->size; i++) {
    const stb_header_t* iter = loader->images[i];
    if (strcmp(iter->name, name) == 0) {
      int w = 0;
      int h = 0;
      int n = 0;
      uint8_t* data = stbi_load_from_memory(iter->data, iter->size, &w, &h, &n, 4);

      return_value_if_fail(data != NULL, RET_NOT_FOUND);

      image->w = w;
      image->h = h;
      image->data = data;
      image->format = IMAGE_FMT_RGBA;
      image->destroy = image_stb_destroy;

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

image_loader_t* image_loader_stb_create(uint32_t capacity) {
  image_loader_stb_t* loader = NULL;
  uint32_t size = sizeof(image_loader_stb_t) + capacity * sizeof(void*);
  return_value_if_fail(capacity > 0, NULL);

  loader = (image_loader_stb_t*)MEM_ALLOC(size);
  return_value_if_fail(loader != NULL, NULL);

  loader->size = 0;
  loader->capacity = capacity;
  loader->loader.load = image_loader_load_img;

  return &(loader->loader);
}

ret_t image_loader_add(image_loader_t* l, const stb_header_t* data) {
  image_loader_stb_t* loader = (image_loader_stb_t*)l;
  return_value_if_fail(loader != NULL && data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(loader->size < loader->capacity, RET_BAD_PARAMS);

  loader->images[loader->size++] = data;

  return RET_OK;
}

static ret_t image_loader_load_file(image_loader_t* l, const char* name, bitmap_t* image) {
  int w = 0;
  int h = 0;
  int n = 0;
  uint8_t* data = stbi_load(name, &w, &h, &n, 4);

  return_value_if_fail(data != NULL, RET_NOT_FOUND);

  image->w = w;
  image->h = h;
  image->data = data;
  image->format = IMAGE_FMT_RGBA;
  image->destroy = image_stb_destroy;

  return RET_OK;
}

image_loader_t* image_loader_stb_file_create() {
  static image_loader_t stb_loader;

  stb_loader.load = image_loader_load_file;

  return &stb_loader;
}
