/**
 * File:   mem_allocator_fixed_block.c
 * Author: AWTK Develop Team
 * Brief:  只能分配固定大小内存块的内存分配器。
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

typedef struct _mem_allocator_fixed_block_node_t mem_allocator_fixed_block_node_t;
struct _mem_allocator_fixed_block_node_t {
  mem_allocator_fixed_block_node_t* prev;
  mem_allocator_fixed_block_node_t* next;
};

typedef struct _mem_allocator_fixed_block_pool_t mem_allocator_fixed_block_pool_t;
struct _mem_allocator_fixed_block_pool_t {
  mem_allocator_fixed_block_pool_t* next;
  /**
   * @property {uint32_t} num
   * @annotation ["readable"]
   * 内存块的数量。
   */
  uint32_t num;
  /**
   * @property {mem_allocator_fixed_block_node_t*} used_list
   * 已使用的内存块信息链表。
   */
  mem_allocator_fixed_block_node_t* used_list;
  /**
   * @property {mem_allocator_fixed_block_node_t*} unused_list
   * 未使用的内存块信息链表。
   */
  mem_allocator_fixed_block_node_t* unused_list;
  /**
   * @property {mem_allocator_fixed_block_node_t} list
   * 内存块信息数组链表。
   */
  mem_allocator_fixed_block_node_t list;
  /**
   * @property {uint8_t*} mem
   * 内存块数组。
   */
  uint8_t mem[];
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

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, num) ((type_size) * (num))

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_START_OFFSET(num) \
  (offsetof(mem_allocator_fixed_block_pool_t, list) +   \
   sizeof(mem_allocator_fixed_block_node_t) * (num))

#define MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(type_size, num) \
  (MEM_ALLOCATOR_FIXED_BLOCK_MEM_START_OFFSET(num) +        \
   MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, num))

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool) \
  ((uint8_t*)(pool) + MEM_ALLOCATOR_FIXED_BLOCK_MEM_START_OFFSET(pool->num))

#define MEM_ALLOCATOR_FIXED_BLOCK_MEM_GET(pool, type_size, index) \
  (MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool) + MEM_ALLOCATOR_FIXED_BLOCK_MEM_SIZE(type_size, index))

#define MEM_ALLOCATOR_FIXED_BLOCK_GET_INDEX(target, start, size) \
  (((uint8_t*)(target) - (uint8_t*)(start)) / (size))

#define MEM_ALLOCATOR_FIXED_BLOCK_NUM_EXTEND(num) (((num) >> 1) + (num) + 1)

#define MEM_ALLOCATOR_FIXED_BLOCK_MOD(num, div) \
  (((div) & ((div)-1) /* 块大小是否为2的幂次方 */) ? (num) % (div) : (num) & ((div)-1))

inline static bool_t mem_allocator_fixed_block_pool_is_full(
    mem_allocator_fixed_block_pool_t* pool) {
  return_value_if_fail(pool != NULL, FALSE);
  return pool->unused_list == NULL;
}

inline static bool_t mem_allocator_fixed_block_pool_is_empty(
    mem_allocator_fixed_block_pool_t* pool) {
  return_value_if_fail(pool != NULL, FALSE);
  return pool->used_list == NULL;
}

inline static bool_t mem_allocator_fixed_block_is_full(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, FALSE);

  for (iter = allocator->pools; iter != NULL; iter = iter->next) {
    if (!mem_allocator_fixed_block_pool_is_full(iter)) {
      return FALSE;
    }
  }

  return TRUE;
}

inline static bool_t mem_allocator_fixed_block_is_empty(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, FALSE);

  for (iter = allocator->pools; iter != NULL; iter = iter->next) {
    if (!mem_allocator_fixed_block_pool_is_empty(iter)) {
      return FALSE;
    }
  }

  return TRUE;
}

