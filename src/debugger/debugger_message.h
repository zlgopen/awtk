/**
 * File:   debugger_message.h
 * Author: AWTK Develop Team
 * Brief:  debugger message
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_MESSAGE_H
#define TK_DEBUGGER_MESSAGE_H

#include "debugger/debugger_fscript.h"

BEGIN_C_DECLS

/**
 * @enum debugger_req_type_t
 * @prefix DEBUGGER_REQ_
 * 请求的消息类型。
 */
typedef enum _debugger_req_type_t {
  DEBUGGER_REQ_NONE = 0,
  /**
   * @const DEBUGGER_REQ_ATTACH
   * attach请求码。
   */
  DEBUGGER_REQ_ATTACH,
  /**
   * @const DEBUGGER_REQ_STOP
   * 停止运行请求码。
   */
  DEBUGGER_REQ_STOP,
  /**
   * @const DEBUGGER_REQ_PAUSE
   * 暂停运行请求码。
   */
  DEBUGGER_REQ_PAUSE,
  /**
   * @const DEBUGGER_REQ_IS_PAUSED
   * 查询是否处于暂停状态请求码。
   */
  DEBUGGER_REQ_IS_PAUSED,
  /**
   * @const DEBUGGER_REQ_NEXT
   * 运行下一步请求码。
   */
  DEBUGGER_REQ_NEXT,
  /**
   * @const DEBUGGER_REQ_STEP_IN
   * 运行进入函数请求码。
   */
  DEBUGGER_REQ_STEP_IN,
  /**
   * @const DEBUGGER_REQ_STEP_OUT
   * 运行退出函数请求码。
   */
  DEBUGGER_REQ_STEP_OUT,
  /**
   * @const DEBUGGER_REQ_NEXT
   * 运行下一行请求码。
   */
  DEBUGGER_REQ_STEP_OVER,
  /**
   * @const DEBUGGER_REQ_CONTINUE
   * 运行到下一个断点请求码。
   */
  DEBUGGER_REQ_CONTINUE,
  /**
   * @const DEBUGGER_REQ_SET_BREAK_POINT
   * 设置断点请求码。
   */
  DEBUGGER_REQ_SET_BREAK_POINT,
  /**
   * @const DEBUGGER_REQ_REMOVE_BREAK_POINT
   * 移除断点请求码。
   */
  DEBUGGER_REQ_REMOVE_BREAK_POINT,
  /**
   * @const DEBUGGER_REQ_CLEAR_BREAK_POINTS
   * 清除断点请求码。
   */
  DEBUGGER_REQ_CLEAR_BREAK_POINTS,
  /**
   * @const DEBUGGER_REQ_GET_SELF
   * 获取self对象请求码。
   */
  DEBUGGER_REQ_GET_SELF,
  /**
   * @const DEBUGGER_REQ_GET_LOCAL
   * 获取局部变量和函数参数对象请求码。
   */
  DEBUGGER_REQ_GET_LOCAL,
  /**
   * @const DEBUGGER_REQ_GET_GLOBAL
   * 获取global对象请求码。
   */
  DEBUGGER_REQ_GET_GLOBAL,
  /**
   * @const DEBUGGER_REQ_GET_CODE
   * 获取源代码请求码。
   */
  DEBUGGER_REQ_GET_CODE,
  /**
   * @const DEBUGGER_REQ_GET_DEBUGGERS
   * 获取调试器列表。
   */
  DEBUGGER_REQ_GET_DEBUGGERS,
  /**
   * @const DEBUGGER_REQ_GET_BREAK_POINTS
   * 获取断点列表。
   */
  DEBUGGER_REQ_GET_BREAK_POINTS,
  /**
   * @const DEBUGGER_REQ_GET_CALLSTACK
   * 获取callstack请求码。
   */
  DEBUGGER_REQ_GET_CALLSTACK,
  /**
   * @const DEBUGGER_REQ_UPDATE_CODE
   * 更新源代码请求码。
   */
  DEBUGGER_REQ_UPDATE_CODE,
  /**
   * @const DEBUGGER_REQ_LAUNCH
   * 执行代码请求码。
   */
  DEBUGGER_REQ_LAUNCH,
  /**
   * @const DEBUGGER_REQ_DEINIT
   * 断开调试器请求码。
   */
  DEBUGGER_REQ_DEINIT,
} debugger_req_type_t;

/**
 * @enum debugger_resp_type_t
 * @prefix DEBUGGER_RESP_
 * 响应/推送的消息类型。
 */
