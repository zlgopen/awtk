/**
 * File:   mem_allocator.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MEM_ALLOCATOR_H
#define TK_MEM_ALLOCATOR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _mem_allocator_t;
typedef struct _mem_allocator_t mem_allocator_t;

typedef void* (*mem_allocator_alloc_t)(mem_allocator_t* allocator, uint32_t size, const char* func,
                                       uint32_t line);
typedef void* (*mem_allocator_realloc_t)(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                         const char* func, uint32_t line);
typedef void (*mem_allocator_free_t)(mem_allocator_t* allocator, void* ptr);
typedef ret_t (*mem_allocator_dump_t)(mem_allocator_t* allocator);
typedef ret_t (*mem_allocator_destroy_t)(mem_allocator_t* allocator);

typedef struct _mem_allocator_vtable_t {
  mem_allocator_alloc_t alloc;
  mem_allocator_realloc_t realloc;
  mem_allocator_free_t free;
  mem_allocator_dump_t dump;
  mem_allocator_destroy_t destroy;
} mem_allocator_vtable_t;

/**
 * @class mem_allocator_t
 * 内存分配接口。
 *
 */
struct _mem_allocator_t {
  const mem_allocator_vtable_t* vt;
};

/**
 * @method mem_allocator_alloc
 * @export none
 * 分配指定大小的内存。
 *
 * @param {mem_allocator_t*} allocator allocator对象。
 * @param {uint32_t} size 内存的大小。
 * @param {const char*}  分配内存的函数(用于调试)。
 * @param {uint32_t} line 分配内存的行数(用于调试)。
 *
 * @return {void*} 成功返回内存块的地址，失败返回NULL。
 */
static inline void* mem_allocator_alloc(mem_allocator_t* allocator, uint32_t size, const char* func,
                                        uint32_t line) {
  return_value_if_fail(allocator != NULL && allocator->vt != NULL && allocator->vt->alloc != NULL,
                       NULL);

  return allocator->vt->alloc(allocator, size, func, line);
}

/**
 * @method mem_allocator_realloc
 * @export none
 * 重新分配指定大小的内存。
 *
 * @param {mem_allocator_t*} allocator allocator对象。
 * @param {void*} ptr 原来内存的地址。
 * @param {uint32_t} size 内存的大小。
 * @param {const char*} 分配内存的函数(用于调试)。
 * @param {uint32_t} line 分配内存的行数(用于调试)。
 *
 * @return {void*} 成功返回内存块的地址，失败返回NULL。
 */
static inline void* mem_allocator_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                          const char* func, uint32_t line) {
  return_value_if_fail(allocator != NULL && allocator->vt != NULL && allocator->vt->realloc != NULL,
                       NULL);

  return allocator->vt->realloc(allocator, ptr, size, func, line);
}

/**
 * @method mem_allocator_free
 * @export none
 * 释放内存。
 *
 * @param {mem_allocator_t*} allocator allocator对象。
 * @param {void*} ptr 内存的地址。
 *
 * @return {void*} 成功返回内存块的地址，失败返回NULL。
 */
static inline void mem_allocator_free(mem_allocator_t* allocator, void* ptr) {
  return_if_fail(allocator != NULL && allocator->vt != NULL && allocator->vt->free != NULL);

  allocator->vt->free(allocator, ptr);
}

/**
 * @method mem_allocator_dump
 * @export none
 * 显示内存信息，用于调试。
 *
 * @param {mem_allocator_t*} allocator allocator对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t mem_allocator_dump(mem_allocator_t* allocator) {
  return_value_if_fail(allocator != NULL && allocator->vt != NULL && allocator->vt->dump != NULL,
                       RET_BAD_PARAMS);

  return allocator->vt->dump(allocator);
}

/**
 * @method mem_allocator_destroy
 * @export none
 * 销毁内存分配器。
 *
 * @param {mem_allocator_t*} allocator allocator对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t mem_allocator_destroy(mem_allocator_t* allocator) {
  return_value_if_fail(allocator != NULL && allocator->vt != NULL && allocator->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return allocator->vt->destroy(allocator);
}

#define MEM_ALLOCATOR(allocator) ((mem_allocator_t*)(allocator))

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_H*/
