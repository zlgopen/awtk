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

#include "tkc/array.h"

BEGIN_C_DECLS

struct _timer_info_t;
typedef struct _timer_info_t timer_info_t;

typedef uint32_t (*timer_get_time_t)();
typedef ret_t (*timer_func_t)(const timer_info_t* timer);

typedef struct _timer_manager_t {
  uint32_t active;
  bool_t dispatching;
  uint32_t last_dispatch_time;
  uint32_t next_timer_id;
  timer_get_time_t get_time;

  struct _timer_info_t* first;
} timer_manager_t;

/**
 * @class timer_info_t
 * 单个定时器的信息。
 */
struct _timer_info_t {
  /**
   * @property {timer_func_t} on_timer
   * @annotation ["readable"]
   * 定时器回调函数。
   */
  timer_func_t on_timer;
  /**
   * @property {void*} ctx
   * @annotation ["readable"]
   * 定时器回调函数的上下文
   */
  void* ctx;
  /**
   * @property {uint32_t} id
   * @annotation ["readable"]
   * 定时器的ID
   *
   * > 为TK\_INVALID\_ID时表示无效定时器。
   */
  uint32_t id;
  /**
   * @property {uint32_t} now
   * @annotation ["readable"]
   * 当前时间(相对时间，单位为毫秒)。
   *
   */
  uint32_t now;
  /**
   * @property {uint32_t} start
   * @annotation ["readable"]
   * 起始时间(相对时间，单位为毫秒)。
   *
   */
  uint32_t start;
  /**
   * @property {uint32_t} duration
   * @annotation ["readable"]
   * 时间间隔(单位为毫秒)。
   *
   */
  uint32_t duration;
  /**
   * @property {tk_destroy_t} on_destroy
   * @annotation ["readable"]
   * 定时器销毁时的回调函数。
   */
  tk_destroy_t on_destroy;
  /**
   * @property {void*} on_destroy_ctx
   * @annotation ["readable"]
   * 定时器销毁时的回调函数上下文。
   */
  void* on_destroy_ctx;
  /**
   * @property {bool_t} user_changed_time
   * @annotation ["readable"]
   * 用户是否修改了系统时间。
   */
  bool_t user_changed_time;

  /*private*/
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
                           uint32_t duration);
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
 * 定时器系统。
 *
 * > 本定时器精度较低，最高精度为1000/FPS，如果需要高精度的定时器，请用OS提供的定时器。
 *
 * 示例：
 *
 * ```c
 * static ret_t my_on_timer(const timer_info_t* info) {
 *  widget_t* widget = WIDGET(info->ctx);
 *  ...
 *  return RET_REPEAT;
 * }
 *
 * ...
 *
 * timer_add(my_on_timer, widget, 1000);
 * ```
 * > 在非GUI线程请用timer\_queue。
 *
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
 * @param {uint32_t} duration 时间。
 *
 * @return {uint32_t} 返回timer的ID，TK_INVALID_ID表示失败。
 */
uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration);

/**
 * @method timer_queue
 * 用于非GUI线程增加一个timer，本函数向主循环的事件队列中发送一个增加timer的请求。
 * @annotation ["static"]
 * @param {timer_func_t} on_timer
 * timer回调函数，回调函数返回RET_REPEAT，则下次继续执行，否则自动移出。
 * @param {void*} ctx timer回调函数的上下文。
 * @param {uint32_t} duration 时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_queue(timer_func_t on_timer, void* ctx, uint32_t duration);

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
