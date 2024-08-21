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

uint64_t named_value_hash_get_hash_from_str(const char* str, uint64_t base) {
  uint64_t ret = 0;
  uint64_t p = 0;
  const char* iter = NULL;
  for (iter = str, p = 1; (*iter) != '\0'; iter++, p *= base) {
    ret += (*iter) * p;
  }
  return ret;
}

ret_t named_value_hash_set_name(named_value_hash_t* nvh, const char* name, uint64_t hash_base) {
  return_value_if_fail(nvh != NULL, RET_BAD_PARAMS);

  named_value_set_name(&nvh->base, name);
  nvh->hash = named_value_hash_get_hash_from_str(name, hash_base);

  return RET_OK;
}

ret_t named_value_hash_deinit(named_value_hash_t* nvh) {
  return_value_if_fail(nvh != NULL, RET_BAD_PARAMS);

  named_value_deinit(&nvh->base);
  memset(nvh, 0, sizeof(named_value_hash_t));

  return RET_OK;
}

named_value_hash_t* named_value_hash_init(named_value_hash_t* nvh, const char* name,
                                          const value_t* value, uint64_t hash_base) {
  ret_t ret = RET_OK;
  return_value_if_fail(nvh != NULL, NULL);

  memset(nvh, 0x00, sizeof(named_value_hash_t));

  if (name != NULL) {
    ret = named_value_hash_set_name(nvh, name, hash_base);
  }

  if (RET_OK == ret) {
    if (value != NULL) {
      named_value_set_value(nvh, value);
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

named_value_hash_t* named_value_hash_create_ex(const char* name, const value_t* value,
                                               uint64_t hash_base) {
  named_value_hash_t* ret = TKMEM_ZALLOC(named_value_hash_t);
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(ret == named_value_hash_init(ret, name, value, hash_base));

  return ret;
error:
  named_value_hash_destroy(ret);
  return NULL;
}

named_value_hash_t* named_value_hash_create(void) {
  return named_value_hash_create_ex(NULL, NULL, HASH_BASE_DEFAULT);
}

int32_t named_value_hash_compare(named_value_hash_t* nvh, const named_value_hash_t* other) {
  int32_t ret = -1;
  return_value_if_fail(nvh != NULL && other != NULL, -1);

  return nvh->hash - other->hash;
}

int32_t named_value_hash_compare_by_hash(named_value_hash_t* nvh, uint64_t hash) {
  int32_t ret = -1;
  return_value_if_fail(nvh != NULL, -1);

  return nvh->hash - hash;
}

named_value_hash_t* named_value_hash_clone(named_value_hash_t* nvh) {
  named_value_hash_t* ret = named_value_hash_create();
  return_value_if_fail(ret != NULL, NULL);

  ret->hash = nvh->hash;

  named_value_set_name(&ret->base, nvh->base.name);

  named_value_set_value(&ret->base, &nvh->base.value);

  return ret;
}
