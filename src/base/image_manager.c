/**
 * File:   image_manager.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap manager
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

#include "base/mem.h"
#include "base/image_manager.h"

typedef struct _image_item_t {
  const char* name;
  bitmap_t* bitmap;
} image_item_t;

typedef struct _image_loader_item_t {
  const char* name;
  image_loader_t* loader;
} image_loader_item_t;

image_manager_t* default_im() {
  static image_manager_t* im = NULL;
  if (im == NULL) {
    im = image_manager_create();
  }

  return im;
}

image_manager_t* image_manager_create() {
  image_manager_t* im = MEM_ZALLOC(image_manager_t);

  return image_manager_init(im);
}

image_manager_t* image_manager_init(image_manager_t* im) {
  return_value_if_fail(im != NULL, NULL);
  memset(im, 0x00, sizeof(image_manager_t));
  return_value_if_fail(array_init(&(im->images), 3) != NULL, NULL);
  return_value_if_fail(array_init(&(im->loaders), 3) != NULL, NULL);

  return im;
}

ret_t image_manager_add_loader(image_manager_t* im, image_loader_t* loader) {
  return_value_if_fail(im != NULL && loader != NULL, RET_BAD_PARAMS);

  array_push(&(im->loaders), loader);

  return RET_OK;
}

static int image_compare(const void* aa, const void* bb) {
  image_item_t* a = (image_item_t*)aa;
  image_item_t* b = (image_item_t*)bb;

  return strcmp(a->name, b->name);
}

bitmap_t* image_manager_find_image(image_manager_t* im, const char* name) {
  image_item_t item;
  image_item_t* ret = NULL;

  item.name = name;
  ret = (image_item_t*)array_find(&(im->images), image_compare, &item);

  return ret ? ret->bitmap : NULL;
}

ret_t image_manager_load(image_manager_t* im, const char* name, bitmap_t* bitmap) {
  uint32_t i = 0;
  uint32_t nr = 0;
  image_loader_t* loader = NULL;
  image_loader_t** elms = NULL;
  return_value_if_fail(im != NULL && name != NULL && bitmap != NULL, RET_BAD_PARAMS);

  elms = (image_loader_t**)(im->loaders.elms);
  nr = im->loaders.size;
  for (i = 0; i < nr; i++) {
    loader = elms[i];
    if (image_loader_load(loader, name, bitmap) == RET_OK) {
      return RET_OK;
    }
  }

  return RET_FAIL;
}

ret_t image_manager_add_image(image_manager_t* im, const char* name, bitmap_t* bitmap) {
  image_item_t* item = NULL;
  return_value_if_fail(im != NULL && name != NULL && bitmap != NULL, RET_BAD_PARAMS);

  item = MEM_ZALLOC(image_item_t);
  return_value_if_fail(item != NULL, RET_FAIL);

  item->name = name;
  item->bitmap = bitmap;
  array_push(&(im->images), item);

  return RET_OK;
}

ret_t image_manager_deinit(image_manager_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t image_manager_destroy(image_manager_t* im) {
  return_value_if_fail(image_manager_deinit(im) == RET_OK, RET_FAIL);

  MEM_FREE(im);

  return RET_OK;
}
