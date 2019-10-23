/**
 * File:   cond_var.c
 * Author: AWTK Develop Team
 * Brief:  cond_var base on tos_event
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tos.h"
#include "tkc/mem.h"
#include "tkc/cond_var.h"

struct _tk_cond_var_t {
  k_event_t event;
};

tk_cond_var_t* tk_cond_var_create(void) {
  tk_cond_var_t* cond_var = TKMEM_ZALLOC(tk_cond_var_t);
  return_value_if_fail(cond_var != NULL, NULL);

  if (tos_event_create(&(cond_var->event), 0) != K_ERR_NONE) {
    TKMEM_FREE(cond_var);
  }

  return cond_var;
}

ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms) {
  k_event_flag_t flag_match = 0;
  k_event_flag_t flag_expect = 1;
  k_opt_t opt = TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR;
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);
  return_value_if_fail(
      tos_event_pend(&(cond_var->event), flag_expect, &flag_match, timeout_ms, opt) == K_ERR_NONE,
      RET_FAIL);

  return RET_OK;
}

ret_t tk_cond_var_awake(tk_cond_var_t* cond_var) {
  k_event_flag_t flag = 1;
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tos_event_post_keep(&(cond_var->event), flag) == K_ERR_NONE, RET_FAIL);

  return RET_OK;
}

ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);

  tos_event_destroy(&(cond_var->event));
  memset(cond_var, 0x00, sizeof(tk_cond_var_t));
  TKMEM_FREE(cond_var);

  return RET_OK;
}
