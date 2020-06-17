/**
 * File:   mem_allocator_std.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_std
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

#ifndef TK_MEM_ALLOCATOR_STD_H
#define TK_MEM_ALLOCATOR_STD_H

#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

/**
 * @class mem_allocator_std_t 
 * @parent mem_allocator_t 
 * @annotation ["fake"]
 * 将标准的C的内存分配函数包装成内存分配器。 
 * 
 */

static inline void* mem_allocator_std_alloc(mem_allocator_t* allocator, uint32_t size,
                                            const char* func, uint32_t line) {
  return malloc(size);
}

static inline void* mem_allocator_std_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                              const char* func, uint32_t line) {
  return realloc(ptr, size);
}

static inline void mem_allocator_std_free(mem_allocator_t* allocator, void* ptr) {
  free(ptr);
}

static inline ret_t mem_allocator_std_dump(mem_allocator_t* allocator) {
  return RET_OK;
}

static inline ret_t mem_allocator_std_destroy(mem_allocator_t* allocator) {
  allocator->vt = NULL;
  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_std_vtable = {
    .alloc = mem_allocator_std_alloc,
    .realloc = mem_allocator_std_realloc,
    .free = mem_allocator_std_free,
    .dump = mem_allocator_std_dump,
    .destroy = mem_allocator_std_destroy};

static inline mem_allocator_t* mem_allocator_std_init(mem_allocator_t* allocator) {
  return_value_if_fail(allocator != NULL, NULL);
  allocator->vt = &s_mem_allocator_std_vtable;

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_STD_H*/
