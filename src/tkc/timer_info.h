/**
 * File:   timer_info.h
 * Author: AWTK Develop Team
 * Brief:  timer info
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIMER_INFO_H
#define TK_TIMER_INFO_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _timer_info_t;
typedef struct _timer_info_t timer_info_t;

struct _timer_manager_t;
typedef struct _timer_manager_t timer_manager_t;

typedef tk_get_time_ms_t timer_get_time_t;
typedef ret_t (*timer_func_t)(const timer_info_t* timer);

/**
 * @class timer_info_t
 * @parent object_t
 * @annotation ["scriptable"]
 * 单个定时器的信息。
 *
 */
struct _timer_info_t {
  object_t object;

  /**
   * @property {timer_func_t} on_timer
   * @annotation ["readable"]
   * 定时器回调函数。
   */
  timer_func_t on_timer;

  /**
   * @property {void*} ctx
   * @annotation ["readable", "scriptable"]
   * 定时器回调函数的上下文
   *
   */
  void* ctx;

  /**
   * @property {uint32_t} id
   * @annotation ["readable", "scriptable"]
   * 定时器的ID
   *
   * > 为TK\_INVALID\_ID时表示无效定时器。
   */
  uint32_t id;

  /**
   * @property {uint64_t} now
   * @annotation ["readable", "scriptable"]
   * 当前时间(相对时间，单位为毫秒)。
   *
   */
  uint64_t now;

  /**
   * @property {uint64_t} start
   * @annotation ["readable"]
   * 起始时间(相对时间，单位为毫秒)。
   *
   */
  uint64_t start;

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

  /*private*/
  bool_t busy;
  uint64_t last_dispatch_time;
  timer_manager_t* timer_manager;
};

/**
 * @method timer_info_cast
 * 转换为timer_info对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {timer_info_t*} timer timer_info对象。
 *
 * @return {timer_info_t*} timer_info对象。
 */
timer_info_t* timer_info_cast(timer_info_t* timer);

/*internal use*/
int timer_info_compare(const void* a, const void* b);
timer_info_t* timer_info_init_dummy(timer_info_t* timer, uint32_t id);
timer_info_t* timer_info_create(timer_manager_t* tm, timer_func_t on_timer, void* ctx,
                                uint32_t duration);

ret_t timer_info_on_timer(timer_info_t* timer, uint64_t now);
bool_t timer_info_is_available(timer_info_t* timer, uint64_t now);

#define TIMER_INFO(o) ((timer_info_t*)(o))

END_C_DECLS

#endif /*TK_TIMER_INFO_H*/
