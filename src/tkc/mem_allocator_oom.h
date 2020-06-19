/**
 * File:   mem_allocator_oom.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_oom
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

#ifndef TK_MEM_ALLOCATOR_OOM_H
#define TK_MEM_ALLOCATOR_OOM_H

#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

/**
 * @class mem_allocator_oom_t 
 * @parent mem_allocator_t 
 * 
 * 对现有的allocator进行包装，如果分配内存失败，调用预先设置的回调函数释放内存，然后再重试。
 * 
 */
typedef struct _mem_allocator_oom_t {
  mem_allocator_t allocator;
  mem_allocator_t* impl;
} mem_allocator_oom_t;

#define MEM_ALLOCATOR_OOM(allocator) ((mem_allocator_oom_t*)(allocator))

#define TRY_MAX_TIMES 5

static void* s_on_out_of_memory_ctx;
static tk_mem_on_out_of_memory_t s_on_out_of_memory;

static ret_t tk_mem_on_out_of_memory(uint32_t tried_times, uint32_t need_size, const char* func,
                                     uint32_t line) {
  log_debug("oom %s:%u times=%u size=%u\n", func, line, tried_times, need_size);

  if (s_on_out_of_memory != NULL) {
    return s_on_out_of_memory(s_on_out_of_memory_ctx, tried_times, need_size);
  }

  return RET_NOT_IMPL;
}

ret_t tk_mem_set_on_out_of_memory(tk_mem_on_out_of_memory_t on_out_of_memory, void* ctx) {
  s_on_out_of_memory = on_out_of_memory;
  s_on_out_of_memory_ctx = ctx;

  return RET_OK;
}

static inline void* mem_allocator_oom_alloc(mem_allocator_t* allocator, uint32_t size,
                                            const char* func, uint32_t line) {
  void* addr = NULL;
  uint32_t tried_times = 0;
  mem_allocator_t* impl = MEM_ALLOCATOR_OOM(allocator)->impl;

  do {
    addr = mem_allocator_alloc(impl, size, func, line);
    if (addr != NULL) {
      break;
    }

    tk_mem_on_out_of_memory(++tried_times, size, func, line);
  } while (tried_times < TRY_MAX_TIMES);

  return addr;
}

static inline void* mem_allocator_oom_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                              const char* func, uint32_t line) {
  void* addr = NULL;
  uint32_t tried_times = 0;
  mem_allocator_t* impl = MEM_ALLOCATOR_OOM(allocator)->impl;

  do {
    addr = mem_allocator_realloc(impl, ptr, size, func, line);
    if (addr != NULL) {
      break;
    }

    tk_mem_on_out_of_memory(++tried_times, size, func, line);
  } while (tried_times < TRY_MAX_TIMES);

  return addr;
}

static inline void mem_allocator_oom_free(mem_allocator_t* allocator, void* ptr) {
  mem_allocator_t* impl = MEM_ALLOCATOR_OOM(allocator)->impl;
  if (ptr != NULL) {
    mem_allocator_free(impl, ptr);
  }
}

static inline ret_t mem_allocator_oom_dump(mem_allocator_t* allocator) {
  mem_allocator_t* impl = MEM_ALLOCATOR_OOM(allocator)->impl;
  mem_allocator_dump(impl);

  return RET_OK;
}

static inline ret_t mem_allocator_oom_destroy(mem_allocator_t* allocator) {
  mem_allocator_t* impl = MEM_ALLOCATOR_OOM(allocator)->impl;
  mem_allocator_destroy(impl);
  allocator->vt = NULL;

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_oom_vtable = {
    .alloc = mem_allocator_oom_alloc,
    .realloc = mem_allocator_oom_realloc,
    .free = mem_allocator_oom_free,
    .dump = mem_allocator_oom_dump,
    .destroy = mem_allocator_oom_destroy};

static inline mem_allocator_t* mem_allocator_oom_init(mem_allocator_oom_t* oom,
                                                      mem_allocator_t* impl) {
  mem_allocator_t* allocator = MEM_ALLOCATOR(oom);
  return_value_if_fail(impl != NULL && oom != NULL, NULL);

  memset(oom, 0x00, sizeof(*oom));
  allocator->vt = &s_mem_allocator_oom_vtable;
  oom->impl = impl;

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_OOM_H*/
