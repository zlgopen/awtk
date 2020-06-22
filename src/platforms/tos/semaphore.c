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

#include "tos_k.h"

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/platform.h"
#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  k_sem_t sem;
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  if (tos_sem_create(&(semaphore->sem), value) != K_ERR_NONE) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  k_tick_t timeout = tos_millisec2tick(timeout_ms);
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  if (tos_sem_pend(&(semaphore->sem), timeout) != K_ERR_NONE) {
    return RET_TIMEOUT;
  }

  return RET_OK;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tos_sem_post(&(semaphore->sem)) == K_ERR_NONE, RET_FAIL);

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  tos_sem_destroy(&(semaphore->sem));
  memset(semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}
