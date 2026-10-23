/**
 * File:   mem_allocator_fixed_block.c
 * Author: AWTK Develop Team
 * Brief:  只能分配固定大小内存块的内存分配器。
 *
 * Copyright (c) 2025 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-04-12 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/mem_allocator_fixed_block.h"
#include "tkc/mem.h"
#include "tkc/str.h"

/**
 * @enum mem_allocator_fixed_block_bit_t
 * @prefix MEM_ALLOCATOR_FIXED_BLOCK_BIT_
 * 每个块的状态位。
 */
typedef enum _mem_allocator_fixed_block_bit_t {
  /**
   * @const MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED
   * 是否已使用。
   */
  MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED = 0,
  MEM_ALLOCATOR_FIXED_BLOCK_BIT_NR
} mem_allocator_fixed_block_bit_t;

typedef union _mem_allocator_fixed_block_node_t mem_allocator_fixed_block_node_t;
union _mem_allocator_fixed_block_node_t {
  /**
   * @property {mem_allocator_fixed_block_node_t*} next
   * 下一个未使用的内存块结点。
   */
  mem_allocator_fixed_block_node_t* next;
  /**
   * @property {uint8_t*} mem
   * 已使用的内存块内容。
   */
  uint8_t* mem;
};

typedef struct _mem_allocator_fixed_block_pool_t mem_allocator_fixed_block_pool_t;
struct _mem_allocator_fixed_block_pool_t {
  /**
   * @property {mem_allocator_fixed_block_pool_t*} next
   * 下一个内存池。
   */
  mem_allocator_fixed_block_pool_t* next;
  /**
   * @property {uint32_t} num
   * @annotation ["readable"]
   * 内存块的数量。
   */
  uint32_t num;
  /**
   * @property {uint32_t} used_num
   * @annotation ["readable"]
   * 已使用内存块的数量。
   */
  uint32_t used_num;
  /**
   * @property {mem_allocator_fixed_block_node_t*} unused_list
   * 未使用的内存块链表。
   */
  mem_allocator_fixed_block_node_t* unused_list;
  /**
   * @property {mem_allocator_fixed_block_node_t} list
   * 内存块数组链表。
   * 注意：块数组起点会按对齐量向上取整，可能越过本字段，
   * 所以 list 不一定就是块 0，块 0 由 MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, allocator->size, 0) 给出。
   */
  mem_allocator_fixed_block_node_t list;
  /**
   * @property {uint8_t*} bits
   * 块状态位图，每块占 BIT_NR 位，位于块数组之后（由 BITS_GET 计算）。
   */
  uint8_t bits[];
};

typedef struct _mem_allocator_fixed_block_t {
  mem_allocator_t base;
  /**
   * @property {mem_allocator_fixed_block_pool_t*} pools
   * 内存池链表。
   */
  mem_allocator_fixed_block_pool_t* pools;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 每个内存块的大小。
   */
  uint32_t size;
} mem_allocator_fixed_block_t;

#define MEM_ALLOCATOR_FIXED_BLOCK_ALIGN_UP(size, align) (((size) + (align) - 1) & ~((align) - 1))

/* 位序号 = 块下标 × BIT_NR + 属性位；字节下标 = 位序号 / 8，位偏移 = 位序号 % 8 */
#define MEM_ALLOCATOR_FIXED_BLOCK_BITS_POS(index, bit) \
  ((index) * MEM_ALLOCATOR_FIXED_BLOCK_BIT_NR + (bit))

/* 位图总字节数：每块 BIT_NR 位，64 位算避免溢出 */
#define MEM_ALLOCATOR_FIXED_BLOCK_BITS_SIZE(num) \
  ((uint32_t)((MEM_ALLOCATOR_FIXED_BLOCK_BITS_POS((uint64_t)(num), 0) + 7) / 8))

#define MEM_ALLOCATOR_FIXED_BLOCK_POWER_OF_2(size) ((size) != 0 && !((size) & ((size) - 1)))

