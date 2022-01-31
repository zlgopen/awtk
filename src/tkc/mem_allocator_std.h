/**
 * File:   mem_allocator_std.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_std
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef WITH_SDL
#ifdef MACOS
#include <malloc/malloc.h>
#define msize(ptr) malloc_size(ptr)
#elif defined(WIN32) && !defined(MINGW)
#define msize(ptr) _msize(ptr)
#elif defined(LINUX)
#include <malloc.h>
#define msize(ptr) malloc_usable_size(ptr)
#elif !defined(msize)
#define msize(ptr) 0
#endif
#else
#define msize(ptr) 0
#endif

BEGIN_C_DECLS

/**
 * @class mem_allocator_std_t 
 * @parent mem_allocator_t 
 * @annotation ["fake"]
 * 将标准的C的内存分配函数包装成内存分配器。 
 * 
 */
typedef struct _mem_allocator_std_t {
  mem_allocator_t allocator;
  uint32_t max_used_nr;
  uint32_t max_used_size;
  uint32_t used_nr;
  uint32_t used_size;
} mem_allocator_std_t;

static ret_t mem_allocator_std_update_status(mem_allocator_std_t* std_allocator) {
  if (std_allocator->used_size > std_allocator->max_used_size) {
    std_allocator->max_used_size = std_allocator->used_size;
  }

  if (std_allocator->used_nr > std_allocator->max_used_nr) {
    std_allocator->max_used_nr = std_allocator->used_nr;
  }

  return RET_OK;
}

static inline void* mem_allocator_std_alloc(mem_allocator_t* allocator, uint32_t size,
                                            const char* func, uint32_t line) {
  void* ptr = NULL;
  mem_allocator_std_t* std_allocator = (mem_allocator_std_t*)allocator;

  size = TK_ROUND_TO_MACH(size);
  ptr = malloc(size);
  if (ptr != NULL) {
    std_allocator->used_nr++;
    std_allocator->used_size += msize(ptr);
    mem_allocator_std_update_status(std_allocator);
  }

  return ptr;
}

static inline void* mem_allocator_std_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                              const char* func, uint32_t line) {
  void* new_ptr = NULL;
  size_t old_size = ptr != NULL ? msize(ptr) : 0;
  mem_allocator_std_t* std_allocator = (mem_allocator_std_t*)allocator;

  size = TK_ROUND_TO_MACH(size);
  new_ptr = realloc(ptr, size);
  if (new_ptr != NULL) {
    size = msize(new_ptr);
    if (ptr != NULL) {
      std_allocator->used_size += size - old_size;
    } else {
      std_allocator->used_nr++;
      std_allocator->used_size += size;
    }
    mem_allocator_std_update_status(std_allocator);
  }

  return new_ptr;
}

static inline void mem_allocator_std_free(mem_allocator_t* allocator, void* ptr) {
  mem_allocator_std_t* std_allocator = (mem_allocator_std_t*)allocator;

  if (ptr != NULL) {
    size_t size = msize(ptr);
    free(ptr);
    std_allocator->used_nr--;
    std_allocator->used_size -= size;
  }
}

static inline ret_t mem_allocator_std_dump(mem_allocator_t* allocator) {
  mem_allocator_std_t* std_allocator = (mem_allocator_std_t*)allocator;
  log_debug("used_nr=%u used_size=%u max_used_nr=%u max_used_size=%u\n", std_allocator->used_nr,
            std_allocator->used_size, std_allocator->max_used_nr, std_allocator->max_used_size);
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

static inline mem_allocator_t* mem_allocator_std_init(mem_allocator_std_t* allocator) {
  return_value_if_fail(allocator != NULL, NULL);
  memset(allocator, 0x00, sizeof(mem_allocator_std_t));
  allocator->allocator.vt = &s_mem_allocator_std_vtable;

  return (mem_allocator_t*)allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_STD_H*/
