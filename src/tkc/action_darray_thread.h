/**
 * File:   action_darray_thread.h
 * Author: AWTK Develop Team
 * Brief:  action_darray_thread
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-3-25 vih created
 *
 */

#ifndef TK_ACTION_DARRAY_THREAD_H
#define TK_ACTION_DARRAY_THREAD_H

#include "tkc/thread.h"
#include "tkc/action_thread.h"
#include "tkc/waitable_action_darray.h"

BEGIN_C_DECLS

struct _action_darray_thread_t;
typedef struct _action_darray_thread_t action_darray_thread_t;

typedef ret_t (*action_darray_thread_on_idle_t)(void* ctx, action_darray_thread_t* thread);
typedef ret_t (*action_darray_thread_on_quit_t)(void* ctx, action_darray_thread_t* thread);

typedef enum {
  ACTION_DARRAY_THREAD_STRATEGY_NONE = 0,
  ACTION_DARRAY_THREAD_STRATEGY_REPLACE, /* 替换旧的 qaction */
  ACTION_DARRAY_THREAD_STRATEGY_FLUSH,   /* 删除旧的 qaction，往尾部添加新的 qaction */
} action_darray_thread_strategy_t;

/**
 * @class action_darray_thread_t
 * 执行action的线程。
 *
 *> 每个线程都有一个action darray，可以是共享的darray，也可以是私有的darray。
 */
struct _action_darray_thread_t {
  /**
   * @property {tk_thread_t*} thread
   * @annotation ["readable"]
   * 线程对象。
   */
  tk_thread_t* thread;

  /**
   * @property {waitable_action_darray_t*} darray
   * @annotation ["readable"]
   * action darray。
   */
  waitable_action_darray_t* darray;

  /**
   * @property {uint32_t} executed_actions_nr
   * @annotation ["readable"]
   * 已经执行action的个数。
   */
  uint32_t executed_actions_nr;

  /*private*/
  /*请求退出*/
  bool_t quit;

  /*已经退出*/
  bool_t quited;

  /*是否是共享的darray*/
  bool_t is_shared_darray;

  void* on_idle_ctx;
  void* on_quit_ctx;
  action_darray_thread_on_idle_t on_idle;
  action_darray_thread_on_quit_t on_quit;
  action_darray_thread_strategy_t strategy;
};

/**
 * @method action_darray_thread_create
 * @annotation ["constructor"]
 * 创建action_darray_thread对象。
 *
 * @return {action_darray_thread_t*} action_darray_thread对象。
 */
action_darray_thread_t* action_darray_thread_create(void);

/**
 * @method action_darray_thread_create_ex
 * @annotation ["constructor"]
 * 创建action_darray_thread对象。
 * 
 * @param {const char*}  name 名称。
 * @param {uint32_t}  stack_size 栈的大小。
 * @param {tk_thread_priority_t}  priority 优先级。
 *
 * @return {action_darray_thread_t*} action_darray_thread对象。
 */
action_darray_thread_t* action_darray_thread_create_ex(const char* name, uint32_t stack_size,
                                                       tk_thread_priority_t priority);

/**
 * @method action_darray_thread_create_with_darray
 * @annotation ["constructor"]
 * 创建action_darray_thread对象。
 *
 * @param {waitable_action_darray_t*} darray darray对象。
 *
 * @return {action_darray_thread_t*} action_darray_thread对象。
 */
action_darray_thread_t* action_darray_thread_create_with_darray(waitable_action_darray_t* darray);

/**
 * @method action_darray_thread_create_with_darray_ex
 * @annotation ["constructor"]
 * 创建action_darray_thread对象。
 *
 * @param {waitable_action_darray_t*} darray darray对象。
 * @param {const char*}  name 名称。
 * @param {uint32_t}  stack_size 栈的大小。
 * @param {tk_thread_priority_t}  priority 优先级。
 *
 * @return {action_darray_thread_t*} action_darray_thread对象。
 */
action_darray_thread_t* action_darray_thread_create_with_darray_ex(waitable_action_darray_t* darray,
                                                                   const char* name,
                                                                   uint32_t stack_size,
                                                                   tk_thread_priority_t priority);

/**
 * @method action_darray_thread_set_strategy
 * 设置策略
 *
 * @param {action_darray_thread_t*}         thread   action_darray_thread对象。
 * @param {action_darray_thread_strategy_t} strategy 策略
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t action_darray_thread_set_strategy(action_darray_thread_t* thread,
                                        action_darray_thread_strategy_t strategy);

/**
 * @method action_darray_thread_exec
 * 让线程执行action。
 *
 * @param {action_darray_thread_t*} thread action_darray_thread对象。
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_darray_thread_exec(action_darray_thread_t* thread, qaction_t* action);

/**
 * @method action_darray_thread_set_on_idle
 * 设置空闲时的回调函数。
 *
 * @param {action_darray_thread_t*} thread action_darray_thread对象。
 * @param {action_darray_thread_on_idle_t} on_idle 空闲时的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_darray_thread_set_on_idle(action_darray_thread_t* thread,
                                       action_darray_thread_on_idle_t on_idle, void* ctx);

/**
 * @method action_darray_thread_set_on_quit
 * 设置退出时的回调函数。
 *
 * @param {action_darray_thread_t*} thread action_darray_thread对象。
 * @param {action_darray_thread_on_quit_t} on_quit 退出时的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_darray_thread_set_on_quit(action_darray_thread_t* thread,
                                       action_darray_thread_on_quit_t on_quit, void* ctx);

/**
 * @method action_darray_thread_destroy
 * 销毁。
 *
 * @param {action_darray_thread_t*} thread action_darray_thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_darray_thread_destroy(action_darray_thread_t* thread);

END_C_DECLS

#endif /*TK_ACTION_DARRAY_THREAD_H*/
