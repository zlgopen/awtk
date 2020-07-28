/**
 * File:   widget.h
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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

#ifndef TK_WIDGET_H
#define TK_WIDGET_H

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/wstr.h"
#include "tkc/value.h"
#include "tkc/darray.h"
#include "tkc/rect.h"
#include "tkc/object.h"
#include "tkc/emitter.h"

#include "base/types_def.h"
#include "base/keys.h"
#include "base/idle.h"
#include "base/timer.h"
#include "base/events.h"
#include "base/canvas.h"
#include "base/style.h"
#include "base/theme.h"
#include "base/layout_def.h"
#include "base/locale_info.h"
#include "base/image_manager.h"
#include "base/widget_consts.h"
#include "base/self_layouter.h"
#include "base/widget_animator.h"
#include "base/children_layouter.h"
#include "base/native_window.h"

BEGIN_C_DECLS

typedef ret_t (*widget_invalidate_t)(widget_t* widget, rect_t* r);
typedef ret_t (*widget_on_event_t)(widget_t* widget, event_t* e);
typedef ret_t (*widget_on_event_before_children_t)(widget_t* widget, event_t* e);
typedef ret_t (*widget_on_paint_background_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_self_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_children_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_border_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_begin_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_end_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_wheel_t)(widget_t* widget, wheel_event_t* e);
typedef ret_t (*widget_on_keydown_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_keyup_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_re_translate_t)(widget_t* widget);
typedef ret_t (*widget_on_pointer_down_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_move_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_up_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_context_menu_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_add_child_t)(widget_t* widget, widget_t* child);
typedef ret_t (*widget_on_remove_child_t)(widget_t* widget, widget_t* child);
typedef ret_t (*widget_on_attach_parent_t)(widget_t* widget, widget_t* parent);
typedef ret_t (*widget_on_detach_parent_t)(widget_t* widget, widget_t* parent);
typedef ret_t (*widget_on_layout_children_t)(widget_t* widget);
typedef ret_t (*widget_get_prop_t)(widget_t* widget, const char* name, value_t* v);
typedef ret_t (*widget_get_prop_default_value_t)(widget_t* widget, const char* name, value_t* v);
typedef ret_t (*widget_set_prop_t)(widget_t* widget, const char* name, const value_t* v);
typedef ret_t (*widget_on_copy_t)(widget_t* widget, widget_t* other);
typedef widget_t* (*widget_find_target_t)(widget_t* widget, xy_t x, xy_t y);
typedef widget_t* (*widget_create_t)(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
typedef ret_t (*widget_on_destroy_t)(widget_t* widget);

typedef ret_t (*widget_exec_t)(widget_t* widget, const char* args);

typedef struct _widget_cmd_t {
  const char* name;
  widget_exec_t exec;
} widget_cmd_t;

struct _widget_vtable_t {
  uint32_t size;
  const char* type;
  /*克隆widget时需要复制的属性*/
  const char* const* clone_properties;
  /*持久化widget时需要保存的属性*/
  const char* const* persistent_properties;
  /**
   * 是否可以滚动。
   */
  uint32_t scrollable : 1;
  /**
   * 是否属于可输入的控件(如：edit/slider等，用户可通过界面输入/改变数据或值)。
   */
  uint32_t inputable : 1;
  /**
   * 是否是focusable。
   *
   *>如编辑器。
   */
  uint32_t focusable : 1;
  /**
   * 收到空格键触发click事件。
   *
   */
  uint32_t space_key_to_activate : 1;
  /**
   * 收到回车键触发click事件。
   *
   */
  uint32_t return_key_to_activate : 1;
  /**
   * 只有active的子控件时可见的。如slide view和pages。
   *
   */
  uint32_t only_active_child_visible : 1;
  /**
   * 是否是窗口。
   */
  uint32_t is_window : 1;
  /**
   * 是否是窗口管理。
   */
  uint32_t is_window_manager : 1;
  /**
   * 是否是设计窗口。
   */
  uint32_t is_designing_window : 1;
  /**
   * 是否是软键盘(点击软键盘不改变编辑器的焦点)。
   */
  uint32_t is_keyboard : 1;

  /**
   * 是否启用pool(如果启用pool，控件需要对其成员全部变量初始化，不能假定成员变量为0)。
   */
  uint32_t enable_pool : 1;

  /**
   * parent class vtable
   */
  const struct _widget_vtable_t* parent;
  /**
   * cursor image name
   */
  const char* pointer_cursor;

  widget_create_t create;
  widget_get_prop_t get_prop;
  widget_set_prop_t set_prop;
  widget_invalidate_t invalidate;
  widget_find_target_t find_target;
  widget_get_prop_default_value_t get_prop_default_value;

  widget_on_copy_t on_copy;
  widget_on_keyup_t on_keyup;
  widget_on_keydown_t on_keydown;
  widget_on_wheel_t on_wheel;
  widget_on_re_translate_t on_re_translate;
  widget_on_paint_background_t on_paint_background;
  widget_on_paint_self_t on_paint_self;
  widget_on_paint_children_t on_paint_children;
  widget_on_paint_border_t on_paint_border;
  widget_on_paint_begin_t on_paint_begin;
  widget_on_paint_end_t on_paint_end;
  widget_on_pointer_down_t on_pointer_down;
  widget_on_pointer_move_t on_pointer_move;
  widget_on_pointer_up_t on_pointer_up;
  widget_on_context_menu_t on_context_menu;
  widget_on_layout_children_t on_layout_children;
  widget_on_add_child_t on_add_child;
  widget_on_remove_child_t on_remove_child;
  widget_on_attach_parent_t on_attach_parent;
  widget_on_detach_parent_t on_detach_parent;
  widget_on_event_t on_event;
  widget_on_event_before_children_t on_event_before_children;
  widget_on_destroy_t on_destroy;
};

