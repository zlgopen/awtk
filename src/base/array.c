/**
 * File:   array.c
 * Author: AWTK Develop Team
 * Brief:  dynamic array.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/array.h"
#include "base/mem.h"

array_t* array_create(uint16_t capacity) {
  array_t* array = TKMEM_ZALLOC(array_t);
  return_value_if_fail(array != NULL, NULL);

  if (array_init(array, capacity)) {
    return array;
  } else {
    TKMEM_FREE(array);

    return NULL;
  }
}

array_t* array_init(array_t* array, uint16_t capacity) {
  return_value_if_fail(array != NULL, NULL);

  array->size = 0;
  array->elms = NULL;

  if (capacity > 0) {
    array->elms = TKMEM_ZALLOCN(void*, capacity);
    return_value_if_fail(array->elms != NULL, NULL);
    array->capacity = capacity;
  }

  return array;
}

static bool_t array_extend(array_t* array) {
  if (array->elms != NULL && array->size < array->capacity) {
    return TRUE;
  } else {
    void* elms = NULL;
    uint16_t capacity = (array->capacity >> 1) + array->capacity + 1;

    elms = TKMEM_REALLOC(void*, array->elms, capacity);
    if (elms) {
      array->elms = elms;
      array->capacity = capacity;

      return TRUE;
    } else {
      return FALSE;
    }
  }
}

int array_find_index(array_t* array, tk_compare_t cmp, void* data) {
  int32_t i = 0;
  int32_t size = 0;
  void** elms = NULL;
  return_value_if_fail(array != NULL, -1);
  elms = array->elms;
  size = array->size;

  if (cmp) {
    for (i = 0; i < size; i++) {
      void* iter = elms[i];
      if (cmp(iter, data) == 0) {
        return i;
      }
    }
  } else {
    for (i = 0; i < size; i++) {
      void* iter = elms[i];
      if (iter == data) {
        return i;
      }
    }
  }

  return -1;
}

ret_t array_remove(array_t* array, tk_compare_t cmp, void* data, tk_destroy_t destroy) {
  int32_t i = 0;
  int32_t pos = 0;
  int32_t size = 0;
  void** elms = NULL;

  return_value_if_fail(array != NULL, RET_BAD_PARAMS);
  pos = array_find_index(array, cmp, data);
  if (pos < 0) {
    return RET_NOT_FOUND;
  }

  elms = array->elms;

  if (destroy != NULL) {
    destroy(elms[pos]);
    elms[pos] = NULL;
  }

  for (size = array->size - 1, i = pos; i < size; i++) {
    elms[i] = elms[i + 1];
  }

  elms[i] = NULL;
  array->size--;

  return RET_OK;
}

ret_t array_remove_all(array_t* array, tk_compare_t cmp, void* data, tk_destroy_t destroy) {
  int32_t i = 0;
  int32_t k = 0;
  int32_t size = 0;
  void** elms = NULL;

  return_value_if_fail(array != NULL, RET_BAD_PARAMS);
  elms = array->elms;
  size = array->size;

  for (i = 0, k = 0; i < size; i++) {
    void* iter = elms[i];
    if ((cmp && cmp(iter, data) == 0) || iter == data) {
      if (destroy != NULL) {
        destroy(iter);
      }
      elms[i] = NULL;
    } else {
      if (k != i) {
        elms[k] = elms[i];
      }

      k++;
    }
  }
  array->size = k;

  return RET_OK;
}

void* array_find(array_t* array, tk_compare_t cmp, void* data) {
  int pos = array_find_index(array, cmp, data);
  if (pos >= 0) {
    return array->elms[pos];
  } else {
    return NULL;
  }
}

void* array_pop(array_t* array) {
  return_value_if_fail(array != NULL && array->size > 0, NULL);

  return array->elms[--array->size];
}

ret_t array_push(array_t* array, void* data) {
  return_value_if_fail(array != NULL, RET_BAD_PARAMS);
  return_value_if_fail(array_extend(array), RET_OOM);

  array->elms[array->size++] = data;

  return RET_OK;
}

void array_deinit(array_t* array) {
  return_if_fail(array != NULL);
  if (array->elms != NULL) {
    TKMEM_FREE(array->elms);
  }
  memset(array, 0x00, sizeof(array_t));

  return;
}

void array_destroy(array_t* array) {
  return_if_fail(array != NULL && array->elms != NULL);
  array_deinit(array);
  TKMEM_FREE(array);

  return;
}
