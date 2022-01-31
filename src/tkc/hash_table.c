/**
 * File:   hash_table.c
 * Author: AWTK Develop Team
 * Brief:  hash table
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/hash_table.h"

hash_table_t* hash_table_create(uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare,
                                tk_hash_t hash) {
  hash_table_t* hash_table = NULL;
  return_value_if_fail(capacity > 0 && hash != NULL && compare != NULL, NULL);
  hash_table = TKMEM_ZALLOC(hash_table_t);
  return_value_if_fail(hash_table != NULL, NULL);

  if (hash_table_init(hash_table, capacity, destroy, compare, hash) == NULL) {
    TKMEM_FREE(hash_table);
  }

  return hash_table;
}

hash_table_t* hash_table_init(hash_table_t* hash_table, uint32_t capacity, tk_destroy_t destroy,
                              tk_compare_t compare, tk_hash_t hash) {
  uint32_t i = 0;
  return_value_if_fail(hash_table != NULL, NULL);
  return_value_if_fail(capacity > 0 && hash != NULL && compare != NULL, NULL);

  memset(hash_table, 0, sizeof(hash_table_t));

  hash_table->hash = hash;
  hash_table->destroy = destroy;
  hash_table->compare = compare;
  goto_error_if_fail(
      darray_init(&(hash_table->buckets), capacity, (tk_destroy_t)darray_destroy, NULL));

  for (i = 0; i < capacity; i++) {
    darray_t* arr = darray_create(5, destroy, compare);
    goto_error_if_fail(arr != NULL);
    ENSURE(darray_push(&(hash_table->buckets), arr) == RET_OK);
  }

  return hash_table;
error:
  hash_table_deinit(hash_table);

  return NULL;
}

static darray_t* hash_table_get_array(hash_table_t* hash_table, void* ctx) {
  uint32_t index = 0;
  return_value_if_fail(hash_table != NULL, NULL);
  return_value_if_fail(hash_table->hash != NULL && hash_table->buckets.size > 0, NULL);

  index = hash_table->hash(ctx) % hash_table->buckets.size;

  return (darray_t*)darray_get(&(hash_table->buckets), index);
}

void* hash_table_find(hash_table_t* hash_table, tk_compare_t cmp, void* ctx) {
  darray_t* arr = hash_table_get_array(hash_table, ctx);
  return_value_if_fail(arr != NULL, NULL);

  return darray_find_ex(arr, cmp, ctx);
}

ret_t hash_table_add(hash_table_t* hash_table, void* data, bool_t replace_if_exist) {
  darray_t* arr = hash_table_get_array(hash_table, data);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return darray_sorted_insert(arr, data, hash_table->compare, replace_if_exist);
}

ret_t hash_table_remove(hash_table_t* hash_table, tk_compare_t cmp, void* ctx) {
  darray_t* arr = hash_table_get_array(hash_table, ctx);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return darray_remove_ex(arr, hash_table->compare, ctx);
}

ret_t hash_table_remove_all(hash_table_t* hash_table, tk_compare_t cmp, void* ctx) {
  uint32_t i = 0;
  uint32_t n = 0;
  darray_t** arrs = NULL;
  return_value_if_fail(hash_table != NULL, 0);

  n = hash_table->buckets.size;
  arrs = (darray_t**)(hash_table->buckets.elms);

  for (i = 0; i < n; i++) {
    darray_t* iter = arrs[i];
    darray_remove_all(iter, cmp, ctx);
  }

  return RET_OK;
}

int32_t hash_table_count(hash_table_t* hash_table, tk_compare_t cmp, void* ctx) {
  uint32_t i = 0;
  uint32_t n = 0;
  int32_t count = 0;
  darray_t** arrs = NULL;
  return_value_if_fail(hash_table != NULL, 0);

  n = hash_table->buckets.size;
  arrs = (darray_t**)(hash_table->buckets.elms);

  for (i = 0; i < n; i++) {
    darray_t* iter = arrs[i];

    iter->compare = cmp;
    count += darray_count(iter, ctx);
    iter->compare = hash_table->compare;
  }

  return count;
}

ret_t hash_table_clear(hash_table_t* hash_table) {
  uint32_t i = 0;
  uint32_t n = 0;
  darray_t** arrs = NULL;
  return_value_if_fail(hash_table != NULL, RET_BAD_PARAMS);

  n = hash_table->buckets.size;
  arrs = (darray_t**)(hash_table->buckets.elms);

  for (i = 0; i < n; i++) {
    darray_t* iter = arrs[i];
    darray_clear(iter);
  }

  return RET_OK;
}

ret_t hash_table_foreach(hash_table_t* hash_table, tk_visit_t visit, void* ctx) {
  uint32_t i = 0;
  uint32_t n = 0;
  darray_t** arrs = NULL;
  return_value_if_fail(hash_table != NULL, RET_BAD_PARAMS);

  n = hash_table->buckets.size;
  arrs = (darray_t**)(hash_table->buckets.elms);

  for (i = 0; i < n; i++) {
    darray_t* iter = arrs[i];
    ret_t ret = darray_foreach(iter, visit, ctx);
    if (ret != RET_OK) {
      return ret;
    }
  }

  return RET_OK;
}

ret_t hash_table_deinit(hash_table_t* hash_table) {
  return_value_if_fail(hash_table != NULL, RET_BAD_PARAMS);

  darray_deinit(&(hash_table->buckets));
  memset(hash_table, 0x00, sizeof(hash_table_t));

  return RET_OK;
}

ret_t hash_table_destroy(hash_table_t* hash_table) {
  return_value_if_fail(hash_table != NULL, RET_BAD_PARAMS);

  hash_table_deinit(hash_table);
  TKMEM_FREE(hash_table);

  return RET_OK;
}

uint32_t hash_table_hash_str(const void* data) {
  const char* p = (const char*)data;
  uint32_t value = 0;
  return_value_if_fail(data != NULL, 0);
  while (*p) {
    value += *p++;
  }

  return value;
}

uint32_t hash_table_hash_int(const void* data) {
  return tk_pointer_to_int(data);
}