/**
 * @class widget_t
 * @annotation ["scriptable","widget"]
 * **widget_t** 是所有控件、窗口和窗口管理器的基类。
 * **widget_t**也是一个容器，可放其它**widget_t**到它的内部，形成一个树形结构。
 *
 * ```graphviz
 *   [default_style]
 *
 *   widget_t -> widget_t[arrowhead = "diamond"]
 *   window_t -> widget_t[arrowhead = "empty"]
 *   window_manager_t -> widget_t[arrowhead = "empty"]
 *   button_t -> widget_t[arrowhead = "empty"]
 *   label_t -> widget_t[arrowhead = "empty"]
 *   xxx_widget_t -> widget_t[arrowhead = "empty"]
 * ```
 *
 * 通常**widget_t**通过一个矩形区域向用户呈现一些信息，接受用户的输入，并据此做出适当的反应。
 * 它负责控件的生命周期、通用状态、事件分发和Style的管理。
 * 本类提供的接口(函数和属性)除非特别说明，一般都适用于子类控件。
 *
 * 为了便于解释，这里特别说明一下几个术语：
 *
 * * **父控件与子控件**：父控件与子控件指的两个控件的组合关系(这是在运行时决定的)。
 * 比如：在窗口中放一个按钮，此时，我们称按钮是窗口的子控件，窗口是按钮的父控件。
 *
 * ```graphviz
 *   [default_style]
 *
 *   子控件 -> 父控件[arrowhead = "ediamond"]
 * ```
 *
 * * **子类控件与父类控件**：子类控件与父类控件指的两类控件的继承关系(这是在设计时决定的)。
 * 比如：我们称**button_t**是**widget_t**的子类控件，**widget_t**是**button_t**的父类控件。
 *
 * ```graphviz
 *   [default_style]
 *
 *   子类控件 -> 父类控件[arrowhead = "empty"]
 *
 * ```
 *
 * widget相关的函数都只能在GUI线程中执行，如果需在非GUI线程中想调用widget相关函数，
 * 请用idle\_queue或timer\_queue进行串行化。
 * 请参考[demo thread](https://github.com/zlgopen/awtk/blob/master/demos/demo_thread_app.c)
 *
 * **widget\_t**是抽象类，不要直接创建**widget\_t**的实例。控件支持两种创建方式：
 *
 * * 通过XML创建。如：
 *
 * ```xml
 * <button x="c" y="m" w="80" h="30" text="OK"/>
 * ```
 *
 * * 通过代码创建。如：
 *
 * ```c
 *  widget_t* button = button_create(win, 10, 10, 128, 30);
 *  widget_set_text(button, L"OK");
 *  widget_on(button, EVT_CLICK, on_click, NULL);
 * ```
 *
 */
struct _widget_t {
  /**
   * @property {xy_t} x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * x坐标(相对于父控件的x坐标)。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * y坐标(相对于父控件的y坐标)。
   */
  xy_t y;
  /**
   * @property {wh_t} w
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 高度。
   */
  wh_t h;
  /**
   * @property {char*} name
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 控件名字。
   */
  char* name;
  /**
   * @property {char*} pointer_cursor
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 鼠标光标图片名称。
   */
  char* pointer_cursor;
  /**
   * @property {char*} tr_text
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 保存用于翻译的字符串。
   */
  char* tr_text;
  /**
   * @property {char*} style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * style的名称。
   */
  char* style;
  /**
   * @property {char*} animation
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 动画参数。请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
   */
  char* animation;
  /**
   * @property {bool_t} enable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 启用/禁用状态。
   */
  uint8_t enable : 1;
  /**
   * @property {bool_t} feedback
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用按键音、触屏音和震动等反馈。
   */
  uint8_t feedback : 1;
  /**
   * @property {bool_t} visible
   * @annotation ["set_prop","get_prop","readable","writable","persitent","design","scriptable"]
   * 是否可见。
   */
  uint8_t visible : 1;
  /**
   * @property {bool_t} sensitive
   * @annotation ["set_prop","get_prop","readable","writable","persitent","design","scriptable"]
   * 是否接受用户事件。
   */
  uint8_t sensitive : 1;
  /**
   * @property {bool_t} focusable
   * @annotation ["set_prop","get_prop","readable","writable","persitent","design","scriptable"]
   * 是否支持焦点停留。
   */
  uint8_t focusable : 1;

  /**
   * @property {bool_t} with_focus_state
   * @annotation ["set_prop","get_prop","readable","writable","persitent","design","scriptable"]
   * 是否支持焦点状态。
   * > 如果希望style支持焦点状态，但有不希望焦点停留，可用本属性。
   */
  uint8_t with_focus_state : 1;

  /**
   * @property {bool_t} focused
   * @annotation ["readable"]
   * 是否得到焦点。
   */
  uint8_t focused : 1;
  /**
   * @property {bool_t} auto_created
   * @annotation ["readable"]
   * 是否由父控件自动创建。
   */
  uint8_t auto_created : 1;
  /**
   * @property {bool_t} dirty
   * @annotation ["readable"]
   * 标识控件是否需要重绘。
   */
  uint8_t dirty : 1;
  /**
   * @property {bool_t} floating
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识控件是否启用浮动布局，不受父控件的children_layout的控制。
   */
  uint8_t floating : 1;
  /**
   * @property {bool_t} need_relayout_children
   * @annotation ["readable"]
   * 标识控件是否需要重新layout子控件。
   */
  uint8_t need_relayout_children : 1;
  /**
   * @property {bool_t} need_update_style
   * @annotation ["readable"]
   * 标识控件是否需要update style。
   */
  uint8_t need_update_style : 1;
  /**
   * @property {int32_t} ref_count
   * @annotation ["readable"]
   * 引用计数，计数为0时销毁。
   */
  int32_t ref_count;

  /**
   * @property {bool_t} initializing
   * @annotation ["readable"]
   * 标识控件正在初始化。
   */
  uint8_t initializing : 1;
  /**
   * @property {bool_t} loading
   * @annotation ["readable"]
   * 标识控件正在加载。
   */
  uint8_t loading : 1;
  /**
   * @property {bool_t} destroying
   * @annotation ["readable"]
   * 标识控件正在被销毁。
   */
  uint8_t destroying : 1;
  /**
   * @property {uint8_t} state
   * @annotation ["readable"]
   * 控件的状态(取值参考widget_state_t)。
   */
  char* state;
  /**
   * @property {uint8_t} opacity
   * @annotation ["readable"]
   * 不透明度(0-255)，0完全透明，255完全不透明。
   */
  uint8_t opacity;
  /**
   * @property {uint16_t} dirty_rect_tolerance 
   * @annotation ["readable","design","scriptable"]
   * 脏矩形超出控件本身大小的最大范围(一般不用指定)。
   *
   *> 如果 border 太粗或 offset 太大等原因，导致脏矩形超出控件本身大小太多（大于缺省值）时，才需要指定。
   */
  uint16_t dirty_rect_tolerance;

  /**
   * @property {wstr_t} text
   * @annotation ["readable"]
   * 文本。用途视具体情况而定。
   */
  wstr_t text;
  /**
   * @property {widget_t*} parent
   * @annotation ["readable", "scriptable"]
   * 父控件
   */
  widget_t* parent;
  /**
   * @property {widget_t*} target
   * @annotation ["private"]
   * 接收事件的子控件。
   */
  widget_t* target;
  /**
   * @property {widget_t*} grab_widget
   * @annotation ["private"]
   * grab事件的子控件。
   */
  widget_t* grab_widget;
  /**
   * @property {int32_t} grab_widget_count
   * @annotation ["private"]
   * 有时子控件和控件自己都会grab widget，所需要grab的计数。
   */
  int32_t grab_widget_count;
  /**
   * @property {widget_t*} key_target
   * @annotation ["private"]
   * 接收按键事件的子控件。
   */
  widget_t* key_target;
  /**
   * @property {darray_t*} children
   * @annotation ["readable"]
   * 全部子控件。
   */
  darray_t* children;
  /**
   * @property {emitter_t*} emitter
   * @annotation ["readable"]
   * 事件发射器。
   */
  emitter_t* emitter;
  /**
   * @property {style_t*} astyle
   * @annotation ["readable"]
   * Style对象。
   */
  style_t* astyle;
  /**
   * @property {children_layouter_t*} children_layout
   * @annotation ["readable", "set_prop", "get_prop"]
   * 子控件布局器。请参考[控件布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)
   */
  children_layouter_t* children_layout;
  /**
   * @property {self_layouter_t*} self_layout
   * @annotation ["readable", "set_prop", "get_prop"]
   * 控件布局器。请参考[控件布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)
   */
  self_layouter_t* self_layout;
  /**
   * @property {object_t*} custom_props
   * @annotation ["readable"]
   * 自定义属性。
   */
  object_t* custom_props;

