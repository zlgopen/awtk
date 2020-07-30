/**
 * File:   event.h
 * Author: AWTK Develop Team
 * Brief:  event
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EVENT_H
#define TK_EVENT_H

#include "tkc/value.h"

BEGIN_C_DECLS

/**
 * @enum event_type_t
 * @annotation ["scriptable"]
 * @prefix EVT_
 * 类型常量定义。
 */
typedef enum _event_base_type_t {
  /**
   * @const EVT_NONE
   * 无效事件名称。
   */
  EVT_NONE = 0,
  /**
   * @const EVT_PROP_WILL_CHANGE
   * 对象的属性即将改变的事件名(prop_change_event_t)。
   */
  EVT_PROP_WILL_CHANGE,
  /**
   * @const EVT_PROP_CHANGED
   * 对象的属性改变的事件名(prop_change_event_t)。
   */
  EVT_PROP_CHANGED,
  /**
   * @const EVT_CMD_WILL_EXEC
   * 对象即将执行命令(cmd_exec_event_t)。
   */
  EVT_CMD_WILL_EXEC,
  /**
   * @const EVT_CMD_EXECED
   * 对象完成执行命令(cmd_exec_event_t)。
   */
  EVT_CMD_EXECED,
  /**
   * @const EVT_CMD_CAN_EXEC
   * 对象命令是否能执行指定的命令(cmd_exec_event_t)。
   */
  EVT_CMD_CAN_EXEC,
  /**
   * @const EVT_ITEMS_WILL_CHANGE
   * 即将增加和删除集合中的项目(event_t)。
   */
  EVT_ITEMS_WILL_CHANGE,
  /**
   * @const EVT_ITEMS_CHANGED
   * 完成增加和删除集合中的项目(event_t)。
   */
  EVT_ITEMS_CHANGED,
  /**
   * @const EVT_PROPS_CHANGED
   * 对象的属性改变的事件名(props_event_t)。
   */
  EVT_PROPS_CHANGED,
  /**
   * @const EVT_PROGRESS
   * 进度状态(progress_event_t)。
   */
  EVT_PROGRESS,
  /**
   * @const EVT_DONE
   * 完成(done_event_t)。
   */
  EVT_DONE,
  /**
   * @const EVT_ERROR
   * 错误(error_event_t)。
   */
  EVT_ERROR,
  /**
   * @const EVT_DESTROY
   * 对象销毁事件名(event_t)。
   */
  EVT_DESTROY
} event_base_type_t;

/**
 * @class event_t
 * @order -100
 * @annotation ["scriptable"]
 * 事件基类。
 */
typedef struct _event_t {
  /**
   * @property {int32_t} type
   * @annotation ["readable", "scriptable"]
   * 类型。
   */
  uint32_t type;
  /**
   * @property {uint64_t} time
   * @annotation ["readable", "scriptable"]
   * 事件发生的时间。
   */
  uint64_t time;
  /**
   * @property {void*} target
   * @annotation ["readable", "scriptable"]
   * 事件发生的目标对象。
   */
  void* target;

  void* native_window_handle;
} event_t;

/*事件处理函数原型*/
typedef ret_t (*event_func_t)(void* ctx, event_t* e);

/**
 * @method event_cast
 * 转换为event对象。
 *
 * > 供脚本语言使用
 * @annotation ["cast", "scriptable"]
 * @param {event_t*} event event对象。
 *
 * @return {event_t*} event对象。
 */
event_t* event_cast(event_t* event);

/**
 * @method event_create
 * @annotation ["constructor", "scriptable", "gc"]
 * 创建event对象。
 *
 * 主要给脚本语言使用。
 * @param {uint32_t} type 事件类型。
 *
 * @return {event_t*} 返回事件对象。
 */
event_t* event_create(uint32_t type);

/**
 * @method event_destroy
 * 销毁事件对象。
 *
 * 主要给脚本语言使用。
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {event_t*} event event对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t event_destroy(event_t* event);

/**
 * @method event_init
 * 初始化事件。
 * @annotation ["constructor"]
 * @param {uint32_t} type 事件类型。
 * @param {void*} target 目标对象。
 *
 * @return {event_t} 事件对象。
 */
event_t event_init(uint32_t type, void* target);

/**
 * @class prop_change_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 对象属性变化事件。
 */
