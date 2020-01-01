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

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "base/glyph_cache.h"

glyph_cache_t* glyph_cache_init(glyph_cache_t* cache, uint32_t capacity,
                                tk_destroy_t destroy_glyph) {
  return_value_if_fail(cache != NULL && capacity > 10, NULL);

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

  if ((cache->size + 1) < cache->capacity) {
    oldest = cache->size++;
    return cache->items + oldest;
  }

  for (i = 0, nr = cache->size; i < nr; i++) {
    glyph_cache_item_t* item = cache->items + i;
    if (item->last_access_time < oldest_time) {
      oldest_time = item->last_access_time;
      oldest = i;
    }
  }

  item = cache->items + oldest;
  if (cache->destroy_glyph) {
    cache->destroy_glyph(item->g);
    memset(item, 0x00, sizeof(glyph_cache_item_t));
  }

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
      cache->destroy_glyph(item->g);
    }
  }

  TKMEM_FREE(cache->items);
  memset(cache, 0x00, sizeof(glyph_cache_t));

  return RET_OK;
}
