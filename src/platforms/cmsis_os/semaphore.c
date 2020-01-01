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

#include "cmsis_os.h"

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/platform.h"
#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  osSemaphoreId sem;
  osSemaphoreDef_t def;
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  osSemaphoreDef_t* def = NULL;
  tk_semaphore_t* semaphore = TKMEM_ZALLOC(tk_semaphore_t);
  return_value_if_fail(semaphore != NULL, NULL);

  memset(def, 0x00, sizeof(*def));
#ifdef _TOS_CONFIG_H_
  def->sem = TKMEM_ALLOC(sizeof(*(def->sem)));
#endif /*_TOS_CONFIG_H_*/

  semaphore->sem = osSemaphoreCreate(def, value);
  if (semaphore->sem == NULL) {
    TKMEM_FREE(semaphore);
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  if (osSemaphoreWait(semaphore->sem, timeout_ms) < 0) {
    return RET_TIMEOUT;
  }

  return RET_OK;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  return_value_if_fail(osSemaphoreRelease(semaphore->sem) == osOK, RET_FAIL);

  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return_value_if_fail(semaphore != NULL, RET_BAD_PARAMS);

  osSemaphoreDelete(semaphore->sem);

#ifdef _TOS_CONFIG_H_
  TKMEM_FREE(semaphore->def.sem);
#endif /*_TOS_CONFIG_H_*/

  memset(&semaphore, 0x00, sizeof(tk_semaphore_t));
  TKMEM_FREE(semaphore);

  return RET_OK;
}
