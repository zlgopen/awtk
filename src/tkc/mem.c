/**
 * File:   mem.c
 * Author: AWTK Develop Team
 * Brief:  simple memory manager
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/mem_allocator_oom.h"
#include "tkc/mem_allocator_pool.h"

static mem_allocator_oom_t s_oom;

#ifdef ENABLE_MEM_LEAK_CHECK
#include "tkc/mem_allocator_debug.h"
static mem_allocator_debug_t s_debug;
#endif /*ENABLE_MEM_LEAK_CHECK*/

static mem_allocator_t* s_allocator = NULL;

#define MAX_BLOCK_SIZE 0xffff0000

#ifdef HAS_STD_MALLOC
#include "tkc/mem_allocator_std.h"

bool_t tk_mem_is_valid_addr(void* addr) {
  return ((uint64_t)addr > 0x10000);
}

static mem_allocator_t* mem_allocator_get(void) {
  static mem_allocator_t std;
  if (s_allocator != NULL) {
    return s_allocator;
  }

  s_allocator = mem_allocator_std_init(&std);
#ifdef ENABLE_MEM_LEAK_CHECK
  s_allocator = mem_allocator_debug_init(&s_debug, s_allocator);
#endif /*ENABLE_MEM_LEAK_CHECK*/
  s_allocator = mem_allocator_oom_init(&s_oom, s_allocator);

  return s_allocator;
}

ret_t tk_mem_init_stage2(void) {
  return RET_OK;
}

#else /*non std memory manager*/
#include "tkc/mem_allocator_lock.h"
#include "tkc/mem_allocator_simple.h"

static mem_allocator_lock_t s_lock;

static void* s_heap_start = NULL;
static uint32_t s_heap_size = 0;

bool_t tk_mem_is_valid_addr(void* addr) {
  uint64_t start = (uint64_t)s_heap_start;
  uint64_t end = start + s_heap_size;

  return (((uint64_t)addr >= (uint64_t)start) && ((uint64_t)addr < end));
}

ret_t tk_mem_init(void* buffer, uint32_t size) {
  static mem_allocator_simple_t simple;
  static mem_allocator_pool_t pool;

  s_heap_size = size;
  s_heap_start = buffer;

  s_allocator = mem_allocator_simple_init(&simple, buffer, size);
  if (size < 100 * 1024) {
    s_allocator = mem_allocator_pool_init(&pool, s_allocator, 100, 100, 80, 80, 32);
  } else if (size < 1000 * 1024) {
    s_allocator = mem_allocator_pool_init(&pool, s_allocator, 500, 500, 500, 200, 200);
  } else {
    s_allocator = mem_allocator_pool_init(&pool, s_allocator, 1000, 1000, 1000, 500, 500);
  }
#ifdef ENABLE_MEM_LEAK_CHECK
  s_allocator = mem_allocator_debug_init(&s_debug, s_allocator);
#endif /*ENABLE_MEM_LEAK_CHECK*/

  return s_allocator != NULL ? RET_OK : RET_FAIL;
}

ret_t tk_mem_init_stage2(void) {
  return_value_if_fail(s_allocator != NULL, RET_FAIL);
  s_allocator = mem_allocator_lock_init(&s_lock, s_allocator);
  s_allocator = mem_allocator_oom_init(&s_oom, s_allocator);

  return RET_OK;
}

static mem_allocator_t* mem_allocator_get(void) {
  return s_allocator;
}

#ifndef WITH_SDL
/*export std malloc*/
void* calloc(size_t count, size_t size) {
  return tk_calloc(count, size, __FUNCTION__, __LINE__);
}

void free(void* ptr) {
  tk_free(ptr);
}

void* malloc(size_t size) {
  return tk_alloc(size, __FUNCTION__, __LINE__);
}

void* realloc(void* ptr, size_t size) {
  return tk_realloc(ptr, size, __FUNCTION__, __LINE__);
}
#endif /*WITH_SDL*/

#endif /*HAS_STD_MALLOC*/

void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line) {
  void* addr = NULL;
  uint32_t total_size = size * nmemb;
  mem_allocator_t* allocator = mem_allocator_get();
  return_value_if_fail(allocator != NULL, NULL);

  addr = mem_allocator_alloc(allocator, total_size, func, line);
  return_value_if_fail(addr != NULL, NULL);
  memset(addr, 0x00, total_size);

  return addr;
}

void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line) {
  mem_allocator_t* allocator = mem_allocator_get();
  return_value_if_fail(allocator != NULL, NULL);

  return mem_allocator_realloc(allocator, ptr, size, func, line);
}

void* tk_alloc(uint32_t size, const char* func, uint32_t line) {
  mem_allocator_t* allocator = mem_allocator_get();
  return_value_if_fail(allocator != NULL, NULL);

  return mem_allocator_alloc(allocator, size, func, line);
}

void tk_free(void* ptr) {
  mem_allocator_t* allocator = mem_allocator_get();
  return_if_fail(allocator != NULL);

  mem_allocator_free(allocator, ptr);
}

void tk_mem_dump(void) {
  mem_allocator_t* allocator = mem_allocator_get();
  return_if_fail(allocator != NULL);

  mem_allocator_dump(allocator);
}
