/**
 * File:   thread.h
 * Author: AWTK Develop Team
 * Brief:  thread
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THREAD_H
#define TK_THREAD_H

#include "tkc/types_def.h"

/**
 * @class tk_thread_t
 * 线程对象。
 */
struct _tk_thread_t;
typedef struct _tk_thread_t tk_thread_t;

typedef void* (*tk_thread_entry_t)(void* args);

BEGIN_C_DECLS

typedef enum _tk_thread_priority_t {
  TK_THREAD_PRIORITY_NORMAL = 0x0,
  TK_THREAD_PRIORITY_IDLE,
  TK_THREAD_PRIORITY_LOWEST,
  TK_THREAD_PRIORITY_BELOW_NORAML,
  TK_THREAD_PRIORITY_ABOVE_NORAML,
  TK_THREAD_PRIORITY_HIGHEST,
  TK_THREAD_PRIORITY_TIME_CRITICAL,
} tk_thread_priority_t;

/**
 * @method tk_thread_create
 * 创建thread对象。
 *
 * @param {tk_thread_entry_t}  entry 线程函数。
 * @param {void*}  args 线程函数的上下文。
 *
 * @return {tk_thread_t*} thread对象。
 */
tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args);

/**
 * @method tk_thread_set_name
 * 设置线程的名称。
 *
 * > 需要在调用start之前调用本函数。
 * @param {tk_thread_t*} thread thread对象。
 * @param {const char*}  name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_set_name(tk_thread_t* thread, const char* name);

/**
 * @method tk_thread_set_stack_size
 * 设置线程的栈大小。
 *
 * > 需要在调用start之前调用本函数。
 * @param {tk_thread_t*} thread thread对象。
 * @param {uint32_t}  stack_size 栈的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_set_stack_size(tk_thread_t* thread, uint32_t stack_size);

/**
 * @method tk_thread_set_priority
 * 设置线程的优先级。
 *
 * > 部分平台支持。
 * @param {tk_thread_t*} thread thread对象。
 * @param {tk_thread_priority_t}  priority 优先级。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_set_priority(tk_thread_t* thread, tk_thread_priority_t priority);

/**
 * @method tk_thread_get_priority_from_platform
 * 获取平台相关的优先级
 * 
 * > 部分平台支持。
 * > 根据 AWTK 通用优先级枚举获取平台相关的优先级
 * @annotation ["static"]
 * @param {tk_thread_priority_t}  priority 优先级。
 *
 * @return {int32_t} 返回平台相关的优先级。
 */
int32_t tk_thread_get_priority_from_platform(tk_thread_priority_t priority);

/**
 * @method tk_thread_start
 * 启动线程。
 * @param {tk_thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_start(tk_thread_t* thread);

/**
 * @method tk_thread_join
 * 等待线程退出。
 * 必须调用 tk_thread_destroy 函数来释放线程资源，以免出现内存泄漏的问题。
 * @param {tk_thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_join(tk_thread_t* thread);

/**
 * @method tk_thread_get_args
 * 获取线程的参数。
 * @param {tk_thread_t*}    thread thread对象。
 *
 * @return {void*} 返回线程的参数。
 */
void* tk_thread_get_args(tk_thread_t* thread);

/**
 * @method tk_thread_destroy
 * 销毁thread对象。
 * 在销毁对象前必须调用 tk_thread_join 函数等待退出线程
 * @param {tk_thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_thread_destroy(tk_thread_t* thread);

/**
 * @method tk_thread_self
 * 获取当前线程的原生句柄。
 *
 * @return {uint64_t} 返回当前线程的原生句柄。
 */
uint64_t tk_thread_self(void);

END_C_DECLS

#endif /*TK_THREAD_H*/
