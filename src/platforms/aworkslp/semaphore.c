/**
 * File:   semaphore.c
 * Author: AWTK Develop Team
 * Brief:  semaphore
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
 * 2021-12-16 Wang LinFu <wanglinfu@zlg.cn> created
 *
 */

#include "aw_sem.h"
#include "aw_mem.h"
#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  AW_SEMC_DECL(__sem);
};

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  tk_semaphore_t* semaphore = (tk_semaphore_t*)aw_mem_alloc(sizeof(tk_semaphore_t));

  if (semaphore) {
    memset(semaphore, 0, sizeof(tk_semaphore_t));
    if (NULL == AW_SEMC_INIT(semaphore->__sem, value, AW_SEM_Q_FIFO)) {
      aw_mem_free(semaphore);
      return NULL;
    }
  }

  return semaphore;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  assert(semaphore);
  aw_err_t err = AW_SEMC_TAKE(semaphore->__sem, timeout_ms);

  switch (err) {
    case AW_OK:
      return RET_OK;
    default:
      return RET_FAIL;
  }
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  assert(semaphore);
  aw_err_t err = AW_SEMC_GIVE(semaphore->__sem);

  switch (err) {
    case AW_OK:
      return RET_OK;
    default:
      return RET_FAIL;
  }
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  assert(semaphore);
  AW_SEMC_TERMINATE(semaphore->__sem);

  aw_mem_free(semaphore);
  return RET_OK;
}
