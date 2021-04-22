/**
 * File:   mem_allocator_composite.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_composite
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MEM_ALLOCATOR_COMPOSITE_H
#define TK_MEM_ALLOCATOR_COMPOSITE_H

#include "tkc/mutex.h"
#include "tkc/mem_allocator_simple.h"

BEGIN_C_DECLS

/*最多支持的内存块数目*/
#ifndef TK_MAX_MEM_BLOCK_NR
#define TK_MAX_MEM_BLOCK_NR 1
#endif /*TK_MAX_MEM_BLOCK_NR*/

/**
 * @class mem_allocator_composite_t 
 * @parent mem_allocator_t 
 * 
 * 支持多个不连续的内存块。
 * 
 */
typedef struct _mem_allocator_composite_t {
  mem_allocator_t allocator;
  mem_allocator_simple_t allocators[TK_MAX_MEM_BLOCK_NR];
} mem_allocator_composite_t;

#define MEM_ALLOCATOR_COMPOSITE(allocator) ((mem_allocator_composite_t*)(allocator))

static inline void* mem_allocator_composite_alloc(mem_allocator_t* allocator, uint32_t size,
                                                  const char* func, uint32_t line) {
  uint32_t i = 0;
  void* addr = NULL;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_is_valid(iter)) {
      addr = mem_allocator_alloc(iter, size, func, line);
      if (addr != NULL) {
        return addr;
      }
    }
  }

  return NULL;
}

static inline bool_t mem_allocator_composite_is_valid_addr(mem_allocator_t* allocator, void* ptr) {
  uint32_t i = 0;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_contains(iter, ptr)) {
      return TRUE;
    }
  }

  return FALSE;
}

static inline void mem_allocator_composite_free(mem_allocator_t* allocator, void* ptr) {
  uint32_t i = 0;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_contains(iter, ptr)) {
      mem_allocator_free(iter, ptr);
      break;
    }
  }

  return;
}

static inline void* mem_allocator_composite_realloc(mem_allocator_t* allocator, void* ptr,
                                                    uint32_t size, const char* func,
                                                    uint32_t line) {
  uint32_t i = 0;
  void* addr = NULL;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  if (ptr == NULL) {
    return mem_allocator_composite_alloc(allocator, size, func, line);
  }

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_contains(iter, ptr)) {
      addr = mem_allocator_realloc(iter, ptr, size, func, line);
      if (addr == NULL) {
        uint32_t old_size = mem_allocator_simple_get_mem_size(iter, ptr);
        addr = mem_allocator_composite_alloc(allocator, size, func, line);
        if (addr != NULL) {
          uint32_t data_size = tk_min(old_size, size);
          memcpy(addr, ptr, data_size);
          mem_allocator_simple_free(iter, ptr);
        }
      }
      break;
    }
  }

  return addr;
}

static inline ret_t mem_allocator_composite_dump(mem_allocator_t* allocator) {
  uint32_t i = 0;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_is_valid(iter)) {
      mem_allocator_dump(iter);
    }
  }

  return RET_OK;
}

static inline ret_t mem_allocator_composite_destroy(mem_allocator_t* allocator) {
  uint32_t i = 0;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (mem_allocator_simple_is_valid(iter)) {
      mem_allocator_destroy(iter);
    }
  }
  allocator->vt = NULL;

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_composite_vtable = {
    .alloc = mem_allocator_composite_alloc,
    .realloc = mem_allocator_composite_realloc,
    .free = mem_allocator_composite_free,
    .dump = mem_allocator_composite_dump,
    .destroy = mem_allocator_composite_destroy};

static inline ret_t mem_allocator_composite_add_mem(mem_allocator_t* allocator, char* addr,
                                                    uint32_t size) {
  uint32_t i = 0;
  mem_allocator_composite_t* impl = MEM_ALLOCATOR_COMPOSITE(allocator);

  for (i = 0; i < ARRAY_SIZE(impl->allocators); i++) {
    mem_allocator_t* iter = MEM_ALLOCATOR(impl->allocators + i);

    if (!mem_allocator_simple_is_valid(iter)) {
      mem_allocator_simple_init(MEM_ALLOCATOR_SIMPLE(iter), addr, size);
      log_debug("add mem block %p %u\n", addr, size);
      return RET_OK;
    }
  }

  log_warn("no space to add mem, please increase TK_MAX_MEM_BLOCK_NR\n");

  return RET_BAD_PARAMS;
}

static inline mem_allocator_t* mem_allocator_composite_init_va(mem_allocator_composite_t* composite,
                                                               void* buffer, uint32_t size,
                                                               va_list va) {
  char* p = NULL;
  mem_allocator_t* allocator = MEM_ALLOCATOR(composite);
  return_value_if_fail(composite != NULL, NULL);

  memset(composite, 0x00, sizeof(*composite));
  allocator->vt = &s_mem_allocator_composite_vtable;

  ENSURE(mem_allocator_composite_add_mem(allocator, buffer, size) == RET_OK);
  do {
    p = va_arg(va, char*);
    if (p != NULL) {
      uint32_t s = va_arg(va, uint32_t);
      ENSURE(mem_allocator_composite_add_mem(allocator, p, s) == RET_OK);
    }
  } while (p != NULL);

  return allocator;
}

static inline mem_allocator_t* mem_allocator_composite_init(mem_allocator_composite_t* composite,
                                                            void* buffer, uint32_t size, ...) {
  va_list va;
  mem_allocator_t* ret = NULL;

  va_start(va, size);
  ret = mem_allocator_composite_init_va(composite, buffer, size, va);
  va_end(va);

  return ret;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_COMPOSITE_H*/
