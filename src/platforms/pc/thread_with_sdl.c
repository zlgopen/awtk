/**
 * File:   thread_with_sdl.c
 * Author: AWTK Develop Team
 * Brief:  sdl implemented thread related functions.
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#if defined(WITH_SDL) && !defined(HAS_PTHREAD)

#include "tkc/mem.h"
#include "tkc/mutex.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/thread.h"
#include "tkc/cond.h"
#include "tkc/semaphore.h"

#include "SDL.h"

struct _tk_mutex_t {
  SDL_mutex* mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);

  mutex->mutex = SDL_CreateMutex();
  if (mutex->mutex == NULL) {
    TKMEM_FREE(mutex);
  }

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  if (SDL_LockMutex(mutex->mutex) != 0) {
    log_debug("SDL_LockMutex fail\n");
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t tk_mutex_try_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  if (SDL_TryLockMutex(mutex->mutex) != 0) {
    log_debug("SDL_LockMutex fail\n");
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);
  if (SDL_UnlockMutex(mutex->mutex) != 0) {
    log_debug("SDL_UnlockMutex fail\n");
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  SDL_DestroyMutex(mutex->mutex);
  memset(mutex, 0x00, sizeof(tk_mutex_t));
  TKMEM_FREE(mutex);

  return RET_OK;
}

/********************************************************/

typedef struct _tk_cond_t {
  SDL_cond* cond;
} tk_cond_t;

tk_cond_t* tk_cond_create(void) {
  tk_cond_t* cond = (tk_cond_t*)TKMEM_ALLOC(sizeof(tk_cond_t));
  return_value_if_fail(cond != NULL, NULL);

  cond->cond = SDL_CreateCond();
  if (cond->cond == NULL) {
    TKMEM_FREE(cond);
  }

  return (cond);
}

ret_t tk_cond_destroy(tk_cond_t* cond) {
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  SDL_DestroyCond(cond->cond);
  memset(cond, 0x00, sizeof(tk_cond_t));
  TKMEM_FREE(cond);

  return RET_OK;
}

ret_t tk_cond_signal(tk_cond_t* cond) {
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  return SDL_CondSignal(cond->cond) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_cond_broadcast(tk_cond_t* cond) {
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  return SDL_CondBroadcast(cond->cond) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_cond_wait_timeout(tk_cond_t* cond, tk_mutex_t* mutex, uint32_t ms) {
  return_value_if_fail(cond != NULL && mutex != NULL, RET_BAD_PARAMS);

  return SDL_CondWaitTimeout(cond->cond, mutex->mutex, ms) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_cond_wait(tk_cond_t* cond, tk_mutex_t* mutex) {
  return_value_if_fail(cond != NULL && mutex != NULL, RET_BAD_PARAMS);

  return SDL_CondWait(cond->cond, mutex->mutex) == 0 ? RET_OK : RET_FAIL;
}

/********************************************************/

struct _tk_semaphore_t {
  SDL_sem* sem;
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  semaphore->sem = SDL_CreateSemaphore(value);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  int ret = 0;
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  ret = SDL_SemWaitTimeout(semaphore->sem, timeout_ms);
  if (ret == SDL_MUTEX_TIMEDOUT) {
    return RET_TIMEOUT;
  } else if (ret == 0) {
    return RET_OK;
  }

  return RET_FAIL;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return SDL_SemPost(semaphore->sem) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  SDL_DestroySemaphore(semaphore->sem);
  memset(semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}

/********************************************************/

struct _tk_thread_t {
  void* args;
  bool_t running;
  SDL_Thread* thread;
  tk_thread_entry_t entry;
  char name[TK_NAME_LEN + 1];
  uint32_t stack_size;
  uint32_t priority;
};

ret_t tk_thread_set_name(tk_thread_t* thread, const char* name) {
  return_value_if_fail(thread != NULL && name != NULL, RET_BAD_PARAMS);

  tk_strncpy(thread->name, name, TK_NAME_LEN);

  return RET_OK;
}

ret_t tk_thread_set_stack_size(tk_thread_t* thread, uint32_t stack_size) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->stack_size = stack_size;

  return RET_OK;
}

ret_t tk_thread_set_priority(tk_thread_t* thread, uint32_t priority) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->priority = priority;

  return RET_OK;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);

  return thread->args;
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  tk_thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (tk_thread_t*)TKMEM_ZALLOC(tk_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;

  return thread;
}

static int entry(void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;

  thread->entry(thread->args);
  thread->running = FALSE;

  return 0;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->thread = SDL_CreateThread((SDL_ThreadFunction)(entry), thread->name, thread);
  thread->running = thread->thread != NULL;

  return thread->running ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  if (thread->running) {
    if (thread->thread) {
      SDL_WaitThread(thread->thread, NULL);
    }
  }

  return RET_OK;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  memset(thread, 0x00, sizeof(tk_thread_t));
  TKMEM_FREE(thread);

  return RET_OK;
}

uint64_t tk_thread_self(void) {
  return (uint64_t)SDL_ThreadID();
}
#endif /*HAS_PTHREAD*/
