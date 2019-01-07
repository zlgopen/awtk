/**
 * File:   progress_circle.h
 * Author: AWTK Develop Team
 * Brief:  progress_circle
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
 * 2018-11-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PROGRESS_CIRCLE_H
#define TK_PROGRESS_CIRCLE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class progress_circle_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 进度圆环控件。
 */
typedef struct _progress_circle_t {
  widget_t widget;
  /**
   * @property {float_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值(缺省为0)。
   */
  float_t value;
  /**
   * @property {uint32_t} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值(缺省为100)。
   */
  uint32_t max;
  /**
   * @property {int32_t} start_angle
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 起始角度(单位为度，缺省-90)。
   */
  int32_t start_angle;
  /**
   * @property {uint32_t} line_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 环线的厚度(缺省为8)。
   */
  uint32_t line_width;
  /**
   * @property {char*} unit
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 单元(缺省无)。
   */
  char* unit;
  /**
   * @property {bool_t} counter_clock_wise
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为逆时针方向(缺省为FALSE)。
   */
  bool_t counter_clock_wise;
  /**
   * @property {bool_t} show_text
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示文本(缺省为TRUE)。
   */
  bool_t show_text;
} progress_circle_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件。
 */

/**
 * @method progress_circle_create
 * @annotation ["constructor", "scriptable"]
 * 创建progress_circle对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* progress_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method progress_circle_cast
 * 转换为progress_circle对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget progress_circle对象。
 *
 * @return {widget_t*} progress_circle对象。
 */
widget_t* progress_circle_cast(widget_t* widget);

/**
 * @method progress_circle_set_value
 * 设置值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {float_t}  value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_value(widget_t* widget, float_t value);

/**
 * @method progress_circle_set_max
 * 设置最大值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t}  max 最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_max(widget_t* widget, uint32_t max);

/**
 * @method progress_circle_set_line_width
 * 设置环线的厚度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t}  line_width 环线的厚度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_line_width(widget_t* widget, uint32_t line_width);

/**
 * @method progress_circle_set_start_angle
 * 设置起始角度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  start_angle 起始角度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_start_angle(widget_t* widget, int32_t start_angle);

/**
 * @method progress_circle_set_unit
 * 设置单位。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  unit 单位。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_unit(widget_t* widget, const char* unit);

/**
 * @method progress_circle_set_show_text
 * 设置是否显示文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  show_text 是否显示文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_show_text(widget_t* widget, bool_t show_text);

/**
 * @method progress_circle_set_counter_clock_wise
 * 设置是否为逆时针方向。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  counter_clock_wise 是否为逆时针方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_circle_set_counter_clock_wise(widget_t* widget, bool_t counter_clock_wise);

#define PROGRESS_CIRCLE_PROP_UNIT "unit"
#define PROGRESS_CIRCLE_PROP_LINE_WIDTH "line_width"
#define PROGRESS_CIRCLE_PROP_START_ANGLE "start_angle"
#define PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE "counter_clock_wise"

#define WIDGET_TYPE_PROGRESS_CIRCLE "progress_circle"
#define PROGRESS_CIRCLE(widget) ((progress_circle_t*)(widget))

END_C_DECLS

#endif /*TK_PROGRESS_CIRCLE_H*/
