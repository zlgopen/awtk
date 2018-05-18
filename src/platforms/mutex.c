/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/mutex.h"

#ifdef WIN32
#include "Windows.h"
#define mutex_handle_t HANDLE
#elif defined(HAS_PTHREAD)
#include "pthread.h"
#define mutex_handle_t pthread_mutex_t
#endif

struct _mutex_t {
  ret_t created;
  mutex_handle_t mutex;
};

mutex_t* mutex_create() {
  mutex_t* mutex = (mutex_t*)TKMEM_ZALLOC(mutex_t);
  return_value_if_fail(mutex != NULL, NULL);
#ifdef WIN32
  mutex->mutex = CreateMutex(NULL, RET_BAD_PARAMS, NULL);
  mutex->created = mutex->mutex != NULL;
#elif defined(HAS_PTHREAD)
  mutex->created = 0 == pthread_mutex_init(&(mutex->mutex), NULL);
#endif

  return mutex;
}

ret_t mutex_lock(mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  WaitForSingleObject(mutex->mutex, INFINITE);
#elif defined(HAS_PTHREAD)
  pthread_mutex_lock(&(mutex->mutex));
#endif

  return RET_OK;
}

ret_t mutex_unlock(mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  ReleaseMutex(mutex->mutex);
#elif defined(HAS_PTHREAD)
  pthread_mutex_unlock(&(mutex->mutex));
#endif

  return RET_OK;
}

ret_t mutex_destroy(mutex_t* mutex) {
  return_value_if_fail(mutex != NULL && mutex->created, RET_BAD_PARAMS);

#ifdef WIN32
  CloseHandle(mutex->mutex);
#elif defined(HAS_PTHREAD)
  pthread_mutex_destroy(&(mutex->mutex));
#endif
  mutex->created = RET_BAD_PARAMS;

  TKMEM_FREE(mutex);

  return RET_OK;
}