typedef enum _debugger_resp_type_t {
  DEBUGGER_RESP_NONE = 0,
  /**
   * @const DEBUGGER_RESP_ATTACH
   * attach响应码。
   */
  DEBUGGER_RESP_ATTACH = DEBUGGER_REQ_ATTACH,
  /**
   * @const DEBUGGER_RESP_STOP
   * 停止运行响应码。
   */
  DEBUGGER_RESP_STOP = DEBUGGER_REQ_STOP,
  /**
   * @const DEBUGGER_RESP_IS_PAUSED
   * 查询是否处于暂停状态响应码。
   */
  DEBUGGER_RESP_IS_PAUSED = DEBUGGER_REQ_IS_PAUSED,
  /**
   * @const DEBUGGER_RESP_PAUSE
   * 暂停运行响应码。
   */
  DEBUGGER_RESP_PAUSE = DEBUGGER_REQ_PAUSE,
  /**
   * @const DEBUGGER_RESP_NEXT
   * 运行下一步响应码。
   */
  DEBUGGER_RESP_NEXT = DEBUGGER_REQ_NEXT,
  /**
   * @const DEBUGGER_RESP_STEP_IN
   * 运行进入函数响应码。
   */
  DEBUGGER_RESP_STEP_IN = DEBUGGER_REQ_STEP_IN,
  /**
   * @const DEBUGGER_RESP_STEP_OUT
   * 运行退出函数响应码。
   */
  DEBUGGER_RESP_STEP_OUT = DEBUGGER_REQ_STEP_OUT,
  /**
   * @const DEBUGGER_RESP_NEXT
   * 运行下一行响应码。
   */
  DEBUGGER_RESP_STEP_OVER = DEBUGGER_REQ_STEP_OVER,
  /**
   * @const DEBUGGER_RESP_CONTINUE
   * 运行到下一个断点响应码。
   */
  DEBUGGER_RESP_CONTINUE = DEBUGGER_REQ_CONTINUE,
  /**
   * @const DEBUGGER_REQ_SET_BREAK_POINT
   * 设置断点响应码。
   */
  DEBUGGER_RESP_SET_BREAK_POINT = DEBUGGER_REQ_SET_BREAK_POINT,
  /**
   * @const DEBUGGER_RESP_REMOVE_BREAK_POINT
   * 移除断点响应码。
   */
  DEBUGGER_RESP_REMOVE_BREAK_POINT = DEBUGGER_REQ_REMOVE_BREAK_POINT,
  /**
   * @const DEBUGGER_RESP_CLEAR_BREAK_POINTS
   * 清除断点响应码。
   */
  DEBUGGER_RESP_CLEAR_BREAK_POINTS = DEBUGGER_REQ_CLEAR_BREAK_POINTS,
  /**
   * @const DEBUGGER_RESP_GET_SELF
   * 获取self对象响应码。
   */
  DEBUGGER_RESP_GET_SELF = DEBUGGER_REQ_GET_SELF,
  /**
   * @const DEBUGGER_RESP_GET_LOCAL
   * 获取局部变量和函数参数对象响应码。
   */
  DEBUGGER_RESP_GET_LOCAL = DEBUGGER_REQ_GET_LOCAL,
  /**
   * @const DEBUGGER_RESP_GET_GLOBAL
   * 获取global对象响应码。
   */
  DEBUGGER_RESP_GET_GLOBAL = DEBUGGER_REQ_GET_GLOBAL,
  /**
   * @const DEBUGGER_RESP_GET_CODE
   * 获取源代码响应码。
   */
  DEBUGGER_RESP_GET_CODE = DEBUGGER_REQ_GET_CODE,
  /**
   * @const DEBUGGER_RESP_GET_DEBUGGERS
   * 获取调试器列表响应码。
   */
  DEBUGGER_RESP_GET_DEBUGGERS = DEBUGGER_REQ_GET_DEBUGGERS,
  /**
   * @const DEBUGGER_RESP_GET_BREAK_POINTS
   * 获取断点列表响应码。
   */
  DEBUGGER_RESP_GET_BREAK_POINTS = DEBUGGER_REQ_GET_BREAK_POINTS,
  /**
   * @const DEBUGGER_RESP_GET_CALLSTACK
   * 获取callstack响应码。
   */
  DEBUGGER_RESP_GET_CALLSTACK = DEBUGGER_REQ_GET_CALLSTACK,
  /**
   * @const DEBUGGER_RESP_UPDATE_CODE
   * 更新源代码响应码。
   */
  DEBUGGER_RESP_UPDATE_CODE = DEBUGGER_REQ_UPDATE_CODE,
  /**
   * @const DEBUGGER_RESP_LAUNCH
   * 执行代码响应码。
   */
  DEBUGGER_RESP_LAUNCH = DEBUGGER_REQ_LAUNCH,
  /**
   * @const DEBUGGER_RESP_DEINIT
   * 断开调试器响应码。
   */
  DEBUGGER_RESP_DEINIT = DEBUGGER_REQ_DEINIT,
  /**
   * @const DEBUGGER_RESP_MSG_LOG
   * 程序打印日志的响应码/事件码。
   */
  DEBUGGER_RESP_MSG_LOG = 0x1000,
  /**
   * @const DEBUGGER_RESP_MSG_ERROR
   * 程序出现错误的响应码/事件码。
   */
  DEBUGGER_RESP_MSG_ERROR,
  /**
   * @const DEBUGGER_RESP_MSG_BREAKED
   * 程序中断的响应码/事件码。
   */
  DEBUGGER_RESP_MSG_BREAKED,
  /**
   * @const DEBUGGER_RESP_MSG_COMPLETED
   * 程序执行完成的响应码/事件码。
   */
  DEBUGGER_RESP_MSG_COMPLETED,
} debugger_resp_type_t;

