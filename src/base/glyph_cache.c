/**
 * File:   glyph_cache.c
 * Author: AWTK Develop Team
 * Brief:  glyph cache
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/glyph_cache.h"

#ifdef WITH_SDL
typedef struct _glyph_cache_key_t {
  font_size_t size;
  uint32_t code;
} glyph_cache_key_t;

#define TK_CACHE_CLOCK_KEY_T glyph_cache_key_t
#define TK_CACHE_CLOCK_DATA_T glyph_t*

inline static uint64_t glyph_cache_key_hash(const glyph_cache_key_t key) {
  uint64_t ret = ((uint64_t)(key.size) << 32) | key.code;
  return ret;
}
#define TK_CACHE_CLOCK_KEY_HASH(key, ctx) glyph_cache_key_hash(key)

inline static int32_t glyph_cache_key_compare(const glyph_cache_key_t a,
                                              const glyph_cache_key_t b) {
  if (a.code != b.code) {
    return a.code > b.code ? 1 : -1;
  }
  return a.size > b.size ? 1 : (a.size < b.size ? -1 : 0);
}
#define TK_CACHE_CLOCK_KEY_COMPARE(a, b, ctx) glyph_cache_key_compare(a, b)
#define TK_CACHE_CLOCK_KEY_IS_SAME_OBJECT(a, b, ctx) (FALSE)

#define TK_CACHE_CLOCK_CTX_T tk_destroy_t
#define TK_CACHE_CLOCK_DATA_DESTROY(g, destroy_glyph) \
  ((destroy_glyph) != NULL ? (destroy_glyph)(g) : RET_OK)

#include "tkc/cache_clock.h"

struct _glyph_cache_t {
  tk_cache_clock_t cache;
};

glyph_cache_t* glyph_cache_create(uint32_t capacity, tk_destroy_t destroy_glyph) {
  glyph_cache_t* cache = NULL;
  return_value_if_fail(capacity > 0, NULL);

  cache = TKMEM_ZALLOC(glyph_cache_t);
  return_value_if_fail(cache != NULL, NULL);

  goto_error_if_fail(RET_OK == tk_cache_clock_init(&(cache->cache), capacity, destroy_glyph));

  return cache;
error:
  TKMEM_FREE(cache);
  return NULL;
}

ret_t glyph_cache_add(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g) {
  glyph_cache_key_t key;
  return_value_if_fail(cache != NULL && g != NULL, RET_BAD_PARAMS);

  key.size = size;
  key.code = code;

  return tk_cache_clock_put(&(cache->cache), key, g);
}

ret_t glyph_cache_lookup(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g) {
  glyph_t* found = NULL;
  glyph_cache_key_t key;
  return_value_if_fail(cache != NULL && g != NULL, RET_BAD_PARAMS);

  key.size = size;
  key.code = code;

  if (tk_cache_clock_get(&(cache->cache), key, &found) != RET_OK) {
    return RET_NOT_FOUND;
  }

  *g = *found;

  return RET_OK;
}

ret_t glyph_cache_shrink(glyph_cache_t* cache, uint32_t cache_size) {
  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  return tk_cache_clock_shrink(&(cache->cache), cache_size);
}

ret_t glyph_cache_destroy(glyph_cache_t* cache) {
  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  tk_cache_clock_deinit(&(cache->cache));
  TKMEM_FREE(cache);

  return RET_OK;
}
#else
#include "tkc/time_now.h"

typedef struct _glyph_cache_item_t {
  uint64_t last_access_time;
  font_size_t size;
  uint32_t code;
  glyph_t* g;
} glyph_cache_item_t;

struct _glyph_cache_t {
  uint32_t size;
  uint32_t capacity;
  glyph_cache_item_t* items;
  tk_destroy_t destroy_glyph;
};

#define GLYPH_CACHE_FREE_ITEM(cache, item)            \
  do {                                                \
    if (cache->destroy_glyph && item->g != NULL) {    \
      cache->destroy_glyph(item->g);                  \
      memset(item, 0x00, sizeof(glyph_cache_item_t)); \
    }                                                 \
  } while (0)

glyph_cache_t* glyph_cache_create(uint32_t capacity, tk_destroy_t destroy_glyph) {
  glyph_cache_t* cache = NULL;
  return_value_if_fail(capacity > 0, NULL);

  cache = TKMEM_ZALLOC(glyph_cache_t);
  return_value_if_fail(cache != NULL, NULL);

  cache->items = TKMEM_ZALLOCN(glyph_cache_item_t, capacity);
  goto_error_if_fail(cache->items != NULL);

  cache->size = 0;
  cache->capacity = capacity;
  cache->destroy_glyph = destroy_glyph;

  return cache;
error:
  TKMEM_FREE(cache);
  return NULL;
}

static glyph_cache_item_t* glyph_cache_get_empty(glyph_cache_t* cache) {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t oldest = 0;
  glyph_cache_item_t* item = NULL;
  uint64_t oldest_time = UINT64_MAX;

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

ret_t glyph_cache_add(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g) {
  glyph_cache_item_t* item = glyph_cache_get_empty(cache);
  return_value_if_fail(cache != NULL && g != NULL && item != NULL, RET_BAD_PARAMS);

  item->g = g;
  item->size = size;
  item->code = code;
  item->last_access_time = time_now_ms();

  return RET_OK;
}

ret_t glyph_cache_lookup(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g) {
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

ret_t glyph_cache_destroy(glyph_cache_t* cache) {
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
  TKMEM_FREE(cache);

  return RET_OK;
}
#endif /*WITH_SDL*/
