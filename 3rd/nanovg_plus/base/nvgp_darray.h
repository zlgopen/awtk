/**
 * File:   nvgp_darray.h
 * Author: AWTK Develop Team
 * Brief:  nanovg darry.
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-10-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */


#ifndef __NANOVG_PLUS_DARRAY_H__
#define __NANOVG_PLUS_DARRAY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "nanovg_plus_type.h"

typedef struct _nvgp_darray_t {
  uint32_t size;
  uint32_t capacity;
  uint32_t data_type_size;
  void** elms;
} nvgp_darray_t;

typedef void (*nvgp_destroy_t)(void* data, void* ctx);

void nvgp_darray_destroy(nvgp_darray_t* darray);
nvgp_darray_t* nvgp_darray_create(uint32_t capacity, uint32_t data_type_size);

void nvgp_darray_deinit(nvgp_darray_t* darray);
nvgp_darray_t* nvgp_darray_init(nvgp_darray_t* darray, uint32_t capacity, uint32_t data_type_size);

void* nvgp_darray_get_empty_data_by_tail(nvgp_darray_t* darray);
void* nvgp_darray_get_empty_data_ptr_by_tail(nvgp_darray_t* darray);
nvgp_error_t nvgp_darray_push(nvgp_darray_t* darray, void* data);
nvgp_error_t nvgp_darray_pop(nvgp_darray_t* darray);
nvgp_error_t nvgp_darray_remove(nvgp_darray_t* darray, uint32_t index, nvgp_destroy_t destroy_func, void* ctx);
nvgp_error_t nvgp_darray_clear(nvgp_darray_t* darray);
nvgp_error_t nvgp_darray_clear_by_destroy_function(nvgp_darray_t* darray, nvgp_destroy_t destroy_func, void* ctx);
nvgp_error_t nvgp_darray_remove_empty_cache(nvgp_darray_t* darray);
nvgp_error_t nvgp_darray_reverse(nvgp_darray_t* darray);

#define nvgp_darray_get_empty_by_tail(darray, type) ((type*)(nvgp_darray_get_empty_data_by_tail(darray)))
#define nvgp_darray_get_empty_ptr_by_tail(darray, type) ((type*)(nvgp_darray_get_empty_data_ptr_by_tail(darray)))

#define nvgp_darray_get_ptr(darray, index, type) (((darray) == NULL || (darray)->size <= (index)) ? NULL : (type*)((darray)->elms[index]))

/* 只能在指针内存大小等于 type 的内存大小才可以使用 */
#define nvgp_darray_get(darray, index, type) (((darray) == NULL || (darray)->size <= (index)) ? 0 : (assert(sizeof(void*) == sizeof(type*)), *(type*)(&(darray)->elms[index]))) 

/* 只能在指针内存大小等于 type 的内存大小才可以使用 */
#define nvgp_darray_memcpy(darray, type, dst, n) {                                     \
  uint32_t i = 0;                                                                      \
  assert(sizeof(void*) == sizeof(type*));                                              \
  if ((darray) == NULL) {                                                              \
    return;                                                                            \
  }                                                                                    \
  for (i = 0; i < (n); i++) {                                                          \
    type* d = (type*)nvgp_darray_get_empty_data_ptr_by_tail(darray);                   \
    NVGP_MEMCPY(d, (dst) + i, sizeof(void*));                                          \
  }                                                                                    \
}                                                                                      \

#ifdef __cplusplus
}
#endif
#endif
