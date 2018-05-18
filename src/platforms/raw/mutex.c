/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex do nothing
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
 * 2018-05-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/mutex.h"

struct _mutex_t {
  uint32_t none;
};

static mutex_t s_mutex_null;

mutex_t* mutex_create() {
  return &s_mutex_null;
}

ret_t mutex_lock(mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}

ret_t mutex_unlock(mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}

ret_t mutex_destroy(mutex_t* mutex) {
  (void)mutex;
  return RET_OK;
}
