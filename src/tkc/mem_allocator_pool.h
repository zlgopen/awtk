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
  mem_pool_t* pool8;
  mem_pool_t* pool16;
  mem_pool_t* pool32;
  mem_pool_t* pool48;
  mem_pool_t* pool64;
} mem_allocator_pool_t;

#define MEM_ALLOCATOR_POOL(allocator) ((mem_allocator_pool_t*)(allocator))

static inline void* mem_allocator_pool_alloc(mem_allocator_t* allocator, uint32_t size,
                                             const char* func, uint32_t line) {
  void* addr = NULL;
  mem_pool_t* pool8 = MEM_ALLOCATOR_POOL(allocator)->pool8;
  mem_pool_t* pool16 = MEM_ALLOCATOR_POOL(allocator)->pool16;
  mem_pool_t* pool32 = MEM_ALLOCATOR_POOL(allocator)->pool32;
  mem_pool_t* pool48 = MEM_ALLOCATOR_POOL(allocator)->pool48;
  mem_pool_t* pool64 = MEM_ALLOCATOR_POOL(allocator)->pool64;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  if (size <= 8) {
    addr = mem_pool_get(pool8);
    if (addr != NULL) {
      return addr;
    }
  } else if (size <= 16) {
    addr = mem_pool_get(pool16);
    if (addr != NULL) {
      return addr;
    }
  } else if (size <= 32) {
    addr = mem_pool_get(pool32);
    if (addr != NULL) {
      return addr;
    }
  } else if (size <= 48) {
    addr = mem_pool_get(pool48);
    if (addr != NULL) {
      return addr;
    }
  } else if (size <= 64) {
    addr = mem_pool_get(pool64);
    if (addr != NULL) {
      return addr;
    }
  }

  addr = mem_allocator_alloc(impl, size, func, line);

  return addr;
}

static inline void* mem_allocator_pool_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                               const char* func, uint32_t line) {
  void* addr = NULL;
  mem_pool_t* pool8 = MEM_ALLOCATOR_POOL(allocator)->pool8;
  mem_pool_t* pool16 = MEM_ALLOCATOR_POOL(allocator)->pool16;
  mem_pool_t* pool32 = MEM_ALLOCATOR_POOL(allocator)->pool32;
  mem_pool_t* pool48 = MEM_ALLOCATOR_POOL(allocator)->pool48;
  mem_pool_t* pool64 = MEM_ALLOCATOR_POOL(allocator)->pool64;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  if (ptr != NULL && mem_pool_get_index(pool8, ptr) >= 0) {
    if (size <= pool8->block_size) {
      return ptr;
    } else {
      addr = mem_allocator_pool_alloc(allocator, size, func, line);
      return_value_if_fail(addr != NULL, NULL);
      memcpy(addr, ptr, pool8->block_size);
      mem_pool_put(pool8, ptr);
      return addr;
    }
  }
  
  if (ptr != NULL && mem_pool_get_index(pool16, ptr) >= 0) {
    if (size <= pool16->block_size) {
      return ptr;
    } else {
      addr = mem_allocator_pool_alloc(allocator, size, func, line);
      return_value_if_fail(addr != NULL, NULL);
      memcpy(addr, ptr, pool16->block_size);
      mem_pool_put(pool16, ptr);
      return addr;
    }
  }


  if (ptr != NULL && mem_pool_get_index(pool32, ptr) >= 0) {
    if (size <= pool32->block_size) {
      return ptr;
    } else {
      addr = mem_allocator_pool_alloc(allocator, size, func, line);
      return_value_if_fail(addr != NULL, NULL);
      memcpy(addr, ptr, pool32->block_size);
      mem_pool_put(pool32, ptr);
      return addr;
    }
  }

  if (ptr != NULL && mem_pool_get_index(pool48, ptr) >= 0) {
    if (size <= pool48->block_size) {
      return ptr;
    } else {
      addr = mem_allocator_pool_alloc(allocator, size, func, line);
      return_value_if_fail(addr != NULL, NULL);
      memcpy(addr, ptr, pool48->block_size);
      mem_pool_put(pool48, ptr);
      return addr;
    }
  }
  
  if (ptr != NULL && mem_pool_get_index(pool64, ptr) >= 0) {
    if (size <= pool64->block_size) {
      return ptr;
    } else {
      addr = mem_allocator_pool_alloc(allocator, size, func, line);
      return_value_if_fail(addr != NULL, NULL);
      memcpy(addr, ptr, pool64->block_size);
      mem_pool_put(pool64, ptr);
      return addr;
    }
  }


  if (ptr != NULL) {
    addr = mem_allocator_realloc(impl, ptr, size, func, line);
  } else {
    addr = mem_allocator_pool_alloc(allocator, size, func, line);
  }

  return addr;
}