  /**
   * @property {widget_vtable_t} vt
   * @annotation ["readable"]
   * 虚函数表。
   */
  const widget_vtable_t* vt;
  /*private*/
  assets_manager_t* assets_manager;
};

/**
 * @event {event_t} EVT_WILL_MOVE
 * 控件移动前触发。
 */

/**
 * @event {event_t} EVT_MOVE
 * 控件移动后触发。
 */

/**
 * @event {event_t} EVT_WILL_RESIZE
 * 控件调整大小前触发。
 */

/**
 * @event {event_t} EVT_RESIZE
 * 控件调整大小后触发。
 */

/**
 * @event {event_t} EVT_WILL_MOVE_RESIZE
 * 控件移动并调整大小前触发。
 */

/**
 * @event {event_t} EVT_MOVE_RESIZE
 * 控件移动并调整大小后触发。
 */

/**
 * @event {prop_change_event_t} EVT_PROP_WILL_CHANGE
 * 控件属性改变前触发(通过set_prop设置属性，才会触发)。
 */

/**
 * @event {prop_change_event_t} EVT_PROP_CHANGED
 * 控件属性改变后触发(通过set_prop设置属性，才会触发)。
 */

/**
 * @event {paint_event_t} EVT_BEFORE_PAINT
 * 控件绘制前触发。
 */

/**
 * @event {paint_event_t} EVT_AFTER_PAINT
 * 控件绘制完成时触发。
 */

/**
 * @event {event_t} EVT_FOCUS
 * 控件得到焦点时触发。
 */

/**
 * @event {event_t} EVT_BLUR
 * 控件失去焦点时触发。
 */

/**
 * @event {wheel_event_t} EVT_WHEEL
 * 鼠标滚轮事件。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_LEAVE
 * 鼠标指针离开控件时触发。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_ENTER
 * 鼠标指针进入控件时触发。
 */

/**
 * @event {pointer_event_t} EVT_KEY_DOWN
 * 键按下事件。
 */

/**
 * @event {pointer_event_t} EVT_KEY_UP
 * 键释放事件。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_DOWN
 * 指针设备按下事件。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_DOWN_ABORT
 * 取消指针设备按下事件。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_MOVE
 * 指针设备移动事件。
 */

/**
 * @event {pointer_event_t} EVT_POINTER_UP
 * 指针设备释放事件。
 */

/**
 * @event {event_t} EVT_DESTROY
 * 控件销毁时触发。
 */

/**
 * @method widget_count_children
 * 获取子控件的个数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {int32_t} 子控件的个数。
 */
int32_t widget_count_children(widget_t* widget);

/**
 * @method widget_get_child
 * 获取指定索引的子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  index 索引。
 *
 * @return {widget_t*} 子控件。
 */
widget_t* widget_get_child(widget_t* widget, int32_t index);

/**
 * @method widget_get_native_window
 * 获取原生窗口对象。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {native_window_t*} 原生窗口对象。
 */
native_window_t* widget_get_native_window(widget_t* widget);

/**
 * @method widget_index_of
 * 获取控件在父控件中的索引编号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {int32_t} 在父控件中的索引编号。
 */
int32_t widget_index_of(widget_t* widget);

/**
 * @method widget_close_window
 * 关闭控件所在的窗口。
 * @annotation ["deconstructor", "scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_close_window(widget_t* widget);

/**
 * @method widget_move
 * 移动控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t}   x x坐标
 * @param {xy_t}   y y坐标
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_move(widget_t* widget, xy_t x, xy_t y);

/**
 * @method widget_resize
 * 调整控件的大小。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_resize(widget_t* widget, wh_t w, wh_t h);

/**
 * @method widget_move_resize
 * 移动控件并调整控件的大小。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t}   x x坐标
 * @param {xy_t}   y y坐标
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method widget_set_value
 * 设置控件的值。
 * 只是对widget\_set\_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_value(widget_t* widget, int32_t value);

/**
 * @method widget_animate_value_to
 * 设置控件的值(以动画形式变化到指定的值)。
 * 只是对widget\_set\_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  value 值。
 * @param {uint32_t}  duration 动画持续时间(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animate_value_to(widget_t* widget, int32_t value, uint32_t duration);

/**
 * @method widget_add_value
 * 增加控件的值。
 * 只是对widget\_set\_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  delta 增量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_add_value(widget_t* widget, int32_t delta);

/**
 * @method widget_set_text
 * 设置控件的文本。
 * 只是对widget\_set\_prop的包装，文本的意义由子类控件决定。
 * @param {widget_t*} widget 控件对象。
 * @param {const wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_text(widget_t* widget, const wchar_t* text);

/**
 * @method widget_use_style
 * 启用指定的主题。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  style style的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_use_style(widget_t* widget, const char* style);

/**
 * @method widget_set_text_utf8
 * 设置控件的文本。
 * 只是对widget\_set\_prop的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @alias set_text
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_text_utf8(widget_t* widget, const char* text);

/**
 * @method widget_set_child_text_utf8
 * 设置子控件的文本。
 * 只是对widget\_set\_prop的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @alias set_child_text
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  name 子控件的名称。
 * @param {const char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_child_text_utf8(widget_t* widget, const char* name, const char* text);

/**
 * @method widget_set_child_text_with_double
 * 用一个浮点数去设置子控件的文本。
 * 只是对widget\_set\_prop的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  name 子控件的名称。
 * @param {const char*}  format 格式字符串(如："%2.2lf")。
 * @param {double} value 浮点数值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_child_text_with_double(widget_t* widget, const char* name, const char* format,
                                        double value);

/**
 * @method widget_set_tr_text
 * 获取翻译之后的文本，然后调用widget_set_text。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_tr_text(widget_t* widget, const char* text);

/**
 * @method widget_get_value
 * 获取控件的值。只是对widget\_get\_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t widget_get_value(widget_t* widget);

/**
 * @method widget_get_text
 * 获取控件的文本。
 * 只是对widget\_get\_prop的包装，文本的意义由子类控件决定。
 *
 * 如果希望获取UTF8格式的文本，可以参考下面的代码：
 *
 * ```c
 *  str_t str;
 *  str_init(&str, 0);
 *  str_from_wstr(&str, widget_get_text(target));
 *  log_debug("%s: %s\n", target->name, str.str);
 *  str_reset(&str);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {const wchar_t*} 返回文本。
 */
const wchar_t* widget_get_text(widget_t* widget);

