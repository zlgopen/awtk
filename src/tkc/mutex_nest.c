/**
 * File:   mutex_nest.c
 * Author: AWTK Develop Team
 * Brief:  mutex recursive
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/thread.h"
#include "tkc/mutex_nest.h"

tk_mutex_nest_t* tk_mutex_nest_create(void) {
  tk_mutex_nest_t* r = NULL;
  tk_mutex_t* mutex = tk_mutex_create();
  return_value_if_fail(mutex != NULL, NULL);

  r = TKMEM_ZALLOC(tk_mutex_nest_t);
  if (r != NULL) {
    r->mutex = mutex;
  } else {
    tk_mutex_destroy(mutex);
  }

  return r;
}

ret_t tk_mutex_nest_lock(tk_mutex_nest_t* mutex_nest) {
  ret_t ret = RET_OK;
  return_value_if_fail(mutex_nest != NULL && mutex_nest->mutex != NULL, RET_BAD_PARAMS);

  if (mutex_nest->owner == tk_thread_self()) {
    mutex_nest->ref++;
  } else {
    if (tk_mutex_lock(mutex_nest->mutex) == RET_OK) {
      mutex_nest->ref = 1;
      mutex_nest->owner = tk_thread_self();
    } else {
      ret = RET_FAIL;
    }
  }

  return ret;
}

ret_t tk_mutex_nest_try_lock(tk_mutex_nest_t* mutex_nest) {
  ret_t ret = RET_OK;
  return_value_if_fail(mutex_nest != NULL && mutex_nest->mutex != NULL, RET_BAD_PARAMS);

  if (mutex_nest->owner == tk_thread_self()) {
    mutex_nest->ref++;
  } else {
    if (tk_mutex_try_lock(mutex_nest->mutex) == RET_OK) {
      mutex_nest->ref = 1;
      mutex_nest->owner = tk_thread_self();
    } else {
      ret = RET_FAIL;
    }
  }

  return ret;
}

ret_t tk_mutex_nest_unlock(tk_mutex_nest_t* mutex_nest) {
  ret_t ret = RET_OK;
  return_value_if_fail(mutex_nest != NULL && mutex_nest->mutex != NULL, RET_BAD_PARAMS);

  if (mutex_nest->owner == tk_thread_self()) {
    mutex_nest->ref--;
    assert(mutex_nest->ref >= 0);

    if (mutex_nest->ref == 0) {
      ret = tk_mutex_unlock(mutex_nest->mutex);
      mutex_nest->owner = 0;
    }
  } else {
    log_debug("not mutex owner\n");
  }

  return ret;
}

ret_t tk_mutex_nest_destroy(tk_mutex_nest_t* mutex_nest) {
  return_value_if_fail(mutex_nest != NULL && mutex_nest->mutex != NULL, RET_BAD_PARAMS);
  tk_mutex_destroy(mutex_nest->mutex);
  memset(mutex_nest, 0x00, sizeof(tk_mutex_nest_t));
  TKMEM_FREE(mutex_nest);

  return RET_OK;
}
