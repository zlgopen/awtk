/**
 * File:   digit_clock.h
 * Author: AWTK Develop Team
 * Brief:  digit_clock
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIGIT_CLOCK_H
#define TK_DIGIT_CLOCK_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class digit_clock_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 数字时钟控件。
 */
typedef struct _digit_clock_t {
  widget_t widget;

  /**
   * @property {bool_t} show_date
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示日期。
   */
  bool_t show_date;
  /**
   * @property {bool_t} show_time
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示时间。
   */
  bool_t show_time;

  /**
   * @property {char*} format
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 显示格式。如日期格式为"%d-%02d-%02d"，显示类似于2018-12-12。
   */
  char* format;

} digit_clock_t;

/**
 * @method digit_clock_create
 * 创建digit_clock对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* digit_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method digit_clock_cast
 * 转换为digit_clock对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget digit_clock对象。
 *
 * @return {widget_t*} digit_clock对象。
 */
widget_t* digit_clock_cast(widget_t* widget);

/**
 * @method digit_clock_set_show_date
 * 设置是否显示日期。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} show_date 是否显示日期。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t digit_clock_set_show_date(widget_t* widget, bool_t show_date);

/**
 * @method digit_clock_set_show_time
 * 设置是否显示时间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} show_time 是否显示时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t digit_clock_set_show_time(widget_t* widget, bool_t show_time);

/**
 * @method digit_clock_set_format
 * 设置显示格式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t digit_clock_set_bg_format(widget_t* widget, const char* format);

#define DIGIT_CLOCK_PROP_FORMAT "format"
#define DIGIT_CLOCK_PROP_SHOW_DATE "show_date"
#define DIGIT_CLOCK_PROP_SHOW_TIME "show_time"

#define WIDGET_TYPE_DIGIT_CLOCK "digit_clock"

#define DIGIT_CLOCK(widget) ((digit_clock_t*)(widget))

END_C_DECLS

#endif /*TK_DIGIT_CLOCK_H*/