/**
 * @method widget_to_local
 * 将屏幕坐标转换成控件内的本地坐标，即相对于控件左上角的坐标。
 * @param {widget_t*} widget 控件对象。
 * @param {point_t*} p 坐标点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_to_local(widget_t* widget, point_t* p);

/**
 * @method widget_to_global
 * 将控件内的本地坐标转换成全局坐标。
 * @param {widget_t*} widget 控件对象。
 * @param {point_t*} p 坐标点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_to_global(widget_t* widget, point_t* p);

/**
 * @method widget_to_screen
 * 将控件内的本地坐标转换成屏幕上的坐标。
 * @param {widget_t*} widget 控件对象。
 * @param {point_t*} p 坐标点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_to_screen(widget_t* widget, point_t* p);

/**
 * @method widget_set_name
 * 设置控件的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_name(widget_t* widget, const char* name);

/**
 * @method widget_set_theme
 * 设置theme的名称，用于动态切换主题。名称与当前主题名称相同，则重新加载全部资源。
 *
 * > 目前只支持带有文件系统的平台。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 主题的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_theme(widget_t* widget, const char* name);

/**
 * @method widget_set_pointer_cursor
 * 设置鼠标指针的图片名。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} cursor 图片名称(无扩展名)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_pointer_cursor(widget_t* widget, const char* cursor);
#define widget_set_cursor widget_set_pointer_cursor

/**
 * @method widget_set_animation
 * 设置控件的动画参数(仅用于在UI文件使用)。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} animation 动画参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_animation(widget_t* widget, const char* animation);

/**
 * @method widget_create_animator
 * 创建动画。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 除非指定auto_start=false，动画创建后自动启动。
 * * 除非指定auto_destroy=false，动画播放完成后自动销毁。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} animation 动画参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_create_animator(widget_t* widget, const char* animation);

/**
 * @method widget_start_animator
 * 播放动画。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 1.widget为NULL时，播放所有名称为name的动画。
 * * 2.name为NULL时，播放所有widget相关的动画。
 * * 3.widget和name均为NULL，播放所有动画。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_start_animator(widget_t* widget, const char* name);

/**
 * @method widget_set_animator_time_scale
 * 设置动画的时间倍率，<0: 时间倒退，<1: 时间变慢，>1 时间变快。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 1.widget为NULL时，设置所有名称为name的动画的时间倍率。
 * * 2.name为NULL时，设置所有widget相关的动画的时间倍率。
 * * 3.widget和name均为NULL，设置所有动画的时间倍率。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 * @param {float_t} time_scale 时间倍率。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_animator_time_scale(widget_t* widget, const char* name, float_t time_scale);

/**
 * @method widget_pause_animator
 * 暂停动画。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 1.widget为NULL时，暂停所有名称为name的动画。
 * * 2.name为NULL时，暂停所有widget相关的动画。
 * * 3.widget和name均为NULL，暂停所有动画。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pause_animator(widget_t* widget, const char* name);

/**
 * @method widget_find_animator
 * 查找指定名称的动画。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_find_animator(widget_t* widget, const char* name);

/**
 * @method widget_stop_animator
 * 停止动画(控件的相应属性回归原位)。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 1.widget为NULL时，停止所有名称为name的动画。
 * * 2.name为NULL时，停止所有widget相关的动画。
 * * 3.widget和name均为NULL，停止所有动画。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_stop_animator(widget_t* widget, const char* name);

/**
 * @method widget_destroy_animator
 * 销毁动画。
 * 请参考[控件动画](https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md)
 *
 * * 1.widget为NULL时，销毁所有名称为name的动画。
 * * 2.name为NULL时，销毁所有widget相关的动画。
 * * 3.widget和name均为NULL，销毁所有动画。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_animator(widget_t* widget, const char* name);

/**
 * @method widget_set_enable
 * 设置控件的可用性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} enable 是否可用性。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_enable(widget_t* widget, bool_t enable);

/**
 * @method widget_set_feedback
 * 设置控件是否启用反馈。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} feedback 是否启用反馈。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_feedback(widget_t* widget, bool_t feedback);

/**
 * @method widget_set_floating
 * 设置控件的floating标志。
 *> floating的控件不受父控件的子控件布局参数的影响。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} floating 是否启用floating布局。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_floating(widget_t* widget, bool_t floating);

/**
 * @method widget_set_focused
 * 设置控件是否获得焦点。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} focused 是否获得焦点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_focused(widget_t* widget, bool_t focused);

/**
 * @method widget_set_focusable
 * 设置控件是否可获得焦点。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} focusable 是否可获得焦点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_focusable(widget_t* widget, bool_t focusable);

/**
 * @method widget_set_state
 * 设置控件的状态。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state 状态(必须为真正的常量字符串，在widget的整个生命周期有效)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_state(widget_t* widget, const char* state);

/**
 * @method widget_set_opacity
 * 设置控件的不透明度。
 *
 *>在嵌入式平台，半透明效果会使性能大幅下降，请谨慎使用。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t} opacity 不透明度(取值0-255，0表示完全透明，255表示完全不透明)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_opacity(widget_t* widget, uint8_t opacity);

/**
 * @method widget_set_dirty_rect_tolerance
 * 设置控件脏矩形超出控件本身大小的最大范围(一般不用指定)。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint16_t} dirty_rect_tolerance 控件脏脏矩形超出控件本身大小的最大范。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_dirty_rect_tolerance(widget_t* widget, uint16_t dirty_rect_tolerance);

/**
 * @method widget_destroy_children
 * 销毁全部子控件。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_children(widget_t* widget);

/**
 * @method widget_add_child
 * 加入一个子控件。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_add_child(widget_t* widget, widget_t* child);

/**
 * @method widget_remove_child
 * 移出指定的子控件(并不销毁)。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_remove_child(widget_t* widget, widget_t* child);

/**
 * @method widget_insert_child
 * 插入子控件到指定的位置。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} index 位置序数(大于等于总个数，则放到最后)。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_insert_child(widget_t* widget, uint32_t index, widget_t* child);

/**
 * @method widget_restack
 * 调整控件在父控件中的位置序数。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} index 位置序数(大于等于总个数，则放到最后)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_restack(widget_t* widget, uint32_t index);

/**
 * @method widget_child
 * 查找指定名称的子控件(同widget_lookup(widget, name, FALSE))。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 子控件的名称。
 *
 * @return {widget_t*} 子控件或NULL。
 */
widget_t* widget_child(widget_t* widget, const char* name);

/**
 * @method widget_lookup
 * 查找指定名称的子控件(返回第一个)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 子控件的名称。
 * @param {bool_t} recursive 是否递归查找全部子控件。
 *
 * @return {widget_t*} 子控件或NULL。
 */
widget_t* widget_lookup(widget_t* widget, const char* name, bool_t recursive);

/**
 * @method widget_lookup_by_type
 * 查找指定类型的子控件(返回第一个)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} type 子控件的名称。
 * @param {bool_t} recursive 是否递归查找全部子控件。
 *
 * @return {widget_t*} 子控件或NULL。
 */
