/**
 * File:   mem_pool.h
 * Author: AWTK Develop Team
 * Brief:  mem_pool
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

#ifndef TK_MEM_POOL_H
#define TK_MEM_POOL_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _mem_pool_t {
  uint32_t block_size : 8;
  uint32_t bits_size : 8;
  uint32_t block_nr : 16;
  uint32_t min_block_size : 16;
  uint32_t used : 16;

  uint32_t* bits;
  uint8_t* start;
} mem_pool_t;

#define TK_SET_BIT(v, n) ((v) |= 1UL << (n))
#define TK_CLEAR_BIT(v, n) ((v) &= ~(1UL << (n)))
#define TK_TOGGLE_BIT(v, n) ((v) ^= (1UL << (n)))
#define TK_TEST_BIT(v, n) (((v) >> (n)) & 1U)

static bool_t mem_pool_match_size(mem_pool_t* pool, uint32_t size) {
  return size >= pool->min_block_size && size <= pool->block_size;
}

static uint32_t mem_pool_get_bits_size(uint32_t block_nr) {
  return (block_nr / 32 + 2) & 0xfffffffe;
}

static uint32_t mem_pool_get_min_size(uint32_t block_size, uint32_t block_nr) {
  uint32_t bits_size = mem_pool_get_bits_size(block_nr);
  uint32_t head_size = sizeof(mem_pool_t);
  return head_size + bits_size * sizeof(uint32_t) + block_nr * block_size;
}

static inline mem_pool_t* mem_pool_init(uint8_t* addr, uint32_t size, uint32_t block_size,
                                        uint32_t block_nr) {
  mem_pool_t* pool = (mem_pool_t*)addr;
  uint32_t bits_size = mem_pool_get_bits_size(block_nr);
  uint32_t min_size = mem_pool_get_min_size(block_size, block_nr);
  assert(size >= min_size);

  memset(addr, 0x00, size);
  pool->block_nr = block_nr;
  pool->block_size = block_size;
  pool->bits_size = bits_size;
  pool->bits = (uint32_t*)(addr + sizeof(*pool));
  pool->start = (uint8_t*)(pool->bits + pool->bits_size);

  return pool;
}

static inline int32_t mem_pool_get_free_index(mem_pool_t* pool) {
  uint32_t i = 0;
  uint32_t index = 0;
  uint32_t* bits = pool->bits;
  for (i = 0; i < pool->bits_size; i++) {
    if (index >= pool->block_nr) {
      break;
    }

    if (bits[i] != 0xffffffff) {
      uint32_t k = 0;
      uint32_t v = bits[i];
      for (k = 0; k < 32; k++) {
        if (TK_TEST_BIT(v, k) == 0) {
          TK_SET_BIT(v, k);
          assert(TK_TEST_BIT(v, k) != 0);
          bits[i] = v;
          return i * 32 + k;
        }
        index++;

        if (index >= pool->block_nr) {
          break;
        }
      }
    } else {
      index += 32;
    }
  }

  return -1;
}

static inline void* mem_pool_get(mem_pool_t* pool) {
  int32_t index = mem_pool_get_free_index(pool);

  if (index >= 0) {
    pool->used++;
    return pool->start + index * pool->block_size;
  }

  return NULL;
}

static inline ret_t mem_pool_put_index(mem_pool_t* pool, uint32_t index) {
  uint32_t i = index / 32;
  uint32_t k = index % 32;
  uint32_t v = pool->bits[i];
  return_value_if_fail(TK_TEST_BIT(v, k) != 0, RET_FAIL);

  TK_CLEAR_BIT(v, k);
  pool->bits[i] = v;
  assert(TK_TEST_BIT(v, k) == 0);

  return RET_OK;
}

static inline int32_t mem_pool_get_index(mem_pool_t* pool, void* ptr) {
  uint8_t* addr = (uint8_t*)ptr;
  if (pool->start <= addr && addr < (pool->start + pool->block_size * pool->block_nr)) {
    uint32_t offset = addr - pool->start;
    uint32_t index = offset / (pool->block_size);
    assert(offset % pool->block_size == 0);
    return index;
  }

  return -1;
}

static inline ret_t mem_pool_put(mem_pool_t* pool, void* ptr) {
  uint8_t* addr = (uint8_t*)ptr;
  int32_t index = mem_pool_get_index(pool, addr);
  if (index >= 0) {
    if (mem_pool_put_index(pool, index) == RET_OK) {
      pool->used--;
      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

END_C_DECLS

#endif /*TK_MEM_POOL_H*/
