/**
 * File:   events.h
 * Author: AWTK Develop Team
 * Brief:  events structs
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_EVENTS_H
#define TK_EVENTS_H

#include "base/value.h"
#include "base/canvas.h"
#include "base/system_info.h"

BEGIN_C_DECLS

/**
 * @enum event_type_t
 * @annotation ["scriptable"]
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
   * 指针按下事件名(pointer_event_t)。
   */
  EVT_POINTER_DOWN,
  /**
   * @const EVT_POINTER_DOWN_ABORT
   * 取消前一个指针按下事件名(pointer_event_t)。
   */
  EVT_POINTER_DOWN_ABORT,
  /**
   * @const EVT_POINTER_MOVE
   * 指针移动事件名(pointer_event_t)。
   */
  EVT_POINTER_MOVE,
  /**
   * @const EVT_POINTER_UP
   * 指针抬起事件名(pointer_event_t)。
   */
  EVT_POINTER_UP,
  /**
   * @const EVT_WHEEL
   * 滚轮事件名(pointer_event_t)。
   */
  EVT_WHEEL,
  /**
   * @const EVT_CONTEXT_MENU
   * 右键/长按弹出上下文菜单的事件名(pointer_event_t)。
   */
  EVT_CONTEXT_MENU,
  /**
   * @const EVT_POINTER_ENTER
   * 指针进入事件名(pointer_event_t)。
   */
  EVT_POINTER_ENTER,
  /**
   * @const EVT_POINTER_LEAVE
   * 指针离开事件名(pointer_event_t)。
   */
  EVT_POINTER_LEAVE,
  /**
   * @const EVT_LONG_PRESS
   * 长按事件名(pointer_event_t)。
   */
  EVT_LONG_PRESS,
  /**
   * @const EVT_CLICK
   * 点击事件名(pointer_event_t)。
   */
  EVT_CLICK,
  /**
   * @const EVT_FOCUS
   * 得到焦点事件名(event_t)。
   */
  EVT_FOCUS,
  /**
   * @const EVT_BLUR
   * 失去焦点事件名(event_t)。
   */
  EVT_BLUR,
  /**
   * @const EVT_KEY_DOWN
   * 键按下事件名(key_event_t)。
   */
  EVT_KEY_DOWN,
  /**
   * @const EVT_KEY_REPEAT
   * 按键repeat事件名(key_event_t)。
   */
  EVT_KEY_REPEAT,
  /**
   * @const EVT_KEY_UP
   * 键抬起事件名(key_event_t)。
   */
  EVT_KEY_UP,
  /**
   * @const EVT_DESTROY
   * 对象销毁事件名(event_t)。
   */
  EVT_DESTROY,
  /**
   * @const EVT_WILL_MOVE
   * 即将移动Widget的事件名(event_t)。
   */
  EVT_WILL_MOVE,
  /**
   * @const EVT_MOVE
   * 移动Widget的事件名(event_t)。
   */
  EVT_MOVE,
  /**
   * @const EVT_WILL_RESIZE
   * 即将调整Widget大小的事件名(event_t)。
   */
  EVT_WILL_RESIZE,
  /**
   * @const EVT_RESIZE
   * 调整Widget大小的事件名(event_t)。
   */
  EVT_RESIZE,
  /**
   * @const EVT_WILL_MOVE_RESIZE
   * 即将调整Widget大小/位置的事件名(event_t)。
   */
  EVT_WILL_MOVE_RESIZE,
  /**
   * @const EVT_MOVE_RESIZE
   * 调整Widget大小/位置的事件名(event_t)。
   */
  EVT_MOVE_RESIZE,
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
   * @const EVT_VALUE_WILL_CHANGE
   * 控件的值即将改变的事件名(event_t)。
   */
  EVT_VALUE_WILL_CHANGE,
  /**
   * @const EVT_VALUE_CHANGED
   * 控件的值改变的事件名(event_t)。
   */
  EVT_VALUE_CHANGED,
  /**
   * @const EVT_VALUE_CHANGING
   * 控件的值持续改变(如编辑器正在编辑)的事件名(event_t)。
   */
  EVT_VALUE_CHANGING,
  /**
   * @const EVT_PAINT
   * 绘制的事件名(paint_event_t)。
   */
  EVT_PAINT,
  /**
   * @const EVT_BEFORE_PAINT
   * 即将绘制的事件名(paint_event_t)。
   */
  EVT_BEFORE_PAINT,
  /**
   * @const EVT_AFTER_PAINT
   * 绘制完成的事件名(paint_event_t)。
   */
  EVT_AFTER_PAINT,
  /**
   * @const EVT_LOCALE_CHANGED
   * locale改变的事件(event_t)。
   */
  EVT_LOCALE_CHANGED,
  /**
   * @const EVT_ANIM_START
   * 控件动画开始事件(event_t)。
   */
  EVT_ANIM_START,
  /**
   * @const EVT_ANIM_STOP
   * 控件动画被主动停止的事件(event_t)。
   */
  EVT_ANIM_STOP,
  /**
   * @const EVT_ANIM_PAUSE
   * 控件动画被暂停的事件(event_t)。
   */
  EVT_ANIM_PAUSE,
  /**
   * @const EVT_ANIM_ONCE
   * 控件动画yoyo/repeat时，完成一次的事件(event_t)。
   */
  EVT_ANIM_ONCE,
  /**
   * @const EVT_ANIM_END
   * 控件动画完成事件(event_t)。
   */
  EVT_ANIM_END,
  /**
   * @const EVT_WINDOW_LOAD
   * 窗口加载完成事件(event_t)。
   */
  EVT_WINDOW_LOAD,
  /**
   * @const EVT_WINDOW_WILL_OPEN
   * 窗口即将打开事件(event_t)。
   * 如果有窗口动画，在窗口动画开始前触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。
   */
  EVT_WINDOW_WILL_OPEN,
  /**
   * @const EVT_WINDOW_OPEN
   * 窗口打开事件(event_t)。
   * 如果有窗口动画，在窗口动画完成时触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。
   */
  EVT_WINDOW_OPEN,
  /**
   * @const EVT_WINDOW_CLOSE
   * 窗口关闭事件。
   */
  EVT_WINDOW_CLOSE,
  /**
   * @const EVT_IM_COMMIT
   * 输入法提交输入的文本事件(im_commit_event_t)。
   */
  EVT_IM_COMMIT,
  /**
   * @const EVT_IM_SHOW_CANDIDATES
   * 输入法请求显示候选字事件(im_candidates_event_t)。
   */
  EVT_IM_SHOW_CANDIDATES,
  /**
   * @const EVT_IM_ACTION
   * 软键盘Action点击事件(event_t)。
   */
  EVT_IM_ACTION,
  /**
   * @const EVT_IM_ACTION_INFO
   * 请求更新软键盘上的Action按钮的信息(im_action_button_info_event_t)。
   */
  EVT_IM_ACTION_INFO,
  /**
   * @const EVT_DRAG_START
   * 开始拖动(event_t)。
   */
  EVT_DRAG_START,
  /**
   * @const EVT_DRAG
   * 拖动(event_t)。
   */
  EVT_DRAG,
  /**
   * @const EVT_DRAG_END
   * 结束拖动(event_t)。
   */
  EVT_DRAG_END,
  /**
   * @const EVT_REQ_START
   * event queue其它请求编号起始值。
   */
  EVT_REQ_START = 0x1000,
  /**
   * @const EVT_USER_START
   * 用户定义事件起始值。
   */
  EVT_USER_START = 0x2000
} event_type_t;

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
 * @class wheel_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 滚轮事件。
 */