/**
 * 平台最大自然对齐的上界。
 *
 * 以 sizeof(long double) 代表平台的最大对齐：
 *   Linux x86-64 / ARM64 = 16，MSVC x64 / ARM32 = 8，i386 = 12。
 * sizeof(long double) 不是 2 的幂时（i386 的 12）不能当对齐量用，退回指针大小（4）。
 * 取 max(..., sizeof(void*)) 保证上限不小于指针大小，块首的空闲链表指针因此始终对齐。
 */
#define MEM_ALLOCATOR_FIXED_BLOCK_MAX_ALIGN                  \
  (MEM_ALLOCATOR_FIXED_BLOCK_POWER_OF_2(sizeof(long double)) \
       ? tk_max(sizeof(long double), sizeof(void*))          \
       : sizeof(void*))

#define MEM_ALLOCATOR_FIXED_BLOCK_SIZE(type_size) \
  MEM_ALLOCATOR_FIXED_BLOCK_ALIGN_UP(type_size, sizeof(mem_allocator_fixed_block_node_t*))

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, num) \
  (MEM_ALLOCATOR_FIXED_BLOCK_SIZE(type_size) * (num))

/**
 * 块对齐量 A：块地址需要满足的对齐粒度（地址 % A == 0）。
 * 取能整除 stride 的最大 2 的幂，并以平台最大自然对齐封顶；
 * A 整除 stride 保证块与块之间不会丢失对齐，对标准类型有 A >= _Alignof(T)。
 */
inline static uint32_t mem_allocator_fixed_block_align(uint32_t type_size) {
  uint32_t stride = MEM_ALLOCATOR_FIXED_BLOCK_SIZE(type_size);
  uint32_t lowbit = stride & (0u - stride);
  return tk_min(lowbit, (uint32_t)MEM_ALLOCATOR_FIXED_BLOCK_MAX_ALIGN);
}

/* 块数组的起始偏移：把 offsetof(list) 向上取整到 A，使块 0 的地址是 A 的倍数。 */
inline static uint32_t mem_allocator_fixed_block_start_offset(uint32_t type_size) {
  uint32_t align = mem_allocator_fixed_block_align(type_size);
  return (uint32_t)MEM_ALLOCATOR_FIXED_BLOCK_ALIGN_UP(
      offsetof(mem_allocator_fixed_block_pool_t, list), align);
}

#define MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(type_size, num) \
  (mem_allocator_fixed_block_start_offset(type_size) +      \
   MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, num) + MEM_ALLOCATOR_FIXED_BLOCK_BITS_SIZE(num))

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool, type_size) \
  ((uint8_t*)(pool) + mem_allocator_fixed_block_start_offset(type_size))

#define MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, type_size, index)                           \
  (mem_allocator_fixed_block_node_t*)(MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool, type_size) + \
                                      MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, index))

#define MEM_ALLOCATOR_FIXED_BLOCK_BITS_GET(pool, type_size) \
  ((uint8_t*)MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, type_size, (pool)->num))

/* 位序号 → 位图字节地址 */
#define MEM_ALLOCATOR_FIXED_BLOCK_BITS_BYTE(bits, index, bit) \
  ((bits) + MEM_ALLOCATOR_FIXED_BLOCK_BITS_POS(index, bit) / 8)

/* 对第 index 块的 bit 位执行 OP：TK_TEST_BIT / TK_SET_BIT / TK_CLEAR_BIT */
#define MEM_ALLOCATOR_FIXED_BLOCK_BITS_OP(OP, bits, index, bit) \
  (OP(*MEM_ALLOCATOR_FIXED_BLOCK_BITS_BYTE(bits, index, bit),   \
      MEM_ALLOCATOR_FIXED_BLOCK_BITS_POS(index, bit) % 8))

#define MEM_ALLOCATOR_FIXED_BLOCK_GET_INDEX(target, start, size) \
  (((uint8_t*)(target) - (uint8_t*)(start)) / MEM_ALLOCATOR_FIXED_BLOCK_SIZE(size))

#define MEM_ALLOCATOR_FIXED_BLOCK_EXTEND_NUM(num) (((num) >> 1) + 1)

