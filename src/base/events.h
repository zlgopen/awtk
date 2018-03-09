/**
 * File:   events.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  events structs
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_EVENTS_H
#define LFTK_EVENTS_H

#include "types_def.h"

BEGIN_C_DECLS

/**
 * @enum event_type_t 
 * @scriptable
 * @prefix EVT_
 * 类型常量定义。
 */
typedef enum _event_type_t {
/**
 * @const EVT_NONE
 * 无效事件名称。
 */
  EVT_NONE = 0,
/**
 * @const EVT_POINTER_DOWN
 * 指针按下事件名。
 */
  EVT_POINTER_DOWN,
/**
 * @const EVT_POINTER_MOVE
 * 指针移动事件名。
 */
  EVT_POINTER_MOVE,
/**
 * @const EVT_POINTER_UP
 * 指针抬起事件名。
 */
  EVT_POINTER_UP,
/**
 * @const EVT_POINTER_ENTER
 * 指针进入事件名。
 */
  EVT_POINTER_ENTER,
/**
 * @const EVT_POINTER_LEAVE
 * 指针离开事件名。
 */
  EVT_POINTER_LEAVE,
/**
 * @const EVT_CLICK
 * 点击事件名。
 */
  EVT_CLICK,
/**
 * @const EVT_KEY_DOWN
 * 键按下事件名。
 */
  EVT_KEY_DOWN,
/**
 * @const EVT_KEY_UP
 * 键抬起事件名。
 */
  EVT_KEY_UP,
/**
 * @const EVT_MOVE
 * Widget移动事件名。
 */
  EVT_MOVE,
/**
 * @const EVT_RESIZE
 * Widget调整大小事件名。
 */
  EVT_RESIZE,
/**
 * @const EVT_DESTROY
 * 对象销毁事件名。
 */
  EVT_DESTROY,
/**
 * @const EVT_MOVE_RESIZE
 * Widget调整大小/移动事件名。
 */
  EVT_MOVE_RESIZE,
/**
 * @const EVT_PROP_CHANGED
 * 对象的属性改变事件名。
 */
  EVT_PROP_CHANGED
}event_type_t;

/**
 * @class event_t
 * @scriptable
 * 事件基类。
 */
typedef struct _event_t {
/**
 * @property {int16_t} type
 * @readonly
 * 类型。
 */
  uint16_t type;
  void* target;
}event_t;

/**
 * @class pointer_event_t
 * @scriptable
 * @parent event_t
 * 指针事件。
 */

typedef struct _pointer_event_t {
  event_t e;
  /**
   * @property {xy_t} x
   * @readonly
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @readonly
   * y坐标。
   */
  xy_t y;
  /**
   * @property {uint8_t} button
   * @readonly
   * button。
   */
  xy_t button;
  /**
   * @property {bool_t} pressed
   * @readonly
   * 指针是否按下。
   */
  uint8_t pressed:1;
}pointer_event_t;

/**
 * @method pointer_event_cast
 * @constructor
 * 把event对象转pointer_event_t对象
 * @param {event_t*} event event对象。
 *
 * @return {pointer_event_t*} 对象。
 */
pointer_event_t* pointer_event_cast(event_t* event);


/**
 * @class key_event_t
 * @scriptable
 * @parent event_t
 * 按键事件。
 */

typedef struct _key_event_t {
  event_t  e;
  /**
   * @property {uint8_t} key
   * @readonly
   * 键值。
   */
  uint8_t key;
  /**
   * @property {bool_t} alt
   * @readonly
   * alt键是否按下。
   */
  uint8_t alt:1;
  /**
   * @property {bool_t} ctrl
   * @readonly
   * ctrl键是否按下。
   */
  uint8_t ctrl:1;
  /**
   * @property {bool_t} shift
   * @readonly
   * shift键是否按下。
   */
  uint8_t shift:1;
}key_event_t;

/**
 * @method key_event_cast
 * @constructor
 * 把event对象转key_event_t对象
 * @param {event_t*} event event对象。
 *
 * @return {key_event_t*} 对象。
 */
key_event_t* key_event_cast(event_t* event);


typedef ret_t (*event_func_t)(void* ctx, event_t* e);

END_C_DECLS

#endif/**LFTK_EVENTS_H*/

