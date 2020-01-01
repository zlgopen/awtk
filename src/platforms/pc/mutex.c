/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex
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

#ifdef WIN32
#include "Windows.h"
#define tk_mutex_handle_t HANDLE
#elif defined(HAS_PTHREAD)
#include "pthread.h"
#define tk_mutex_handle_t pthread_mutex_t
#else
#define tk_mutex_handle_t int
#endif

struct _tk_mutex_t {
  bool_t created;
  tk_mutex_handle_t mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = (tk_mutex_t*)TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);
#ifdef WIN32
  mutex->mutex = CreateMutex(NULL, FALSE, NULL);
  mutex->created = mutex->mutex != NULL;
#elif defined(HAS_PTHREAD)
  mutex->created = 0 == pthread_mutex_init(&(mutex->mutex), NULL);
#endif

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  WaitForSingleObject(mutex->mutex, INFINITE);
#elif defined(HAS_PTHREAD)
  pthread_mutex_lock(&(mutex->mutex));
#endif

  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  ReleaseMutex(mutex->mutex);
#elif defined(HAS_PTHREAD)
  pthread_mutex_unlock(&(mutex->mutex));
#endif

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  CloseHandle(mutex->mutex);
#elif defined(HAS_PTHREAD)
  pthread_mutex_destroy(&(mutex->mutex));
#endif
  mutex->created = FALSE;

  TKMEM_FREE(mutex);

  return RET_OK;
}
