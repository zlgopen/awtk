/**
 * File:   main_loop.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  main_loop interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

ret_t main_loop_remove_timer(main_loop_t* l, id_t id) {
  return_value_if_fail(l != NULL && l->remove_timer != NULL, RET_BAD_PARAMS);

  return l->remove_timer(l, id);
}

id_t main_loop_add_timer(main_loop_t* l, uint32_t time_ms, event_handler on_event, void* ctx) {
  return_value_if_fail(l != NULL && l->add_timer != NULL && on_event != NULL, RET_BAD_PARAMS);

  return l->add_timer(l, time_ms, on_event, ctx);
}
