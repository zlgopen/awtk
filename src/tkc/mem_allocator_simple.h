/**
 * File:   mem_allocator_simple.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_simple
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

#ifndef TK_MEM_ALLOCATOR_SIMPLE_H
#define TK_MEM_ALLOCATOR_SIMPLE_H

#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

typedef struct _free_node_t {
  uint32_t size;
  struct _free_node_t* next;
  struct _free_node_t* prev;
} free_node_t;

typedef struct _mem_info_t {
  char* buffer;
  uint32_t size;
  uint32_t used_bytes;
  uint32_t used_block_nr;
  free_node_t* free_list;
} mem_info_t;

/**
 * @class mem_allocator_simple_t 
 * @parent mem_allocator_t 
 * 
 * 简单的内存分配器。 
 * 
 */
typedef struct _mem_allocator_simple_t {
  mem_allocator_t allocator;
  mem_info_t info;
} mem_allocator_simple_t;

#define MEM_ALLOCATOR_SIMPLE(allocator) ((mem_allocator_simple_t*)(allocator))

#define MIN_SIZE TK_ROUND_TO8(sizeof(free_node_t))
#define REAL_SIZE(size) \
  TK_ROUND_TO8((size > sizeof(free_node_t) ? size : MIN_SIZE) + sizeof(uint32_t));

static void* tk_alloc_impl(mem_allocator_t* allocator, uint32_t s) {
  mem_info_t* info = &(MEM_ALLOCATOR_SIMPLE(allocator)->info);

  free_node_t* iter = NULL;
  uint32_t size = REAL_SIZE(s);

  /*查找第一个满足条件的空闲块*/
  for (iter = info->free_list; iter != NULL; iter = iter->next) {
    if (iter->size > size) {
      break;
    }
  }

  if (iter == NULL) {
    log_debug("%s: Out of memory(%d):\n", __FUNCTION__, (int)size);
  }

  return_value_if_fail(iter != NULL, NULL);

  /*如果找到的空闲块刚好满足需求，就从空闲块链表中移出它*/
  if (iter->size < (size + MIN_SIZE)) {
    if (info->free_list == iter) {
      info->free_list = iter->next;
    }

    if (iter->prev != NULL) {
      iter->prev->next = iter->next;
    }
    if (iter->next != NULL) {
      iter->next->prev = iter->prev;
    }
  } else {
    /*如果找到的空闲块比较大，就把它拆成两个块，把多余的空闲内存放回去*/
    free_node_t* new_iter = (free_node_t*)((char*)iter + size);

    new_iter->size = iter->size - size;
    new_iter->next = iter->next;
    new_iter->prev = iter->prev;

    if (iter->prev != NULL) {
      iter->prev->next = new_iter;
    }
    if (iter->next != NULL) {
      iter->next->prev = new_iter;
    }

    if (info->free_list == iter) {
      info->free_list = new_iter;
    }
    iter->size = size;
  }
  /*返回可用的内存*/
  info->used_block_nr++;
  info->used_bytes += iter->size;

  return (char*)iter + sizeof(uint32_t);
}

static void node_merge2(mem_info_t* info, free_node_t* prev, free_node_t* next) {
  return_if_fail(prev != NULL && next != NULL && prev->next == next);

  prev->next = next->next;
  if (next->next != NULL) {
    next->next->prev = prev;
  }
  prev->size += next->size;

  if (info->free_list == next) {
    info->free_list = prev;
  }

  return;
}

static void node_merge(mem_info_t* info, free_node_t* iter) {
  free_node_t* prev = iter->prev;
  free_node_t* next = iter->next;

  if (prev != NULL && ((char*)prev + prev->size) == (char*)iter) {
    node_merge2(info, prev, iter);
    node_merge(info, prev);
  } else if (next != NULL && ((char*)iter + iter->size) == (char*)next) {
    node_merge2(info, iter, next);
    node_merge(info, iter);
  }

  return;
}

