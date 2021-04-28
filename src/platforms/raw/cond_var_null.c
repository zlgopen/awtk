/**
 * File:   cond_var_null.c
 * Author: AWTK Develop Team
 * Brief:  cond_var do nothing
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
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/cond_var.h"

struct _tk_cond_t {
  uint32_t value;
};

tk_cond_t* tk_cond_create(void) {
  return NULL;
}

ret_t tk_cond_destroy(tk_cond_t* cond) {
  return RET_NOT_IMPL;
}

ret_t tk_cond_signal(tk_cond_t* cond) {
  return RET_NOT_IMPL;
}

ret_t tk_cond_broadcast(tk_cond_t* cond) {
  return RET_NOT_IMPL;
}

ret_t tk_cond_wait_timeout(tk_cond_t* cond, tk_mutex_t* mutex, uint32_t ms) {
  return RET_NOT_IMPL;
}

ret_t tk_cond_wait(tk_cond_t* cond, tk_mutex_t* mutex) {
  return RET_NOT_IMPL;
}