inline static uint32_t mem_allocator_fixed_block_num(mem_allocator_fixed_block_t* allocator,
                                                     bool_t only_empty) {
  uint32_t ret = 0;
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, 0);

  for (iter = allocator->pools; iter != NULL; iter = iter->next) {
    if (!only_empty || mem_allocator_fixed_block_pool_is_empty(iter)) {
      ret += iter->num;
    }
  }

  return ret;
}

inline static uint32_t mem_allocator_fixed_block_pool_num(mem_allocator_fixed_block_t* allocator,
                                                          bool_t only_empty) {
  uint32_t ret = 0;
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, 0);

  for (iter = allocator->pools; iter != NULL; iter = iter->next) {
    if (!only_empty || mem_allocator_fixed_block_pool_is_empty(iter)) {
      ret++;
    }
  }

  return ret;
}

static ret_t mem_allocator_fixed_block_pool_init(mem_allocator_fixed_block_t* allocator,
                                                 mem_allocator_fixed_block_pool_t* pool) {
  uint32_t i = 0;
  return_value_if_fail(allocator != NULL && pool != NULL, RET_BAD_PARAMS);
  return_value_if_fail(pool->num > 0, RET_BAD_PARAMS);

  pool->used_list = NULL;
  pool->unused_list = &pool->list;

  (&pool->list)[0].prev = NULL;
  for (i = 0; i < pool->num - 1; i++) {
    (&pool->list)[i].next = &(&pool->list)[i + 1];
    (&pool->list)[i + 1].prev = &(&pool->list)[i];
  }
  (&pool->list)[pool->num - 1].next = NULL;

  return RET_OK;
}

inline static mem_allocator_fixed_block_pool_t* mem_allocator_fixed_block_pool_create(
    mem_allocator_fixed_block_t* allocator, uint32_t num) {
  mem_allocator_fixed_block_pool_t* ret = NULL;
  return_value_if_fail(allocator != NULL && num > 0, NULL);

  ret = TKMEM_ALLOC(MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(allocator->size, num));
  return_value_if_fail(ret != NULL, NULL);

  ret->num = num;
  ret->next = NULL;

  mem_allocator_fixed_block_pool_init(allocator, ret);

  return ret;
}

static ret_t mem_allocator_fixed_block_pools_destroy(mem_allocator_fixed_block_t* allocator,
                                                     bool_t only_empty) {
  mem_allocator_fixed_block_pool_t *iter = NULL, *prev = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  iter = allocator->pools;
  allocator->pools = NULL;

  while (iter != NULL) {
    mem_allocator_fixed_block_pool_t* next = iter->next;
    if (!only_empty || mem_allocator_fixed_block_pool_is_empty(iter)) {
      if (prev != NULL && prev->next == iter) {
        prev->next = NULL;
      }
      TKMEM_FREE(iter);
    } else {
      if (prev != NULL) {
        prev->next = iter;
      } else {
        allocator->pools = iter;
      }
      prev = iter;
    }
    iter = next;
  }

  return RET_OK;
}

static ret_t mem_allocator_fixed_block_pools_merge(mem_allocator_fixed_block_t* allocator) {
  uint32_t num = 0;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  mem_allocator_fixed_block_pool_t *iter = NULL, *prev = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  num = mem_allocator_fixed_block_num(allocator, TRUE);
  return_value_if_fail(num > 0, RET_FAIL);

  /* 保留一个空间最大的空内存池复用 */
  for (iter = allocator->pools, prev = NULL; iter != NULL; prev = iter, iter = iter->next) {
    if (mem_allocator_fixed_block_pool_is_empty(iter)) {
      if (pool == NULL || pool->num < iter->num) {
        pool = iter;
        prev_pool = prev;
      }
    }
  }
  return_value_if_fail(pool != NULL, RET_FAIL);

  if (allocator->pools == pool) {
    allocator->pools = pool->next;
  } else {
    prev_pool->next = pool->next;
  }
  pool->next = NULL;

  mem_allocator_fixed_block_pools_destroy(allocator, TRUE);

  if (allocator->pools != NULL) {
    pool->next = allocator->pools;
  }
  allocator->pools = pool;

  pool = TKMEM_REALLOC(pool, MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(allocator->size, num));
  return_value_if_fail(pool != NULL, RET_OOM);

  pool->num = num;

  allocator->pools = pool;

  return mem_allocator_fixed_block_pool_init(allocator, allocator->pools);
}

