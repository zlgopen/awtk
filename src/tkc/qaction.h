/**
 * File:   qaction.h
 * Author: AWTK Develop Team
 * Brief:  qaction
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

#ifndef TK_QACTION_H
#define TK_QACTION_H

#include "tkc/event.h"

BEGIN_C_DECLS

struct _qaction_t;
typedef struct _qaction_t qaction_t;

typedef ret_t (*qaction_exec_t)(qaction_t* action);
typedef ret_t (*qaction_on_event_t)(qaction_t* action, event_t* event);

/**
 * @class qaction_t
 * 代表一个action，放在action queue中。
 */
struct _qaction_t {
  /**
   * @property {qaction_exec_t} exec
   * @annotation ["readable"]
   * 执行函数。
   */
  qaction_exec_t exec;

  /**
   * @property {qaction_on_event_t} on_event
   * @annotation ["readable"]
   * 事件处理函数。如进度、错误和完成等。
   */
  qaction_on_event_t on_event;

  /**
   * @property {uint32_t*} args
   * @annotation ["readable"]
   * exec的参数(视具体的action而不同)。
   */
  uint32_t args[1];
};

/**
 * @method qaction_create
 * 创建action对象。
 *
 * @param {qaction_exec_t} exec 执行函数。
 * @param {void*} args 参数。
 * @param {uint32_t} args_size 参数长度。
 *
 * @return {qaction_t*} 返回action对象。
 */
qaction_t* qaction_create(qaction_exec_t exec, void* args, uint32_t args_size);

/**
 * @method qaction_set_on_event
 * 设置事件处理函数(回调函数在后台线程执行)。
 *
 *> exec执行完成后，会触发EVT\_DONE事件，一般在EVT\_DONE事件中调用qaction\_destroy函数销毁action。
 *
 * @param {qaction_t*} action action对象。
 * @param {qaction_on_event_t} on_event 事件处理函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_set_on_event(qaction_t* action, qaction_on_event_t on_event);

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
 * @method qaction_destroy
 * 销毁。
 *
 * @param {qaction_t*} q qaction对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qaction_destroy(qaction_t* q);

END_C_DECLS

#endif /*TK_QACTION_H*/
