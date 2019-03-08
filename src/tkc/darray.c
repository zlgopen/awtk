/**
 * File:   darray.c
 * Author: AWTK Develop Team
 * Brief:  dynamic darray.
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static bool_t darray_extend(darray_t* darray) {
  if (darray->elms != NULL && darray->size < darray->capacity) {
    return TRUE;
  } else {
    void* elms = NULL;
    uint32_t capacity = (darray->capacity >> 1) + darray->capacity + 1;

    elms = TKMEM_REALLOCT(void*, darray->elms, capacity);
    if (elms) {
      darray->elms = elms;
      darray->capacity = capacity;

      return TRUE;
    } else {
      return FALSE;
    }
  }
}

int darray_find_index(darray_t* darray, void* data) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(darray != NULL, -1);

  elms = darray->elms;
  size = darray->size;
  for (i = 0; i < size; i++) {
    void* iter = elms[i];
    if (darray->compare(iter, data) == 0) {
      return i;
    }
  }

  return -1;
}

ret_t darray_remove_index(darray_t* darray, uint32_t index) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(darray != NULL && index < darray->size, RET_BAD_PARAMS);

  elms = darray->elms;
  darray->destroy(elms[index]);
  elms[index] = NULL;

  for (size = darray->size - 1, i = index; i < size; i++) {
    elms[i] = elms[i + 1];
  }

  elms[i] = NULL;
  darray->size--;

  return RET_OK;
}

ret_t darray_remove(darray_t* darray, void* data) {
  int32_t index = 0;
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  index = darray_find_index(darray, data);
  if (index < 0) {
    return RET_NOT_FOUND;
  } else {
    return darray_remove_index(darray, index);
  }
}

ret_t darray_remove_all(darray_t* darray, void* data) {
  int32_t i = 0;
  int32_t k = 0;
  int32_t size = 0;
  void** elms = NULL;

  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);
  elms = darray->elms;
  size = darray->size;

  for (i = 0, k = 0; i < size; i++) {
    void* iter = elms[i];
    if (darray->compare(iter, data) == 0) {
      darray->destroy(iter);
      elms[i] = NULL;
    } else {
      if (k != i) {
        elms[k] = elms[i];
      }
      k++;
    }
  }
  darray->size = k;

  return RET_OK;
}

void* darray_find(darray_t* darray, void* data) {
  int pos = darray_find_index(darray, data);

  return pos >= 0 ? darray->elms[pos] : NULL;
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

ret_t darray_push(darray_t* darray, void* data) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);
  return_value_if_fail(darray_extend(darray), RET_OOM);

  darray->elms[darray->size++] = data;

  return RET_OK;
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
  return_value_if_fail(darray != NULL && visit != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    uint32_t i = 0;
    void** elms = darray->elms;

    for (i = 0; i < darray->size; i++) {
      void* iter = elms[i];
      ret_t ret = visit(ctx, iter);
      if (ret != RET_OK) {
        return ret;
      }
    }
  }

  return RET_OK;
}

ret_t darray_clear(darray_t* darray) {
  return_value_if_fail(darray != NULL, RET_BAD_PARAMS);

  if (darray->elms != NULL) {
    uint32_t i = 0;
    void** elms = darray->elms;

    for (i = 0; i < darray->size; i++) {
      void* iter = elms[i];
      darray->destroy(iter);
    }

    darray->size = 0;
  }

  return RET_OK;
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
  return_value_if_fail(darray != NULL && darray->elms != NULL, RET_BAD_PARAMS);

  darray_deinit(darray);
  TKMEM_FREE(darray);

  return RET_OK;
}