static inline void mem_allocator_pool_free(mem_allocator_t* allocator, void* ptr) {
  mem_pool_t* pool8 = MEM_ALLOCATOR_POOL(allocator)->pool8;
  mem_pool_t* pool16 = MEM_ALLOCATOR_POOL(allocator)->pool16;
  mem_pool_t* pool32 = MEM_ALLOCATOR_POOL(allocator)->pool32;
  mem_pool_t* pool48 = MEM_ALLOCATOR_POOL(allocator)->pool48;
  mem_pool_t* pool64 = MEM_ALLOCATOR_POOL(allocator)->pool64;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  if (mem_pool_get_index(pool8, ptr) >= 0) {
    mem_pool_put(pool8, ptr);
    return;
  }

  if (mem_pool_get_index(pool16, ptr) >= 0) {
    mem_pool_put(pool16, ptr);
    return;
  }

  if (mem_pool_get_index(pool32, ptr) >= 0) {
    mem_pool_put(pool32, ptr);
    return;
  }

  if (mem_pool_get_index(pool48, ptr) >= 0) {
    mem_pool_put(pool48, ptr);
    return;
  }

  if (mem_pool_get_index(pool64, ptr) >= 0) {
    mem_pool_put(pool64, ptr);
    return;
  }

  mem_allocator_free(impl, ptr);
}

static inline ret_t mem_allocator_pool_dump(mem_allocator_t* allocator) {
  mem_pool_t* pool8 = MEM_ALLOCATOR_POOL(allocator)->pool8;
  mem_pool_t* pool16 = MEM_ALLOCATOR_POOL(allocator)->pool16;
  mem_pool_t* pool32 = MEM_ALLOCATOR_POOL(allocator)->pool32;
  mem_pool_t* pool48 = MEM_ALLOCATOR_POOL(allocator)->pool48;
  mem_pool_t* pool64 = MEM_ALLOCATOR_POOL(allocator)->pool64;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  uint32_t used = pool8->used + pool16->used + pool32->used + pool48->used + pool64->used;
  uint32_t block_nr = pool8->block_nr + pool16->block_nr + pool32->block_nr + pool48->block_nr + pool64->block_nr;

  mem_allocator_dump(impl);
  log_debug("total: used=%u total=%u\n", used, block_nr);
  log_debug("pool8: used=%u total=%u\n", pool8->used, pool8->block_nr);
  log_debug("pool16: used=%u total=%u\n", pool16->used, pool16->block_nr);
  log_debug("pool32: used=%u total=%u\n", pool32->used, pool32->block_nr);
  log_debug("pool48: used=%u total=%u\n", pool48->used, pool48->block_nr);
  log_debug("pool64: used=%u total=%u\n", pool64->used, pool64->block_nr);

  (void)used;
  (void)block_nr;
  
  return RET_OK;
}

static inline ret_t mem_allocator_pool_destroy(mem_allocator_t* allocator) {
  mem_pool_t* pool8 = MEM_ALLOCATOR_POOL(allocator)->pool8;
  mem_pool_t* pool16 = MEM_ALLOCATOR_POOL(allocator)->pool16;
  mem_pool_t* pool32 = MEM_ALLOCATOR_POOL(allocator)->pool32;
  mem_pool_t* pool48 = MEM_ALLOCATOR_POOL(allocator)->pool48;
  mem_pool_t* pool64 = MEM_ALLOCATOR_POOL(allocator)->pool64;
  mem_allocator_t* impl = MEM_ALLOCATOR_POOL(allocator)->impl;

  mem_allocator_free(impl, pool8);
  mem_allocator_free(impl, pool16);
  mem_allocator_free(impl, pool32);
  mem_allocator_free(impl, pool48);
  mem_allocator_free(impl, pool64);
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

static inline mem_allocator_t* mem_allocator_pool_init(mem_allocator_pool_t* pool,
                                                       mem_allocator_t* impl, uint32_t pool8_nr,
                                                       uint32_t pool16_nr, uint32_t pool32_nr,
                                                       uint32_t pool48_nr, uint32_t pool64_nr) {
  uint32_t size = 0;
  uint8_t* addr = NULL;
  mem_allocator_t* allocator = MEM_ALLOCATOR(pool);
  return_value_if_fail(impl != NULL, NULL);

  memset(pool, 0x00, sizeof(*pool));
  allocator->vt = &s_mem_allocator_pool_vtable;

  pool->impl = impl;
  pool8_nr = tk_max(32, pool8_nr);
  pool16_nr = tk_max(32, pool16_nr);
  pool32_nr = tk_max(32, pool32_nr);
  pool48_nr = tk_max(32, pool48_nr);
  pool64_nr = tk_max(32, pool64_nr);

  size = mem_pool_get_min_size(8, pool8_nr);
  addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  pool->pool8 = mem_pool_init(addr, size, 8, pool8_nr);
  ENSURE(pool->pool8 != NULL);

  size = mem_pool_get_min_size(16, pool16_nr);
  addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  pool->pool16 = mem_pool_init(addr, size, 16, pool16_nr);
  ENSURE(pool->pool16 != NULL);

  size = mem_pool_get_min_size(32, pool32_nr);
  addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  pool->pool32 = mem_pool_init(addr, size, 32, pool32_nr);
  ENSURE(pool->pool32 != NULL);

  size = mem_pool_get_min_size(48, pool48_nr);
  addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  pool->pool48 = mem_pool_init(addr, size, 48, pool48_nr);
  ENSURE(pool->pool48 != NULL);

  size = mem_pool_get_min_size(64, pool64_nr);
  addr = (uint8_t*)mem_allocator_alloc(impl, size, __FUNCTION__, __LINE__);
  pool->pool64 = mem_pool_init(addr, size, 64, pool64_nr);
  ENSURE(pool->pool64 != NULL);

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_POOL_H*/
