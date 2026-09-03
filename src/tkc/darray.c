/**
 * File:   darray.c
 * Author: AWTK Develop Team
 * Brief:  dynamic darray.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/darray.h"
#include "tkc/utils.h"
#include "tkc/mem.h"

darray_t* darray_create(uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare) {
  darray_t* darray = TKMEM_ZALLOC(darray_t);
  return_value_if_fail(darray != NULL, NULL);

  if (darray_init(darray, capacity, destroy, compare)) {
    return darray;
  } else {
    TKMEM_FREE(darray);

    return NULL;
  }
}

darray_t* darray_init(darray_t* darray, uint32_t capacity, tk_destroy_t destroy,
                      tk_compare_t compare) {
  return_value_if_fail(darray != NULL, NULL);

  darray->size = 0;
  darray->capacity = 0;
  darray->elms = NULL;
  darray->destroy = destroy != NULL ? destroy : dummy_destroy;
  darray->compare = compare != NULL ? compare : pointer_compare;

  if (capacity > 0) {
    darray->elms = TKMEM_ZALLOCN(void*, capacity);
    return_value_if_fail(darray->elms != NULL, NULL);
    darray->capacity = capacity;
  }

  return darray;
}

ret_t darray_extend(darray_t* darray, uint32_t capacity) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (0 == capacity || (darray->elms != NULL && capacity <= darray->capacity)) {
    return RET_OK;
  } else {
    uint32_t old_capacity = darray->capacity;
    void* elms = TKMEM_REALLOCT(void*, darray->elms, capacity);
    if (elms) {
      darray->elms = elms;
      darray->capacity = capacity;

      /* 将新分配的内存区域初始化为NULL */
      if (capacity > old_capacity) {
        memset(darray->elms + old_capacity, 0, (capacity - old_capacity) * sizeof(void*));
      }

      return RET_OK;
    } else {
      return RET_FAIL;
    }
  }
}

inline static bool_t darray_auto_extend(darray_t* darray) {
  if (darray->elms != NULL && darray->size < darray->capacity) {
    return TRUE;
  } else {
    uint32_t capacity = (darray->capacity >> 1) + darray->capacity + 1;
    return (RET_OK == darray_extend(darray, capacity));
  }
}

int32_t darray_find_index_ex(darray_t* darray, tk_compare_t compare, void* data) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(darray != NULL, -1);
  elms = darray->elms;
  size = darray->size;

  if (compare == NULL) {
    compare = darray->compare ? darray->compare : pointer_compare;
  }

  for (i = 0; i < size; i++) {
    void* iter = elms[i];
    if (compare(iter, data) == 0) {
      return i;
    }
  }

  return -1;
}

int32_t darray_find_index(darray_t* darray, void* data) {
  return darray_find_index_ex(darray, NULL, data);
}

ret_t darray_remove_index(darray_t* darray, uint32_t index) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  void* iter = NULL;
  return_value_if_fail(darray != NULL && index < darray->size, RET_BAD_PARAMS);

  elms = darray->elms;
  iter = elms[index];

  for (size = darray->size - 1, i = index; i < size; i++) {
    elms[i] = elms[i + 1];
  }

  elms[darray->size - 1] = NULL;
  darray->size--;
  darray->destroy(iter);

  return RET_OK;
}

ret_t darray_remove_range(darray_t* darray, uint32_t start, uint32_t end) {
  return_value_if_fail(darray != NULL && start < end && end <= darray->size, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    uint32_t i = 0;
    uint32_t range_size = end - start;
    uint32_t old_size = darray->size;
    uint32_t new_size = old_size - range_size;
    void** elms = darray->elms;

    for (i = start; i < new_size; i++) {
      tk_swap(elms[i], elms[i + range_size], void*);
    }

    darray->size = new_size;

    for (i = new_size; i < old_size; i++) {
      void* iter = elms[i];
      elms[i] = NULL;
      darray->destroy(iter);
    }
  }

  return RET_OK;
}

ret_t darray_remove_ex(darray_t* darray, tk_compare_t cmp, void* ctx) {
  int32_t index = 0;
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  index = darray_find_index_ex(darray, cmp, ctx);
  if (index < 0) {
    return RET_NOT_FOUND;
  } else {
    return darray_remove_index(darray, index);
  }
}

