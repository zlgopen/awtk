/**
 * File:   image_manager.c
 * Author: AWTK Develop Team
 * Brief:  bitmap manager
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/locale_info.h"
#include "base/image_manager.h"

typedef struct _bitmap_cache_t {
  bitmap_t image;
  char* name;
  uint32_t access_count;
  uint32_t created_time;
  uint32_t last_access_time;
} bitmap_cache_t;

static int bitmap_cache_cmp_time(bitmap_cache_t* a, bitmap_cache_t* b) {
  return (a->last_access_time <= b->last_access_time) ? 0 : -1;
}

static int bitmap_cache_cmp_name(bitmap_cache_t* a, bitmap_cache_t* b) {
  return strcmp(a->name, b->name);
}

static int bitmap_cache_cmp_data(bitmap_cache_t* a, bitmap_cache_t* b) {
  return (char*)(a->image.buffer) - (char*)(b->image.buffer);
}

static ret_t bitmap_cache_destroy(bitmap_cache_t* cache) {
  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  log_debug("unload image %s\n", cache->name);
  bitmap_destroy(&(cache->image));
  TKMEM_FREE(cache->name);
  TKMEM_FREE(cache);

  return RET_OK;
}

static image_manager_t* s_image_manager = NULL;
image_manager_t* image_manager() {
  return s_image_manager;
}

ret_t image_manager_set(image_manager_t* imm) {
  s_image_manager = imm;

  return RET_OK;
}

image_manager_t* image_manager_create(void) {
  image_manager_t* imm = TKMEM_ZALLOC(image_manager_t);
  return_value_if_fail(imm != NULL, NULL);

  return image_manager_init(imm);
}

image_manager_t* image_manager_init(image_manager_t* imm) {
  return_value_if_fail(imm != NULL, NULL);

  darray_init(&(imm->images), 0, (tk_destroy_t)bitmap_cache_destroy, NULL);
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
  cache->name = tk_strdup(name);
  cache->image.name = cache->name;
  cache->last_access_time = cache->created_time;

  return darray_push(&(imm->images), cache);
}

ret_t image_manager_lookup(image_manager_t* imm, const char* name, bitmap_t* image) {
  bitmap_cache_t info;
  bitmap_cache_t* iter = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  memset(&info, 0x00, sizeof(info));

  info.name = (char*)name;
  imm->images.compare = (tk_compare_t)bitmap_cache_cmp_name;
  iter = darray_find(&(imm->images), &info);

  if (iter != NULL) {
    *image = iter->image;
    image->destroy = NULL;
    image->image_manager = imm;
    image->specific_destroy = NULL;
    image->should_free_data = FALSE;

    iter->access_count++;
    iter->last_access_time = time_now_s();

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t image_manager_update_specific(image_manager_t* imm, bitmap_t* image) {
  bitmap_cache_t info;
  bitmap_cache_t* iter = NULL;
  return_value_if_fail(imm != NULL && image != NULL, RET_BAD_PARAMS);

  if (image->image_manager != NULL) {
    imm = image->image_manager;
  }

  info.image.buffer = image->buffer;
  imm->images.compare = (tk_compare_t)bitmap_cache_cmp_data;
  iter = darray_find(&(imm->images), &info);

  if (iter != NULL) {
    iter->image.flags = image->flags;
    iter->image.specific = image->specific;
    iter->image.specific_ctx = image->specific_ctx;
    iter->image.specific_destroy = image->specific_destroy;

    image->specific_destroy = NULL;
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_manager_get_bitmap_impl(image_manager_t* imm, const char* name,
                                           bitmap_t* image) {
  const asset_info_t* res = NULL;
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  memset(image, 0x00, sizeof(bitmap_t));
  if (image_manager_lookup(imm, name, image) == RET_OK) {
    return RET_OK;
  }

  res = assets_manager_ref(imm->assets_manager, ASSET_TYPE_IMAGE, name);
  if (res == NULL) {
    return RET_NOT_FOUND;
  }

  memset(image, 0x00, sizeof(bitmap_t));
  if (res->subtype == ASSET_TYPE_IMAGE_RAW) {
    const bitmap_header_t* header = (const bitmap_header_t*)res->data;

    image->w = header->w;
    image->h = header->h;
    image->flags = header->flags;
    image->format = header->format;
    image->name = res->name;
    image->image_manager = imm;
    bitmap_set_line_length(image, image->line_length);
    image->buffer = GRAPHIC_BUFFER_CREATE_WITH_DATA(header->data, header->w, header->h,
                                                    (bitmap_format_t)(header->format));
    image->should_free_data = image->buffer != NULL;
    image_manager_add(imm, name, image);

    return RET_OK;
  } else if (res->subtype != ASSET_TYPE_IMAGE_BSVG) {
    ret_t ret = image_loader_load_image(res, image);
    if (ret == RET_OK) {
      image_manager_add(imm, name, image);
      assets_manager_unref(imm->assets_manager, res);
    }

    return image_manager_lookup(imm, name, image);
  } else {
    return RET_NOT_FOUND;
  }
}

typedef struct _imm_expr_info_t {
  image_manager_t* imm;
  bitmap_t* image;
} imm_expr_info_t;

static ret_t image_manager_on_expr_result(void* ctx, const void* data) {
  imm_expr_info_t* info = (imm_expr_info_t*)ctx;
  const char* name = (const char*)data;

  return image_manager_get_bitmap_impl(info->imm, name, info->image);
}

ret_t image_manager_get_bitmap_exprs(image_manager_t* imm, const char* exprs, bitmap_t* image) {
  imm_expr_info_t ctx = {imm, image};

  return system_info_eval_exprs(system_info(), exprs, image_manager_on_expr_result, &ctx);
}

ret_t image_manager_get_bitmap(image_manager_t* imm, const char* name, bitmap_t* image) {
  return_value_if_fail(imm != NULL && name != NULL && image != NULL, RET_BAD_PARAMS);

  if (strstr(name, TK_LOCALE_MAGIC) != NULL) {
    char locale[TK_NAME_LEN + 1];
    char real_name[TK_NAME_LEN + 1];
    const char* language = locale_info()->language;
    const char* country = locale_info()->country;

    tk_snprintf(locale, sizeof(locale) - 1, "%s_%s", language, country);
    tk_replace_locale(name, real_name, locale);
    if (image_manager_get_bitmap_impl(imm, real_name, image) == RET_OK) {
      return RET_OK;
    }

    tk_replace_locale(name, real_name, language);
    if (image_manager_get_bitmap_impl(imm, real_name, image) == RET_OK) {
      return RET_OK;
    }

    tk_replace_locale(name, real_name, "");
    if (image_manager_get_bitmap_impl(imm, real_name, image) == RET_OK) {
      return RET_OK;
    }

    return RET_FAIL;
  } else if (strchr(name, '$') != NULL || strchr(name, ',') != NULL) {
    return image_manager_get_bitmap_exprs(imm, name, image);
  } else {
    return image_manager_get_bitmap_impl(imm, name, image);
  }
}

ret_t image_manager_preload(image_manager_t* imm, const char* name) {
  bitmap_t image;
  return_value_if_fail(imm != NULL && name != NULL && *name, RET_BAD_PARAMS);

  return image_manager_get_bitmap(imm, name, &image);
}

ret_t image_manager_set_assets_manager(image_manager_t* imm, assets_manager_t* am) {
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  imm->assets_manager = am;

  return RET_OK;
}

ret_t image_manager_unload_unused(image_manager_t* imm, uint32_t time_delta_s) {
  bitmap_cache_t b;
  b.last_access_time = time_now_s() - time_delta_s;
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  imm->images.compare = (tk_compare_t)bitmap_cache_cmp_time;
  return darray_remove_all(&(imm->images), NULL, &b);
}

ret_t image_manager_unload_all(image_manager_t* imm) {
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  return darray_clear(&(imm->images));
}

ret_t image_manager_unload_bitmap(image_manager_t* imm, bitmap_t* image) {
  bitmap_cache_t b;
  return_value_if_fail(imm != NULL && image != NULL, RET_BAD_PARAMS);

  b.image.buffer = image->buffer;
  imm->images.compare = (tk_compare_t)bitmap_cache_cmp_data;

  return darray_remove_all(&(imm->images), NULL, &b);
}

ret_t image_manager_deinit(image_manager_t* imm) {
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  darray_deinit(&(imm->images));

  return RET_OK;
}

ret_t image_manager_destroy(image_manager_t* imm) {
  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);

  image_manager_deinit(imm);
  TKMEM_FREE(imm);

  return RET_OK;
}