/**
 * @class debugger_req_t
 * 调试器请求头。
 */
typedef struct _debugger_req_t {
  /**
   * @property {uint32_t} code
   * @annotation ["readable"]
   * 请求码。
   */
  uint32_t code;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 数据长度(除请求头外)。
   */
  uint32_t size;
  /**
   * @property {uint32_t} version
   * @annotation ["readable"]
   * 版本号。
   */
  uint32_t version;
  /**
   * @property {uint32_t} data
   * @annotation ["readable"]
   * 数据。
   */
  uint32_t data;
} debugger_req_t;

/**
 * @class debugger_resp_t
 * 调试器响应头。
 */
typedef struct _debugger_resp_t {
  /**
   * @property {uint32_t} code
   * @annotation ["readable"]
   * 响应码。
   */
  uint32_t code;
  /**
   * @property {uint32_t} error
   * @annotation ["readable"]
   * 错误码(定义同ret_t)。
   */
  uint32_t error;
  /**
   * @property {uint32_t} version
   * @annotation ["readable"]
   * 版本号。
   */
  uint32_t version;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 数据长度(除请求头外)。
   */
  uint32_t size;
} debugger_resp_t;

/**
 * @class debugger_log_event_t
 * @parent event_t
 * 调试器打印日志的事件。
 */
typedef struct _debugger_log_event_t {
  event_t e;
  /**
   * @property {uint32_t} line
   * @annotation ["readable"]
   * 打印日志的行号。
   */
  uint32_t line;
  /**
   * @property {const char*} message
   * @annotation ["readable"]
   * 日志消息。
   */
  const char* message;
} debugger_log_event_t;

/**
 * @method debugger_log_event_init
 * 初始调试器打印日志的事件。
 *
 * @param {debugger_log_event_t*} event event对象。
 * @param {uint32_t} line 打印日志的行号。
 * @param {const char*} message 日志。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* debugger_log_event_init(debugger_log_event_t* event, uint32_t line, const char* message);

/**
 * @method debugger_log_event_cast
 * @annotation ["cast"]
 *
 * 把event对象转debugger_log_event_t对象。
 * @param {event_t*} event event对象。
 *
 * @return {debugger_log_event_t*}  返回event对象。
 */
debugger_log_event_t* debugger_log_event_cast(event_t* event);

/**
 * @class debugger_error_event_t
 * @parent event_t
 * 程序错误事件。
 */
typedef struct _debugger_error_event_t {
  event_t e;
  /**
   * @property {uint32_t} line
   * @annotation ["readable"]
   * 出现错误的行号。
   */
  uint32_t line;
  /**
   * @property {const char*} message
   * @annotation ["readable"]
   * 错误消息。
   */
  const char* message;
} debugger_error_event_t;

/**
 * @method debugger_error_event_init
 * 初始程序错误的事件。
 *
 * @param {debugger_error_event_t*} event event对象。
 * @param {uint32_t} line 出现错误的行号。
 * @param {const char*} message 错误信息。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* debugger_error_event_init(debugger_error_event_t* event, uint32_t line,
                                   const char* message);

/**
 * @method debugger_error_event_cast
 * @annotation ["cast"]
 *
 * 把event对象转debugger_error_event_t对象。
 * @param {event_t*} event event对象。
 *
 * @return {debugger_error_event_t*}  返回event对象。
 */
debugger_error_event_t* debugger_error_event_cast(event_t* event);

/**
 * @class debugger_breaked_event_t
 * @parent event_t
 * 调试器中断运行的事件。
 */
typedef struct _debugger_breaked_event_t {
  event_t e;
  /**
   * @property {uint32_t} line
   * @annotation ["readable"]
   * 中断运行的行号。
   */
  uint32_t line;
} debugger_breaked_event_t;

/**
 * @method debugger_breaked_event_init
 * 初始调试器中断运行的事件。
 *
 * @param {debugger_breaked_event_t*} event event对象。
 * @param {uint32_t} line 中断运行的行号。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* debugger_breaked_event_init(debugger_breaked_event_t* event, uint32_t line);

/**
 * @method debugger_breaked_event_cast
 * @annotation ["cast"]
 *
 * 把event对象转debugger_breaked_event_t对象。
 * @param {event_t*} event event对象。
 *
 * @return {debugger_breaked_event_t*}  返回event对象。
 */
debugger_breaked_event_t* debugger_breaked_event_cast(event_t* event);

END_C_DECLS

#endif /*TK_DEBUGGER_MESSAGE_H*/
