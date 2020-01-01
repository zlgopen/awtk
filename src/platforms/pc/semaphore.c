/**
 * File:   semaphore.c
 * Author: AWTK Develop Team
 * Brief:  semaphore
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
 * 2019-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifdef WIN32
#include <Windows.h>
#include <process.h>
struct _tk_semaphore_t {
  HANDLE sem;
};
#elif defined(HAS_PTHREAD)
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
struct _tk_semaphore_t {
  sem_t* sem;
};
#endif

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/platform.h"
#include "tkc/semaphore.h"

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

#ifdef HAS_PTHREAD
  semaphore->sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
    semaphore = NULL;
  }
#elif defined(WIN32)
  semaphore->sem = CreateSemaphore(NULL, value, INT_MAX, name);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
    semaphore = NULL;
  }
#endif /*HAS_PTHREAD*/

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  uint32_t start = time_now_ms();
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

#ifdef HAS_PTHREAD
  do {
    if (sem_trywait(semaphore->sem) == 0) {
      return RET_OK;
    }

    if ((time_now_ms() - start) >= timeout_ms) {
      return RET_TIMEOUT;
    }

    sleep_ms(10);
  } while (TRUE);
#elif defined(WIN32)
  if (WaitForSingleObject(semaphore->sem, timeout_ms) == WAIT_TIMEOUT) {
    return RET_TIMEOUT;
  }

  return RET_OK;
#endif /*HAS_PTHREAD*/
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

#ifdef HAS_PTHREAD
  return_value_if_fail(sem_post(semaphore->sem) == 0, RET_FAIL);
#elif defined(WIN32)
  return_value_if_fail(ReleaseSemaphore(semaphore->sem, 1, NULL), RET_FAIL);
#endif /*HAS_PTHREAD*/

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

#ifdef HAS_PTHREAD
  sem_close(semaphore->sem);
#elif defined(WIN32)
  CloseHandle(semaphore->sem);
#endif /*HAS_PTHREAD*/

  memset(&semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}
