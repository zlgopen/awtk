/**
 * File:   cond_var_null.c
 * Author: AWTK Develop Team
 * Brief:  cond_var
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
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/cond_var.h"

struct _tk_cond_var_t {
  bool_t inited;
  bool_t has_signal;
};

tk_cond_var_t* tk_cond_var_create(void) {
  tk_cond_var_t* cond_var = TKMEM_ZALLOC(tk_cond_var_t);
  return_value_if_fail(cond_var != NULL, NULL);

  cond_var->inited = TRUE;

  return cond_var;
}

ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t tk_cond_var_awake(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  memset(cond_var, 0x00, sizeof(tk_cond_var_t));
  TKMEM_FREE(cond_var);

  return RET_OK;
}
