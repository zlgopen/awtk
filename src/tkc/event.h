/**
 * File:   event.h
 * Author: AWTK Develop Team
 * Brief:  event
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @enum event_base_type_t
 * @annotation ["scriptable"]
 * @prefix EVT_
 * 类型常量定义。
 */
typedef enum _event_base_type_t {
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
   * @const EVT_DESTROY
   * 对象销毁事件名(event_t)。
   */
  EVT_DESTROY
} event_base_type_t;

/**
 * @class event_t
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
   * @property {int32_t} time
   * @annotation ["readable", "scriptable"]
   * 事件发生的时间。
   */
  uint32_t time;
  /**
   * @property {void*} target
   * @annotation ["readable", "scriptable"]
   * 事件发生的目标对象。
   */
  void* target;
} event_t;

/**
 * @method event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转wheel_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {event_t*} 对象。
 */
event_t* event_cast(event_t* event);

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

/*事件处理函数原型*/
typedef ret_t (*event_func_t)(void* ctx, event_t* e);

/**
 * @class prop_change_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 对象属性变化事件。
 */
typedef struct _prop_change_event_t {
  event_t e;
  /**
   * @property {char*} name
   * @annotation ["readable", "scriptable"]
   * 属性的名称。
   */
  const char* name;
  /**
   * @property {value_t*} value
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
 * @return {prop_change_event_t*} 对象。
 */
prop_change_event_t* prop_change_event_cast(event_t* event);

END_C_DECLS

#endif /**TK_EVENT_H*/
