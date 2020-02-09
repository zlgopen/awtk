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

struct _action_thread_pool_t;
typedef struct _action_thread_pool_t action_thread_pool_t;

/**
 * @class action_thread_t
 * 执行action的线程。
 *
 *> 每个线程都有一个action queue。
 */
typedef struct _action_thread_t {
  /**
   * @property {bool_t} running
   * @annotation ["readable"]
   * 当前是否在运行。
   */
  bool_t running;

  /**
   * @property {tk_thread_t*} thread
   * @annotation ["readable"]
   * 线程对象。
   */
  tk_thread_t* thread;

  /**
   * @property {action_thread_pool_t*} thread_pool
   * @annotation ["readable"]
   * 线程池对象。
   */
  action_thread_pool_t* thread_pool;

  /**
   * @property {waitable_action_queue_t*} queue
   * @annotation ["readable"]
   * action queue。
   */
  waitable_action_queue_t* queue;

  /**
   * @property {uint32_t} max_actions_nr
   * @annotation ["readable"]
   * 执行到max_actions_nr个action后自动回收。
   */
  uint32_t max_actions_nr;

  /**
   * @property {uint32_t} executed_actions_nr
   * @annotation ["readable"]
   * 已经执行action的个数。
   */
  uint32_t executed_actions_nr;

  /*private*/
  bool_t quit;
} action_thread_t;

/**
 * @method action_thread_create
 * @annotation ["constructor"]
 * @param {action_thread_pool_t*} thread_pool thread_pool对象。
 * 创建action_thread对象。
 *
 * @return {action_thread_t*} action_thread对象。
 */
action_thread_t* action_thread_create(action_thread_pool_t* thread_pool);

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
 * @method action_thread_set_max_actions_nr
 * 设置max_actions_nr，让线程执行max_actions_nr个action后，自动回收。
 *
 * @param {action_thread_t*} thread action_thread对象。
 * @param {uint32_t} max_actions_nr 最大action个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_set_max_actions_nr(action_thread_t* thread, uint32_t max_actions_nr);

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