inline static bool_t mem_allocator_fixed_block_pool_overflow(uint32_t type_size, uint32_t num) {
  if (num > 0) {
    uint64_t stride = MEM_ALLOCATOR_FIXED_BLOCK_SIZE(type_size);
    uint64_t limit = (uint64_t)UINT32_MAX - mem_allocator_fixed_block_start_offset(type_size);
    uint64_t bits = MEM_ALLOCATOR_FIXED_BLOCK_BITS_SIZE(num);
    /* 位图已占满可用空间，或 stride * num 放不下；先比较再减，避免 limit - bits 下溢 */
    return bits >= limit || stride > (limit - bits) / num;
  }
  return FALSE;
}

#define MEM_ALLOCATOR_FIXED_BLOCK_MOD(num, div) \
  (MEM_ALLOCATOR_FIXED_BLOCK_POWER_OF_2(div) ? (num) & ((div) - 1) : (num) % (div))

#define MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH(pools, iter) \
  for (iter = pools; iter != NULL; iter = iter->next)

#define MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH_EX(pools, iter, init, step) \
  for (iter = pools, init; iter != NULL; step, iter = iter->next)

inline static bool_t mem_allocator_fixed_block_pool_is_full(
    mem_allocator_fixed_block_pool_t* pool) {
  return_value_if_fail(pool != NULL, FALSE);
  return NULL == pool->unused_list;
}

inline static bool_t mem_allocator_fixed_block_pool_is_empty(
    mem_allocator_fixed_block_pool_t* pool) {
  return_value_if_fail(pool != NULL, FALSE);
  return 0 == pool->used_num;
}

inline static bool_t mem_allocator_fixed_block_is_full(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, FALSE);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH(allocator->pools, iter) {
    if (!mem_allocator_fixed_block_pool_is_full(iter)) {
      return FALSE;
    }
  }

  return TRUE;
}

inline static bool_t mem_allocator_fixed_block_is_empty(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, FALSE);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH(allocator->pools, iter) {
    if (!mem_allocator_fixed_block_pool_is_empty(iter)) {
      return FALSE;
    }
  }

  return TRUE;
}

inline static ret_t mem_allocator_fixed_block_pools_push(mem_allocator_fixed_block_pool_t** head,
                                                         mem_allocator_fixed_block_pool_t* pool) {
  return_value_if_fail(head != NULL && pool != NULL, RET_BAD_PARAMS);

  pool->next = *head;
  ENSURE(pool != *head); /* 防止形成循环链表 */
  *head = pool;

  return RET_OK;
}

inline static mem_allocator_fixed_block_pool_t* mem_allocator_fixed_block_pools_pop(
    mem_allocator_fixed_block_pool_t** head) {
  mem_allocator_fixed_block_pool_t* ret = NULL;
  return_value_if_fail(head != NULL, NULL);

  if (*head != NULL) {
    ret = *head;
    *head = ret->next;
    ret->next = NULL;
  }

  return ret;
}

inline static ret_t mem_allocator_fixed_block_pool_link(mem_allocator_fixed_block_pool_t** head,
                                                        mem_allocator_fixed_block_pool_t* pool,
                                                        mem_allocator_fixed_block_pool_t* prev) {
  return_value_if_fail(head != NULL && pool != NULL, RET_BAD_PARAMS);

  if (prev == NULL) {
    return mem_allocator_fixed_block_pools_push(head, pool);
  }

  pool->next = prev->next;
  ENSURE(prev->next != pool); /* 防止形成循环链表 */
  prev->next = pool;

  return RET_OK;
}

inline static ret_t mem_allocator_fixed_block_pool_unlink(mem_allocator_fixed_block_pool_t** head,
                                                          mem_allocator_fixed_block_pool_t* pool,
                                                          mem_allocator_fixed_block_pool_t* prev) {
  return_value_if_fail(head != NULL && pool != NULL, RET_BAD_PARAMS);

  if (*head == pool) {
    mem_allocator_fixed_block_pools_pop(head);
    return RET_OK;
  }

  ENSURE(prev != NULL);
  prev->next = pool->next;
  pool->next = NULL;

  return RET_OK;
}

