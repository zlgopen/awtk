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
#include "base/time.h"
#include "base/image_manager.h"
#include "base/resource_manager.h"

typedef struct _bitmap_cache_t {
  bitmap_t image;
  char name[NAME_LEN + 1];
  uint32_t access_count;
  uint32_t created_time;
  uint32_t last_access_time;
} bitmap_cache_t;

static image_manager_t* s_imm = NULL;
image_manager_t* image_manager() { return s_imm; }

ret_t image_manager_set(image_manager_t* imm) {
  s_imm = imm;

  return RET_OK;
}

image_manager_t* image_manager_create(image_loader_t* loader) {
  image_manager_t* imm = TKMEM_ZALLOC(image_manager_t);
  return_value_if_fail(imm != NULL, NULL);

  return image_manager_init(imm, loader);
}

image_manager_t* image_manager_init(image_manager_t* imm, image_loader_t* loader) {
  return_value_if_fail(imm != NULL, NULL);

  imm->loader = loader;
  array_init(&(imm->images), 0);

  return imm;
}

ret_t image_manager_add(image_manager_t* imm, const char* name, const bitmap_t* image) {
  bitmap_cache_t* cache = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  cache = TKMEM_ZALLOC(bitmap_cache_t);
  return_value_if_fail(cache != NULL, RET_OOM);

  cache->image = *image;
  cache->access_count = 1;
  cache->created_time = time_now_s();
  strncpy(cache->name, name, NAME_LEN);
  cache->last_access_time = cache->created_time;
  cache->image.name = cache->name;

  return array_push(&(imm->images), cache);
}

ret_t image_manager_lookup(image_manager_t* imm, const char* name, bitmap_t* image) {
  uint32_t i = 0;
  uint32_t nr = 0;
  bitmap_cache_t* iter = NULL;
  bitmap_cache_t** all = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  all = (bitmap_cache_t**)(imm->images.elms);
  for (i = 0, nr = imm->images.size; i < nr; i++) {
    iter = all[i];
    if (strcmp(name, iter->name) == 0) {
      *image = iter->image;
      iter->access_count++;
      iter->last_access_time = time_now_s();

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

ret_t image_manager_load(image_manager_t* imm, const char* name, bitmap_t* image) {
  const resource_info_t* res = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  if (image_manager_lookup(imm, name, image) == RET_OK) {
    return RET_OK;
  }

  res = resource_manager_ref(resource_manager(), RESOURCE_TYPE_IMAGE, name);
  return_value_if_fail(res != NULL, RET_NOT_FOUND);

  memset(image, 0x00, sizeof(bitmap_t));
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
    ret_t ret = image_loader_load(imm->loader, res->data, res->size, image);
    image_manager_add(imm, name, image);
    resource_manager_unref(resource_manager(), res);

    return ret;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t image_manager_unload_unused(image_manager_t* imm, uint32_t time_delta_s) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t nr = 0;
  uint32_t now = time_now_s();
  bitmap_cache_t* iter = NULL;
  bitmap_cache_t** all = NULL;
  return_value_if_fail(imm != NULL && imm->loader != NULL, RET_BAD_PARAMS);

  all = (bitmap_cache_t**)(imm->images.elms);
  for (i = 0, nr = imm->images.size; i < nr; i++) {
    iter = all[i];
    if ((iter->last_access_time + time_delta_s) <= now) {
      bitmap_destroy(&(iter->image));
      TKMEM_FREE(iter);
      all[i] = NULL;
    } else {
      if (j != i) {
        all[j] = all[i];
      }
      j++;
    }
  }

  imm->images.size = j;

  return RET_OK;
}

ret_t image_manager_deinit(image_manager_t* imm) {
  uint32_t i = 0;
  uint32_t nr = 0;
  bitmap_cache_t* iter = NULL;
  bitmap_cache_t** all = NULL;
  return_value_if_fail(imm != NULL && imm->loader != NULL, RET_BAD_PARAMS);

  all = (bitmap_cache_t**)(imm->images.elms);
  for (i = 0, nr = imm->images.size; i < nr; i++) {
    iter = all[i];
    TKMEM_FREE(iter);
  }

  array_deinit(&(imm->images));
  imm->loader = NULL;

  return RET_OK;
}

ret_t image_manager_destroy(image_manager_t* imm) {
  return_value_if_fail(imm != NULL && imm->loader != NULL, RET_BAD_PARAMS);

  image_manager_deinit(imm);
  TKMEM_FREE(imm);

  return RET_OK;
}