widget_t* widget_lookup_by_type(widget_t* widget, const char* type, bool_t recursive);

/**
 * @method widget_set_visible
 * 设置控件的可见性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} visible 是否可见。
 * @param {bool_t} recursive 是否递归设置全部子控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_visible(widget_t* widget, bool_t visible, bool_t recursive);

/**
 * @method widget_set_visible_only
 * 设置控件的可见性(不触发repaint和relayout)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} visible 是否可见。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_visible_only(widget_t* widget, bool_t visible);

/**
 * @method widget_set_sensitive
 * 设置控件是否接受用户事件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} sensitive 是否接受用户事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_sensitive(widget_t* widget, bool_t sensitive);

/**
 * @method widget_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 * 使用示例：
 *
 * ```c
 * widget_t* ok = button_create(win, 10, 10, 80, 30);
 * widget_on(ok, EVT_CLICK, on_click, NULL);
 *
 * ```
 *
 * @return {uint32_t} 返回id，用于widget_off。
 */
uint32_t widget_on(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx);

/**
 * @method widget_on_with_tag
 * 注册指定tag的事件处理函数。
 *
 * > 注册时指定一个tag，可用widget\_off\_by\_tag注销相同tag的事件处理函数。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 * @param {uint32_t} tag tag。
 *
 * @return {uint32_t} 返回id，用于widget_off。
 */
uint32_t widget_on_with_tag(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx,
                            uint32_t tag);

/**
 * @method widget_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} id widget_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off(widget_t* widget, uint32_t id);

/**
 * @method widget_child_on
 * 为指定名称的子控件注册指定事件的处理函数。
 * 递归查找指定名称的子控件，然后为其注册指定事件的处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 子控件的名称。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {uint32_t} 返回id，用于widget_off。
 */
uint32_t widget_child_on(widget_t* widget, const char* name, uint32_t type, event_func_t on_event,
                         void* ctx);

/**
 * @method widget_off_by_func
 * 注销指定函数的事件处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off_by_func(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx);

/**
 * @method widget_off_by_ctx
 * 注销指定ctx的事件处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off_by_ctx(widget_t* widget, void* ctx);

/**
 * @method widget_off_by_tag
 * 注销指定tag的事件处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} tag tag。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off_by_tag(widget_t* widget, uint32_t tag);

/**
 * @method widget_invalidate
 * 请求重绘指定的区域，如果widget->dirty已经为TRUE，直接返回。
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} r 矩形对象(widget本地坐标)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate(widget_t* widget, rect_t* r);

/**
 * @method widget_invalidate_force
 * 请求强制重绘控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} r 矩形对象(widget本地坐标)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate_force(widget_t* widget, rect_t* r);

/**
 * @method widget_paint
 * 绘制控件到一个canvas上。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint(widget_t* widget, canvas_t* c);

/**
 * @method widget_draw_text_in_rect
 * 在canvas绘制一行文本。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {const wchar_t*} str 文本。
 * @param {uint32_t} size 文本长度。
 * @param {rect_t*} r 矩形区域。
 * @param {bool_t} ellipses 宽度不够时是否显示省略号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t widget_draw_text_in_rect(widget_t* widget, canvas_t* c, const wchar_t* str, uint32_t size,
                               rect_t* r, bool_t ellipses);

/**
 * @method widget_dispatch
 * 分发一个事件。
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch(widget_t* widget, event_t* e);

/**
 * @method widget_dispatch_simple_event
 * 分发一个简单事件。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} type 事件类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_simple_event(widget_t* widget, uint32_t type);

/**
 * @method widget_dispatch_recursive
 * 分发一个事件控件本身及所有子控件。
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_recursive(widget_t* widget, event_t* e);

/**
 * @method widget_get_prop
 * 获取控件指定属性的值。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_get_prop_default_value
 * 获取控件指定属性的缺省值(在持久化控件时，无需保存缺省值)。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的缺省值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop_default_value(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_set_prop
 * 设置控件指定属性的值。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v);

/**
 * @method widget_set_prop_str
 * 设置字符串格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {const char*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_str(widget_t* widget, const char* name, const char* v);

/**
 * @method widget_get_prop_str
 * 获取字符串格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {const char*} defval 缺省值。
 *
 * @return {const char*} 返回属性的值。
 */
const char* widget_get_prop_str(widget_t* widget, const char* name, const char* defval);

/**
 * @method widget_set_prop_pointer
 * 设置指针格式的属性。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {void**} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_pointer(widget_t* widget, const char* name, void* v);

/**
 * @method widget_get_prop_pointer
 * 获取指针格式的属性。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {void*} 返回属性的值。
 */
void* widget_get_prop_pointer(widget_t* widget, const char* name);

/**
 * @method widget_set_prop_int
 * 设置整数格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_int(widget_t* widget, const char* name, int32_t v);

/**
 * @method widget_get_prop_int
 * 获取整数格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回属性的值。
 */
int32_t widget_get_prop_int(widget_t* widget, const char* name, int32_t defval);

/**
 * @method widget_set_prop_bool
 * 设置布尔格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_bool(widget_t* widget, const char* name, bool_t v);

/**
 * @method widget_get_prop_bool
 * 获取布尔格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回属性的值。
 */
bool_t widget_get_prop_bool(widget_t* widget, const char* name, bool_t defval);

/**
 * @method widget_is_window_opened
 * 判断当前控件所在的窗口是否已经打开。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {bool_t} 返回当前控件所在的窗口是否已经打开。
 */
bool_t widget_is_window_opened(widget_t* widget);

/**
 * @method widget_is_parent_of
 * 判断当前控件是否是指定控件的父控件(包括非直系)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 控件对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t widget_is_parent_of(widget_t* widget, widget_t* child);

/**
 * @method widget_is_direct_parent_of
 * 判断当前控件是否是指定控件的直系父控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 控件对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t widget_is_direct_parent_of(widget_t* widget, widget_t* child);

/**
 * @method widget_is_window
 * 判断当前控件是否是窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {bool_t} 返回当前控件是否是窗口。
 */
bool_t widget_is_window(widget_t* widget);

/**
 * @method widget_is_designing_window
 * 判断当前控件是否是设计窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {bool_t} 返回当前控件是否是设计窗口。
 */
bool_t widget_is_designing_window(widget_t* widget);

/**
 * @method widget_is_window_manager
 * 判断当前控件是否是窗口管理器。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {bool_t} 返回当前控件是否是窗口管理器。
 */
bool_t widget_is_window_manager(widget_t* widget);

/**
 * @method widget_grab
 * 让指定子控件抓住事件。
 * 控件抓住之后，事件直接分发给该控件。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_grab(widget_t* widget, widget_t* child);

/**
 * @method widget_ungrab
 * 让指定子控件放弃抓住事件。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_ungrab(widget_t* widget, widget_t* child);

/**
 * @method widget_foreach
 * 遍历当前控件及子控件。
 * @annotation ["scriptable:custom"]
 * @param {widget_t*} widget 控件对象。
 * @param {tk_visit_t} visit 遍历的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_foreach(widget_t* widget, tk_visit_t visit, void* ctx);

/**
 * @method widget_get_window
 * 获取当前控件所在的窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {widget_t*} 窗口对象。
 */
