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
#include "base/resource_manager.h"

image_manager_t* image_manager() {
  static image_manager_t* imm = NULL;
  if (imm == NULL) {
    imm = image_manager_create();
  }

  return imm;
}

image_manager_t* image_manager_create() {
  image_manager_t* imm = MEM_ZALLOC(image_manager_t);

  return imm;
}

image_manager_t* image_manager_init(image_manager_t* imm, image_loader_t* loader) {
  return_value_if_fail(imm != NULL, NULL);

  imm->loader = loader;

  return imm;
}

ret_t image_manager_load(image_manager_t* imm, const char* name, bitmap_t* image) {
  const resource_info_t* res = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  res = resource_manager_ref(RESOURCE_TYPE_IMAGE, name);
  return_value_if_fail(res != NULL, RET_NOT_FOUND);

  if (res->subtype == RESOURCE_TYPE_IMAGE_RAW) {
    const bitmap_header_t* header = (const bitmap_header_t*)res->data;
    image->w = header->w;
    image->h = header->h;
    image->flags = header->flags;
    image->format = header->format;
    image->name = res->name;
    image->data = header->data;
    return RET_OK;
  } else if (imm->loader != NULL) {
    return image_loader_load(imm->loader, res->data, res->size, image);
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t image_manager_deinit(image_manager_t* imm) {
  imm->loader = NULL;

  return RET_OK;
}

ret_t image_manager_destroy(image_manager_t* imm) {
  MEM_FREE(imm);

  return RET_OK;
}