static ret_t mem_allocator_fixed_block_extend(mem_allocator_fixed_block_t* allocator) {
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (mem_allocator_fixed_block_is_full(allocator)) {
    uint32_t num = mem_allocator_fixed_block_num(allocator, FALSE);
    uint32_t extend_num = MEM_ALLOCATOR_FIXED_BLOCK_NUM_EXTEND(num);
    mem_allocator_fixed_block_pool_t* iter = NULL;
    mem_allocator_fixed_block_pool_t* pool =
        mem_allocator_fixed_block_pool_create(allocator, extend_num - num);
    return_value_if_fail(pool != NULL, RET_OOM);

    if (allocator->pools != NULL) {
      pool->next = allocator->pools;
    }
    allocator->pools = pool;
  }

  return RET_OK;
}

static void* mem_allocator_fixed_block_allocate(mem_allocator_fixed_block_t* allocator) {
  uint32_t index = 0;
  mem_allocator_fixed_block_node_t* node = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL, NULL);
  return_value_if_fail(RET_OK == mem_allocator_fixed_block_extend(allocator), NULL);

  for (pool = allocator->pools, prev_pool = NULL; pool != NULL;
       prev_pool = pool, pool = pool->next) {
    if (!mem_allocator_fixed_block_pool_is_full(pool)) {
      break;
    }
  }
  return_value_if_fail(pool != NULL, NULL);

  node = pool->unused_list;
  pool->unused_list = node->next;
  if (pool->unused_list != NULL) {
    pool->unused_list->prev = NULL;
  }

  node->next = pool->used_list;
  if (pool->used_list != NULL) {
    pool->used_list->prev = node;
  }
  pool->used_list = node;

  ENSURE(node->prev == NULL);

  /* 将当前池未满，则移至顶部 */
  if (allocator->pools != pool && !mem_allocator_fixed_block_pool_is_full(pool)) {
    prev_pool->next = pool->next;
    pool->next = allocator->pools;
    allocator->pools = pool;
  }

  index = MEM_ALLOCATOR_FIXED_BLOCK_GET_INDEX(node, &pool->list,
                                              sizeof(mem_allocator_fixed_block_node_t));

  return MEM_ALLOCATOR_FIXED_BLOCK_MEM_GET(pool, allocator->size, index);
}

