/**
 * File:   main_loop.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  main_loop interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/main_loop.h"

ret_t main_loop_run(main_loop_t* l) {
  return_value_if_fail(l != NULL && l->run != NULL, RET_BAD_PARAMS);
  l->running = TRUE;

  return l->run(l);
}

ret_t main_loop_quit(main_loop_t* l) {
  return_value_if_fail(l != NULL && l->destroy != NULL, RET_BAD_PARAMS);
  l->running = FALSE;

  return l->quit(l);
}

ret_t main_loop_destroy(main_loop_t* l) {
  return_value_if_fail(l != NULL && l->destroy != NULL, RET_BAD_PARAMS);
  l->running = FALSE;

  return l->destroy(l);
}

static main_loop_t* s_default_main_loop = NULL;

main_loop_t* main_loop(void) { return s_default_main_loop; }

ret_t main_loop_set_default(main_loop_t* loop) {
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);
  s_default_main_loop = loop;

  return RET_OK;
}
