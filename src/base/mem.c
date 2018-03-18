/**
 * File:   mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  simple memory manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"

#ifdef HAS_STD_MALLOC
ret_t mem_init(void* buffer, uint32_t length) {
  (void)buffer;
  (void)length;
  return RET_OK;
}

mem_stat_t mem_stat(void) {
  mem_stat_t stat;
  memset(&stat, 0x00, sizeof(stat));
  return stat;
}

void mem_info_dump(void) {}

#else
typedef struct _free_node_t {
  uint32_t length;
  struct _free_node_t* next;
  struct _free_node_t* prev;
} free_node_t;

typedef struct _mem_info_t {
  char* buffer;
  uint32_t length;
  uint32_t used_block_nr;
  free_node_t* free_list;
} mem_info_t;

#define R8B(size) (((size + 7) >> 3) << 3)
#define MIN_SIZE R8B(sizeof(free_node_t))
#define REAL_SIZE(size) R8B((size > sizeof(free_node_t) ? size : MIN_SIZE) + sizeof(uint32_t));

static mem_info_t mem_info;

void* lftk_calloc(uint32_t nmemb, uint32_t size) {
  uint32_t length = nmemb * size;
  char* ptr = lftk_alloc(length);

  if (ptr != NULL) {
    memset(ptr, 0x00, length);
  }

  return ptr;
}

void* lftk_alloc(uint32_t size) {
  free_node_t* iter = NULL;
  uint32_t length = REAL_SIZE(size);

  /*查找第一个满足条件的空闲块*/
  for (iter = mem_info.free_list; iter != NULL; iter = iter->next) {
    if (iter->length > length) {
      break;
    }
  }

  if (iter == NULL) {
    log_debug("%s: Out of memory(%d):\n", __func__, size);
    mem_info_dump();
  }

  return_value_if_fail(iter != NULL, NULL);

  /*如果找到的空闲块刚好满足需求，就从空闲块链表中移出它*/
  if (iter->length < (length + MIN_SIZE)) {
    if (mem_info.free_list == iter) {
      mem_info.free_list = iter->next;
    }

    if (iter->prev != NULL) {
      iter->prev->next = iter->next;
    }
    if (iter->next != NULL) {
      iter->next->prev = iter->prev;
    }
  } else {
    /*如果找到的空闲块比较大，就把它拆成两个块，把多余的空闲内存放回去*/
    free_node_t* new_iter = (free_node_t*)((char*)iter + length);

    new_iter->length = iter->length - length;
    new_iter->next = iter->next;
    new_iter->prev = iter->prev;

    if (iter->prev != NULL) {
      iter->prev->next = new_iter;
    }
    if (iter->next != NULL) {
      iter->next->prev = new_iter;
    }

    if (mem_info.free_list == iter) {
      mem_info.free_list = new_iter;
    }
    iter->length = length;
  }
  /*返回可用的内存*/
  mem_info.used_block_nr++;

  return (char*)iter + sizeof(uint32_t);
}

static void node_merge2(free_node_t* prev, free_node_t* next) {
  return_if_fail(prev != NULL && next != NULL && prev->next == next);

  prev->next = next->next;
  if (next->next != NULL) {
    next->next->prev = prev;
  }
  prev->length += next->length;

  if (mem_info.free_list == next) {
    mem_info.free_list = prev;
  }

  return;
}

static void node_merge(free_node_t* iter) {
  free_node_t* prev = iter->prev;
  free_node_t* next = iter->next;

  if (prev != NULL && ((char*)prev + prev->length) == (char*)iter) {
    node_merge2(prev, iter);
    node_merge(prev);
  }

  if (next != NULL && ((char*)iter + iter->length) == (char*)next) {
    node_merge2(iter, next);
    node_merge(iter);
  }

  return;
}

void lftk_free(void* ptr) {
  free_node_t* iter = NULL;
  free_node_t* free_iter = NULL;

  return_if_fail(ptr != NULL);

  free_iter = (free_node_t*)((char*)ptr - sizeof(uint32_t));

  free_iter->prev = NULL;
  free_iter->next = NULL;

  if (mem_info.free_list == NULL) {
    mem_info.free_list = free_iter;

    return;
  }
  /*根据内存块地址的大小，把它插入到适当的位置。*/
  for (iter = mem_info.free_list; iter != NULL; iter = iter->next) {
    if ((char*)iter > (char*)free_iter) {
      free_iter->next = iter;
      free_iter->prev = iter->prev;
      if (iter->prev != NULL) {
        iter->prev->next = free_iter;
      }
      iter->prev = free_iter;

      if (mem_info.free_list == iter) {
        mem_info.free_list = free_iter;
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
  node_merge(free_iter);
  mem_info.used_block_nr--;

  return;
}

void* lftk_realloc(void* ptr, uint32_t size) {
  void* new_ptr = NULL;

  if (ptr != NULL) {
    uint32_t old_size = *(uint32_t*)((char*)ptr - sizeof(uint32_t)) - sizeof(uint32_t);
    if (old_size >= size && old_size <= (size + MIN_SIZE)) {
      return ptr;
    }

    new_ptr = lftk_alloc(size);
    if (new_ptr != NULL) {
      memcpy(new_ptr, ptr, size < old_size ? size : old_size);
      lftk_free(ptr);
    }
  } else {
    new_ptr = lftk_alloc(size);
  }

  return new_ptr;
}

ret_t mem_init(void* buffer, uint32_t length) {
  return_value_if_fail(buffer != NULL && length > MIN_SIZE, RET_BAD_PARAMS);

  mem_info.buffer = buffer;
  mem_info.length = length;
  mem_info.free_list = (free_node_t*)buffer;
  mem_info.free_list->prev = NULL;
  mem_info.free_list->next = NULL;
  mem_info.free_list->length = length;
  mem_info.used_block_nr = 0;

  return RET_OK;
}

void mem_info_dump() {
  int32_t i = 0;
  free_node_t* iter = NULL;
  mem_stat_t st = mem_stat();

  for (iter = mem_info.free_list; iter != NULL; iter = iter->next, i++) {
    log_debug("[%d] %p %d\n", i, iter, iter->length);
  }

  log_debug("total=%d used=%d free=%d free_block_nr=%d used_block_nr=%d\n", st.total, st.used,
            st.free, st.free_block_nr, st.used_block_nr);
  return;
}

mem_stat_t mem_stat() {
  mem_stat_t st;
  free_node_t* iter = NULL;

  memset(&st, 0x00, sizeof(st));

  st.total = mem_info.length;
  for (iter = mem_info.free_list; iter != NULL; iter = iter->next) {
    st.free += iter->length;
    st.free_block_nr++;
  }

  st.used = st.total - st.free;
  st.used_block_nr = mem_info.used_block_nr;

  return st;
}
#endif