typedef struct _wheel_event_t {
  event_t e;
  /**
   * @property {int32_t} dx
   * @annotation ["readable", "scriptable"]
   * 滚轮的x值。
   */
  int32_t dx;
  /**
   * @property {int32_t} dy
   * @annotation ["readable", "scriptable"]
   * 滚轮的x值。
   */
  int32_t dy;
  /**
   * @property {bool_t} alt
   * @annotation ["readable", "scriptable"]
   * alt键是否按下。
   */
  uint8_t alt : 1;
  /**
   * @property {bool_t} ctrl
   * @annotation ["readable", "scriptable"]
   * ctrl键是否按下。
   */
  uint8_t ctrl : 1;
  /**
   * @property {bool_t} shift
   * @annotation ["readable", "scriptable"]
   * shift键是否按下。
   */
  uint8_t shift : 1;
} wheel_event_t;

/**
 * @method wheel_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转wheel_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {wheel_event_t*} 对象。
 */
wheel_event_t* wheel_event_cast(event_t* event);

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

/**
 * @class pointer_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 指针事件。
 */
typedef struct _pointer_event_t {
  event_t e;
  /**
   * @property {xy_t} x
   * @annotation ["readable", "scriptable"]
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["readable", "scriptable"]
   * y坐标。
   */
  xy_t y;
  /**
   * @property {uint8_t} button
   * @annotation ["readable", "scriptable"]
   * button。
   */
  xy_t button;
  /**
   * @property {bool_t} pressed
   * @annotation ["readable", "scriptable"]
   * 指针是否按下。
   */
  uint8_t pressed : 1;

  /**
   * @property {bool_t} alt
   * @annotation ["readable", "scriptable"]
   * alt键是否按下。
   */
  uint8_t alt : 1;
  /**
   * @property {bool_t} ctrl
   * @annotation ["readable", "scriptable"]
   * ctrl键是否按下。
   */
  uint8_t ctrl : 1;
  /**
   * @property {bool_t} shift
   * @annotation ["readable", "scriptable"]
   * shift键是否按下。
   */
  uint8_t shift : 1;
} pointer_event_t;

