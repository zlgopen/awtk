/**
 * File:   widget.h
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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

#ifndef TK_WIDGET_H
#define TK_WIDGET_H

#include "base/str.h"
#include "base/mem.h"
#include "base/wstr.h"
#include "base/value.h"
#include "base/rect.h"
#include "base/timer.h"
#include "base/events.h"
#include "base/emitter.h"
#include "base/canvas.h"
#include "base/style.h"
#include "base/theme.h"
#include "base/layout_def.h"
#include "base/widget_consts.h"
#include "base/custom_props.h"

BEGIN_C_DECLS

typedef ret_t (*widget_invalidate_t)(widget_t* widget, rect_t* r);
typedef ret_t (*widget_on_event_t)(widget_t* widget, event_t* e);
typedef ret_t (*widget_on_paint_background_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_self_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_children_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_border_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_begin_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_end_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_keydown_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_keyup_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_pointer_down_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_move_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_up_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_add_child_t)(widget_t* widget, widget_t* child);
typedef ret_t (*widget_on_remove_child_t)(widget_t* widget, widget_t* child);
typedef ret_t (*widget_on_layout_children_t)(widget_t* widget);
typedef ret_t (*widget_get_prop_t)(widget_t* widget, const char* name, value_t* v);
typedef ret_t (*widget_set_prop_t)(widget_t* widget, const char* name, const value_t* v);
typedef widget_t* (*widget_find_target_t)(widget_t* widget, xy_t x, xy_t y);
typedef widget_t* (*widget_create_t)(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
typedef ret_t (*widget_destroy_t)(widget_t* widget);

typedef struct _widget_vtable_t {
  uint32_t size;
  const char* type;
  /*克隆widget时需要复制的属性*/
  const char** clone_properties;
  /*持久化widget时需要保存的属性*/
  const char** persistent_properties;

  widget_create_t create;
  widget_get_prop_t get_prop;
  widget_set_prop_t set_prop;
  widget_on_keyup_t on_keyup;
  widget_on_keydown_t on_keydown;
  widget_on_paint_background_t on_paint_background;
  widget_on_paint_self_t on_paint_self;
  widget_on_paint_children_t on_paint_children;
  widget_on_paint_border_t on_paint_border;
  widget_on_paint_begin_t on_paint_begin;
  widget_on_paint_end_t on_paint_end;
  widget_on_pointer_down_t on_pointer_down;
  widget_on_pointer_move_t on_pointer_move;
  widget_on_pointer_up_t on_pointer_up;
  widget_on_layout_children_t on_layout_children;
  widget_invalidate_t invalidate;
  widget_on_add_child_t on_add_child;
  widget_on_remove_child_t on_remove_child;
  widget_on_event_t on_event;
  widget_find_target_t find_target;
  widget_destroy_t destroy;
} widget_vtable_t;

/**
 * @class widget_t
 * @annotation ["scriptable"]
 * 窗口基类。
 */
struct _widget_t {
  /**
   * @property {xy_t} x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * y坐标。
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
   * @property {char*} tr_text
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 如果需要在切换语言时实时生效，则需要保存用于翻译的字符串tr_text。
   */
  char* tr_text;
  /**
   * @property {char*} style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * Style Type。
   */
  const char* style;
  /**
   * @property {char*} animation
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 动画参数。
   */
  char* animation;
  /**
   * @property {bool_t} enable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 启用/禁用状态。
   */
  uint8_t enable : 1;
  /**
   * @property {bool_t} visible
   * @annotation ["set_prop","get_prop","readable","writable","persitent","design","scriptable"]
   * 是否可见。
   */
  uint8_t visible : 1;
  /**
   * @property {bool_t} focused
   * @annotation ["readable"]
   * 是否得到焦点。
   */
  uint8_t focused : 1;
  /**
   * @property {bool_t} auto_created
   * @annotation ["readable"]
   * 是否有父控件自动创建。
   */
  uint8_t auto_created : 1;
  /**
   * @property {bool_t} dirty
   * @annotation ["readable"]
   * 标识控件是否需要重绘。
   */
  uint8_t dirty : 1;
  /**
   * @property {bool_t} is_window
   * @annotation ["readable"]
   * 是否是窗口。
   */
  uint8_t is_window : 1;
  /**
   * @property {bool_t} is_designing_window
   * @annotation ["readable"]
   * 是否是设计窗口。
   */
  uint8_t is_designing_window : 1;

