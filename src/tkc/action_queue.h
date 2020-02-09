/**
 * File:   action_queue.h
 * Author: AWTK Develop Team
 * Brief:  action_queue
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
 * 2020-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ACTION_QUEUE_H
#define TK_ACTION_QUEUE_H

#include "tkc/event.h"

BEGIN_C_DECLS

struct _qaction_t;
typedef struct _qaction_t qaction_t;

typedef ret_t (*qaction_exec_t)(qaction_t* action);
typedef ret_t (*qaction_on_event_t)(qaction_t* action, event_t* event);

/**
 * @class qaction_t
 * 代表一个action，放在action_queue中。
 */
struct _qaction_t {
  /**
   * @property {void*} on_event_ctx 
   * @annotation ["readable"]
   * on_event_ctx。
   */
  void* on_event_ctx;
  
  /**
   * @property {qaction_on_event_t} on_event
   * @annotation ["readable"]
   * 事件处理函数。如进度、错误和完成等。
   */
  qaction_on_event_t on_event;
  
  /**
   * @property {qaction_exec_t} exec
   * @annotation ["readable"]
   * 执行函数。
   */
  qaction_exec_t exec;

  /**
   * @property {uint32_t*} args
   * @annotation ["readable"]
   * exec的参数(视具体的action而不同)。
   */
  uint32_t args[8];
};

/**
 * @method qaction_init
 * 初始化。
 *
 * @param {qaction_t*} action action对象。
 * @param {qaction_exec_t} exec 执行函数。
 * @param {void*} args 参数。
 * @param {uint32_t} args_size 参数长度(小余等于sizeof(action->args))。
 *
 * @return {qaction_t*} 返回action对象。
 */
qaction_t* qaction_init(qaction_t* action, qaction_exec_t exec, void* args, uint32_t args_size); 

/**
 * @method qaction_set_on_event
 * 设置事件处理函数(在后台线程调用)。
 *
 * @param {qaction_t*} action action对象。
 * @param {qaction_on_event_t} on_event 事件处理函数。
 * @param {void*} on_event_ctx 事件处理函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_set_on_event(qaction_t* action, qaction_on_event_t on_event, void* on_event_ctx);

/**
 * @method qaction_notify
 * 事件通知。
 *
 * @param {qaction_t*} action action对象。
 * @param {event_t**} event event对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_notify(qaction_t* action, event_t* event);

/**
 * @method qaction_exec
 * 执行。
 *
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_exec(qaction_t* action);

typedef struct _action_queue_t {
  uint16_t r;
  uint16_t w;
  uint16_t full;
  uint16_t capacity;
  qaction_t actions[1];
} action_queue_t;

/**
 * @method action_queue_create
 * @annotation ["constructor"]
 * 创建action_queue对象。
 *
 * @param {uint32_t} capacity action的容量。
 *
 * @return {action_queue_t*} action_queue对象。
 */
action_queue_t* action_queue_create(uint16_t capacity);

/**
 * @method action_queue_recv
 * 接收一个请求。
 *
 * @param {action_queue_t*} q action_queue对象。
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_queue_recv(action_queue_t* q, qaction_t* action);

/**
 * @method action_queue_send
 * 发送一个请求。
 *
 * @param {action_queue_t*} q action_queue对象。
 * @param {const qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_queue_send(action_queue_t* q, const qaction_t* action);

/**
 * @method action_queue_destroy
 * 销毁。
 *
 * @param {action_queue_t*} q action_queue对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t action_queue_destroy(action_queue_t* q);

END_C_DECLS

#endif /*TK_ACTION_QUEUE_H*/
