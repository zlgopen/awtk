/**
 * File:   waitable_action_queue.h
 * Author: AWTK Develop Team
 * Brief:  waitable_action_queue
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
 * 2020-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WAITABLE_ACTION_QUEUE_H
#define TK_WAITABLE_ACTION_QUEUE_H

#include "tkc/mutex.h"
#include "tkc/semaphore.h"
#include "tkc/action_queue.h"

BEGIN_C_DECLS

/**
 * @class waitable_action_queue_t
 * 
 * waitable actionqueue 
 */
typedef struct _waitable_action_queue_t {
  action_queue_t* queue;
  tk_mutex_t* mutex;
  tk_semaphore_t* sema_recv;
  tk_semaphore_t* sema_send;
} waitable_action_queue_t;

/**
 * @method waitable_action_queue_create
 * @annotation ["constructor"]
 * 创建waitable_action_queue对象。
 *
 * @param {uint32_t} capacity action的容量。
 *
 * @return {waitable_action_queue_t*} waitable_action_queue对象。
 */
waitable_action_queue_t* waitable_action_queue_create(uint16_t capacity);

/**
 * @method waitable_action_queue_recv
 * 接收一个请求。
 *
 * @param {waitable_action_queue_t*} q waitable_action_queue对象。
 * @param {qaction_t**} action 用于返回action对象。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_queue_recv(waitable_action_queue_t* q, qaction_t** action,
                                 uint32_t timeout_ms);

/**
 * @method waitable_action_queue_send
 * 发送一个请求。
 *
 * @param {waitable_action_queue_t*} q waitable_action_queue对象。
 * @param {qaction_t*} action action对象。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_queue_send(waitable_action_queue_t* q, qaction_t* action,
                                 uint32_t timeout_ms);

/**
 * @method waitable_action_queue_destroy
 * 销毁。
 *
 * @param {waitable_action_queue_t*} q waitable_action_queue对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_queue_destroy(waitable_action_queue_t* q);

END_C_DECLS

#endif /*TK_WAITABLE_ACTION_QUEUE_H*/