ret_t darray_remove(darray_t* darray, void* data) {
  return darray_remove_ex(darray, NULL, data);
}

ret_t darray_remove_all(darray_t* darray, tk_compare_t cmp, void* ctx) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    int32_t i = 0;
    int32_t new_size = 0;
    int32_t old_size = darray->size;
    void** elms = darray->elms;

    cmp = cmp != NULL ? cmp : darray->compare;

    for (i = 0, new_size = 0; i < old_size; i++) {
      if (0 == cmp(elms[i], ctx)) {
        continue;
      }
      if (i != new_size) {
        tk_swap(elms[i], elms[new_size], void*);
      }
      new_size++;
    }

    darray->size = new_size;

    for (i = new_size; i < old_size; i++) {
      void* iter = elms[i];
      if (iter != NULL) {
        elms[i] = NULL;
        darray->destroy(iter);
      }
    }
  }

  return RET_OK;
}

ret_t darray_sort(darray_t* darray, tk_compare_t cmp) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);
  cmp = cmp != NULL ? cmp : darray->compare;

  return tk_qsort(darray->elms, darray->size, cmp);
}

ret_t darray_find_all(darray_t* darray, tk_compare_t cmp, void* ctx, darray_t* matched) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(darray != NULL && matched != NULL, RET_BAD_PARAMS);

  elms = darray->elms;
  size = darray->size;
  cmp = cmp != NULL ? cmp : darray->compare;

  for (i = 0; i < size; i++) {
    void* iter = elms[i];
    if (cmp(iter, ctx) == 0) {
      return_value_if_fail(darray_push(matched, iter) == RET_OK, RET_OOM);
    }
  }

  return RET_OK;
}

void* darray_find_ex(darray_t* darray, tk_compare_t cmp, void* ctx) {
  int32_t pos = darray_find_index_ex(darray, cmp, ctx);

  return pos >= 0 ? darray->elms[pos] : NULL;
}

void* darray_find(darray_t* darray, void* data) {
  return darray_find_ex(darray, NULL, data);
}

void* darray_pop(darray_t* darray) {
  return_value_if_fail(darray != NULL, NULL);

  if (darray->size > 0) {
    return darray->elms[--darray->size];
  } else {
    return NULL;
  }
}

void* darray_tail(darray_t* darray) {
  return_value_if_fail(darray != NULL && darray->size > 0, NULL);

  return darray->elms[darray->size - 1];
}

void* darray_head(darray_t* darray) {
  return_value_if_fail(darray != NULL && darray->size > 0, NULL);

  return darray->elms[0];
}

ret_t darray_insert(darray_t* darray, uint32_t index, void* data) {
  void** s = NULL;
  void** d = NULL;
  void** p = NULL;
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);
  index = tk_min(index, darray->size);
  return_value_if_fail(darray_auto_extend(darray), RET_OOM);

  p = darray->elms + index;
  d = darray->elms + darray->size;
  s = d - 1;

  while (s >= p) {
    *d-- = *s--;
  }
  *p = data;
  darray->size++;

  return RET_OK;
}

ret_t darray_sorted_insert(darray_t* darray, void* data, tk_compare_t cmp,
                           bool_t replace_if_exist) {
  int32_t low = -1;
  int32_t index = 0;
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->size == 0) {
    return darray_push(darray, data);
  }

  index = darray_bsearch_index_ex(darray, cmp, data, &low);
  if (index >= 0) {
    if (replace_if_exist) {
      return darray_replace(darray, index, data);
    } else {
      /* 元素已存在但不替换，插入到找到位置之后 */
      index++;
    }
  } else {
    index = low;
  }

  if (index >= (int32_t)darray->size) {
    return darray_push(darray, data);
  } else {
    return darray_insert(darray, index, data);
  }
}

ret_t darray_push(darray_t* darray, void* data) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  return darray_insert(darray, darray->size, data);
}

