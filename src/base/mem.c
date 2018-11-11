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
#include "base/time_now.h"

static void tk_free_impl(void* ptr);
static void* tk_alloc_impl(uint32_t size);
static void* tk_realloc_impl(void* ptr, uint32_t size);
static void* tk_calloc_impl(uint32_t nmemb, uint32_t size);

#ifdef HAS_STD_MALLOC

#define MEM_MAGIC 0x11223344
typedef struct _mem_block_t {
  uint32_t magic;
  uint32_t size;
} mem_block_t;

static mem_stat_t s_mem_stat;
mem_stat_t tk_mem_stat(void) {
  return s_mem_stat;
}

ret_t tk_mem_init(void* buffer, uint32_t size) {
  (void)buffer;
  (void)size;

  return RET_OK;
}

static void* tk_alloc_impl(uint32_t size) {
  uint32_t s = size + sizeof(mem_block_t);
  void* ptr = malloc(s);

  if (ptr != NULL) {
    mem_block_t* head = (mem_block_t*)ptr;

    head->size = s;
    head->magic = MEM_MAGIC;

    s_mem_stat.used_bytes += head->size;
    s_mem_stat.used_block_nr++;
  }

  return (char*)ptr + sizeof(mem_block_t);
}

static void* tk_realloc_impl(void* ptr, uint32_t size) {
  if (ptr != NULL) {
    mem_block_t* head = (mem_block_t*)((char*)ptr - sizeof(mem_block_t));
    uint32_t old_size = head->size - sizeof(mem_block_t);

    if (size <= old_size) {
      return ptr;
    } else {
      void* newptr = tk_alloc_impl(size);
      if (newptr) {
        memcpy(newptr, ptr, tk_min(size, old_size));
        tk_free_impl(ptr);

        return newptr;
      } else {
        return ptr;
      }
    }

  } else {
    return tk_alloc_impl(size);
  }
}

static void tk_free_impl(void* ptr) {
  if (ptr != NULL) {
    mem_block_t* head = (mem_block_t*)((char*)ptr - sizeof(mem_block_t));

    assert(head->magic == MEM_MAGIC);
    s_mem_stat.used_bytes -= head->size;
    s_mem_stat.used_block_nr--;

    free(head);
  }
}

#else /*non std memory manager*/
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

#define R8B(size) (((size + 7) >> 3) << 3)
#define MIN_SIZE R8B(sizeof(free_node_t))
#define REAL_SIZE(size) R8B((size > sizeof(free_node_t) ? size : MIN_SIZE) + sizeof(uint32_t));

static mem_info_t s_mem_info;

static void* tk_alloc_impl(uint32_t s) {
  free_node_t* iter = NULL;
  uint32_t size = REAL_SIZE(s);

  /*查找第一个满足条件的空闲块*/
  for (iter = s_mem_info.free_list; iter != NULL; iter = iter->next) {
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

    if (s_mem_info.free_list == iter) {
      s_mem_info.free_list = new_iter;
    }
    iter->size = size;
  }
  /*返回可用的内存*/
  s_mem_info.used_block_nr++;
  s_mem_info.used_bytes += size;

  return (char*)iter + sizeof(uint32_t);
}

static void node_merge2(free_node_t* prev, free_node_t* next) {
  return_if_fail(prev != NULL && next != NULL && prev->next == next);

  prev->next = next->next;
  if (next->next != NULL) {
    next->next->prev = prev;
  }
  prev->size += next->size;

  if (s_mem_info.free_list == next) {
    s_mem_info.free_list = prev;
  }

  return;
}

static void node_merge(free_node_t* iter) {
  free_node_t* prev = iter->prev;
  free_node_t* next = iter->next;

  if (prev != NULL && ((char*)prev + prev->size) == (char*)iter) {
    node_merge2(prev, iter);
    node_merge(prev);
  } else if (next != NULL && ((char*)iter + iter->size) == (char*)next) {
    node_merge2(iter, next);
    node_merge(iter);
  }

  return;
}

static void tk_free_impl(void* ptr) {
  uint32_t size = 0;
  free_node_t* iter = NULL;
  free_node_t* free_iter = NULL;

  return_if_fail(ptr != NULL);

  free_iter = (free_node_t*)((char*)ptr - sizeof(uint32_t));

  size = free_iter->size;
  free_iter->prev = NULL;
  free_iter->next = NULL;

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
  s_mem_info.used_bytes -= size;

  return;
}

static void* tk_realloc_impl(void* ptr, uint32_t size) {
  void* new_ptr = NULL;

  if (ptr != NULL) {
    uint32_t old_size = *(uint32_t*)((char*)ptr - sizeof(uint32_t)) - sizeof(uint32_t);
    if (old_size >= size && old_size <= (size + MIN_SIZE)) {
      return ptr;
    }

    new_ptr = tk_alloc_impl(size);
    if (new_ptr != NULL) {
      memcpy(new_ptr, ptr, size < old_size ? size : old_size);
      tk_free_impl(ptr);
    }
  } else {
    new_ptr = tk_alloc_impl(size);
  }

  return new_ptr;
}

ret_t tk_mem_init(void* buffer, uint32_t size) {
  return_value_if_fail(buffer != NULL && size > MIN_SIZE, RET_BAD_PARAMS);

  memset(buffer, 0x00, size);
  s_mem_info.buffer = (char*)buffer;
  s_mem_info.size = size;
  s_mem_info.free_list = (free_node_t*)buffer;
  s_mem_info.free_list->prev = NULL;
  s_mem_info.free_list->next = NULL;
  s_mem_info.free_list->size = size;
  s_mem_info.used_block_nr = 0;

  return RET_OK;
}

