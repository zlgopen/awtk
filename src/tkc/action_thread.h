/**
 * File:   action_thread.h
 * Author: AWTK Develop Team
 * Brief:  action_thread
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ACTION_THREAD_H
#define TK_ACTION_THREAD_H

#include "tkc/thread.h"
#include "tkc/waitable_action_queue.h"

BEGIN_C_DECLS

struct _action_thread_t;
typedef struct _action_thread_t action_thread_t;

typedef ret_t (*action_thread_on_idle_t)(void* ctx, action_thread_t* thread);
typedef ret_t (*action_thread_on_quit_t)(void* ctx, action_thread_t* thread);

/**
 * @class action_thread_t
 * 执行action的线程。
 *
 *> 每个线程都有一个action queue，可以是共享的queue，也可以是私有的queue。
 */
struct _action_thread_t {
  /**
   * @property {tk_thread_t*} thread
   * @annotation ["readable"]
   * 线程对象。
   */
  tk_thread_t* thread;

  /**
   * @property {waitable_action_queue_t*} queue
   * @annotation ["readable"]
   * action queue。
   */
  waitable_action_queue_t* queue;

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

  /*是否是共享的queue*/
  bool_t is_shared_queue;

  void* on_idle_ctx;
  void* on_quit_ctx;
  action_thread_on_idle_t on_idle;
  action_thread_on_quit_t on_quit;
};

/**
 * @method action_thread_create
 * @annotation ["constructor"]
 * 创建action_thread对象。
 *
 * @return {action_thread_t*} action_thread对象。
 */
action_thread_t* action_thread_create(void);

/**
 * @method action_thread_create_with_queue
 * @annotation ["constructor"]
 * 创建action_thread对象。
 *
 * @param {waitable_action_queue_t*} queue queue对象。
 *
 * @return {action_thread_t*} action_thread对象。
 */
action_thread_t* action_thread_create_with_queue(waitable_action_queue_t* queue);

/**
 * @method action_thread_exec
 * 让线程执行action。
 *
 * @param {action_thread_t*} thread action_thread对象。
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_exec(action_thread_t* thread, qaction_t* action);

/**
 * @method action_thread_set_on_idle
 * 设置空闲时的回调函数。
 *
 * @param {action_thread_t*} thread action_thread对象。
 * @param {action_thread_on_idle_t} on_idle 空闲时的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_set_on_idle(action_thread_t* thread, action_thread_on_idle_t on_idle,
                                void* ctx);

/**
 * @method action_thread_set_on_quit
 * 设置退出时的回调函数。
 *
 * @param {action_thread_t*} thread action_thread对象。
 * @param {action_thread_on_quit_t} on_quit 退出时的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_set_on_quit(action_thread_t* thread, action_thread_on_quit_t on_quit,
                                void* ctx);

/**
 * @method action_thread_destroy
 * 销毁。
 *
 * @param {action_thread_t*} thread action_thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_destroy(action_thread_t* thread);

END_C_DECLS

#endif /*TK_ACTION_THREAD_H*/
