/**
 * File:   switch.h
 * Author: AWTK Develop Team
 * Brief:  switch
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
 * 2018-09-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SWITCH_H
#define TK_SWITCH_H

#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class switch_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 开关控件。
 *
 * switch\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于switch\_t控件。
 *
 * 在xml中使用"switch"标签创建开关控件。如：
 *
 * ```xml
 * <switch x="10" y="60" w="60" h="22" />
 * ```
 *
 * > 更多用法请参考：[switch.xml](
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/switch.xml)
 *
 * 在c代码中使用函数switch\_create创建开关控件。如：
 *
 * ```c
 *  widget_t* sw = switch_create(win, 10, 10, 80, 30);
 *  widget_on(sw, EVT_VALUE_CHANGED, on_changed, NULL);
 * ```
 *
 * > 完整示例请参考：[switch demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/switch.c)
 *
 * 可用通过style来设置控件的显示风格，如背景图片等。如：
 *
 * ```xml
 * <style name="default">
 *   <normal  bg_image="switch" />
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L452)
 *
 */
typedef struct _switch_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  bool_t value;

  /**
   * @property {float_t} max_xoffset_ratio
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当开关处于关闭时，图片偏移相对于图片宽度的比例(缺省为1/3)。
   */
  float_t max_xoffset_ratio;

  /*private*/
  int32_t xdown;
  int32_t xoffset;
  int32_t xoffset_save;
  velocity_t velocity;
  widget_animator_t* wa;
  bool_t point_down_aborted;
  bool_t pressed;
} switch_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(开关状态)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(开关状态)改变事件。
 */

/**
 * @method switch_create
 * 创建switch对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* switch_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method switch_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget switch对象。
 * @param {bool_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t switch_set_value(widget_t* widget, bool_t value);

/**
 * @method switch_cast
 * 转换switch对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget switch对象。
 *
 * @return {widget_t*} switch对象。
 */
widget_t* switch_cast(widget_t* widget);

#define SWITCH_PROP_MAX_XOFFSET_RATIO "max_xoffset_ratio"

#define WIDGET_TYPE_SWITCH "switch"
#define SWITCH(widget) ((switch_t*)(switch_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(switch);

END_C_DECLS

#endif /*TK_SWITCH_H*/