mem_stat_t tk_mem_stat() {
  mem_stat_t st;

  st.used_bytes = s_mem_info.size;
  st.used_block_nr = s_mem_info.used_block_nr;

  return st;
}

/*export std malloc*/
void* calloc(size_t count, size_t size) {
  return tk_calloc_impl(count, size);
}

void free(void* ptr) {
  tk_free_impl(ptr);
}

void* malloc(size_t size) {
  return tk_alloc_impl(size);
}

void* realloc(void* ptr, size_t size) {
  return tk_realloc_impl(ptr, size);
}
#endif /*HAS_STD_MALLOC*/

static void* tk_calloc_impl(uint32_t nmemb, uint32_t s) {
  uint32_t size = nmemb * s;
  void* ptr = tk_alloc_impl(size);

  if (ptr != NULL) {
    memset(ptr, 0x00, size);
  }

  return ptr;
}

#ifdef ENABLE_MEM_LEAK_CHECK
#include "base/time_now.h"

typedef struct _mem_record_t {
  void* ptr;
  const char* func;
  uint32_t line;
  uint32_t size;
  uint32_t time;
} mem_record_t;

#ifndef MEM_MAX_RECORDS
#define MEM_MAX_RECORDS 4 * 1024
#endif /*MEM_MAX_RECORDS*/

static mem_record_t s_records[MEM_MAX_RECORDS];

static void tk_remove_record(void* ptr) {
  uint32_t i = 0;

  for (i = 0; i < MEM_MAX_RECORDS; i++) {
    mem_record_t* r = (mem_record_t*)s_records + i;

    if (r->ptr == ptr) {
      memset(r, 0x00, sizeof(mem_record_t));
      break;
    }
  }
}

static void* tk_add_record(void* ptr, uint32_t size, const char* func, uint32_t line) {
  uint32_t i = 0;

  for (i = 0; i < MEM_MAX_RECORDS; i++) {
    mem_record_t* r = (mem_record_t*)s_records + i;

    if (r->ptr == NULL) {
      r->ptr = ptr;
      r->func = func;
      r->line = line;
      r->size = size;
      r->time = time_now_s();

      break;
    }
  }

  return ptr;
}

void tk_mem_dump(void) {
  uint32_t i = 0;
  uint32_t size = 0;
  uint32_t big_size = 10240;
  uint32_t time = time_now_s();
  mem_stat_t s = tk_mem_stat();

  log_debug("===============large=========================\n");
  for (i = 0; i < MEM_MAX_RECORDS; i++) {
    mem_record_t* r = (mem_record_t*)s_records + i;

    if (r->ptr != NULL) {
      if (r->size >= big_size) {
        log_debug("%p size=%u time=%d, %s:%u\n", r->ptr, r->size, r->time, r->func, r->line);
        size += r->size;
      }
    }
  }
  log_debug("large blocks size: %d\n", size);
  log_debug("================small========================\n");
  for (i = 0, size = 0; i < MEM_MAX_RECORDS; i++) {
    mem_record_t* r = (mem_record_t*)s_records + i;

    if (r->ptr != NULL) {
      if (r->size < big_size) {
        log_debug("%p size=%u time=%d, %s:%u\n", r->ptr, r->size, r->time, r->func, r->line);
        size += r->size;
      }
    }
  }
  log_debug("small blocks size: %d\n", size);

  log_debug("================recent========================\n");
  for (i = 0, size = 0; i < MEM_MAX_RECORDS; i++) {
    mem_record_t* r = (mem_record_t*)s_records + i;

    if (r->ptr != NULL) {
      if ((time - r->time) < 10) {
        log_debug("%p size=%u time=%d, %s:%u\n", r->ptr, r->size, r->time, r->func, r->line);
        size += r->size;
      }
    }
  }
  log_debug("recent blocks size: %d\n", size);

  log_debug("used: %d bytes %d blocks\n", s.used_bytes, s.used_block_nr);
}

void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line) {
  return tk_add_record(tk_calloc_impl(nmemb, size), nmemb * size, func, line);
}

void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line) {
  void* newptr = tk_realloc_impl(ptr, size);

  if (newptr != ptr) {
    tk_remove_record(ptr);
  }

  return tk_add_record(newptr, size, func, line);
}

void* tk_alloc(uint32_t size, const char* func, uint32_t line) {
  return tk_add_record(tk_alloc_impl(size), size, func, line);
}

void tk_free(void* ptr) {
  if (ptr != NULL) {
    tk_remove_record(ptr);
    tk_free_impl(ptr);
  }
}
#else
void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line) {
  (void)func;
  (void)line;

  return tk_calloc_impl(nmemb, size);
}

void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line) {
  (void)func;
  (void)line;

  return tk_realloc_impl(ptr, size);
}

void* tk_alloc(uint32_t size, const char* func, uint32_t line) {
  (void)func;
  (void)line;

  return tk_alloc_impl(size);
}

void tk_free(void* ptr) {
  if (ptr != NULL) {
    tk_free_impl(ptr);
  }
}

void tk_mem_dump(void) {
  mem_stat_t s = tk_mem_stat();
  log_debug("used: %d bytes %d blocks\n", s.used_bytes, s.used_block_nr);
}

#endif /*ENABLE_MEM_LEAK_CHECK*/