ret_t darray_push_unique(darray_t* darray, void* data) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);
  if (darray_find_index(darray, data) >= 0) {
    return RET_FAIL;
  }

  return darray_insert(darray, darray->size, data);
}

int32_t darray_count(darray_t* darray, void* data) {
  int32_t n = 0;
  return_value_if_fail(darray != NULL, 0);

  if (darray->elms != NULL) {
    uint32_t i = 0;
    void** elms = darray->elms;

    for (i = 0; i < darray->size; i++) {
      void* iter = elms[i];
      if (darray->compare(iter, data) == 0) {
        n++;
      }
    }
  }

  return n;
}

ret_t darray_foreach(darray_t* darray, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(darray != NULL && visit != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    uint32_t i = 0;
    for (i = 0; i < darray->size;) {
      void* iter = darray->elms[i];
      ret = visit(ctx, iter);
      TK_FOREACH_VISIT_RESULT_PROCESSING(ret, darray_remove_index(darray, i));
      i++;
    }
  }

  return ret;
}

ret_t darray_clear(darray_t* darray) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    int32_t i = 0;
    void** elms = darray->elms;
    uint32_t count = darray->size;

    for (i = count - 1; i >= 0; i--) {
      void* iter = elms[i];
      elms[i] = NULL;
      darray->size--;
      darray->destroy(iter);
    }

    darray->size = 0;
  }

  return RET_OK;
}

ret_t darray_set(darray_t* darray, uint32_t index, void* data) {
  return_value_if_fail(darray != NULL && index < darray->size, RET_BAD_PARAMS);

  darray->elms[index] = data;

  return RET_OK;
}

ret_t darray_replace(darray_t* darray, uint32_t index, void* data) {
  return_value_if_fail(darray != NULL && index < darray->size, RET_BAD_PARAMS);

  if (darray->destroy != NULL) {
    darray->destroy(darray->elms[index]);
  }
  darray->elms[index] = data;

  return RET_OK;
}

void* darray_get(darray_t* darray, uint32_t index) {
  return_value_if_fail(darray != NULL && index < darray->size, NULL);

  return darray->elms[index];
}

ret_t darray_deinit(darray_t* darray) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    darray_clear(darray);
    TKMEM_FREE(darray->elms);
    memset(darray, 0x00, sizeof(darray_t));
  }

  return RET_OK;
}

ret_t darray_destroy(darray_t* darray) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  darray_deinit(darray);
  TKMEM_FREE(darray);

  return RET_OK;
}

int32_t darray_bsearch_index_ex(darray_t* darray, tk_compare_t cmp, void* ctx, int32_t* ret_low) {
  int32_t low = 0;
  int32_t mid = 0;
  int32_t high = 0;
  int32_t result = 0;
  void* iter = NULL;
  return_value_if_fail(darray != NULL, -1);

  if (darray->size == 0) {
    if (ret_low != NULL) {
      *ret_low = 0;
    }
    return -1;
  }
  if (cmp == NULL) {
    cmp = darray->compare;
  }

  high = darray->size - 1;
  while (low <= high) {
    mid = low + ((high - low) >> 1);
    iter = darray->elms[mid];

    result = cmp(iter, ctx);

    if (result == 0) {
      return mid;
    } else if (result < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  if (ret_low != NULL) {
    *ret_low = low;
  }

  return -1;
}

int32_t darray_bsearch_index(darray_t* darray, tk_compare_t cmp, void* ctx) {
  return darray_bsearch_index_ex(darray, cmp, ctx, NULL);
}

void* darray_bsearch(darray_t* darray, tk_compare_t cmp, void* ctx) {
  int32_t index = darray_bsearch_index(darray, cmp, ctx);
  if (index >= 0) {
    return darray->elms[index];
  } else {
    return NULL;
  }
}

inline static void darray_reverse_range(darray_t* darray, uint32_t start, uint32_t end) {
  void** elms = darray->elms;
  for (; start < end; start++, end--) {
    tk_swap(elms[start], elms[end], void*);
  }
}

ret_t darray_reverse(darray_t* darray) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->size < 2) {
    return RET_OK;
  }

  darray_reverse_range(darray, 0, darray->size - 1);

  return RET_OK;
}
