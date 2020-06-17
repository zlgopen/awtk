/**
 * File:   mem_allocator_debug.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_debug
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

#ifndef TK_MEM_ALLOCATOR_DEBUG_H
#define TK_MEM_ALLOCATOR_DEBUG_H

#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

/**
 * @class mem_allocator_debug_t 
 * @parent mem_allocator_t 
 * 
 * 对现有的allocator进行包装，记录分配的内存，用于帮助分析内存的使用和泄露。
 * 
 */
typedef struct _mem_allocator_debug_t {
  mem_allocator_t allocator;
  mem_allocator_t* impl;
} mem_allocator_debug_t;

#define MEM_ALLOCATOR_DEBUG(allocator) ((mem_allocator_debug_t*)(allocator))

typedef struct _mem_entry_t {
  void* addr;
  const char* func;
  uint32_t line;
  uint32_t size;
} mem_entry_t;

static mem_entry_t s_mem_log[10 * 1024];
#define MEM_ALLOCATOR_STD(allocator) ((mem_allocator_std_t*)(allocator))

static inline ret_t mem_entry_add(void* addr, uint32_t size, const char* func, uint32_t line) {
  uint32_t i = 0;
  uint32_t n = ARRAY_SIZE(s_mem_log);

  for (i = 0; i < n; i++) {
    mem_entry_t* iter = s_mem_log + i;
    if (iter->addr == NULL) {
      iter->addr = addr;
      iter->size = size;
      iter->func = func;
      iter->line = line;

      return RET_OK;
    }
  }

  return RET_FAIL;
}

static inline ret_t mem_entry_remove(void* addr) {
  uint32_t i = 0;
  uint32_t n = ARRAY_SIZE(s_mem_log);

  for (i = 0; i < n; i++) {
    mem_entry_t* iter = s_mem_log + i;
    if (iter->addr == addr) {
      iter->addr = NULL;
      iter->func = NULL;
      iter->size = 0;
      iter->line = 0;

      return RET_OK;
    }
  }

  return RET_FAIL;
}

static inline void mem_entry_dump(void) {
  uint32_t i = 0;
  uint32_t leq8 = 0;
  uint32_t leq16 = 0;
  uint32_t leq32 = 0;
  uint32_t leq48 = 0;
  uint32_t leq64 = 0;
  uint32_t geq1024 = 0;
  uint32_t geq10240 = 0;
  uint32_t geq102400 = 0;
  uint32_t used_block = 0;
  uint32_t used_size = 0;

  uint32_t n = ARRAY_SIZE(s_mem_log);

  for (i = 0; i < n; i++) {
    mem_entry_t* iter = s_mem_log + i;
    if (iter->addr != NULL) {
      used_block++;
      used_size += iter->size;

      if (iter->size <= 8) {
        leq8++;
      } else if (iter->size <= 16) {
        leq16++;
      } else if (iter->size <= 32) {
        leq32++;
      } else if (iter->size <= 48) {
        leq48++;
      } else if (iter->size <= 64) {
        leq64++;
      } else if (iter->size >= 1024) {
        geq1024++;
      } else if (iter->size >= 10240) {
        geq10240++;
      } else if (iter->size >= 102400) {
        geq102400++;
      }

      log_debug("[%d] %p %u %s:%u\n", i, iter->addr, iter->size, iter->func, iter->line);
    }
  }

  log_info(
      "mem_info: used_block=%u used_size=%u <=8=%u <=16=%u <=32=%u <=48=%u <=64=%u >=1024=%u "
      ">=10240=%u >=102400=%u\n",
      used_block, used_size, leq8, leq16, leq32, leq48, leq64, geq1024, geq10240, geq102400);

  return;
}

static inline void* mem_allocator_debug_alloc(mem_allocator_t* allocator, uint32_t size,
                                              const char* func, uint32_t line) {
  void* addr = NULL;
  mem_allocator_t* impl = MEM_ALLOCATOR_DEBUG(allocator)->impl;

  addr = mem_allocator_alloc(impl, size, func, line);
  if (addr != NULL) {
    mem_entry_add(addr, size, func, line);
  }

  return addr;
}

static inline void* mem_allocator_debug_realloc(mem_allocator_t* allocator, void* ptr,
                                                uint32_t size, const char* func, uint32_t line) {
  void* addr = NULL;
  mem_allocator_t* impl = MEM_ALLOCATOR_DEBUG(allocator)->impl;

  addr = mem_allocator_realloc(impl, ptr, size, func, line);
  if (addr != NULL) {
    if (ptr != NULL) {
      mem_entry_remove(ptr);
    }
    mem_entry_add(addr, size, func, line);
  }

  return addr;
}

static inline void mem_allocator_debug_free(mem_allocator_t* allocator, void* ptr) {
  mem_allocator_t* impl = MEM_ALLOCATOR_DEBUG(allocator)->impl;
  if (ptr != NULL) {
    mem_entry_remove(ptr);
    mem_allocator_free(impl, ptr);
  }
}

static inline ret_t mem_allocator_debug_dump(mem_allocator_t* allocator) {
  mem_allocator_t* impl = MEM_ALLOCATOR_DEBUG(allocator)->impl;

  mem_entry_dump();
  mem_allocator_dump(impl);

  return RET_OK;
}

static inline ret_t mem_allocator_debug_destroy(mem_allocator_t* allocator) {
  mem_allocator_t* impl = MEM_ALLOCATOR_DEBUG(allocator)->impl;
  mem_allocator_destroy(impl);
  allocator->vt = NULL;

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_debug_vtable = {
    .alloc = mem_allocator_debug_alloc,
    .realloc = mem_allocator_debug_realloc,
    .free = mem_allocator_debug_free,
    .dump = mem_allocator_debug_dump,
    .destroy = mem_allocator_debug_destroy};

static inline mem_allocator_t* mem_allocator_debug_init(mem_allocator_debug_t* debug,
                                                        mem_allocator_t* impl) {
  mem_allocator_t* allocator = MEM_ALLOCATOR(debug);
  return_value_if_fail(impl != NULL && debug != NULL, NULL);

  memset(debug, 0x00, sizeof(*debug));

  debug->impl = impl;
  allocator->vt = &s_mem_allocator_debug_vtable;

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_DEBUG_H*/
