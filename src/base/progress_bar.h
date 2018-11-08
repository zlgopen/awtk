/**
 * File:   progress_bar.h
 * Author: AWTK Develop Team
 * Brief:  progress_bar
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PROGRESS_BAR_H
#define TK_PROGRESS_BAR_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class progress_bar_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 进度条控件。
 */
typedef struct _progress_bar_t {
  widget_t widget;
  /**
   * @property {uint8_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的值[0-100]。
   */
  uint8_t value;
  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的是否为垂直方向。
   */
  bool_t vertical;
  /**
   * @property {bool_t} show_text
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示文本。
   */
  bool_t show_text;
} progress_bar_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件。
 */

/**
 * @method progress_bar_create
 * @annotation ["constructor", "scriptable"]
 * 创建progress_bar对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method progress_bar_cast
 * 转换为progress_bar对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget progress_bar对象。
 *
 * @return {widget_t*} progress_bar对象。
 */
widget_t* progress_bar_cast(widget_t* widget);

/**
 * @method progress_bar_set_value
 * 设置进度条的进度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  value 进度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_value(widget_t* widget, uint8_t value);

/**
 * @method progress_bar_set_vertical
 * 设置进度条的方向。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  vertical 是否为垂直方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_vertical(widget_t* widget, bool_t vertical);

/**
 * @method progress_bar_set_show_text
 * 设置进度条的是否显示文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  show_text 是否显示文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_bar_set_show_text(widget_t* widget, bool_t show_text);

#define PROGRESS_BAR(widget) ((progress_bar_t*)(widget))

END_C_DECLS

#endif /*TK_PROGRESS_BAR_H*/
