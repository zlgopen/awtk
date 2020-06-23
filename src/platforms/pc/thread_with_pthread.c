/**
 * File:   pthread_impl.c
 * Author: AWTK Develop Team
 * Brief:  pthread implemented thread related functions.
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

#include "tkc/mem.h"
#include "tkc/mutex.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/thread.h"
#include "tkc/mutex.h"
#include "tkc/cond.h"
#include "tkc/semaphore.h"

#if defined(HAS_PTHREAD)

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

struct _tk_mutex_t {
  pthread_mutex_t mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = (tk_mutex_t*)TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);
  if (pthread_mutex_init(&(mutex->mutex), NULL) != 0) {
    TKMEM_FREE(mutex);
  }

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return pthread_mutex_lock(&(mutex->mutex)) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_mutex_try_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return pthread_mutex_trylock(&(mutex->mutex)) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return pthread_mutex_unlock(&(mutex->mutex)) == 0 ? RET_OK : RET_FAIL;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  pthread_mutex_destroy(&(mutex->mutex));
  memset(mutex, 0x00, sizeof(tk_mutex_t));
  TKMEM_FREE(mutex);

  return RET_OK;
}

/********************************************************/

struct _tk_cond_t {
  pthread_cond_t cond;
};

tk_cond_t* tk_cond_create(void) {
  tk_cond_t* cond = (tk_cond_t*)TKMEM_ALLOC(sizeof(tk_cond_t));
  return_value_if_fail(cond != NULL, NULL);

  if (pthread_cond_init(&cond->cond, NULL) != 0) {
    log_error("pthread_cond_init() failed");
    TKMEM_FREE(cond);
    cond = NULL;
  }

  return (cond);
}

ret_t tk_cond_destroy(tk_cond_t* cond) {
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  pthread_cond_destroy(&cond->cond);
  memset(cond, 0x00, sizeof(tk_cond_t));
  TKMEM_FREE(cond);

  return RET_OK;
}

ret_t tk_cond_signal(tk_cond_t* cond) {
  ret_t ret = RET_OK;
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  if (pthread_cond_signal(&cond->cond) != 0) {
    log_error("pthread_cond_signal() failed");
    ret = RET_FAIL;
  }

  return ret;
}

ret_t tk_cond_broadcast(tk_cond_t* cond) {
  ret_t ret = RET_OK;
  return_value_if_fail(cond != NULL, RET_BAD_PARAMS);

  if (pthread_cond_broadcast(&cond->cond) != 0) {
    ret = RET_FAIL;
    log_error("pthread_cond_broadcast() failed");
  }

  return ret;
}

ret_t tk_cond_wait_timeout(tk_cond_t* cond, tk_mutex_t* mutex, uint32_t ms) {
  int code = 0;
  ret_t ret = RET_OK;
#ifndef HAVE_CLOCK_GETTIME
  struct timeval delta;
#endif
  struct timespec abstime;
  return_value_if_fail(cond != NULL && mutex != NULL, RET_BAD_PARAMS);

#ifdef HAVE_CLOCK_GETTIME
  clock_gettime(CLOCK_REALTIME, &abstime);

  abstime.tv_nsec += (ms % 1000) * 1000000;
  abstime.tv_sec += ms / 1000;
#else
  gettimeofday(&delta, NULL);

  abstime.tv_sec = delta.tv_sec + (ms / 1000);
  abstime.tv_nsec = (delta.tv_usec + (ms % 1000) * 1000) * 1000;
#endif
  if (abstime.tv_nsec > 1000000000) {
    abstime.tv_sec += 1;
    abstime.tv_nsec -= 1000000000;
  }

tryagain:
  code = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
  switch (code) {
    case EINTR:
      goto tryagain;
      /* break; -Wunreachable-code-break */
    case ETIMEDOUT:
      ret = RET_TIMEOUT;
      break;
    case 0:
      break;
    default: {
      log_error("pthread_cond_timedwait() failed");
      ret = RET_FAIL;
    }
  }

  return ret;
}

ret_t tk_cond_wait(tk_cond_t* cond, tk_mutex_t* mutex) {
  return_value_if_fail(cond != NULL && mutex != NULL, RET_BAD_PARAMS);

  if (pthread_cond_wait(&cond->cond, &mutex->mutex) != 0) {
    log_error("pthread_cond_wait() failed");

    return RET_FAIL;
  }

  return RET_OK;
}

/********************************************************/

#ifdef IOS
#include <sys/semaphore.h>
#else
#include <semaphore.h>
#include "tkc/semaphore.h"
#endif

struct _tk_semaphore_t {
  sem_t* sem;
  char name[TK_NAME_LEN + 1];
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  if (name == NULL) {
    tk_snprintf(semaphore->name, TK_NAME_LEN, "%p", semaphore);
  } else {
    tk_strncpy(semaphore->name, name, TK_NAME_LEN);
  }

#ifdef HAS_SEM_OPEN
  sem_unlink(semaphore->name);
  semaphore->sem = sem_open(semaphore->name, O_CREAT, S_IRUSR | S_IWUSR, value);
#else
  semaphore->sem = TKMEM_ZALLOC(sem_t);
  sem_init(semaphore->sem, 0, value);
#endif
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  uint32_t start = time_now_ms();
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  do {
    if (sem_trywait(semaphore->sem) == 0) {
      return RET_OK;
    }

    if ((time_now_ms() - start) >= timeout_ms) {
      return RET_TIMEOUT;
    }

    sleep_ms(10);
  } while (TRUE);

  return RET_FAIL;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return_value_if_fail(sem_post(semaphore->sem) == 0, RET_FAIL);

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

#ifdef HAS_SEM_OPEN
  sem_close(semaphore->sem);
  sem_unlink(semaphore->name);
#else
  sem_destroy(semaphore->sem);
  TKMEM_FREE(semaphore->sem);
#endif /*HAS_SEM_OPEN*/
  memset(semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}

/********************************************************/

struct _tk_thread_t {
  void* args;
  bool_t running;
  pthread_t thread;
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

static void* entry(void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;

  thread->entry(thread->args);
  thread->running = FALSE;

  return NULL;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  int ret = pthread_create(&(thread->thread), NULL, entry, thread);
  thread->running = ret == 0;

  return thread->running ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  if (thread->running) {
    void* ret = NULL;
    if (thread->thread) {
      pthread_join(thread->thread, &ret);
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
  return (uint64_t)pthread_self();
}
#endif /*HAS_PTHREAD*/
