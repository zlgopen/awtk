/**
 * File:   mem.c
 * Author: AWTK Develop Team
 * Brief:  simple memory manager
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/time.h"

#ifdef HAS_STD_MALLOC
ret_t tk_mem_init(void* buffer, uint32_t length) {
  (void)buffer;
  (void)length;
  return RET_OK;
}

mem_stat_t tk_mem_stat(void) {
  mem_stat_t stat;
  memset(&stat, 0x00, sizeof(stat));
  return stat;
}

void tk_mem_info_dump(void) {
}

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

static mem_info_t s_mem_info;

#ifdef MEM_DEBUG
static void check_node(free_node_t* node) {
  if (node->prev) {
    assert(node->prev->next == node);
  }

  if (node->next) {
    assert(node->next->prev == node);
  }
}

static void check_all_nodes() {
  free_node_t* iter = NULL;
  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next) {
    check_node(iter);
  }
}

#define END_MARK 0xefefefef
#define END_MARK_SIZE sizeof(uint32_t);
static void mem_check_end_mark(free_node_t* iter) {
  char* mark_addr = (char*)iter + iter->length - sizeof(uint32_t);
  uint32_t mark = *((uint32_t*)mark_addr);
  assert(mark == END_MARK);
}

static void mem_set_end_mark(free_node_t* iter) {
  char* mark_addr = (char*)iter + iter->length - sizeof(uint32_t);
  *((uint32_t*)mark_addr) = END_MARK;
  mem_check_end_mark(iter);
}
#else
#define END_MARK_SIZE sizeof(uint32_t)
#define mem_check_end_mark(iter) (void)iter;
#define mem_set_end_mark(iter) (void)iter;
#define check_all_nodes()
#endif /**/

void* tk_calloc(uint32_t nmemb, uint32_t size) {
  uint32_t length = nmemb * size;
  char* ptr = tk_alloc(length);

  if (ptr != NULL) {
    memset(ptr, 0x00, length);
  }

  return ptr;
}

void* tk_alloc(uint32_t size) {
  free_node_t* iter = NULL;
  uint32_t length = REAL_SIZE(size);

  length += END_MARK_SIZE;

  /*查找第一个满足条件的空闲块*/
  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next) {
    if (iter->length > length) {
      break;
    }
  }

  if (iter == NULL) {
    log_debug("%s: Out of memory(%d):\n", __FUNCTION__, (int)size);
    tk_mem_info_dump();
  }

  return_value_if_fail(iter != NULL, NULL);

  /*如果找到的空闲块刚好满足需求，就从空闲块链表中移出它*/
  if (iter->length < (length + MIN_SIZE)) {
    if (s_mem_info.free_list == iter) {
      s_mem_info.free_list = iter->next;
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

    if (s_mem_info.free_list == iter) {
      s_mem_info.free_list = new_iter;
    }
    iter->length = length;
  }
  /*返回可用的内存*/
  s_mem_info.used_block_nr++;

  check_all_nodes();
  mem_set_end_mark(iter);

  return (char*)iter + sizeof(uint32_t);
}

static void node_merge2(free_node_t* prev, free_node_t* next) {
  return_if_fail(prev != NULL && next != NULL && prev->next == next);

  prev->next = next->next;
  if (next->next != NULL) {
    next->next->prev = prev;
  }
  prev->length += next->length;

  if (s_mem_info.free_list == next) {
    s_mem_info.free_list = prev;
  }

  return;
}

static void node_merge(free_node_t* iter) {
  free_node_t* prev = iter->prev;
  free_node_t* next = iter->next;

  if (prev != NULL && ((char*)prev + prev->length) == (char*)iter) {
    node_merge2(prev, iter);
    node_merge(prev);
  } else if (next != NULL && ((char*)iter + iter->length) == (char*)next) {
    node_merge2(iter, next);
    node_merge(iter);
  }

  return;
}