  /**
   * @property {uint8_t} state
   * @annotation ["readable"]
   * 控件的状态(取值参考widget_state_t)。
   */
  uint8_t state;
  /**
   * @property {uint8_t} opacity;
   * @annotation ["readable"]
   * 不透明度(0-255)，0完全透明，255完全不透明。
   */
  uint8_t opacity;
  /**
   * @property {wstr_t} text
   * @annotation ["readable"]
   * 文本。用途视具体情况而定。
   */
  wstr_t text;
  /**
   * @property {widget_t*} parent
   * @annotation ["readable"]
   * 父控件
   */
  widget_t* parent;
  /**
   * @property {widget_t*} target
   * @annotation ["readable"]
   * 接收事件的子控件。
   */
  widget_t* target;
  /**
   * @property {widget_t*} grab_widget
   * @annotation ["readable"]
   * grab事件的子控件。
   */
  widget_t* grab_widget;
  /**
   * @property {widget_t*} key_target
   * @annotation ["readable"]
   * 接收按键事件的子控件。
   */
  widget_t* key_target;
  /**
   * @property {array_t*} children
   * @annotation ["readable"]
   * 全部子控件。
   */
  array_t* children;
  /**
   * @property {emitter*} emitter
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
   * @property {layout_params_t*} layout_params
   * @annotation ["readable"]
   * 布局参数。
   */
  layout_params_t* layout_params;
  /**
   * @property {custom_props_t*} custom_props
   * @annotation ["readable"]
   * 自定义属性。
   */
  custom_props_t* custom_props;
  /**
   * @property {widget_vtable_t} vt
   * @annotation ["readable"]
   * 虚函数表。
   */
  const widget_vtable_t* vt;
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
 * @event {pointer_event_t} EVT_DESTROY
 * 控件销毁时触发。
 */

/**
 * @method widget_init
 * 初始化控件。仅在子类控件构造函数中使用。
 * @annotation ["private"]
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
 * @method widget_update_style
 * 让控件根据自己当前状态更新style。
 * @annotation ["private"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_update_style(widget_t* widget);

/**
 * @method widget_get_state_for_style
 * 把控件的状态转成获取style选要的状态，一般只在子类中使用。
 * @annotation ["private"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} active 控件是否为当前项或选中项。
 *
 * @return {widget_state_t} 返回状态值。
 */
widget_state_t widget_get_state_for_style(widget_t* widget, bool_t active);

/**
 * @method widget_update_style_recursive
 * 让控件及其全部子控件根据自己当前状态更新style。
 * @annotation ["private"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_update_style_recursive(widget_t* widget);

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
 * @param {int32_t}  index 索引
 *
 * @return {widget_t*} 子控件。
 */
widget_t* widget_get_child(widget_t* widget, int32_t index);

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
 * 设置控件的值。只是对widget_set_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_value(widget_t* widget, int32_t value);

/**
 * @method widget_add_value
 * 增加控件的值。只是对widget_set_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  delta 增量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_add_value(widget_t* widget, int32_t delta);

/**
 * @method widget_set_text
 * 设置控件的文本。只是对widget_set_prop的包装，文本的意义由子类控件决定。
 * @param {widget_t*} widget 控件对象。
 * @param {wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_text(widget_t* widget, const wchar_t* text);

/**
 * @method widget_use_style
 * 启用指定的主题。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*}  style style的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_use_style(widget_t* widget, const char* style);

/**
 * @method widget_set_text_utf8
 * 设置控件的文本。只是对widget_set_prop的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @alias set_text
 * @param {widget_t*} widget 控件对象。
 * @param {char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_text_utf8(widget_t* widget, const char* text);

/**
 * @method widget_set_tr_text
 * 获取翻译之后的文本，然后调用widget_set_text。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_tr_text(widget_t* widget, const char* text);

/**
 * @method widget_re_translate_text
 * 语言改变后，重新翻译控件上的文本(包括子控件)。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_re_translate_text(widget_t* widget);

/**
 * @method widget_get_value
 * 获取控件的值。只是对widget_get_prop的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t widget_get_value(widget_t* widget);

/**
 * @method widget_get_text
 * 获取控件的文本。只是对widget_get_prop的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {wchar_t*} 返回文本。
 */
const wchar_t* widget_get_text(widget_t* widget);

/**
 * @method widget_to_local
 * 将全局坐标转换成控件内的本地坐标，即相对于控件左上角的坐标。
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
 * @param {char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_name(widget_t* widget, const char* name);

/**
 * @method widget_set_cursor
 * 设置鼠标指针的图片名。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} cursor 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_cursor(widget_t* widget, const char* cursor);

/**
 * @method widget_set_animation
 * 设置控件的动画参数(仅用于在UI文件使用)。
 * 参数的格式类似函数调用。如:
 * opacity(from=0, to=255, yoyo_times=1000, duration=1000)
 * move(x_from=10, x_to=100, y_from=10, y_to=100, duration=1000)
 * 多个animation参数用『;』分开。
 * 参考：https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md
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
 * 除非指定auto_start=false，动画创建后自动启动。
 * 除非指定auto_destroy=false，动画播放完成后自动销毁。
 * 参数的格式类似函数调用, 如:
 * opacity(from=0, to=255, yoyo_times=1000, duration=1000)
 * move(x_from=10, x_to=100, y_from=10, y_to=100, duration=1000)
 * 参考：https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md
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
 * 1.widget为NULL时，播放所有名称为name的动画。
 * 2.name为NULL时，播放所有widget相关的动画。
 * 3.widget和name均为NULL，播放所有动画。
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
 * 1.widget为NULL时，设置所有名称为name的动画的时间倍率。
 * 2.name为NULL时，设置所有widget相关的动画的时间倍率。
 * 3.widget和name均为NULL，设置所有动画的时间倍率。
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
 * 1.widget为NULL时，暂停所有名称为name的动画。
 * 2.name为NULL时，暂停所有widget相关的动画。
 * 3.widget和name均为NULL，暂停所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pause_animator(widget_t* widget, const char* name);

/**
 * @method widget_stop_animator
 * 停止动画(控件的相应属性回归原位)。
 * 1.widget为NULL时，停止所有名称为name的动画。
 * 2.name为NULL时，停止所有widget相关的动画。
 * 3.widget和name均为NULL，停止所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_stop_animator(widget_t* widget, const char* name);

/**
 * @method widget_destroy_animator
 * 销毁动画。
 * 1.widget为NULL时，销毁所有名称为name的动画。
 * 2.name为NULL时，销毁所有widget相关的动画。
 * 3.widget和name均为NULL，销毁所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_animator(widget_t* widget, const char* name);

/**
 * @method widget_set_enable
 * 设置控件的可用性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} enable 是否启动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_enable(widget_t* widget, bool_t enable);

/**
 * @method widget_set_focused
 * 设置控件的是否聚焦。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} focused 是否聚焦。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_focused(widget_t* widget, bool_t focused);

/**
 * @method widget_set_state
 * 设置控件的状态。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_state_t} state 状态。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_state(widget_t* widget, widget_state_t state);

/**
 * @method widget_set_opacity
 * 设置控件的不透明度。
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t} opacity 不透明度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_opacity(widget_t* widget, uint8_t opacity);

/**
 * @method widget_destroy_children
 * 销毁全部子控件。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_children(widget_t* widget);

/**
 * @method widget_add_child
 * 加入一个子控件。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_add_child(widget_t* widget, widget_t* child);

/**
 * @method widget_remove_child
 * 移出指定的子控件(并不销毁)。
 * @param {widget_t*} widget 控件对象。
 * @param {widget_t*} child 子控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_remove_child(widget_t* widget, widget_t* child);

/**
 * @method widget_find_target
 * 查找x/y坐标对应的子控件。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {widget*} 子控件或NULL。
 */
widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y);

