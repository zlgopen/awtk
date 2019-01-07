/**
 * File:   thread.c
 * Author: AWTK Develop Team
 * Brief:  thread
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/thread.h"

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#define thread_handle_t HANDLE
#elif defined(HAS_PTHREAD)
#include "pthread.h"
#define thread_handle_t pthread_t
#else
#define thread_handle_t int
#endif

struct _thread_t {
  void* args;
  bool_t running;
  thread_handle_t thread;
  thread_entry_t entry;
};

thread_t* thread_create(thread_entry_t entry, void* args) {
  thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (thread_t*)TKMEM_ZALLOC(thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;

  return thread;
}

#ifdef WIN32
static unsigned __stdcall entry(void* arg) {
#elif defined(HAS_PTHREAD)
static void* entry(void* arg) {
#else
static void* entry(void* arg) {
#endif
  thread_t* thread = (thread_t*)arg;

  thread->entry(thread->args);
  thread->running = FALSE;
#ifdef WIN32
  return 0;
#elif defined(HAS_PTHREAD)
  return NULL;
#else
  return NULL;
#endif
}

ret_t thread_start(thread_t* thread) {
#ifdef WIN32
  unsigned h = 0;
  uint32_t stack_size = 1024 * 1024;
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  stack_size = 1024 * 1024;
  thread->thread = (HANDLE)_beginthreadex(NULL, 0, entry, thread, 0, &h);
  thread->running = thread->thread != NULL;
#elif defined(HAS_PTHREAD)
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  int ret = pthread_create(&(thread->thread), NULL, entry, thread);
  thread->running = ret == 0;
#else
  entry(thread);
  thread->running = FALSE;
#endif

  return thread->running ? RET_OK : RET_FAIL;
}

ret_t thread_join(thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  if (thread->running) {
#ifdef WIN32
    WaitForSingleObject(thread->thread, INFINITE);
#elif defined(HAS_PTHREAD)
    void* ret = NULL;
    if (thread->thread) {
      pthread_join(thread->thread, &ret);
    }
#endif
  }

  return RET_OK;
}

ret_t thread_destroy(thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  memset(thread, 0x00, sizeof(thread_t));
  TKMEM_FREE(thread);

  return RET_OK;
}