widget_t* widget_get_window(widget_t* widget);

/**
 * @method widget_get_window_manager
 * 获取当前的窗口管理器。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {widget_t*} 窗口管理器对象。
 */
widget_t* widget_get_window_manager(widget_t* widget);

/**
 * @method widget_get_type
 * 获取当前控件的类型名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {const char*} 返回类型名。
 */
const char* widget_get_type(widget_t* widget);

/**
 * @method widget_clone
 * clone。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} parent clone新控件的parent对象。
 *
 * @return {widget_t*} 返回clone的对象。
 */
widget_t* widget_clone(widget_t* widget, widget_t* parent);

/**
 * @method widget_equal
 * 判断两个widget是否相同。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} other 要比较的控件对象。
 *
 * @return {bool_t} 返回TRUE表示相同，否则表示不同。
 */
bool_t widget_equal(widget_t* widget, widget_t* other);

/**
 * @method widget_add_timer
 * 创建定时器。
 * 该定时器在控件销毁时自动销毁，**timer\_info\_t**的ctx为widget。
 * 如果定时器的生命周期与控件无关，请直接调用**timer_add**，以避免不必要的内存开销。
 * @param {widget_t*} widget 控件对象。
 * @param {timer_func_t} on_timer timer回调函数。
 * @param {uint32_t} duration_ms 时间。
 *
 * 使用示例：
 *
 * ```c
 * static ret_t digit_clock_on_timer(const timer_info_t* info) {
 *  widget_t* widget = WIDGET(info->ctx);
 *
 *  return RET_REPEAT;
 * }
 * ...
 * widget_add_timer(widget, digit_clock_on_timer, 1000);
 *
 * ```
 *
 * @return {uint32_t} 返回timer的ID，TK_INVALID_ID表示失败。
 */
uint32_t widget_add_timer(widget_t* widget, timer_func_t on_timer, uint32_t duration_ms);

/**
 * @method widget_remove_timer
 * 删除指定的timer。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} timer_id timerID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_remove_timer(widget_t* widget, uint32_t timer_id);

/**
 * @method widget_add_idle
 * 创建idle。
 * 该idle在控件销毁时自动销毁，**idle\_info\_t**的ctx为widget。
 * 如果idle的生命周期与控件无关，请直接调用**idle_add**，以避免不必要的内存开销。
 * @param {widget_t*} widget 控件对象。
 * @param {idle_func_t} on_idle idle回调函数。
 *
 * @return {uint32_t} 返回idle的ID，TK_INVALID_ID表示失败。
 */
uint32_t widget_add_idle(widget_t* widget, idle_func_t on_idle);

/**
 * @method widget_load_image
 * 加载图片。
 * 返回的bitmap对象只在当前调用有效，请不保存对bitmap对象的引用。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  name 图片名(不带扩展名)。
 * @param {bitmap_t*} bitmap 返回图片对象。
 *
 * 使用示例：
 *
 * ```c
 * bitmap_t bitmap;
 * widget_load_image(widget, "myimage", &bitmap);
 *
 * ```
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_load_image(widget_t* widget, const char* name, bitmap_t* bitmap);

/**
 * @method widget_unload_image
 * 卸载图片。
 *
 *> 一般不需要调用，只有确认在图片不再需要时才调用本函数卸载。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {bitmap_t*} bitmap 图片对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_unload_image(widget_t* widget, bitmap_t* bitmap);

/**
 * @method widget_load_asset
 * 加载资源。
 * @param {widget_t*} widget 控件对象。
 * @param {asset_type_t} type 资源类型。
 * @param {const char*}  name 资源名。
 *
 * 使用示例：
 *
 * ```c
 * const asset_info_t* asset = widget_load_asset(widget, ASSET_TYPE_IMAGE, "mysvg");
 * ...
 * widget_unload_asset(widget, asset);
 * ```
 *
 * @return {const asset_info_t*} 返回资源句柄。
 */
const asset_info_t* widget_load_asset(widget_t* widget, asset_type_t type, const char* name);

/**
 * @method widget_unload_asset
 * 卸载资源。
 * @param {widget_t*} widget 控件对象。
 * @param {const asset_info_t*}  asset 资源句柄。
 *
 * 使用示例：
 *
 * ```c
 * const asset_info_t* asset = widget_load_asset(widget, ASSET_TYPE_IMAGE, "mysvg");
 * ...
 * widget_unload_asset(widget, asset);
 * ```
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_unload_asset(widget_t* widget, const asset_info_t* asset);

/**
 * @method widget_cast
 * 转换为widget对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {widget_t*} widget对象。
 */
widget_t* widget_cast(widget_t* widget);

/**
 * @method widget_destroy
 * 从父控件中移除控件，并调用unref函数销毁控件。
 * 
 * > 一般无需直接调用，关闭窗口时，自动销毁相关控件。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy(widget_t* widget);

/**
 * @method widget_destroy_async
 * 从父控件中移除控件，并调用unref函数销毁控件。
 * 
 * > 一般无需直接调用，关闭窗口时，自动销毁相关控件。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_async(widget_t* widget);

/**
 * @method widget_ref
 * 增加控件的引用计数。
 * 
 * @param {widget_t*} widget 控件对象。
 *
 * @return {widget_t*} 返回控件对象。
 */
widget_t* widget_ref(widget_t* widget);

/**
 * @method widget_unref
 * 减少控件的引用计数。引用计数为0时销毁控件。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_unref(widget_t* widget);

#define WIDGET_FOR_EACH_CHILD_BEGIN(twidget, iter, i)             \
  if (twidget->children != NULL && twidget->children->size > 0) { \
    int32_t i = 0;                                                \
    int32_t nr = twidget->children->size;                         \
    widget_t** children = (widget_t**)(twidget->children->elms);  \
    for (i = 0; i < nr; i++) {                                    \
      widget_t* iter = children[i];

#define WIDGET_FOR_EACH_CHILD_BEGIN_R(twidget, iter, i)           \
  if (twidget->children != NULL && twidget->children->size > 0) { \
    int32_t i = 0;                                                \
    int32_t nr = twidget->children->size;                         \
    widget_t** children = (widget_t**)(twidget->children->elms);  \
    for (i = nr - 1; i >= 0; i--) {                               \
      widget_t* iter = children[i];

#define WIDGET_FOR_EACH_CHILD_END() \
  }                                 \
  }

/*简化控件实现的函数*/