void tk_free(void* ptr) {
  free_node_t* iter = NULL;
  free_node_t* free_iter = NULL;

  return_if_fail(ptr != NULL);

  free_iter = (free_node_t*)((char*)ptr - sizeof(uint32_t));

  free_iter->prev = NULL;
  free_iter->next = NULL;

  check_all_nodes();
  mem_check_end_mark(free_iter);

  if (s_mem_info.free_list == NULL) {
    s_mem_info.free_list = free_iter;

    return;
  }
  /*根据内存块地址的大小，把它插入到适当的位置。*/
  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next) {
    if ((char*)iter > (char*)free_iter) {
      free_iter->next = iter;
      free_iter->prev = iter->prev;
      if (iter->prev != NULL) {
        iter->prev->next = free_iter;
      }
      iter->prev = free_iter;

      if (s_mem_info.free_list == iter) {
        s_mem_info.free_list = free_iter;
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
  s_mem_info.used_block_nr--;
  check_all_nodes();

  return;
}

void* tk_realloc(void* ptr, uint32_t size) {
  void* new_ptr = NULL;

  if (ptr != NULL) {
    uint32_t old_size = *(uint32_t*)((char*)ptr - sizeof(uint32_t)) - sizeof(uint32_t);
    if (old_size >= size && old_size <= (size + MIN_SIZE)) {
      return ptr;
    }

    new_ptr = tk_alloc(size);
    if (new_ptr != NULL) {
      memcpy(new_ptr, ptr, size < old_size ? size : old_size);
      tk_free(ptr);
    }
  } else {
    new_ptr = tk_alloc(size);
  }

  return new_ptr;
}

ret_t tk_mem_init(void* buffer, uint32_t length) {
  return_value_if_fail(buffer != NULL && length > MIN_SIZE, RET_BAD_PARAMS);

  memset(buffer, 0x00, length);
  s_mem_info.buffer = buffer;
  s_mem_info.length = length;
  s_mem_info.free_list = (free_node_t*)buffer;
  s_mem_info.free_list->prev = NULL;
  s_mem_info.free_list->next = NULL;
  s_mem_info.free_list->length = length;
  s_mem_info.used_block_nr = 0;

  return RET_OK;
}

void tk_mem_info_dump() {
  int32_t i = 0;
  free_node_t* iter = NULL;
  mem_stat_t st = tk_mem_stat();

  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next, i++) {
    log_debug("[%d] %p %d\n", (int)i, iter, (int)(iter->length));
  }

  log_debug("total=%d used=%d free=%d free_block_nr=%d used_block_nr=%d\n", (int)(st.total),
            (int)(st.used), (int)(st.free), (int)(st.free_block_nr), (int)(st.used_block_nr));
  return;
}

mem_stat_t tk_mem_stat() {
  mem_stat_t st;
  free_node_t* iter = NULL;

  memset(&st, 0x00, sizeof(st));

  st.total = s_mem_info.length;
  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next) {
    st.free += iter->length;
    st.free_block_nr++;
  }

  st.used = st.total - st.free;
  st.used_block_nr = s_mem_info.used_block_nr;

  return st;
}

/*export std malloc*/
void* calloc(size_t count, size_t size) {
  size_t s = count * size;
  void* p = tk_alloc(s);
  if (p != NULL) {
    memset(p, 0x00, s);
  }

  return p;
}

void free(void* ptr) {
  tk_free(ptr);
}

void* malloc(size_t size) {
  return tk_alloc(size);
}

void* realloc(void* ptr, size_t size) {
  return tk_realloc(ptr, size);
}
#endif /*HAS_STD_MALLOC*/

uint32_t tk_mem_speed_test(void* buffer, uint32_t length, uint32_t* pmemcpy_speed,
                           uint32_t* pmemset_speed) {
  uint32_t i = 0;
  uint32_t cost = 0;
  uint32_t total_cost = 0;
  uint32_t memcpy_speed;
  uint32_t memset_speed;
  uint32_t max_size = 100 * 1024 * 1024;
  uint32_t start = time_now_ms();
  uint32_t nr = max_size / length;

  for (i = 0; i < nr; i++) {
    memset(buffer, 0x00, length);
  }
  cost = time_now_ms() - start;
  total_cost = cost;
  if (cost) {
    memset_speed = ((max_size / cost) * 1000) / 1024;
  }

  start = time_now_ms();
  for (i = 0; i < nr; i++) {
    uint32_t half = length >> 1;
    memcpy(buffer, (char*)buffer + half, half);
    memcpy((char*)buffer + half, buffer, half);
  }
  cost = time_now_ms() - start;
  total_cost += cost;

  if (cost) {
    memcpy_speed = ((max_size / cost) * 1000) / 1024;
  }

  if (pmemset_speed != NULL) {
    *pmemset_speed = memset_speed;
  }

  if (pmemcpy_speed != NULL) {
    *pmemcpy_speed = memcpy_speed;
  }

  return total_cost;
}