inline static ret_t mem_allocator_fixed_block_pool_move_to_top(
    mem_allocator_fixed_block_pool_t** head, mem_allocator_fixed_block_pool_t* pool,
    mem_allocator_fixed_block_pool_t* prev) {
  return_value_if_fail(head != NULL && pool != NULL, RET_BAD_PARAMS);

  if (*head != pool) {
    ENSURE(prev != NULL);
    prev->next = pool->next;
    pool->next = *head;
    *head = pool;
  }

  return RET_OK;
}

/* 由块地址反查块下标；位图按块下标寻址 */
inline static uint32_t mem_allocator_fixed_block_node_index(mem_allocator_fixed_block_pool_t* pool,
                                                            mem_allocator_fixed_block_node_t* node,
                                                            uint32_t type_size) {
  return MEM_ALLOCATOR_FIXED_BLOCK_GET_INDEX(
      node, MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool, type_size), type_size);
}

inline static mem_allocator_fixed_block_node_t* mem_allocator_fixed_block_pool_allocate(
    mem_allocator_fixed_block_pool_t* pool, uint32_t type_size) {
  mem_allocator_fixed_block_node_t* ret = NULL;
  uint8_t* bits = NULL;
  uint32_t index = 0;
  bool_t used = FALSE;
  return_value_if_fail(pool != NULL, NULL);
  return_value_if_fail(!mem_allocator_fixed_block_pool_is_full(pool), NULL);

  ENSURE(pool->unused_list != NULL);
  ret = pool->unused_list;
  pool->unused_list = pool->unused_list->next;

  pool->used_num++;
  ENSURE(pool->used_num <= pool->num);

  bits = MEM_ALLOCATOR_FIXED_BLOCK_BITS_GET(pool, type_size);
  index = mem_allocator_fixed_block_node_index(pool, ret, type_size);

  used = MEM_ALLOCATOR_FIXED_BLOCK_BITS_OP(TK_TEST_BIT, bits, index,
                                           MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED);
  ENSURE(!used);
  MEM_ALLOCATOR_FIXED_BLOCK_BITS_OP(TK_SET_BIT, bits, index, MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED);

  return ret;
}

inline static ret_t mem_allocator_fixed_block_pool_deallocate(
    mem_allocator_fixed_block_pool_t* pool, mem_allocator_fixed_block_node_t* node,
    uint32_t type_size) {
  uint8_t* bits = NULL;
  uint32_t index = 0;
  bool_t used = FALSE;
  return_value_if_fail(pool != NULL && node != NULL, RET_BAD_PARAMS);
  ENSURE(!mem_allocator_fixed_block_pool_is_empty(pool));
  return_value_if_fail(!mem_allocator_fixed_block_pool_is_empty(pool), RET_FAIL);

  bits = MEM_ALLOCATOR_FIXED_BLOCK_BITS_GET(pool, type_size);
  index = mem_allocator_fixed_block_node_index(pool, node, type_size);
  used = MEM_ALLOCATOR_FIXED_BLOCK_BITS_OP(TK_TEST_BIT, bits, index,
                                           MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED);

  /* 未置位说明重复释放，或这个块不属于当前池 */
  ENSURE(used);
  return_value_if_fail(used, RET_FAIL);
  MEM_ALLOCATOR_FIXED_BLOCK_BITS_OP(TK_CLEAR_BIT, bits, index, MEM_ALLOCATOR_FIXED_BLOCK_BIT_USED);

  ENSURE(pool->used_num > 0);
  pool->used_num--;

  node->next = pool->unused_list;
  pool->unused_list = node;

  return RET_OK;
}

typedef enum _mem_allocator_fixed_block_num_type_t {
  MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_BLOCK = 0,
  MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_POOL
} mem_allocator_fixed_block_num_type_t;