typedef struct _prop_change_event_t {
  event_t e;
  /**
   * @property {const char*} name
   * @annotation ["readable", "scriptable"]
   * 属性的名称。
   */
  const char* name;
  /**
   * @property {const value_t*} value
   * @annotation ["readable", "scriptable"]
   * 属性的值。
   */
  const value_t* value;
} prop_change_event_t;

/**
 * @method prop_change_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转prop_change_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {prop_change_event_t*}  返回event对象。
 */
prop_change_event_t* prop_change_event_cast(event_t* event);

/**
 * @method prop_change_event_init
 * 初始prop change event。
 * 
 * @param {prop_change_event_t*} event event对象。
 * @param {uint32_t} type 事件类型。
 * @param {const char*} name 属性名。
 * @param {const value_t*} value 属性的值。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* prop_change_event_init(prop_change_event_t* event, uint32_t type, const char* name,
                                const value_t* value);

/**
 * @class progress_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 进度变化事件。
 */
typedef struct _progress_event_t {
  event_t e;
  /**
   * @property {uint32_t} percent
   * @annotation ["readable", "scriptable"]
   * 进度百分比。
   */
  uint32_t percent;
} progress_event_t;

/**
 * @method progress_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转progress_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {progress_event_t*}  返回event对象。
 */
progress_event_t* progress_event_cast(event_t* event);

/**
 * @method progress_event_init
 * 初始progress event。
 * 
 * @param {progress_event_t*} event event对象。
 * @param {uint32_t} percent 进度。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* progress_event_init(progress_event_t* event, uint32_t percent);

/**
 * @class done_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 执行完成事件。
 */
typedef struct _done_event_t {
  event_t e;
  /**
   * @property {ret_t} result
   * @annotation ["readable", "scriptable"]
   * 执行结果。
   */
  ret_t result;
} done_event_t;

/**
 * @method done_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转done_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {done_event_t*}  返回event对象。
 */
done_event_t* done_event_cast(event_t* event);

/**
 * @method done_event_init
 * 初始done event。
 * 
 * @param {done_event_t*} event event对象。
 * @param {ret_t} result 结果。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* done_event_init(done_event_t* event, ret_t result);

/**
 * @class error_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 执行完成事件。
 */
typedef struct _error_event_t {
  event_t e;
  /**
   * @property {int32_t} code
   * @annotation ["readable", "scriptable"]
   * 错误码。
   */
  int32_t code;

  /**
   * @property {const char*} message
   * @annotation ["readable", "scriptable"]
   * 错误信息。
   */
  const char* message;
} error_event_t;

/**
 * @method error_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转error_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {error_event_t*}  返回event对象。
 */
error_event_t* error_event_cast(event_t* event);

/**
 * @method error_event_init
 * 初始error event。
 * 
 * @param {error_event_t*} event event对象。
 * @param {int32_t} code 错误码。
 * @param {const char*} message 错误消息。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* error_event_init(error_event_t* event, int32_t code, const char* message);

/**
 * @class cmd_exec_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 对象执行命令的事件。
 */
typedef struct _cmd_exec_event_t {
  event_t e;
  /**
   * @property {const char*} name
   * @annotation ["readable", "scriptable"]
   * 命令的名称。
   */
  const char* name;
  /**
   * @property {const char*} args
   * @annotation ["readable", "scriptable"]
   * 命令的参数。
   */
  const char* args;

  /**
   * @property {ret_t} result
   * @annotation ["readable", "scriptable"]
   * 执行结果(适用于EXECED)。
   */
  ret_t result;

  /**
   * @property {bool_t} can_exec
   * @annotation ["readable", "scriptable"]
   * 执行结果(适用于CAN_EXEC)。
   */
  bool_t can_exec;
} cmd_exec_event_t;

/**
 * @method cmd_exec_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转cmd_exec_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {cmd_exec_event_t*}  返回event对象。
 */
cmd_exec_event_t* cmd_exec_event_cast(event_t* event);

/**
 * @method cmd_exec_event_init
 * 初始命令执行事件。
 * 
 * @param {cmd_exec_event_t*} event event对象。
 * @param {uint32_t} type 事件类型。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令参数。
 *
 * @return {event_t*} 返回event对象。
 */
event_t* cmd_exec_event_init(cmd_exec_event_t* event, uint32_t type, const char* name,
                             const char* args);

END_C_DECLS

#endif /**TK_EVENT_H*/
