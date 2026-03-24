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

typedef struct _tk_raw_darray_header_t {
  size_t size;
  size_t capacity;
} TK_ALIGN(tk_raw_darray_header_t, 16);

inline static bool_t tk_raw_darray_overflow(size_t cap, size_t elm_size) {
  return cap > 0 && ((elm_size > SIZE_MAX / cap) ||
                     ((cap * elm_size) > (SIZE_MAX - sizeof(tk_raw_darray_header_t))));
}

inline static void* tk_raw_darray_create(size_t cap, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(!tk_raw_darray_overflow(cap, elm_size), NULL);

  header = (tk_raw_darray_header_t*)TKMEM_ALLOC(sizeof(tk_raw_darray_header_t) + cap * elm_size);
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

inline static ret_t tk_raw_darray_set_capacity(void** p_arr, size_t cap, size_t elm_size) {
  tk_raw_darray_header_t *header = NULL, *new_header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);
  return_value_if_fail(!tk_raw_darray_overflow(cap, elm_size), RET_FAIL);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  new_header = (tk_raw_darray_header_t*)TKMEM_REALLOC(
      header, sizeof(tk_raw_darray_header_t) + cap * elm_size);

  if (NULL != new_header) {
    new_header->capacity = cap;
    if (cap < new_header->size) {
      new_header->size = cap;
    }
    *p_arr = (void*)(new_header + 1);
    return RET_OK;
  } else {
    return RET_OOM;
  }
}

inline static size_t tk_raw_darray_extend_capacity(size_t cap) {
  /* (cap >> 1) + cap + 1 */
  size_t sum = 0;
  size_t half_cap = cap >> 1;
  if (cap > SIZE_MAX - half_cap) {
    return SIZE_MAX;
  }
  sum = cap + half_cap;
  if (sum > SIZE_MAX - 1) {
    return SIZE_MAX;
  }
  return sum + 1;
}

inline static ret_t tk_raw_darray_extend(void** p_arr, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;
  if (header->capacity < header->size) {
    size_t new_capacity = tk_raw_darray_extend_capacity(header->capacity);
    new_capacity = tk_max(new_capacity, header->size);
    return tk_raw_darray_set_capacity(p_arr, new_capacity, elm_size);
  }

  return RET_OK;
}