static void tk_free_impl(mem_allocator_t* allocator, void* ptr) {
  uint32_t size = 0;
  free_node_t* iter = NULL;
  free_node_t* free_iter = NULL;
  mem_info_t* info = &(MEM_ALLOCATOR_SIMPLE(allocator)->info);

  return_if_fail(ptr != NULL);

  free_iter = (free_node_t*)((char*)ptr - sizeof(uint32_t));

  size = free_iter->size;
  free_iter->prev = NULL;
  free_iter->next = NULL;

  if (info->free_list == NULL) {
    info->free_list = free_iter;

    return;
  }
  /*根据内存块地址的大小，把它插入到适当的位置。*/
  for (iter = info->free_list; iter != NULL; iter = iter->next) {
    if ((char*)iter > (char*)free_iter) {
      free_iter->next = iter;
      free_iter->prev = iter->prev;
      if (iter->prev != NULL) {
        iter->prev->next = free_iter;
      }
      iter->prev = free_iter;

      if (info->free_list == iter) {
        info->free_list = free_iter;
      }

      break;
    }

    if (iter->next == NULL) {
      iter->next = free_iter;
      free_iter->prev = iter;

      break;
    }
  }

  /*对相邻居的内存进行合并*/
  node_merge(info, free_iter);
  info->used_block_nr--;
  info->used_bytes -= size;

  return;
}

static void* tk_realloc_impl(mem_allocator_t* allocator, void* ptr, uint32_t size) {
  void* new_ptr = NULL;
  if (ptr != NULL) {
    uint32_t old_size = *(uint32_t*)((char*)ptr - sizeof(uint32_t)) - sizeof(uint32_t);
    if (old_size >= size && old_size <= (size + MIN_SIZE)) {
      return ptr;
    }

    new_ptr = tk_alloc_impl(allocator, size);
    if (new_ptr != NULL) {
      memcpy(new_ptr, ptr, size < old_size ? size : old_size);
      tk_free_impl(allocator, ptr);
    }
  } else {
    new_ptr = tk_alloc_impl(allocator, size);
  }

  return new_ptr;
}

static ret_t tk_mem_init_impl(mem_allocator_t* allocator, void* buffer, uint32_t size) {
  mem_info_t* info = &(MEM_ALLOCATOR_SIMPLE(allocator)->info);
  return_value_if_fail(buffer != NULL && size > MIN_SIZE, RET_BAD_PARAMS);

  if (info->buffer != NULL) {
    return RET_FAIL;
  }

  memset(buffer, 0x00, size);
  info->buffer = (char*)buffer;
  info->size = size;
  info->free_list = (free_node_t*)buffer;
  info->free_list->prev = NULL;
  info->free_list->next = NULL;
  info->free_list->size = size;
  info->used_block_nr = 0;

  return RET_OK;
}

static inline void* mem_allocator_simple_alloc(mem_allocator_t* allocator, uint32_t size,
                                               const char* func, uint32_t line) {
  return tk_alloc_impl(allocator, size);
}

static inline void* mem_allocator_simple_realloc(mem_allocator_t* allocator, void* ptr,
                                                 uint32_t size, const char* func, uint32_t line) {
  return tk_realloc_impl(allocator, ptr, size);
}

static inline void mem_allocator_simple_free(mem_allocator_t* allocator, void* ptr) {
  tk_free_impl(allocator, ptr);
}

static inline ret_t mem_allocator_simple_dump(mem_allocator_t* allocator) {
  mem_info_t* info = &(MEM_ALLOCATOR_SIMPLE(allocator)->info);
  log_debug("used: %d bytes %d blocks\n", info->used_bytes, info->used_block_nr);
  (void)info;

  return RET_OK;
}

static inline ret_t mem_allocator_simple_destroy(mem_allocator_t* allocator) {
  allocator->vt = NULL;
  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_simple_vtable = {
    .alloc = mem_allocator_simple_alloc,
    .realloc = mem_allocator_simple_realloc,
    .free = mem_allocator_simple_free,
    .dump = mem_allocator_simple_dump,
    .destroy = mem_allocator_simple_destroy};

static inline mem_allocator_t* mem_allocator_simple_init(mem_allocator_simple_t* allocator_simple,
                                                         void* buffer, uint32_t size) {
  mem_allocator_t* allocator = MEM_ALLOCATOR(allocator_simple);
  return_value_if_fail(allocator != NULL, NULL);

  memset(allocator_simple, 0x00, sizeof(mem_allocator_simple_t));

  allocator->vt = &s_mem_allocator_simple_vtable;
  return_value_if_fail(tk_mem_init_impl(allocator, buffer, size) == RET_OK, NULL);

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_SIMPLE_H*/
