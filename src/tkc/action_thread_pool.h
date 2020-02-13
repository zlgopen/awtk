/**
 * File:   action_thread_pool.h
 * Author: AWTK Develop Team
 * Brief:  action_thread_pool
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

#ifndef TK_ACTION_THREAD_POOL_H
#define TK_ACTION_THREAD_POOL_H

#include "tkc/action_thread.h"

BEGIN_C_DECLS

/**
 * @class action_thread_pool_t
 * action线程池。
 */
typedef struct _action_thread_pool_t {
  /**
   * @property {uint32_t} max_thread_nr
   * @annotation ["readable"]
   * 最大线程数。
   */
  uint32_t max_thread_nr;

  /**
   * @property {uint32_t} min_idle_nr
   * @annotation ["readable"]
   * 最小空闲线程数
   */
  uint32_t min_idle_nr;

  /*private*/
  tk_mutex_t* mutex;
  waitable_action_queue_t* queue;
  action_thread_t* threads[1];
} action_thread_pool_t;

/**
 * @method action_thread_pool_create
 * @annotation ["constructor"]
 * 创建action_thread_pool对象。
 *
 * @param {uint16_t} max_thread_nr 最大线程数。
 * @param {uint16_t} min_idle_nr 最小空闲线程数。
 *
 * @return {action_thread_pool_t*} action_thread_pool对象。
 */
action_thread_pool_t* action_thread_pool_create(uint16_t max_thread_nr, uint16_t min_idle_nr);

/**
 * @method action_thread_pool_exec
 * 执行action。
 *
 * @param {action_thread_pool_t*} thread_pool action_thread_pool对象。
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_pool_exec(action_thread_pool_t* thread_pool, qaction_t* action);

/**
 * @method action_thread_pool_destroy
 * 销毁。
 *
 * @param {action_thread_pool_t*} thread_pool action_thread_pool对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_thread_pool_destroy(action_thread_pool_t* thread_pool);

END_C_DECLS

#endif /*TK_ACTION_THREAD_POOL_H*/