inline static uint32_t mem_allocator_fixed_block_num(mem_allocator_fixed_block_t* allocator,
                                                     mem_allocator_fixed_block_num_type_t type,
                                                     bool_t only_empty) {
  uint32_t ret = 0;
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, 0);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH(allocator->pools, iter) {
    if (!only_empty || mem_allocator_fixed_block_pool_is_empty(iter)) {
      switch (type) {
        case MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_BLOCK: {
          /* 饱和而不是回绕：块数多到溢出时，后续的池大小检查会拒绝创建 */
          return_value_if_fail(ret <= UINT32_MAX - iter->num, UINT32_MAX);
          ret += iter->num;
        } break;
        case MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_POOL: {
          ret++;
        } break;
      }
    }
  }

  return ret;
}

static ret_t mem_allocator_fixed_block_pool_init(mem_allocator_fixed_block_t* allocator,
                                                 mem_allocator_fixed_block_pool_t* pool) {
  mem_allocator_fixed_block_node_t* node = NULL;
  uint32_t i = 0;
  return_value_if_fail(allocator != NULL && pool != NULL, RET_BAD_PARAMS);
  ENSURE(pool->num > 0);
  return_value_if_fail(pool->num > 0, RET_BAD_PARAMS);

  pool->used_num = 0;

  /* 块 0 由起始偏移决定：偏移为 0 时等于 &pool->list，否则在其后（对齐填充之后） */
  pool->unused_list = MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, allocator->size, 0);

  for (i = 0; i < pool->num - 1; i++) {
    node = MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, allocator->size, i);
    node->next = MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, allocator->size, i + 1);
  }
  node = MEM_ALLOCATOR_FIXED_BLOCK_NODE_GET(pool, allocator->size, pool->num - 1);
  node->next = NULL;

  memset(MEM_ALLOCATOR_FIXED_BLOCK_BITS_GET(pool, allocator->size), 0,
         MEM_ALLOCATOR_FIXED_BLOCK_BITS_SIZE(pool->num));

  return RET_OK;
}

inline static mem_allocator_fixed_block_pool_t* mem_allocator_fixed_block_pool_create(
    mem_allocator_fixed_block_t* allocator, uint32_t num) {
  mem_allocator_fixed_block_pool_t* ret = NULL;
  ENSURE(num > 0);
  return_value_if_fail(allocator != NULL && num > 0, NULL);
  return_value_if_fail(!mem_allocator_fixed_block_pool_overflow(allocator->size, num), NULL);

  ret = TKMEM_ALLOC(MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(allocator->size, num));
  return_value_if_fail(ret != NULL, NULL);

  ret->num = num;
  ret->next = NULL;

  mem_allocator_fixed_block_pool_init(allocator, ret);

  return ret;
}

static ret_t mem_allocator_fixed_block_pools_destroy(mem_allocator_fixed_block_t* allocator,
                                                     bool_t only_empty) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  mem_allocator_fixed_block_pool_t* prev = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  iter = allocator->pools;

  while (iter != NULL) {
    mem_allocator_fixed_block_pool_t* next = iter->next;
    if (only_empty) {
      if (mem_allocator_fixed_block_pool_is_empty(iter)) {
        mem_allocator_fixed_block_pool_unlink(&allocator->pools, iter, prev);
        TKMEM_FREE(iter);
      } else {
        prev = iter;
      }
    } else {
      TKMEM_FREE(iter);
    }
    iter = next;
  }

  return RET_OK;
}