/**
 * @method widget_child
 * 查找指定名称的子控件(同widget_lookup(widget, name, FALSE))。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 子控件的名称。
 *
 * @return {widget_t*} 子控件或NULL。
 */
widget_t* widget_child(widget_t* widget, const char* name);

/**
 * @method widget_lookup
 * 查找指定名称的子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 子控件的名称。
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
 * @param {char*} type 子控件的名称。
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
 * @method widget_on
 * 注册指定事件的处理函数。
 * @annotation ["scriptable:custom"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {int32_t} 返回id，用于widget_off。
 */
int32_t widget_on(widget_t* widget, event_type_t type, event_func_t on_event, void* ctx);

/**
 * @method widget_off
 * 注销指定事件的处理函数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} id widget_on返回的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off(widget_t* widget, int32_t id);

/**
 * @method widget_child_on
 * 注册指定事件的处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 子控件的名称。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {int32_t} 返回id，用于widget_off。
 */
int32_t widget_child_on(widget_t* widget, const char* name, event_type_t type,
                        event_func_t on_event, void* ctx);

/**
 * @method widget_off_by_func
 * 注销指定事件的处理函数。
 * @param {widget_t*} widget 控件对象。
 * @param {event_type_t} type 事件类型。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_off_by_func(widget_t* widget, event_type_t type, event_func_t on_event, void* ctx);

/**
 * @method widget_invalidate
 * 请求重绘指定的区域，如果widget->dirty已经为TRUE，直接返回。
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} r 矩形对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate(widget_t* widget, rect_t* r);

/**
 * @method widget_invalidate_force
 * 强制重绘控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate_force(widget_t* widget);

/**
 * @method widget_paint
 * 绘制控件到一个canvas上。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint(widget_t* widget, canvas_t* c);

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
 * @method widget_get_prop
 * 通用的获取控件属性的函数。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_set_prop
 * 通用的设置控件属性的函数。
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
 * @method widget_set_prop_int
 * 设置字符串格式的属性。
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
 * 获取字符串格式的属性。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回属性的值。
 */
