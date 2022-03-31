/**
 * File:   waitable_action_darray.h
 * Author: AWTK Develop Team
 * Brief:  waitable_action_darray
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

#ifndef TK_WAITABLE_ACTION_DARRAY_H
#define TK_WAITABLE_ACTION_DARRAY_H

#include "tkc/mutex.h"
#include "tkc/mutex.h"
#include "tkc/darray.h"
#include "tkc/qaction.h"
#include "tkc/semaphore.h"

BEGIN_C_DECLS

/**
 * @class waitable_action_darray_t
 * 
 * waitable actionqueue 
 */
typedef struct _waitable_action_darray_t {
  darray_t* darray;
  tk_mutex_t* mutex;
  tk_semaphore_t* sema_recv;
  tk_semaphore_t* sema_send;
} waitable_action_darray_t;

/**
 * @method waitable_action_darray_create
 * @annotation ["constructor"]
 * 创建waitable_action_darray对象。
 *
 * @param {uint32_t} capacity action的容量。
 *
 * @return {waitable_action_darray_t*} waitable_action_darray对象。
 */
waitable_action_darray_t* waitable_action_darray_create(uint16_t capacity);

/**
 * @method waitable_action_darray_recv
 * 接收一个请求。
 *
 * @param {waitable_action_darray_t*} q waitable_action_darray对象。
 * @param {qaction_t**} action 用于返回action对象。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_darray_recv(waitable_action_darray_t* q, qaction_t** action,
                                  uint32_t timeout_ms);

/**
 * @method waitable_action_darray_send
 * 发送一个请求。
 *
 * @param {waitable_action_darray_t*} q waitable_action_darray对象。
 * @param {qaction_t*} action action对象。
 * @param {uint32_t} timeout_ms 超时时间(ms)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_darray_send(waitable_action_darray_t* q, qaction_t* action,
                                  uint32_t timeout_ms);

/**
 * @method waitable_action_darray_find
 * 查找 qaction
 *
 * @param {waitable_action_darray_t*} q    waitable_action_darray对象。
 * @param {qaction_exec_t}            exec action 中的函数回调
 *
 * @return {qaction_t*} 返回 qaction_t对象
 */
qaction_t* waitable_action_darray_find(waitable_action_darray_t* q, qaction_exec_t exec);

/**
 * @method waitable_action_darray_find_ex
 * 查找 qaction
 *
 * @param {waitable_action_darray_t*} q   waitable_action_darray对象。
 * @param {tk_compare_t}              cmp 比较函数
 * @param {void*}                     ctx 比较函数参数
 *
 * @return {qaction_t*} 返回 qaction_t对象
 */
qaction_t* waitable_action_darray_find_ex(waitable_action_darray_t* q, tk_compare_t cmp, void* ctx);

/**
 * @method waitable_action_darray_replace
 * 替换 qaction，旧的 qaction 会被销毁
 *
 * @param {waitable_action_darray_t*} q          waitable_action_darray对象。
 * @param {qaction_t*}                new_action 新的 action 对象
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t waitable_action_darray_replace(waitable_action_darray_t* q, qaction_t* new_action);

/**
 * @method waitable_action_darray_replace_ex
 * 替换 qaction
 *
 * @param {waitable_action_darray_t*} q          waitable_action_darray对象。
 * @param {qaction_exec_t}            exec       旧 action 中的函数回调
 * @param {qaction_t*}                new_action 新的 action 对象
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t waitable_action_darray_replace_ex(waitable_action_darray_t* q, qaction_exec_t exec,
                                        qaction_t* new_action);

/**
 * @method waitable_action_darray_remove
 * 从 darray 中移除并销毁 qaction 对象
 *
 * @param {waitable_action_darray_t*} q    waitable_action_darray对象。
 * @param {qaction_exec_t}            exec action 中的函数回调
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t waitable_action_darray_remove(waitable_action_darray_t* q, qaction_exec_t exec);

/**
 * @method waitable_action_darray_remove_ex
 * 从 darray 中移除并销毁 qaction 对象
 *
 * @param {waitable_action_darray_t*} q   waitable_action_darray对象。
 * @param {tk_compare_t}              cmp 比较函数
 * @param {void*}                     ctx 比较函数参数
 *
 * @return {ret_t} 返回 ret_t值
 */
ret_t waitable_action_darray_remove_ex(waitable_action_darray_t* q, tk_compare_t cmp, void* ctx);

/**
 * @method waitable_action_darray_destroy
 * 销毁。
 *
 * @param {waitable_action_darray_t*} q waitable_action_darray对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t waitable_action_darray_destroy(waitable_action_darray_t* q);

END_C_DECLS

#endif /*TK_WAITABLE_ACTION_DARRAY_H*/
