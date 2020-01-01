/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex do nothing
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
 * 2018-05-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/mutex.h"

struct _tk_mutex_t {
  uint32_t none;
};

static tk_mutex_t s_tk_mutex_null;

tk_mutex_t* tk_mutex_create() {
  return &s_tk_mutex_null;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}