/**
 * @method widget_is_keyboard
 * 判断当前控件是否是keyboard。
 *
 *> keyboard收到pointer事件时，不会让当前控件失去焦点。
 *
 * 在自定义软键盘时，将所有按钮放到一个容器当中，并设置为is_keyboard。
 *
 * ```c
 * widget_set_prop_bool(group, WIDGET_PROP_IS_KEYBOARD, TRUE);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
bool_t widget_is_keyboard(widget_t* widget);

/**
 * @method widget_paint_helper
 * 帮助子控件实现自己的绘制函数。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {const char*} icon 图标的名称。
 * @param {wstr_t*} text 文字。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint_helper(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text);

/**
 * @method widget_draw_background
 * 根据控件的style绘制背景矩形。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_draw_background(widget_t* widget, canvas_t* c);

/**
 * @method widget_stroke_border_rect
 * 根据控件的style绘制边框矩形。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {rect_t*} r 矩形区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_stroke_border_rect(widget_t* widget, canvas_t* c, rect_t* r);

/**
 * @method widget_fill_bg_rect
 * 根据控件的style绘制背景矩形。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {rect_t*} r 矩形区域。
 * @param {image_draw_type_t} draw_type 图片缺省绘制方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_fill_bg_rect(widget_t* widget, canvas_t* c, rect_t* r, image_draw_type_t draw_type);

/**
 * @method widget_fill_fg_rect
 * 根据控件的style绘制前景矩形。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {rect_t*} r 矩形区域。
 * @param {image_draw_type_t} draw_type 图片缺省绘制方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_fill_fg_rect(widget_t* widget, canvas_t* c, rect_t* r, image_draw_type_t draw_type);

/**
 * @method widget_prepare_text_style
 * 从widget的style中取出字体名称、大小和颜色数据，设置到canvas中。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_prepare_text_style(widget_t* widget, canvas_t* c);

/**
 * @method widget_measure_text
 * 计算文本的宽度。
 * 字体由控件当前的状态和style决定。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {wchar_t*} text 文本。
 *
 * @return {float_t} 返回文本的宽度。
 */
float_t widget_measure_text(widget_t* widget, const wchar_t* text);

/**
 * @method widget_dispatch_event_to_target_recursive
 * 递归分发事件到targe控件。
 *
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*}  e 事件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_event_to_target_recursive(widget_t* widget, event_t* e);

/**
 * @method widget_is_point_in
 * 判断一个点是否在控件内。
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {bool_t} is_local TRUE表示是相对与控件左上角的坐标，否则表示全局坐标。
 *
 * @return {bool_t} 返回RET_OK表示成功，否则表示失败。
 */
bool_t widget_is_point_in(widget_t* widget, xy_t x, xy_t y, bool_t is_local);

/**
 * @method widget_dispatch_to_target
 * 递归的分发一个事件到所有target子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_to_target(widget_t* widget, event_t* e);

/**
 * @method widget_dispatch_to_key_target
 * 递归的分发一个事件到所有key_target子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_to_key_target(widget_t* widget, event_t* e);

/**
 * @method widget_find_target
 * 查找x/y坐标对应的子控件。
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {widget*} 子控件或NULL。
 */
widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y);

/**
 * @method widget_re_translate_text
 * 语言改变后，重新翻译控件上的文本(包括子控件)。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_re_translate_text(widget_t* widget);

/**
 * @method widget_init
 * 初始化控件。仅在子类控件构造函数中使用。
 *
 * > 请用widget\_create代替本函数。
 *
 * @depreated
 * @param {widget_t*} widget widget对象。
 * @param {widget_t*} parent widget的父控件。
 * @param {widget_vtable_t*} vt 虚表。
 * @param {xy_t}   x x坐标
 * @param {xy_t}   y y坐标
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {widget_t*} widget对象本身。
 */
widget_t* widget_init(widget_t* widget, widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y,
                      wh_t w, wh_t h);

/**
 * @method widget_create
 * 创建控件。仅在子类控件构造函数中使用。
 * @param {widget_t*} parent widget的父控件。
 * @param {widget_vtable_t*} vt 虚表。
 * @param {xy_t}   x x坐标
 * @param {xy_t}   y y坐标
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {widget_t*} widget对象本身。
 */
widget_t* widget_create(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                        wh_t h);

/**
 * @method widget_update_style
 * 让控件根据自己当前状态更新style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_update_style(widget_t* widget);

/**
 * @method widget_update_style_recursive
 * 让控件及子控件根据自己当前状态更新style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_update_style_recursive(widget_t* widget);

/**
 * @method widget_set_as_key_target
 * 递归的把父控件的key_target设置为自己。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_as_key_target(widget_t* widget);

/**
 * @method widget_focus_next
 * 把焦点移动下一个控件。
 *
 *>widget必须是当前焦点控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_focus_next(widget_t* widget);

/**
 * @method widget_focus_prev
 * 把焦点移动前一个控件。
 *
 *>widget必须是当前焦点控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_focus_prev(widget_t* widget);

/**
 * @method widget_get_state_for_style
 * 把控件的状态转成获取style选要的状态，一般只在子类中使用。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} active 控件是否为当前项。
 * @param {bool_t} checked 控件是否为选中项。
 *
 * @return {const char*} 返回状态值。
 */
const char* widget_get_state_for_style(widget_t* widget, bool_t active, bool_t checked);

/**
 * @method widget_is_system_bar
 * 检查控件是否是system bar类型。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @return {bool_t} 返回FALSE表示不是，否则表示是。
 */
bool_t widget_is_system_bar(widget_t* widget);

/**
 * @method widget_is_normal_window
 * 检查控件是否是普通窗口类型。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @return {bool_t} 返回FALSE表示不是，否则表示是。
 */
bool_t widget_is_normal_window(widget_t* widget);

/**
 * @method widget_is_dialog
 * 检查控件是否是对话框类型。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @return {bool_t} 返回FALSE表示不是，否则表示是。
 */
bool_t widget_is_dialog(widget_t* widget);

/**
 * @method widget_is_popup
 * 检查控件是否是弹出窗口类型。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @return {bool_t} 返回FALSE表示不是，否则表示是。
 */
bool_t widget_is_popup(widget_t* widget);

/**
 * @method widget_is_opened_popup
 * 检查控件弹出窗口控件是否已经打开了（而非挂起状态）。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @return {bool_t} 返回FALSE表示不是，否则表示是。
 */
bool_t widget_is_opened_popup(widget_t* widget);

/**
 * @method widget_layout
 * 布局当前控件及子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout(widget_t* widget);

/**
 * @method widget_layout_children
 * layout子控件。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout_children(widget_t* widget);

/**
 * @method widget_set_self_layout
 * 设置控件自己的布局参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} params 布局参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_self_layout(widget_t* widget, const char* params);

/**
 * @method widget_set_children_layout
 * 设置子控件的布局参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} params 布局参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_children_layout(widget_t* widget, const char* params);

/**
 * @method widget_set_self_layout_params
 * 设置控件自己的布局(缺省布局器)参数(过时，请用widget\_set\_self\_layout)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} x x参数。
 * @param {const char*} y y参数。
 * @param {const char*} w w参数。
 * @param {const char*} h h参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w,
                                    const char* h);

/**
 * @method widget_set_style_int
 * 设置整数类型的style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {int32_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_style_int(widget_t* widget, const char* state_and_name, int32_t value);

/**
 * @method widget_set_style_str
 * 设置字符串类型的style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {const char*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_style_str(widget_t* widget, const char* state_and_name, const char* value);

/**
 * @method widget_set_style_color
 * 设置颜色类型的style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {uint32_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_style_color(widget_t* widget, const char* state_and_name, uint32_t value);

/**
 * @method widget_take_snapshot
 * 创建一个bitmap对象，将控件绘制到bitmap上，并返回bitmap对象。
 *
 * 调用者需要调用bitmap_destroy销毁返回的bitmap对象。
 *
 *```c
 * bitmap_t* bitmap = widget_take_snapshot(window_manager());
 * bitmap_save_png(bitmap, "test.png");
 * bitmap_destroy(bitmap);
 *``` 
 *
 * @param {widget_t*} widget 控件对象。
 *
 * @return {bitmap_t*} 返回位图对象。
 */
