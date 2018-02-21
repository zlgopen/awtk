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

#include "image_loader/image_loader_bitmap.h"
#include "base/mem.h"

typedef struct _image_loader_bitmap_t {
  image_loader_t loader;
  uint16_t size;
  uint16_t capacity;
  const bitmap_header_t* images[1];
} image_loader_bitmap_t;

static ret_t image_loader_load_img(image_loader_t* l, const char* name, bitmap_t* image) {
  uint16_t i = 0;
  image_loader_bitmap_t* loader = (image_loader_bitmap_t*)l;

  for (i = 0; i < loader->size; i++) {
    const bitmap_header_t* iter = loader->images[i];
    if (strcmp(iter->name, name) == 0) {
      image->w = iter->w;
      image->h = iter->h;
      image->format = iter->format;
      image->data = iter->data;

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

image_loader_t* image_loader_bitmap_create(uint32_t capacity) {
  image_loader_bitmap_t* loader = NULL;
  size_t size = sizeof(image_loader_bitmap_t) + capacity * sizeof(bitmap_header_t*);
  return_value_if_fail(capacity > 0, NULL);

  loader = (image_loader_bitmap_t*)MEM_ALLOC(size);
  return_value_if_fail(loader != NULL, NULL);
  loader->size = 0;
  loader->capacity = capacity;
  loader->loader.load = image_loader_load_img;

  return &(loader->loader);
}

ret_t image_loader_bitmap_add(image_loader_t* l, const uint8_t* data) {
  image_loader_bitmap_t* loader = (image_loader_bitmap_t*)l;
  return_value_if_fail(loader != NULL && data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(loader->size < loader->capacity, RET_BAD_PARAMS);

  loader->images[loader->size++] = (const bitmap_header_t*)data;

  return RET_OK;
}