inline static ret_t tk_raw_darray_shrink(void** p_arr, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;
  if (header->capacity > header->size) {
    return tk_raw_darray_set_capacity(p_arr, header->size, elm_size);
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

inline static ret_t tk_raw_darray_push_prepare(void** p_arr, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  if (header->size < SIZE_MAX) {
    return tk_raw_darray_resize(p_arr, header->size + 1, elm_size);
  } else {
    return RET_FAIL;
  }
}

inline static ret_t tk_raw_darray_insert_prepare(void** p_arr, size_t idx, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  if (idx <= header->size && header->size < SIZE_MAX) {
    return tk_raw_darray_resize(p_arr, header->size + 1, elm_size);
  } else {
    return RET_FAIL;
  }
}

inline static ret_t tk_raw_darray_remove_prepare(void** p_arr, size_t idx, size_t elm_size) {
  tk_raw_darray_header_t* header = NULL;
  return_value_if_fail(NULL != p_arr && NULL != *p_arr, RET_BAD_PARAMS);

  header = (tk_raw_darray_header_t*)(*p_arr) - 1;

  if (idx < header->size) {
    return tk_raw_darray_resize(p_arr, header->size - 1, elm_size);
  } else {
    return RET_FAIL;
  }
}

/**
 * @class TK_RAW_DARRAY_T
 * @annotation ["fake"]
 * @export none
 * 原生动态数组。
 */
#define TK_RAW_DARRAY_T(type) type*

/**
 * @method TK_RAW_DARRAY_CREATE
 * @annotation ["static"]
 * @export none
 * 创建原生动态数组。
 * @param {T} type 类型。
 * @param {size_t} cap 容量。
 *
 * @return {T*} 返回原生动态数组。
 */
#define TK_RAW_DARRAY_CREATE(type, cap) \
  (0 <= (cap) ? (TK_RAW_DARRAY_T(type))tk_raw_darray_create(cap, sizeof(type)) : NULL)

/**
 * @method TK_RAW_DARRAY_DESTROY
 * @annotation ["static"]
 * @export none
 * 销毁原生动态数组。
 * @param {T*} arr 原生动态数组。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_DESTROY(arr) \
  ((RET_OK == tk_raw_darray_destroy((void*)arr)) ? ((arr) = NULL, RET_OK) : RET_FAIL)

/**
 * @method TK_RAW_DARRAY_CAPACITY
 * @annotation ["static"]
 * @export none
 * 获取原生动态数组的容量。
 * @param {T*} arr 原生动态数组。
 *
 * @return {size_t} 返回原生动态数组的容量。
 */
#define TK_RAW_DARRAY_CAPACITY(arr) \
  (NULL != (arr) ? ((tk_raw_darray_header_t*)(arr) - 1)->capacity : 0)

/**
 * @method TK_RAW_DARRAY_SIZE
 * @annotation ["static"]
 * @export none
 * 获取原生动态数组的大小。
 * @param {T*} arr 原生动态数组。
 *
 * @return {size_t} 返回原生动态数组的大小。
 */
#define TK_RAW_DARRAY_SIZE(arr) (NULL != (arr) ? ((tk_raw_darray_header_t*)(arr) - 1)->size : 0)

/**
 * @method TK_RAW_DARRAY_RESIZE
 * @annotation ["static"]
 * @export none
 * 调整原生动态数组的大小。
 * @param {T*} arr 原生动态数组。
 * @param {size_t} size 大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_RESIZE(arr, size) \
  (0 <= (size) ? tk_raw_darray_resize((void**)&(arr), size, sizeof(*(arr))) : RET_FAIL)

/**
 * @method TK_RAW_DARRAY_SHRINK
 * @annotation ["static"]
 * @export none
 * 缩小容量，释放未使用空间。
 * @param {T*} arr 原生动态数组。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_SHRINK(arr) tk_raw_darray_shrink((void**)&(arr), sizeof(*(arr)))

/**
 * @method TK_RAW_DARRAY_PUSH
 * @annotation ["static"]
 * @export none
 * 向原生动态数组中添加元素。
 * @param {T*} arr 原生动态数组。
 * @param {T} elm 元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_PUSH(arr, elm)                                    \
  (RET_OK == tk_raw_darray_push_prepare((void**)&(arr), sizeof(*(arr))) \
       ? ((arr)[TK_RAW_DARRAY_SIZE(arr) - 1] = (elm), RET_OK)           \
       : RET_FAIL)

/**
 * @method TK_RAW_DARRAY_INSERT
 * @annotation ["static"]
 * @export none
 * 向原生动态数组中插入元素。
 * @param {T*} arr 原生动态数组。
 * @param {size_t} idx 索引。
 * @param {T} elm 元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_INSERT(arr, idx, elm)                                                    \
  ((0 <= (idx) && RET_OK == tk_raw_darray_insert_prepare((void**)&(arr), idx, sizeof(*(arr)))) \
       ? (memmove(&(arr)[(idx) + 1], &(arr)[(idx)],                                            \
                  (TK_RAW_DARRAY_SIZE(arr) - (idx) - 1) * sizeof(*(arr))),                     \
          (arr)[idx] = (elm), RET_OK)                                                          \
       : RET_FAIL)

/**
 * @method TK_RAW_DARRAY_REMOVE
 * @annotation ["static"]
 * @export none
 * 从原生动态数组中删除元素。
 * @param {T*} arr 原生动态数组。
 * @param {size_t} idx 索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
#define TK_RAW_DARRAY_REMOVE(arr, idx)                                                         \
  ((0 <= (idx) && RET_OK == tk_raw_darray_remove_prepare((void**)&(arr), idx, sizeof(*(arr)))) \
       ? (memmove(&(arr)[(idx)], &(arr)[(idx) + 1],                                            \
                  (TK_RAW_DARRAY_SIZE(arr) - (idx)) * sizeof(*(arr))),                         \
          RET_OK)                                                                              \
       : RET_FAIL)

END_C_DECLS

#endif /*TK_RAW_DARRAY_H*/