int32_t widget_get_prop_int(widget_t* widget, const char* name, int32_t defval);

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
 * @method widget_layout_children
 * layout子控件。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout_children(widget_t* widget);

/**
 * @method widget_grab
 * 让指定子控件抓住事件。抓住之后，事件由窗口直接分发给该控件。
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
 * @return {char*} 返回类型名。
 */
const char* widget_get_type(widget_t* widget);

/**
 * @method widget_dispatch_to_target
 * 递归的分发一个事件到所有target子控件。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_to_target(widget_t* widget, event_t* e);

/**
 * @method widget_dispatch_to_key_target
 * 递归的分发一个事件到所有key_target子控件。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {event_t*} e 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_dispatch_to_key_target(widget_t* widget, event_t* e);

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
 * @method widget_set_self_layout_params
 * 设置widget自身的layout参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} x x布局参数
 * @param {char*} y y布局参数
 * @param {char*} w w布局参数
 * @param {char*} h h布局参数
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w,
                                    const char* h);

/**
 * @method widget_set_children_layout_params
 * 设置widget子控件的layout参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} params 子控件布局参数(参考docs/layout.md)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_children_layout_params(widget_t* widget, const char* params);

/**
 * @method widget_layout
 * 重新排版widget及其子控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_layout(widget_t* widget);

/**
 * @method widget_add_timer
 * 创建一个timer，该timer在控件销毁时自动销毁，timer_info_t的ctx为widget。
 * @param {widget_t*} widget 控件对象。
 * @param {timer_func_t} on_timer timer回调函数。
 * @param {uint32_t} duration_ms 时间。
 *
 * @return {uint32_t} 返回timer的ID，TK_INVALID_ID表示失败。
 */
uint32_t widget_add_timer(widget_t* widget, timer_func_t on_timer, uint32_t duration_ms);

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
 * 销毁控件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy(widget_t* widget);

#define WIDGET(w) ((widget_t*)(w))

/*简化控件实现的函数*/

/**
 * @method widget_paint_helper
 * 帮助子控件实现自己的绘制函数。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @param {char*} icon 图标的名称。
 * @param {wstr_t*} text 文字。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint_helper(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text);

/**
 * @method widget_prepare_text_style
 * 从widget的style中取出字体名称、大小和颜色数据，设置到canvas中。
 * @annotation ["private"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_prepare_text_style(widget_t* widget, canvas_t* c);

/**
 * @method widget_measure_text
 * 计算文本的宽度。
 * @param {widget_t*} widget 控件对象。
 * @param {wchar_t*} text 文本。
 *
 * @return {float_t} 返回文本的宽度。
 */
float_t widget_measure_text(widget_t* widget, const wchar_t* text);

/**
 * @method widget_load_image
 * 加载图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  name 图片名。
 * @param {bitmap_t*} bitmap 图片对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_load_image(widget_t* widget, const char* name, bitmap_t* bitmap);

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

/*虚函数的包装*/
ret_t widget_on_paint(widget_t* widget, canvas_t* c);
ret_t widget_on_keydown(widget_t* widget, key_event_t* e);
ret_t widget_on_keyup(widget_t* widget, key_event_t* e);
ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e);
ret_t widget_on_paint_background(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_self(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_children(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_border(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_begin(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_end(widget_t* widget, canvas_t* c);

END_C_DECLS

#endif /*TK_WIDGET_H*/
