/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex base on freertos
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-05-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "FreeRTOS.h"
#include "semphr.h"

#include "tkc/mem.h"
#include "tkc/mutex.h"

struct _tk_mutex_t {
  SemaphoreHandle_t mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);

  mutex->mutex = xSemaphoreCreateMutex();
  if (mutex->mutex == NULL) {
    TKMEM_FREE(mutex);
  }

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(xSemaphoreTake(mutex->mutex, 0xffffff) == pdTRUE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_try_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(xSemaphoreTake(mutex->mutex, 0) == pdTRUE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(xSemaphoreGive(mutex->mutex) == pdTRUE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  vSemaphoreDelete(mutex->mutex);
  memset(mutex, 0x00, sizeof(tk_mutex_t));
  TKMEM_FREE(mutex);

  return RET_OK;
}
