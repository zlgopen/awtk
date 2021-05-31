/**
 * File:   semaphore.c
 * Author: AWTK Develop Team
 * Brief:  semaphore base on freertos
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
#include "tkc/time_now.h"
#include "tkc/platform.h"
#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  SemaphoreHandle_t sem;
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  semaphore->sem = xSemaphoreCreateCounting(0xff, value);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  uint32_t ticks = timeout_ms * portTICK_PERIOD_MS;
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  if (xSemaphoreTake(semaphore->sem, ticks) != pdTRUE) {
    return RET_TIMEOUT;
  }

  return RET_OK;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return_value_if_fail(xSemaphoreGive(semaphore->sem) == pdTRUE, RET_FAIL);

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  vSemaphoreDelete(semaphore->sem);
  memset(semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}
