/**
 * File:   glyph_cache.c
 * Author: AWTK Develop Team
 * Brief:  glyph cache
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
 * 2018-06-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define GLYPH_CACHE_FREE_ITEM(cache, item)            \
  {                                                   \
    if (cache->destroy_glyph && item->g != NULL) {    \
      cache->destroy_glyph(item->g);                  \
      memset(item, 0x00, sizeof(glyph_cache_item_t)); \
    }                                                 \
  }

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "base/glyph_cache.h"

glyph_cache_t* glyph_cache_init(glyph_cache_t* cache, uint32_t capacity,
                                tk_destroy_t destroy_glyph) {
  return_value_if_fail(cache != NULL && capacity > 0, NULL);

  cache->items = TKMEM_ZALLOCN(glyph_cache_item_t, capacity);
  return_value_if_fail(cache->items != NULL, NULL);

  cache->size = 0;
  cache->capacity = capacity;
  cache->destroy_glyph = destroy_glyph;

  return cache;
}

static glyph_cache_item_t* glyph_cache_get_empty(glyph_cache_t* cache) {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t oldest = 0;
  glyph_cache_item_t* item = NULL;
  uint32_t oldest_time = 0xffffffff;

  return_value_if_fail(cache != NULL && cache->items != NULL, NULL);

  if (cache->size < cache->capacity) {
    oldest = cache->size++;
    return cache->items + oldest;
  }

  for (i = 0, nr = cache->size; i < nr; i++) {
    glyph_cache_item_t* item = cache->items + i;
    if (item->last_access_time < oldest_time) {
      oldest_time = item->last_access_time;
      oldest = i;
      if (oldest_time == 0) {
        break;
      }
    }
  }

  item = cache->items + oldest;
  GLYPH_CACHE_FREE_ITEM(cache, item);

  return item;
}

ret_t glyph_cache_add(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g) {
  glyph_cache_item_t* item = glyph_cache_get_empty(cache);
  return_value_if_fail(cache != NULL && g != NULL && item != NULL, RET_BAD_PARAMS);

  item->g = g;
  item->size = size;
  item->code = code;
  item->last_access_time = time_now_ms();

  return RET_OK;
}

ret_t glyph_cache_lookup(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g) {
  uint32_t i = 0;
  uint32_t nr = 0;

  return_value_if_fail(cache != NULL && g != NULL, RET_BAD_PARAMS);
  for (i = 0, nr = cache->size; i < nr; i++) {
    glyph_cache_item_t* item = cache->items + i;
    if (item->code == code && item->size == size) {
      *g = *(item->g);
      item->last_access_time = time_now_ms();

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

ret_t glyph_cache_deinit(glyph_cache_t* cache) {
  uint32_t i = 0;
  uint32_t nr = 0;

  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  if (cache->destroy_glyph != NULL) {
    for (i = 0, nr = cache->size; i < nr; i++) {
      glyph_cache_item_t* item = cache->items + i;

      GLYPH_CACHE_FREE_ITEM(cache, item);
    }
  }

  TKMEM_FREE(cache->items);
  memset(cache, 0x00, sizeof(glyph_cache_t));

  return RET_OK;
}

static int glyph_cache_item_compare_by_time(const void* a, const void* b) {
  glyph_cache_item_t* aa = (glyph_cache_item_t*)a;
  glyph_cache_item_t* bb = (glyph_cache_item_t*)b;

  return bb->last_access_time - aa->last_access_time;
}

ret_t glyph_cache_shrink(glyph_cache_t* cache, uint32_t cache_size) {
  return_value_if_fail(cache != NULL && cache->items != NULL, RET_BAD_PARAMS);

  if (cache->size > cache_size) {
    uint32_t i = 0;
    qsort(cache->items, cache->size, sizeof(glyph_cache_item_t), glyph_cache_item_compare_by_time);

    for (i = cache_size; i < cache->size; i++) {
      glyph_cache_item_t* item = cache->items + i;
      GLYPH_CACHE_FREE_ITEM(cache, item);
    }

    cache->size = cache_size;
  }

  return RET_OK;
}
