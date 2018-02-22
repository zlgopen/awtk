/**
 * File:   timer.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_TIMER_H
#define LFTK_TIMER_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct _timer_t;
typedef struct _timer_t f_timer_t;

typedef uint32_t (*timer_get_time_t)();
typedef ret_t (*timer_func_t)(const f_timer_t* timer);

typedef struct _timer_t {
  timer_func_t on_timer;
  void* user_data;
  uint32_t start;
  uint32_t duration_ms; 
  bool_t repeat;
}f_timer_t;

ret_t timer_init(timer_get_time_t get_time);
ret_t timer_add(timer_func_t on_timer, void* user_data, uint32_t duration_ms);
ret_t timer_remove(timer_func_t on_timer, void* user_data, uint32_t duration_ms);
ret_t timer_check();
ret_t timer_count();

END_C_DECLS

#endif/*LFTK_TIMER_H*/