/**
 * @method pointer_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转pointer_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {pointer_event_t*} 对象。
 */
pointer_event_t* pointer_event_cast(event_t* event);

/**
 * @class key_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 按键事件。
 */
typedef struct _key_event_t {
  event_t e;
  /**
   * @property {uint32_t} key
   * @annotation ["readable", "scriptable"]
   * 键值。
   */
  uint32_t key;
  /**
   * @property {bool_t} alt
   * @annotation ["readable", "scriptable"]
   * alt键是否按下。
   */
  uint8_t alt : 1;
  /**
   * @property {bool_t} ctrl
   * @annotation ["readable", "scriptable"]
   * ctrl键是否按下。
   */
  uint8_t ctrl : 1;
  /**
   * @property {bool_t} shift
   * @annotation ["readable", "scriptable"]
   * shift键是否按下。
   */
  uint8_t shift : 1;
  /**
   * @property {bool_t} caplock
   * @annotation ["readable", "scriptable"]
   * caplock键是否按下。
   */
  uint8_t caplock : 1;
} key_event_t;

/**
 * @method key_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转key_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {key_event_t*} 对象。
 */
key_event_t* key_event_cast(event_t* event);

/**
 * @class paint_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 绘制事件。
 */
typedef struct _paint_event_t {
  event_t e;
  /**
   * @property {canvas_t*} c
   * @annotation ["readable", "scriptable"]
   * canvas。
   */
  canvas_t* c;
} paint_event_t;

/**
 * @method paint_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转paint_event_t对象。主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {paint_event_t*} 对象。
 */
paint_event_t* paint_event_cast(event_t* event);

/**
 * @method event_init
 * 初始化事件。
 * @param {uint32_t} type 事件类型。
 * @param {void*} target 目标对象。
 *
 * @return {event_t} 事件对象。
 */
event_t event_init(uint32_t type, void* target);

/**
 * @method pointer_event_rotate
 * 根据屏幕旋转方向修正pointer_event中的坐标。
 * @param {pointer_event_t*} evt 指针事件对象。
 * @param {system_info_t*} info 系统信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t pointer_event_rotate(pointer_event_t* evt, system_info_t* info);

/*事件处理函数原型*/
typedef ret_t (*event_func_t)(void* ctx, event_t* e);

END_C_DECLS

#endif /**TK_EVENTS_H*/