static ret_t mem_allocator_fixed_block_pools_merge(mem_allocator_fixed_block_t* allocator) {
  uint32_t num = 0;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL, *new_prev_pool = NULL;
  mem_allocator_fixed_block_pool_t *iter = NULL, *prev = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  num = mem_allocator_fixed_block_num(allocator, MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_BLOCK, TRUE);
  return_value_if_fail(num > 0, RET_FAIL);
  return_value_if_fail(!mem_allocator_fixed_block_pool_overflow(allocator->size, num), RET_FAIL);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH_EX(allocator->pools, iter, prev = NULL, prev = iter) {
    /* 保留一个空间最大的空内存池复用 */
    if (mem_allocator_fixed_block_pool_is_empty(iter)) {
      if (NULL == pool || pool->num < iter->num) {
        pool = iter;
        prev_pool = prev;
      }
    }
    /* 复用的空池放到最后一个非满池的后面，让分配器优先分配非满池里的内存 */
    else if (!mem_allocator_fixed_block_pool_is_full(iter)) {
      new_prev_pool = iter;
    }
  }
  return_value_if_fail(pool != NULL, RET_FAIL);

  mem_allocator_fixed_block_pool_unlink(&allocator->pools, pool, prev_pool);

  mem_allocator_fixed_block_pools_destroy(allocator, TRUE);

  mem_allocator_fixed_block_pool_link(&allocator->pools, pool, new_prev_pool);

  pool = TKMEM_REALLOC(pool, MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(allocator->size, num));
  return_value_if_fail(pool != NULL, RET_OOM);

  pool->num = num;

  if (NULL == new_prev_pool) {
    allocator->pools = pool;
  } else {
    new_prev_pool->next = pool;
  }

  return mem_allocator_fixed_block_pool_init(allocator, pool);
}

static ret_t mem_allocator_fixed_block_extend(mem_allocator_fixed_block_t* allocator) {
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (mem_allocator_fixed_block_is_full(allocator)) {
    uint32_t num =
        mem_allocator_fixed_block_num(allocator, MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_BLOCK, FALSE);
    mem_allocator_fixed_block_pool_t* pool =
        mem_allocator_fixed_block_pool_create(allocator, MEM_ALLOCATOR_FIXED_BLOCK_EXTEND_NUM(num));
    return_value_if_fail(pool != NULL, RET_OOM);

    return mem_allocator_fixed_block_pools_push(&allocator->pools, pool);
  }

  return RET_OK;
}

static void* mem_allocator_fixed_block_allocate(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_node_t* node = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL, NULL);
  return_value_if_fail(RET_OK == mem_allocator_fixed_block_extend(allocator), NULL);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH_EX(allocator->pools, pool, prev_pool = NULL,
                                             prev_pool = pool) {
    if (!mem_allocator_fixed_block_pool_is_full(pool)) {
      break;
    }
  }
  return_value_if_fail(pool != NULL, NULL);

  node = mem_allocator_fixed_block_pool_allocate(pool, allocator->size);
  ENSURE(node != NULL);

  if (!mem_allocator_fixed_block_pool_is_full(pool)) {
    mem_allocator_fixed_block_pool_move_to_top(&allocator->pools, pool, prev_pool);
  }

  return (void*)&node->mem;
}

static mem_allocator_fixed_block_node_t* mem_allocator_fixed_block_node_find(
    mem_allocator_fixed_block_t* allocator, void* ptr, mem_allocator_fixed_block_pool_t** p_pool,
    mem_allocator_fixed_block_pool_t** p_prev_pool) {
  mem_allocator_fixed_block_node_t* ret = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL && allocator->size > 0, NULL);

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH_EX(allocator->pools, pool, prev_pool = NULL,
                                             prev_pool = pool) {
    if (pool->num > 0) {
      uint8_t* start = MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool, allocator->size);
      if (start <= (uint8_t*)(ptr)) {
        uint8_t* end = start + MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(allocator->size, pool->num - 1);
        if ((uint8_t*)(ptr) <= end) {
          const ptrdiff_t offset = (uint8_t*)(ptr)-start;

          /* 地址对齐检查 */
          ENSURE(allocator->size > 0);
          return_value_if_fail(0 == MEM_ALLOCATOR_FIXED_BLOCK_MOD(
                                        offset, MEM_ALLOCATOR_FIXED_BLOCK_SIZE(allocator->size)),
                               NULL);

          ret = (mem_allocator_fixed_block_node_t*)(ptr);
          if (p_pool != NULL) {
            *p_pool = pool;
          }
          if (p_prev_pool != NULL) {
            *p_prev_pool = prev_pool;
          }
          break;
        }
      }
    }
  }

  return ret;
}