bitmap_t* widget_take_snapshot(widget_t* widget);

/**
 * @method widget_take_snapshot_rect
 * 创建一个bitmap对象，将控件绘制到bitmap上并且可以设置该控件的截屏区域，返回bitmap对象。
 *
 * 调用者需要调用bitmap_destroy销毁返回的bitmap对象。
 *
 *```c
 * bitmap_t* bitmap = widget_take_snapshot_rect(window_manager(), NULL);
 * bitmap_save_png(bitmap, "test.png");
 * bitmap_destroy(bitmap);
 *``` 
 *
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} r 截屏区域（输入NULL，则为控件全区域截屏）。
 *
 * @return {bitmap_t*} 返回位图对象。
 */
bitmap_t* widget_take_snapshot_rect(widget_t* widget, rect_t* r);

/**
 * @method widget_get_canvas
 * 获取canvas对象。
 *
 * @return {canvas_t*} 返回canvas对象。
 */
canvas_t* widget_get_canvas(widget_t* widget);

/**
 * @method widget_reset_canvas
 * 重置canvas对象。for designer only,调用者需要unload全部图片
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_reset_canvas(widget_t* widget);

/*虚函数的包装*/
ret_t widget_on_paint(widget_t* widget, canvas_t* c);
ret_t widget_on_keydown(widget_t* widget, key_event_t* e);
ret_t widget_on_wheel(widget_t* widget, wheel_event_t* e);
ret_t widget_on_keyup(widget_t* widget, key_event_t* e);
ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e);
ret_t widget_on_context_menu(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_paint_background
 * 绘制背景。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回。
 */
ret_t widget_on_paint_background(widget_t* widget, canvas_t* c);
/**
 * @method widget_on_paint_self
 * 绘制自身。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回。
 */
ret_t widget_on_paint_self(widget_t* widget, canvas_t* c);
/**
 * @method widget_on_paint_children
 * 绘制子控件。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回。
 */
ret_t widget_on_paint_children(widget_t* widget, canvas_t* c);
/**
 * @method widget_on_paint_border
 * 绘制边框。
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回。
 */
ret_t widget_on_paint_border(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_begin(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_end(widget_t* widget, canvas_t* c);

#define WIDGET(w) ((widget_t*)(w))

const char* const* widget_get_persistent_props(void);

/**
 * @method widget_is_instance_of
 * 检查控件是否是指定的类型。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_vtable_t*} vt 虚表。
 *
 *  @return {bool_t} 返回TRUE表示是，FALSE表示否。
 */
bool_t widget_is_instance_of(widget_t* widget, const widget_vtable_t* vt);

#define WIDGET_IS_INSTANCE_OF(widget, name) widget_is_instance_of(widget, TK_REF_VTABLE(name))

/*public for subclass*/
TK_EXTERN_VTABLE(widget);

/**
 * @method widget_set_need_relayout_children
 * 设置控件需要relayout标识。
 * @param {widget_t*} widget 控件对象。
 *
 *  @return {ret_t} 返回。
 */
ret_t widget_set_need_relayout_children(widget_t* widget);
/**
 * @method widget_ensure_visible_in_viewport
 * 使控件滚动到可见区域。
 * @param {widget_t*} widget 控件对象。
 *
 *  @return {ret_t} 返回。
 */
ret_t widget_ensure_visible_in_viewport(widget_t* widget);
ret_t widget_set_need_update_style(widget_t* widget);
bool_t widget_is_activate_key(widget_t* widget, key_event_t* e);
ret_t widget_remove_child_prepare(widget_t* widget, widget_t* child);

/*public for test*/

/**
 * @method widget_focus_first
 * 置焦点于第一个控件。
 * @param {widget_t*} widget 控件对象。
 *
 *  @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_focus_first(widget_t* widget);

/**
 * @method widget_get_locale_info
 * 获取locale_info对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {locale_info_t*} 返回locale_info对象。
 */
locale_info_t* widget_get_locale_info(widget_t* widget);

/**
 * @method widget_get_image_manager
 * 获取image_manager对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {image_manager_t*} 返回image_manager对象。
 */
image_manager_t* widget_get_image_manager(widget_t* widget);

/**
 * @method widget_get_assets_manager
 * 获取assets_manager对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {assets_manager_t*} 返回assets_manager对象。
 */
assets_manager_t* widget_get_assets_manager(widget_t* widget);

/**
 * @method widget_get_font_manager
 * 获取font_manager对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {font_manager_t*} 返回font_manager对象。
 */
font_manager_t* widget_get_font_manager(widget_t* widget);

/**
 * @method widget_update_pointer_cursor
 * 更新鼠标指针。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t widget_update_pointer_cursor(widget_t* widget);

/**
 * @method widget_begin_wait_pointer_cursor
 * 开始等待鼠标指针。
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} ignore_user_input 是否忽略用户输入。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t widget_begin_wait_pointer_cursor(widget_t* widget, bool_t ignore_user_input);

/**
 * @method widget_end_wait_pointer_cursor
 * 结束等待鼠标指针。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t widget_end_wait_pointer_cursor(widget_t* widget);

/**
 * @method widget_set_style
 * 设置widget私有样式。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 样式对应类型与名字。
 * @param {const value_t*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t widget_set_style(widget_t* widget, const char* state_and_name, const value_t* value);
/**
 * @method widget_calc_icon_text_rect
 * 计算icon text的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t widget_calc_icon_text_rect(const rect_t* ir, int32_t font_size, float_t text_size,
                                 int32_t icon_at, uint32_t img_w, uint32_t img_h, int32_t spacer,
                                 rect_t* r_text, rect_t* r_icon);

ret_t widget_focus_prev(widget_t* widget);
ret_t widget_focus_next(widget_t* widget);
ret_t widget_focus_up(widget_t* widget);
ret_t widget_focus_down(widget_t* widget);
ret_t widget_focus_left(widget_t* widget);
ret_t widget_focus_right(widget_t* widget);

ret_t widget_set_focused_internal(widget_t* widget, bool_t focused);
bool_t widget_has_focused_widget_in_window(widget_t* widget);

END_C_DECLS

#endif /*TK_WIDGET_H*/
