/**
 * File:   named_value_hash.c
 * Author: AWTK Develop Team
 * Brief:  带有散列值的named value
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-08-21 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"

#include "tkc/named_value_hash.h"

#define NAMED_VALUE_HASH_BKDR_HASH_BASE 131
/*在Brian Kernighan与Dennis Ritchie的《The C Programming Language》一书被展示而得名，是一种简单快捷的hash算法，也是Java目前采用的字符串的Hash算法。*/
static inline uint64_t bkdr_hash(const char* str) {
  const char* iter = NULL;
  uint64_t ret = 0;
  for (ret = 0, iter = str; (*iter) != '\0'; iter++) {
    ret = ret * NAMED_VALUE_HASH_BKDR_HASH_BASE + (*iter);
  }
  return ret;
}
#undef NAMED_VALUE_HASH_BKDR_HASH_BASE

uint64_t named_value_hash_get_hash_from_str(const char* str) {
  return bkdr_hash(str);
}

ret_t named_value_hash_set_name(named_value_hash_t* nvh, const char* name) {
  return_value_if_fail(nvh != NULL, RET_BAD_PARAMS);

  named_value_set_name(&nvh->base, name);
  nvh->hash = named_value_hash_get_hash_from_str(name);

  return RET_OK;
}

ret_t named_value_hash_deinit(named_value_hash_t* nvh) {
  return_value_if_fail(nvh != NULL, RET_BAD_PARAMS);

  named_value_deinit(&nvh->base);
  memset(nvh, 0, sizeof(named_value_hash_t));

  return RET_OK;
}

named_value_hash_t* named_value_hash_init(named_value_hash_t* nvh, const char* name,
                                          const value_t* value) {
  ret_t ret = RET_OK;
  return_value_if_fail(nvh != NULL, NULL);

  memset(nvh, 0x00, sizeof(named_value_hash_t));

  if (name != NULL) {
    ret = named_value_hash_set_name(nvh, name);
  }

  if (RET_OK == ret) {
    if (value != NULL) {
      named_value_set_value((named_value_t*)nvh, value);
    }
  }

  return (RET_OK == ret) ? nvh : NULL;
}

ret_t named_value_hash_destroy(named_value_hash_t* nvh) {
  return_value_if_fail(nvh != NULL, RET_BAD_PARAMS);

  named_value_hash_deinit(nvh);
  TKMEM_FREE(nvh);

  return RET_OK;
}

named_value_hash_t* named_value_hash_create_ex(const char* name, const value_t* value) {
  named_value_hash_t* ret = TKMEM_ZALLOC(named_value_hash_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(ret == named_value_hash_init(ret, name, value));

  return ret;
error:
  named_value_hash_destroy(ret);
  return NULL;
}

named_value_hash_t* named_value_hash_create(void) {
  return named_value_hash_create_ex(NULL, NULL);
}

static int32_t named_value_hash_hash_compare(uint64_t hash1, uint64_t hash2) {
  if (hash1 > hash2) {
    // uint64_t d = hash1 - hash2;
    // return tk_min(d, INT32_MAX);
    return 1;
  } else if (hash1 < hash2) {
    // uint64_t d = hash2 - hash1;
    // int64_t t = -tk_min(d, -(int64_t)INT32_MIN);
    // return tk_max(INT32_MIN, t);
    return -1;
  } else {
    return 0;
  }
}

int32_t named_value_hash_compare(const named_value_hash_t* nvh, const named_value_hash_t* other) {
  return_value_if_fail(nvh != NULL && other != NULL, -1);

  return named_value_hash_hash_compare(nvh->hash, other->hash);
}

int32_t named_value_hash_compare_by_hash(const named_value_hash_t* nvh, uint64_t hash) {
  return_value_if_fail(nvh != NULL, -1);

  return named_value_hash_hash_compare(nvh->hash, hash);
}

named_value_hash_t* named_value_hash_clone(named_value_hash_t* nvh) {
  named_value_hash_t* ret = named_value_hash_create();
  return_value_if_fail(ret != NULL, NULL);

  ret->hash = nvh->hash;
  named_value_set_name(&ret->base, nvh->base.name);
  named_value_set_value(&ret->base, &nvh->base.value);

  return ret;
}