static ret_t mem_allocator_fixed_block_deallocate(mem_allocator_fixed_block_t* allocator,
                                                  void* ptr) {
  mem_allocator_fixed_block_node_t* node = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (NULL == ptr) {
    return RET_OK;
  }

  node = mem_allocator_fixed_block_node_find(allocator, ptr, &pool, &prev_pool);
  return_value_if_fail(node != NULL, RET_NOT_FOUND);

  ENSURE(pool != NULL);

  return_value_if_fail(
      RET_OK == mem_allocator_fixed_block_pool_deallocate(pool, node, allocator->size), RET_FAIL);

  if (mem_allocator_fixed_block_pool_is_empty(pool) &&
      mem_allocator_fixed_block_num(allocator, MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_POOL, TRUE) > 1) {
    return mem_allocator_fixed_block_pools_merge(allocator);
  }

  if (mem_allocator_fixed_block_pool_is_full(allocator->pools)) {
    mem_allocator_fixed_block_pool_move_to_top(&allocator->pools, pool, prev_pool);
  }

  return RET_OK;
}

inline static ret_t mem_allocator_fixed_block_clear(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (NULL == allocator->pools) {
    return RET_OK;
  }

  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH(allocator->pools, iter) {
    iter->used_num = 0;
    iter->unused_list = NULL;
  }

  if (mem_allocator_fixed_block_num(allocator, MEM_ALLOCATOR_FIXED_BLOCK_NUM_TYPE_POOL, FALSE) >
      1) {
    return mem_allocator_fixed_block_pools_merge(allocator);
  } else {
    return mem_allocator_fixed_block_pool_init(allocator, allocator->pools);
  }
}

inline static ret_t mem_allocator_fixed_block_deinit(mem_allocator_fixed_block_t* allocator) {
  return mem_allocator_fixed_block_pools_destroy(allocator, FALSE);
}

static ret_t mem_allocator_fixed_block_destroy(mem_allocator_t* allocator) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  return_value_if_fail(fixed_block_allocator != NULL, RET_BAD_PARAMS);

  mem_allocator_fixed_block_deinit(fixed_block_allocator);
  TKMEM_FREE(fixed_block_allocator);

  return RET_OK;
}

static void* mem_allocator_fixed_block_alloc(mem_allocator_t* allocator, uint32_t size,
                                             const char* func, uint32_t line) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  (void)func, (void)line;
  return_value_if_fail(fixed_block_allocator != NULL, NULL);
  return_value_if_fail(
      0 < size && size <= MEM_ALLOCATOR_FIXED_BLOCK_SIZE(fixed_block_allocator->size), NULL);

  return mem_allocator_fixed_block_allocate(fixed_block_allocator);
}

static void* mem_allocator_fixed_block_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                               const char* func, uint32_t line) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  return_value_if_fail(fixed_block_allocator != NULL, NULL);
  return_value_if_fail(
      0 < size && size <= MEM_ALLOCATOR_FIXED_BLOCK_SIZE(fixed_block_allocator->size), NULL);

  if (ptr != NULL) {
    return_value_if_fail(
        mem_allocator_fixed_block_node_find(fixed_block_allocator, ptr, NULL, NULL) != NULL, NULL);
    return ptr;
  } else {
    return mem_allocator_fixed_block_alloc(allocator, size, func, line);
  }
}

static void mem_allocator_fixed_block_free(mem_allocator_t* allocator, void* ptr) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  mem_allocator_fixed_block_deallocate(fixed_block_allocator, ptr);
}

