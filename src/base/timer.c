/**
 * File:   timer.c
 * Author: AWTK Develop Team
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"

ret_t timer_prepare(timer_get_time_t get_time) {
  return_value_if_fail(get_time != NULL, RET_BAD_PARAMS);
  return_value_if_fail(timer_manager() == NULL, RET_FAIL);

  return timer_manager_set(timer_manager_create(get_time));
}

uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration) {
  return timer_manager_add(timer_manager(), on_timer, ctx, duration);
}

uint32_t timer_add_with_type(timer_func_t on_timer, void* ctx, uint32_t duration, uint16_t type) {
  return timer_manager_add_with_type(timer_manager(), on_timer, ctx, duration, type);
}

ret_t timer_remove(uint32_t timer_id) {
  return timer_manager_remove(timer_manager(), timer_id);
}

ret_t timer_remove_all_by_ctx_and_type(uint16_t type, void* ctx) {
  return timer_manager_all_remove_by_ctx_and_type(timer_manager(), type, ctx);
}

ret_t timer_remove_all_by_ctx(void* ctx) {
  return timer_manager_all_remove_by_ctx(timer_manager(), ctx);
}

ret_t timer_reset(uint32_t timer_id) {
  return timer_manager_reset(timer_manager(), timer_id);
}

ret_t timer_suspend(uint32_t timer_id) {
  timer_info_t* timer = (timer_info_t*)timer_find(timer_id);
  return_value_if_fail(timer != NULL, RET_BAD_PARAMS);
  timer->suspend = TRUE;
  return RET_OK;
}

ret_t timer_resume(uint32_t timer_id) {
  timer_info_t* timer = (timer_info_t*)timer_find(timer_id);
  return_value_if_fail(timer != NULL, RET_BAD_PARAMS);
  timer->suspend = FALSE;

  return timer_reset(timer_id);
}

const timer_info_t* timer_find(uint32_t timer_id) {
  return timer_manager_find(timer_manager(), timer_id);
}

ret_t timer_dispatch(void) {
  return timer_manager_dispatch(timer_manager());
}

ret_t timer_modify(uint32_t timer_id, uint32_t duration) {
  timer_info_t* info = (timer_info_t*)timer_manager_find(timer_manager(), timer_id);
  return_value_if_fail(info != NULL, RET_NOT_FOUND);

  info->duration = duration;
  info->start = timer_manager()->get_time();

  return RET_OK;
}

uint32_t timer_count(void) {
  return timer_manager_count(timer_manager());
}

uint32_t timer_next_time(void) {
  return timer_manager_next_time(timer_manager());
}

#include "base/main_loop.h"

ret_t timer_queue_ex(timer_func_t on_timer, void* ctx, uint32_t duration, tk_destroy_t on_destroy,
                     void* on_destroy_ctx) {
#ifdef AWTK_WEB
  timer_add(on_timer, ctx, duration);

  return RET_OK;
#else
  event_queue_req_t r;
  r.add_timer.func = on_timer;
  r.add_timer.duration = duration;
  r.add_timer.e.target = ctx;
  r.add_timer.e.type = REQ_ADD_TIMER;
  r.add_timer.on_destroy = on_destroy;
  r.add_timer.on_destroy_ctx = on_destroy_ctx;
  return main_loop_queue_event(main_loop(), &r);
#endif /*AWTK_WEB*/
}

ret_t timer_queue(timer_func_t on_timer, void* ctx, uint32_t duration) {
  return timer_queue_ex(on_timer, ctx, duration, NULL, NULL);
}

ret_t timer_set_on_destroy(uint32_t timer_id, tk_destroy_t on_destroy, void* on_destroy_ctx) {
  timer_info_t* timer = (timer_info_t*)timer_find(timer_id);
  return_value_if_fail(timer != NULL, RET_BAD_PARAMS);

  timer->on_destroy = on_destroy;
  timer->on_destroy_ctx = on_destroy_ctx;

  return RET_OK;
}
