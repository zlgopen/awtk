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

#ifndef TK_TIMER_H
#define TK_TIMER_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct timer_info_t;
typedef struct timer_info_t timer_info_t;

typedef uint32_t (*timer_get_time_t)();
typedef ret_t (*timer_func_t)(const timer_info_t* timer);

typedef struct timer_info_t {
  timer_func_t on_timer;
  void* ctx;
  uint32_t id;
  uint32_t start;
  uint32_t duration_ms;
  bool_t repeat;
} timer_info_t;

/**
 * @class timer_t
 * @scriptable
 * @fake
 * 定时器。
 */

/**
 * @method timer_init
 * 初始化定时器系统。
 * @private
 * @static
 * @param {timer_get_time_t} get_time 获取当前时间的函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_init(timer_get_time_t get_time);

/**
 * @method timer_add
 * 增加一个定时器。
 * @static
 * @scriptable custom
 * @param {timer_func_t} on_timer 定时器回调函数。
 * @param {void*} ctx 定时器回调函数的上下文。
 * @param {uint32_t} duration_ms 时间。
 *
 * @return {uint32_t} 返回定时器的ID，0表示失败。
 */
uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration_ms);

/**
 * @method timer_remove
 * 删除指定的定时器。
 * @static
 * @scriptable custom
 * @param {uint32_t} timer_id 定时器ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_remove(uint32_t timer_id);

/**
 * @method timer_find
 * 查找指定ID的定时器。
 * @private
 *
 * @return {timer_info_t*} 返回timer的信息。
 */
const timer_info_t* timer_find(uint32_t timer_id);

/**
 * @method timer_check
 * 检查定时器，并调用到期定时器的函数。
 * @private
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_check(void);

/**
 * @method timer_count
 * 返回定时器的个数。
 * @static
 *
 * @return {uint32_t} 返回定时器的个数。
 */
uint32_t timer_count(void);

END_C_DECLS

#endif /*TK_TIMER_H*/