static ret_t mem_allocator_fixed_block_dump(mem_allocator_t* allocator) {
  str_t dump;
  mem_allocator_fixed_block_pool_t* pool = NULL;
  uint32_t total_bytes = 0;
  uint32_t total_nr = 0;
  uint32_t total_used_nr = 0;
  uint32_t i = 0;
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  return_value_if_fail(fixed_block_allocator != NULL, RET_BAD_PARAMS);

  if (log_get_log_level() > LOG_LEVEL_DEBUG) {
    return RET_OK;
  }

  str_init(&dump, 512);

  str_append_format(&dump, 40, "%s: {\n", __FUNCTION__);
  str_append(&dump, "  pools: [\n");
  MEM_ALLOCATOR_FIXED_BLOCK_POOLS_FOREACH_EX(fixed_block_allocator->pools, pool, i = 0, i++) {
    uint32_t used_nr = pool->used_num;
    uint32_t bytes = MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(fixed_block_allocator->size, pool->num);
    total_bytes += bytes;
    total_nr += pool->num;
    total_used_nr += used_nr;

    str_append_format(&dump, 48, "    [%u]: {\n", i);
    str_append_format(&dump, 64, "      ptr:       %p\n", pool);
    str_append_format(&dump, 64, "      mem:       %u bytes\n", bytes);
    str_append_format(&dump, 64, "      nr:        %u\n", pool->num);
    str_append_format(&dump, 88, "      used rate: %u/%u = %.2f%%\n", used_nr, pool->num,
                      used_nr * 100.0f / (pool->num * 1.0f));
    str_append(&dump, "    }\n");
  }
  str_append(&dump, "  ]\n");
  str_append_format(&dump, 64, "  type size:       %u\n", fixed_block_allocator->size);
  str_append_format(&dump, 64, "  mem:             %u bytes\n", total_bytes);
  str_append_format(&dump, 64, "  nr:              %u\n", total_nr);
  str_append_format(&dump, 96, "  used rate:       %u/%u = %.2f%%\n", total_used_nr, total_nr,
                    total_used_nr * 100.0f / (total_nr * 1.0f));
  str_append_format(&dump, 96, "  mem utilization: %u/%u = %.2f%%\n",
                    fixed_block_allocator->size * total_nr, total_bytes,
                    fixed_block_allocator->size * total_nr * 100.0f / (total_bytes * 1.0f));
  str_append(&dump, "}\n");

  log_debug("%s", dump.str);

  str_reset(&dump);

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_fixed_block_vtable = {
    .alloc = mem_allocator_fixed_block_alloc,
    .realloc = mem_allocator_fixed_block_realloc,
    .free = mem_allocator_fixed_block_free,
    .dump = mem_allocator_fixed_block_dump,
    .destroy = mem_allocator_fixed_block_destroy,
};

inline static ret_t mem_allocator_fixed_block_init(mem_allocator_fixed_block_t* allocator,
                                                   uint32_t size, uint32_t num) {
  return_value_if_fail(allocator != NULL && size > 0, RET_BAD_PARAMS);

  memset(allocator, 0, sizeof(mem_allocator_fixed_block_t));

  allocator->base.vt = &s_mem_allocator_fixed_block_vtable;

  allocator->size = size;

  if (num > 0) {
    allocator->pools = mem_allocator_fixed_block_pool_create(allocator, num);
    goto_error_if_fail(allocator->pools != NULL);
  }

  return RET_OK;
error:
  mem_allocator_fixed_block_deinit(allocator);
  return RET_OOM;
}

mem_allocator_t* mem_allocator_fixed_block_create(uint32_t size, uint32_t num) {
  mem_allocator_fixed_block_t* ret = NULL;
  return_value_if_fail(size > 0, NULL);

  /* 取整对齐后不应小于原尺寸，否则说明 size 已经大到回绕 */
  ENSURE(MEM_ALLOCATOR_FIXED_BLOCK_SIZE(size) >= size);
  return_value_if_fail(MEM_ALLOCATOR_FIXED_BLOCK_SIZE(size) >= size, NULL);

  ENSURE(!mem_allocator_fixed_block_pool_overflow(size, num));
  return_value_if_fail(!mem_allocator_fixed_block_pool_overflow(size, num), NULL);

  ret = TKMEM_ALLOC(sizeof(mem_allocator_fixed_block_t));
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == mem_allocator_fixed_block_init(ret, size, num));

  return &ret->base;
error:
  mem_allocator_fixed_block_deinit(ret);
  return NULL;
}

uint32_t mem_allocator_fixed_block_size(mem_allocator_t* allocator) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  return_value_if_fail(fixed_block_allocator != NULL, 0);
  return fixed_block_allocator->size;
}
