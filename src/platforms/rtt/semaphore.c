/**
 * File:   semaphore.c
 * Author: AWTK Develop Team
 * Brief:  semaphore base on rtthread
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
 * 2019-11-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "rtthread.h"

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/platform.h"
#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  rt_sem_t sem;
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  semaphore->sem = rt_sem_create(name, value, 0);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  if (rt_sem_take(semaphore->sem, timeout_ms) != RT_EOK) {
    return RET_TIMEOUT;
  }

  return RET_OK;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return_value_if_fail(rt_sem_release(semaphore->sem) == RT_EOK, RET_FAIL);

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  rt_sem_delete(semaphore->sem);
  memset(&semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}
