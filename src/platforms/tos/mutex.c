/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex base on tos_mutex
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
 * 2019-10-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tos_k.h"
#include "tkc/mem.h"
#include "tkc/mutex.h"

struct _tk_mutex_t {
  k_mutex_t mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);

  if (tos_mutex_create(&(mutex->mutex)) != K_ERR_NONE) {
    TKMEM_FREE(mutex);
  }

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tos_mutex_pend(&(mutex->mutex)) == K_ERR_NONE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_try_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tos_mutex_pend_timed(&(mutex->mutex), 0) == K_ERR_NONE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tos_mutex_post(&(mutex->mutex)) == K_ERR_NONE, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  tos_mutex_destroy(&(mutex->mutex));
  memset(mutex, 0x00, sizeof(tk_mutex_t));
  TKMEM_FREE(mutex);

  return RET_OK;
}
