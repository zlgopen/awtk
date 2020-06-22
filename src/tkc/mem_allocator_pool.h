/**
 * File:   mem_allocator_pool.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_pool
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

#ifndef TK_MEM_ALLOCATOR_POOL_H
#define TK_MEM_ALLOCATOR_POOL_H

#include "tkc/mem_pool.h"
#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

#define TK_MEM_POOLS_NR 5
/**
 * @class mem_allocator_pool_t 
 * @parent mem_allocator_t 
 * 
 * 对现有的allocator进行包装，预先分配一部分内存，用于分配小块内存，可以避免内存碎片。
 * 
 */

typedef struct _mem_allocator_pool_t {
  mem_allocator_t allocator;

  mem_allocator_t* impl;
  mem_pool_t* pools[TK_MEM_POOLS_NR];
} mem_allocator_pool_t;

#define MEM_ALLOCATOR_POOL(allocator) ((mem_allocator_pool_t*)(allocator))

static inline void* mem_allocator_pool_alloc(mem_allocator_t* allocator, uint32_t size,
                                             const char* func, uint32_t line) {
  uint32_t i = 0;
  void* addr = NULL;
  mem_pool_t** pools = MEM_ALLOCATOR_POOL(allocator)->pools;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    mem_pool_t* p = pools[i];
    if (mem_pool_match_size(p, size)) {
      addr = mem_pool_get(p);
      if (addr != NULL) {
        return addr;
      } else {
        break;
      }
    }
  }

  addr = mem_allocator_alloc(impl, size, func, line);

  return addr;
}

static inline void* mem_allocator_pool_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                               const char* func, uint32_t line) {
  uint32_t i = 0;
  void* addr = NULL;
  mem_pool_t** pools = MEM_ALLOCATOR_POOL(allocator)->pools;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  if (ptr == NULL) {
    return mem_allocator_pool_alloc(allocator, size, func, line);
  }

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    mem_pool_t* p = pools[i];
    if (mem_pool_get_index(p, ptr) >= 0) {
      if (mem_pool_match_size(p, size)) {
        return ptr;
      } else {
        addr = mem_allocator_pool_alloc(allocator, size, func, line);
        return_value_if_fail(addr != NULL, NULL);
        memcpy(addr, ptr, p->block_size);
        mem_pool_put(p, ptr);
        return addr;
      }
    }
  }

  addr = mem_allocator_realloc(impl, ptr, size, func, line);

  return addr;
}

static inline void mem_allocator_pool_free(mem_allocator_t* allocator, void* ptr) {
  uint32_t i = 0;
  mem_pool_t** pools = MEM_ALLOCATOR_POOL(allocator)->pools;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  if (ptr == NULL) {
    return;
  }

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    mem_pool_t* p = pools[i];
    if (mem_pool_get_index(p, ptr) >= 0) {
      mem_pool_put(p, ptr);
      return;
    }
  }

  mem_allocator_free(impl, ptr);
}

static inline ret_t mem_allocator_pool_dump(mem_allocator_t* allocator) {
  uint32_t i = 0;
  uint32_t used = 0;
  mem_pool_t** pools = MEM_ALLOCATOR_POOL(allocator)->pools;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  mem_allocator_dump(impl);

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    mem_pool_t* p = pools[i];
    used += p->used;
    log_debug("%u: block_size=%u block_nr=%u used=%u\n", i, p->block_size, p->block_nr, p->used);
  }
  log_debug("total_used=%u\n", used);

  return RET_OK;
}

static inline ret_t mem_allocator_pool_destroy(mem_allocator_t* allocator) {
  uint32_t i = 0;
  mem_pool_t** pools = MEM_ALLOCATOR_POOL(allocator)->pools;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    mem_pool_t* p = pools[i];
    mem_allocator_free(impl, p);
    pools[i] = NULL;
  }

  mem_allocator_destroy(impl);
  allocator->vt = NULL;

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_pool_vtable = {
    .alloc = mem_allocator_pool_alloc,
    .realloc = mem_allocator_pool_realloc,
    .free = mem_allocator_pool_free,
    .dump = mem_allocator_pool_dump,
    .destroy = mem_allocator_pool_destroy};

static mem_pool_t* mem_pool_create(mem_allocator_t* impl, uint32_t block_size, uint32_t block_nr) {
  uint32_t size = mem_pool_get_min_size(block_size, block_nr);
  uint8_t* addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  return_value_if_fail(addr != NULL, NULL);

  return mem_pool_init(addr, size, block_size, block_nr);
}

static inline mem_allocator_t* mem_allocator_pool_init(mem_allocator_pool_t* pool,
                                                       mem_allocator_t* impl, uint32_t pool8_nr,
                                                       uint32_t pool16_nr, uint32_t pool32_nr,
                                                       uint32_t pool48_nr, uint32_t pool64_nr) {
  uint32_t i = 0;
  mem_allocator_t* allocator = MEM_ALLOCATOR(pool);
  uint32_t blocks_size[TK_MEM_POOLS_NR] = {8, 16, 32, 48, 64};
  uint32_t blocks_nr[TK_MEM_POOLS_NR] = {pool8_nr, pool16_nr, pool32_nr, pool48_nr, pool64_nr};

  return_value_if_fail(pool != NULL && impl != NULL, NULL);

  memset(pool, 0x00, sizeof(*pool));
  allocator->vt = &s_mem_allocator_pool_vtable;
  pool->impl = impl;

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    pool->pools[i] = mem_pool_create(impl, blocks_size[i], tk_max(32, blocks_nr[i]));
  }

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_POOL_H*/
