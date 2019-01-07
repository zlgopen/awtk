/**
 * File:   switch.h
 * Author: AWTK Develop Team
 * Brief:  switch
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
 * @annotation ["scriptable"]
 * 开关控件。
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

  /**
   * @property {int32_t} round_radius
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的圆角半径。
   */
  int32_t round_radius;

  /*private*/
  int32_t xdown;
  int32_t xoffset;
  int32_t xoffset_save;
  velocity_t velocity;
  widget_animator_t* wa;
  bool_t point_down_aborted;
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

#define SWITCH_PROP_ROUND_RADIUS "round_radius"
#define SWITCH_PROP_MAX_XOFFSET_RATIO "max_xoffset_ratio"

#define WIDGET_TYPE_SWITCH "switch"
#define SWITCH(widget) ((switch_t*)(widget))

END_C_DECLS

#endif /*TK_SWITCH_H*/
