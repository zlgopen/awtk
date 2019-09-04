/**
 * File:   timer.h
 * Author: AWTK Develop Team
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIMER_MANAGER_H
#define TK_TIMER_MANAGER_H

#include "tkc/slist.h"
#include "tkc/timer_info.h"

BEGIN_C_DECLS

struct _timer_manager_t {
  uint32_t next_timer_id;
  uint64_t last_dispatch_time;
  timer_get_time_t get_time;

  slist_t timers;
};

timer_manager_t* timer_manager(void);
ret_t timer_manager_set(timer_manager_t* timer_manager);

timer_manager_t* timer_manager_create(timer_get_time_t get_time);
timer_manager_t* timer_manager_init(timer_manager_t* timer_manager, timer_get_time_t get_time);
ret_t timer_manager_deinit(timer_manager_t* timer_manager);
ret_t timer_manager_destroy(timer_manager_t* timer_manager);

uint32_t timer_manager_add(timer_manager_t* timer_manager, timer_func_t on_timer, void* ctx,
                           uint32_t duration);
ret_t timer_manager_set_on_destroy(timer_manager_t* timer_manager, uint32_t timer_id,
                                   tk_destroy_t on_destroy, void* on_destroy_ctx);
ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id);
ret_t timer_manager_reset(timer_manager_t* timer_manager, uint32_t timer_id);
const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id);
ret_t timer_manager_dispatch(timer_manager_t* timer_manager);
uint32_t timer_manager_count(timer_manager_t* timer_manager);
uint64_t timer_manager_next_time(timer_manager_t* timer_manager);
ret_t timer_manager_append(timer_manager_t* timer_manager, timer_info_t* timer);

END_C_DECLS

#endif /*TK_TIMER_MANAGER_H*/
