/**
 * File:   raw_darray.h
 * Author: AWTK Develop Team
 * Brief:  原生动态数组。
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-03-02 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 * Inspired by <Insane Shadow Data Technique in C>[https://www.youtube.com/watch?v=gtk3RZHwJUA]
 */

#ifndef TK_RAW_DARRAY_H
#define TK_RAW_DARRAY_H

#include "tkc/types_def.h"
#include "tkc/mem.h"

BEGIN_C_DECLS

#ifdef _MSC_VER
#define TK_ALIGN(v, len) __declspec(align(len)) v
#else
#define TK_ALIGN(v, len) v __attribute__((aligned(len)))
#endif /*_MSC_VER*/

typedef struct _tk_raw_darray_header_t {
  size_t size;
  size_t capacity;
} TK_ALIGN(tk_raw_darray_header_t, 16);

inline static void* tk_raw_darray_create(size_t cap, size_t elm_size) {
  tk_raw_darray_header_t* header =
      (tk_raw_darray_header_t*)TKMEM_ALLOC(sizeof(tk_raw_darray_header_t) + elm_size * cap);
  if (NULL != header) {
    memset(header, 0, sizeof(tk_raw_darray_header_t));
    header->capacity = cap;
    return (void*)(header + 1);
  }
  return NULL;
}

inline static ret_t tk_raw_darray_destroy(void* arr) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(arr)-1;
  TKMEM_FREE(header);

  return RET_OK;
}

inline static ret_t tk_raw_darray_extend(void** p_arr, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;
  if (header->capacity < header->size) {
    void* new_header = NULL;
    size_t new_capacity = (header->capacity >> 1) + header->capacity + 1;
    new_capacity = tk_max(new_capacity, header->size);
    new_header = TKMEM_REALLOC(header, sizeof(tk_raw_darray_header_t) + elm_size * new_capacity);
    if (NULL != new_header) {
      header = (tk_raw_darray_header_t*)new_header;
      header->capacity = new_capacity;
      *p_arr = (void*)(header + 1);
    } else {
      return RET_OOM;
    }
  }

  return RET_OK;
}

inline static ret_t tk_raw_darray_resize(void** p_arr, size_t size, size_t elm_size) {
  ret_t ret = RET_OK;
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  tk_swap(size, header->size, size_t);

  ret = tk_raw_darray_extend(p_arr, elm_size);
  goto_error_if_fail(RET_OK == ret);

  return ret;
error:
  tk_swap(size, header->size, size_t);
  return ret;
}

inline bool_t tk_raw_darray_can_push(void** p_arr, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, FALSE);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  return (header->size < SIZE_MAX) &&
         (RET_OK == tk_raw_darray_resize(p_arr, header->size + 1, elm_size));
}

inline bool_t tk_raw_darray_can_insert(void** p_arr, size_t idx, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, FALSE);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  return (idx <= header->size) && (header->size < SIZE_MAX) &&
         (RET_OK == tk_raw_darray_resize(p_arr, header->size + 1, elm_size));
}

inline bool_t tk_raw_darray_can_remove(void** p_arr, size_t idx, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, FALSE);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  return (idx < header->size) &&
         (RET_OK == tk_raw_darray_resize(p_arr, header->size - 1, elm_size));
}

#define TK_RAW_DARRAY_T(type) type*

#define TK_RAW_DARRAY_CREATE(type, cap) \
  (TK_RAW_DARRAY_T(type)) tk_raw_darray_create(cap, sizeof(type))

#define TK_RAW_DARRAY_DESTROY(arr) \
  ((RET_OK == tk_raw_darray_destroy((void*)arr)) ? (arr = NULL, RET_OK) : RET_FAIL)

#define TK_RAW_DARRAY_SIZE(arr) (NULL != (arr) ? ((tk_raw_darray_header_t*)(arr) - 1)->size : 0)

#define TK_RAW_DARRAY_RESIZE(arr, size) tk_raw_darray_resize((void**)&(arr), size, sizeof(*(arr)))

#define TK_RAW_DARRAY_PUSH(arr, elm)                        \
  (tk_raw_darray_can_push((void**)&(arr), sizeof(*(arr)))   \
       ? ((arr)[TK_RAW_DARRAY_SIZE(arr) - 1] = elm, RET_OK) \
       : RET_FAIL)

#define TK_RAW_DARRAY_INSERT(arr, idx, elm)                                    \
  ((0 <= idx && tk_raw_darray_can_insert((void**)&(arr), idx, sizeof(*(arr)))) \
       ? (memmove(&(arr)[(idx) + 1], &(arr)[(idx)],                            \
                  (TK_RAW_DARRAY_SIZE(arr) - (idx)) * sizeof(*(arr))),         \
          (arr)[idx] = elm, RET_OK)                                            \
       : RET_FAIL)

#define TK_RAW_DARRAY_REMOVE(arr, idx)                                         \
  ((0 <= idx && tk_raw_darray_can_remove((void**)&(arr), idx, sizeof(*(arr)))) \
       ? (memmove(&(arr)[(idx)], &(arr)[(idx) + 1],                            \
                  (TK_RAW_DARRAY_SIZE(arr) - (idx)) * sizeof(*(arr))),         \
          RET_OK)                                                              \
       : RET_FAIL)

END_C_DECLS

#endif /*TK_RAW_DARRAY_H*/