static mem_allocator_fixed_block_node_t* mem_allocator_fixed_block_node_find(
    mem_allocator_fixed_block_t* allocator, void* ptr, mem_allocator_fixed_block_pool_t** p_pool,
    mem_allocator_fixed_block_pool_t** p_prev_pool) {
  mem_allocator_fixed_block_node_t* ret = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL, NULL);

  for (pool = allocator->pools, prev_pool = NULL; pool != NULL;
       prev_pool = pool, pool = pool->next) {
    if (pool->num > 0) {
      uint8_t* start = MEM_ALLOCATOR_FIXED_BLOCK_MEM_START(pool);
      uint8_t* end = MEM_ALLOCATOR_FIXED_BLOCK_MEM_GET(pool, allocator->size, pool->num - 1);

      if (start <= (uint8_t*)(ptr) && (uint8_t*)(ptr) <= end) {
        uint32_t index = 0;
        const ptrdiff_t offset = (uint8_t*)(ptr)-start;
        return_value_if_fail(allocator->size > 0, NULL);

        /* 地址对齐检查 */
        return_value_if_fail(0 == MEM_ALLOCATOR_FIXED_BLOCK_MOD(offset, allocator->size), NULL);

        index = MEM_ALLOCATOR_FIXED_BLOCK_GET_INDEX(ptr, start, allocator->size);

        ret = &(&pool->list)[index];
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

  return ret;
}

static ret_t mem_allocator_fixed_block_deallocate(mem_allocator_fixed_block_t* allocator,
                                                  void* ptr) {
  mem_allocator_fixed_block_node_t* node = NULL;
  mem_allocator_fixed_block_pool_t *pool = NULL, *prev_pool = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (ptr == NULL) {
    return RET_OK;
  }

  node = mem_allocator_fixed_block_node_find(allocator, ptr, &pool, &prev_pool);
  return_value_if_fail(node != NULL, RET_NOT_FOUND);

  if (node == pool->used_list) {
    pool->used_list = node->next;
  }

  if (mem_allocator_fixed_block_pool_is_empty(pool) &&
      mem_allocator_fixed_block_pool_num(allocator, TRUE) > 1) {
    mem_allocator_fixed_block_pools_merge(allocator);
  } else {
    if (node->prev != NULL) {
      node->prev->next = node->next;
    }
    if (node->next != NULL) {
      node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = pool->unused_list;
    if (pool->unused_list != NULL) {
      pool->unused_list->prev = node;
    }
    pool->unused_list = node;

    /* 如果顶池已满，将当前活跃池移至顶部 */
    if (allocator->pools != pool && mem_allocator_fixed_block_pool_is_full(allocator->pools)) {
      prev_pool->next = pool->next;
      pool->next = allocator->pools;
      allocator->pools = pool;
    }
  }

  return RET_OK;
}

inline static ret_t mem_allocator_fixed_block_clear(mem_allocator_fixed_block_t* allocator) {
  mem_allocator_fixed_block_pool_t* iter = NULL;
  return_value_if_fail(allocator != NULL, RET_BAD_PARAMS);

  if (allocator->pools == NULL) {
    return RET_OK;
  }

  for (iter = allocator->pools; iter != NULL; iter = iter->next) {
    iter->used_list = NULL;
    iter->unused_list = NULL;
  }

  if (mem_allocator_fixed_block_pool_num(allocator, FALSE) > 1) {
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
  return_value_if_fail(fixed_block_allocator->size == size, NULL);

  return mem_allocator_fixed_block_allocate(fixed_block_allocator);
}

static void* mem_allocator_fixed_block_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                               const char* func, uint32_t line) {
  mem_allocator_fixed_block_t* fixed_block_allocator = (mem_allocator_fixed_block_t*)allocator;
  return_value_if_fail(fixed_block_allocator != NULL, NULL);
  return_value_if_fail(fixed_block_allocator->size == size, NULL);

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
  for (pool = fixed_block_allocator->pools, i = 0; pool != NULL; pool = pool->next, i++) {
    mem_allocator_fixed_block_node_t* node = NULL;
    uint32_t used_nr = 0;
    uint32_t bytes = MEM_ALLOCATOR_FIXED_BLOCK_POOL_SIZE(fixed_block_allocator->size, pool->num);
    total_bytes += bytes;
    total_nr += pool->num;
    for (node = pool->used_list; node != NULL; node = node->next) {
      used_nr++;
    }
    total_used_nr += used_nr;

    str_append_format(&dump, 48, "    [%u]: {\n", i);
    str_append_format(&dump, 64, "      ptr:        %p\n", pool);
    str_append_format(&dump, 64, "      mem:        %u bytes\n", bytes);
    str_append_format(&dump, 64, "      nr:         %u\n", pool->num);
    str_append_format(&dump, 88, "      used rate:  %u/%u = %.2f%%\n", used_nr, pool->num,
                      used_nr * 100.0f / pool->num);
    str_append(&dump, "    }\n");
  }
  str_append(&dump, "  ]\n");
  str_append_format(&dump, 64, "  type size: %u\n", fixed_block_allocator->size);
  str_append_format(&dump, 64, "  mem:       %u bytes\n", total_bytes);
  str_append_format(&dump, 64, "  nr:        %u\n", total_nr);
  str_append_format(&dump, 88, "  used rate: %u/%u = %.2f%%\n", total_used_nr, total_nr,
                    total_used_nr * 100.0f / total_nr);
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
