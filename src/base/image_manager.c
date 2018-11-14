/**
 * File:   image_manager.c
 * Author: AWTK Develop Team
 * Brief:  bitmap manager
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/time_now.h"
#include "base/image_manager.h"

typedef struct _bitmap_cache_t {
  bitmap_t image;
  char name[NAME_LEN + 1];
  uint32_t access_count;
  uint32_t created_time;
  uint32_t last_access_time;
} bitmap_cache_t;

static image_manager_t* s_image_manager = NULL;
image_manager_t* image_manager() {
  return s_image_manager;
}

ret_t image_manager_set(image_manager_t* imm) {
  s_image_manager = imm;

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
  imm->assets_manager = assets_manager();

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
  cache->image.should_free_handle = FALSE;

  tk_strncpy(cache->name, name, NAME_LEN);
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
      image->destroy = NULL;
      image->specific_destroy = NULL;
      image->should_free_data = FALSE;

      iter->access_count++;
      iter->last_access_time = time_now_s();

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

ret_t image_manager_update_specific(image_manager_t* imm, bitmap_t* image) {
  uint32_t i = 0;
  uint32_t nr = 0;
  bitmap_cache_t* iter = NULL;
  bitmap_cache_t** all = NULL;
  return_value_if_fail(imm != NULL && image != NULL, RET_BAD_PARAMS);

  all = (bitmap_cache_t**)(imm->images.elms);
  for (i = 0, nr = imm->images.size; i < nr; i++) {
    iter = all[i];
    if (image->data == iter->image.data) {
      iter->image.flags = image->flags;
      iter->image.specific = image->specific;
      iter->image.specific_ctx = image->specific_ctx;
      iter->image.specific_destroy = image->specific_destroy;

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

ret_t image_manager_load(image_manager_t* imm, const char* name, bitmap_t* image) {
  const asset_info_t* res = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  memset(image, 0x00, sizeof(bitmap_t));
  if (image_manager_lookup(imm, name, image) == RET_OK) {
    return RET_OK;
  }

  res = assets_manager_ref(imm->assets_manager, ASSET_TYPE_IMAGE, name);
  return_value_if_fail(res != NULL, RET_NOT_FOUND);

  memset(image, 0x00, sizeof(bitmap_t));
  if (res->subtype == ASSET_TYPE_IMAGE_RAW) {
    const bitmap_header_t* header = (const bitmap_header_t*)res->data;

    image->w = header->w;
    image->h = header->h;
    image->flags = header->flags;
    image->format = header->format;
    image->name = res->name;
    image->data = header->data;

#if defined(WITH_NANOVG_GPU) || defined(WITH_NANOVG_SOFT)
    image_manager_add(imm, name, image);
#endif
    return RET_OK;
  } else if (imm->loader != NULL) {
    ret_t ret = image_loader_load(imm->loader, res->data, res->size, image);
    if (ret == RET_OK) {
      image_manager_add(imm, name, image);
      assets_manager_unref(imm->assets_manager, res);
    }

    return image_manager_lookup(imm, name, image);
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t image_manager_set_assets_manager(image_manager_t* imm, assets_manager_t* am) {
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  imm->assets_manager = am;

  return RET_OK;
}

static int bitmap_cache_cmp_time(bitmap_cache_t* a, bitmap_cache_t* b) {
  return (a->last_access_time <= b->last_access_time) ? 0 : -1;
}

static ret_t bitmap_cache_destroy(bitmap_cache_t* cache) {
  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  bitmap_destroy(&(cache->image));
  TKMEM_FREE(cache);

  return RET_OK;
}

ret_t image_manager_unload_unused(image_manager_t* imm, uint32_t time_delta_s) {
  bitmap_cache_t b;
  b.last_access_time = time_now_s() - time_delta_s;
  return_value_if_fail(imm != NULL && imm->loader != NULL, RET_BAD_PARAMS);

  return array_remove_all(&(imm->images), (tk_compare_t)bitmap_cache_cmp_time, &b,
                          (tk_destroy_t)bitmap_cache_destroy);
}

ret_t image_manager_deinit(image_manager_t* imm) {
  uint32_t i = 0;
  uint32_t nr = 0;
  bitmap_cache_t** all = NULL;
  return_value_if_fail(imm != NULL && imm->loader != NULL, RET_BAD_PARAMS);

  all = (bitmap_cache_t**)(imm->images.elms);
  for (i = 0, nr = imm->images.size; i < nr; i++) {
    bitmap_cache_destroy(all[i]);
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
