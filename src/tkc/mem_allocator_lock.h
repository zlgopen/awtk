/**
 * File:   mem_allocator_lock.h
 * Author: AWTK Develop Team
 * Brief:  mem_allocator_lock
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

#ifndef TK_MEM_ALLOCATOR_LOCK_H
#define TK_MEM_ALLOCATOR_LOCK_H

#include "tkc/mutex.h"
#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

/**
 * @class mem_allocator_lock_t 
 * @parent mem_allocator_t 
 * 
 * 对现有的allocator进行包装，提供互斥功能。
 * 
 */
typedef struct _mem_allocator_lock_t {
  mem_allocator_t allocator;
  mem_allocator_t* impl;
  tk_mutex_t* mutex;
} mem_allocator_lock_t;

#define MEM_ALLOCATOR_LOCK(allocator) ((mem_allocator_lock_t*)(allocator))

static inline void* mem_allocator_lock_alloc(mem_allocator_t* allocator, uint32_t size,
                                             const char* func, uint32_t line) {
  void* addr = NULL;
  tk_mutex_t* mutex = MEM_ALLOCATOR_LOCK(allocator)->mutex;
  mem_allocator_t* impl = MEM_ALLOCATOR_LOCK(allocator)->impl;

  if (tk_mutex_lock(mutex) == RET_OK) {
    addr = mem_allocator_alloc(impl, size, func, line);
    ENSURE(tk_mutex_unlock(mutex) == RET_OK);
  }

  return addr;
}

static inline void* mem_allocator_lock_realloc(mem_allocator_t* allocator, void* ptr, uint32_t size,
                                               const char* func, uint32_t line) {
  void* addr = NULL;
  tk_mutex_t* mutex = MEM_ALLOCATOR_LOCK(allocator)->mutex;
  mem_allocator_t* impl = MEM_ALLOCATOR_LOCK(allocator)->impl;

  if (tk_mutex_lock(mutex) == RET_OK) {
    addr = mem_allocator_realloc(impl, ptr, size, func, line);
    ENSURE(tk_mutex_unlock(mutex) == RET_OK);
  }

  return addr;
}

static inline void mem_allocator_lock_free(mem_allocator_t* allocator, void* ptr) {
  tk_mutex_t* mutex = MEM_ALLOCATOR_LOCK(allocator)->mutex;
  mem_allocator_t* impl = MEM_ALLOCATOR_LOCK(allocator)->impl;

  if (tk_mutex_lock(mutex) == RET_OK) {
    mem_allocator_free(impl, ptr);
    ENSURE(tk_mutex_unlock(mutex) == RET_OK);
  }
}

static inline ret_t mem_allocator_lock_dump(mem_allocator_t* allocator) {
  tk_mutex_t* mutex = MEM_ALLOCATOR_LOCK(allocator)->mutex;
  mem_allocator_t* impl = MEM_ALLOCATOR_LOCK(allocator)->impl;

  if (tk_mutex_lock(mutex) == RET_OK) {
    mem_allocator_dump(impl);
    ENSURE(tk_mutex_unlock(mutex) == RET_OK);
  }

  return RET_OK;
}

static inline ret_t mem_allocator_lock_destroy(mem_allocator_t* allocator) {
  tk_mutex_t* mutex = MEM_ALLOCATOR_LOCK(allocator)->mutex;
  mem_allocator_t* impl = MEM_ALLOCATOR_LOCK(allocator)->impl;
  mem_allocator_destroy(impl);
  tk_mutex_destroy(mutex);
  allocator->vt = NULL;

  return RET_OK;
}

static const mem_allocator_vtable_t s_mem_allocator_lock_vtable = {
    .alloc = mem_allocator_lock_alloc,
    .realloc = mem_allocator_lock_realloc,
    .free = mem_allocator_lock_free,
    .dump = mem_allocator_lock_dump,
    .destroy = mem_allocator_lock_destroy};

static inline mem_allocator_t* mem_allocator_lock_init(mem_allocator_lock_t* lock,
                                                       mem_allocator_t* impl) {
  mem_allocator_t* allocator = MEM_ALLOCATOR(lock);
  return_value_if_fail(impl != NULL && lock != NULL, NULL);

  memset(lock, 0x00, sizeof(*lock));
  allocator->vt = &s_mem_allocator_lock_vtable;
  lock->mutex = tk_mutex_create();
  lock->impl = impl;

  return allocator;
}

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_LOCK_H*/
