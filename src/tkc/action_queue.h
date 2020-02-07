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

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _qaction_t;
typedef struct _qaction_t qaction_t;

typedef ret_t (*qaction_exec_t)(qaction_t* action);
typedef ret_t (*qaction_destroy_t)(qaction_t* action);

typedef struct _qaction_vtable_t {
  qaction_exec_t exec;
  qaction_destroy_t destroy;
} qaction_vtable_t;

/**
 * @class qaction_t
 * 代表一个action，放在action_queue中。
 */
struct _qaction_t {
  /**
   * @property {uint32_t*} extra
   * @annotation ["readable"]
   * 额外的参数(视具体的action而不同)。
   */
  uint32_t extra[8];

  /*private*/
  qaction_vtable_t* vt;
};

/**
 * @method qaction_exec
 * 执行。
 *
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_exec(qaction_t* action);

/**
 * @method qaction_destroy
 * 销毁。
 *
 * @param {qaction_t*} action action对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_destroy(qaction_t* action);

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
