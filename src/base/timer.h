/**
 * File:   timer.h
 * Author: AWTK Develop Team
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIMER_H
#define TK_TIMER_H

#include "base/array.h"

BEGIN_C_DECLS

struct _timer_info_t;
typedef struct _timer_info_t timer_info_t;

typedef uint32_t (*timer_get_time_t)();
typedef ret_t (*timer_func_t)(const timer_info_t* timer);

typedef struct _timer_manager_t {
  uint32_t active;
  uint32_t next_timer_id;
  timer_get_time_t get_time;
  bool_t dispatching;
  struct _timer_info_t* first;
} timer_manager_t;

struct _timer_info_t {
  timer_func_t on_timer;
  void* ctx;
  uint32_t id;
  uint32_t now;
  uint32_t start;
  uint32_t duration_ms;
  void* on_destroy_ctx;
  tk_destroy_t on_destroy;
  timer_manager_t* timer_manager;
  bool_t pending_destroy;

  struct _timer_info_t* next;
};

timer_manager_t* timer_manager(void);
ret_t timer_manager_set(timer_manager_t* timer_manager);

timer_manager_t* timer_manager_create(timer_get_time_t get_time);
timer_manager_t* timer_manager_init(timer_manager_t* timer_manager, timer_get_time_t get_time);
ret_t timer_manager_deinit(timer_manager_t* timer_manager);
ret_t timer_manager_destroy(timer_manager_t* timer_manager);

uint32_t timer_manager_add(timer_manager_t* timer_manager, timer_func_t on_timer, void* ctx,
                           uint32_t duration_ms);
ret_t timer_manager_set_on_destroy(timer_manager_t* timer_manager, uint32_t timer_id,
                                   tk_destroy_t on_destroy, void* on_destroy_ctx);
ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id);
const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id);
ret_t timer_manager_dispatch(timer_manager_t* timer_manager);
uint32_t timer_manager_count(timer_manager_t* timer_manager);
uint32_t timer_manager_next_time(timer_manager_t* timer_manager);

/**
 * @class timer_t
 * @annotation ["scriptable", "fake"]
 * timer函数在paint之后执行。
 */

/**
 * @method timer_init
 * 初始化定时器系统。
 * @annotation ["private", "static"]
 * @param {timer_get_time_t} get_time 获取当前时间的函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_init(timer_get_time_t get_time);

/**
 * @method timer_add
 * 增加一个timer。
 * @annotation ["scriptable:custom", "static"]
 * @param {timer_func_t} on_timer timer回调函数。
 * @param {void*} ctx timer回调函数的上下文。
 * @param {uint32_t} duration_ms 时间。
 *
 * @return {uint32_t} 返回timer的ID，TK_INVALID_ID表示失败。
 */
uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration_ms);

/**
 * @method timer_queue
 * 用于非GUI线程增加一个timer，本函数向主循环的事件队列中发送一个增加timer的请求。
 * @annotation ["static"]
 * @param {timer_func_t} on_timer
 * timer回调函数，回调函数返回RET_REPEAT，则下次继续执行，否则自动移出。
 * @param {void*} ctx timer回调函数的上下文。
 * @param {uint32_t} duration_ms 时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_queue(timer_func_t on_timer, void* ctx, uint32_t duration_ms);

/**
 * @method timer_remove
 * 删除指定的timer。
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} timer_id timerID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_remove(uint32_t timer_id);

/**
 * @method timer_set_on_destroy
 * 设置一个回调函数，在timer被销毁时调用(方便脚本语言去释放回调函数)。
 * @param {uint32_t} timer_id timerID。
 * @param {tk_destroy_t} on_destroy 回调函数。
 * @param {void*} on_destroy_ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_set_on_destroy(uint32_t timer_id, tk_destroy_t on_destroy, void* on_destroy_ctx);

/**
 * @method timer_find
 * 查找指定ID的timer。
 * @annotation ["private"]
 *
 * @return {timer_info_t*} 返回timer的信息。
 */
const timer_info_t* timer_find(uint32_t timer_id);

/**
 * @method timer_dispatch
 * 检查全部timer的函数，如果时间到期，调用相应的timer函数。
 * @annotation ["private"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_dispatch(void);

/**
 * @method timer_count
 * 返回timer的个数。
 * @annotation ["static"]
 *
 * @return {uint32_t} 返回timer的个数。
 */
uint32_t timer_count(void);

/**
 * @method timer_next_time
 * 返回最近的timer到期时间。
 * @annotation ["static"]
 *
 * @return {uint32_t} 返回最近的timer到期时间。
 */
uint32_t timer_next_time(void);

/**
 * @method timer_now
 * 获取当前时间(ms)。
 * @annotation ["static"]
 *
 * @return {uint32_t} 返回获取当前时间(ms)。
 */
uint32_t timer_now(void);

END_C_DECLS

#endif /*TK_TIMER_H*/
