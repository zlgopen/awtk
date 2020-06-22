/**
 * File:   semaphore_null.c
 * Author: AWTK Develop Team
 * Brief:  semaphore do nothing
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
 * 2020-05-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/semaphore.h"

struct _tk_semaphore_t {
  uint32_t none;
};

static const tk_semaphore_t s_semaphore_null;

tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name) {
  return (tk_semaphore_t*)&s_semaphore_null;
}

ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms) {
  return RET_OK;
}

ret_t tk_semaphore_post(tk_semaphore_t* semaphore) {
  return RET_OK;
}

ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore) {
  return RET_OK;
}
