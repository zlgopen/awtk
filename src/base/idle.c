/**
 * File:   idle.c
 * Author: AWTK Develop Team
 * Brief:  idle manager
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "tkc/idle_manager.h"

uint32_t idle_add(idle_func_t on_idle, void* ctx) {
  return idle_manager_add(idle_manager(), on_idle, ctx);
}

bool_t idle_exist(idle_func_t on_idle, void* ctx) {
  return idle_manager_exist(idle_manager(), on_idle, ctx);
}

ret_t idle_remove(uint32_t idle_id) {
  return idle_manager_remove(idle_manager(), idle_id);
}

const idle_info_t* idle_find(uint32_t idle_id) {
  return idle_manager_find(idle_manager(), idle_id);
}

ret_t idle_dispatch(void) {
  return idle_manager_dispatch(idle_manager());
}

uint32_t idle_count(void) {
  return idle_manager_count(idle_manager());
}

#include "base/main_loop.h"

ret_t idle_queue(idle_func_t on_idle, void* ctx) {
#ifdef AWTK_WEB
  idle_add(on_idle, ctx);

  return RET_OK;
#else
  event_queue_req_t r;
  r.add_idle.func = on_idle;
  r.add_idle.e.target = ctx;
  r.add_idle.e.type = REQ_ADD_IDLE;

  return main_loop_queue_event(main_loop(), &r);
#endif /*AWTK_WEB*/
}

ret_t idle_set_on_destroy(uint32_t idle_id, tk_destroy_t on_destroy, void* on_destroy_ctx) {
  idle_info_t* item = (idle_info_t*)idle_find(idle_id);
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);

  item->on_destroy = on_destroy;
  item->on_destroy_ctx = on_destroy_ctx;

  return RET_OK;
}
