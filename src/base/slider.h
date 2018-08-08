/**
 * File:   slider.h
 * Author: AWTK Develop Team
 * Brief:  slider
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
 * 2018-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SLIDER_H
#define TK_SLIDER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class slider_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 滑块控件。
 */
typedef struct _slider_t {
  widget_t widget;
  /**
   * @property {uint16_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  uint16_t value;

  /**
   * @property {uint16_t} min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小值。
   */
  uint16_t min;

  /**
   * @property {uint16_t} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值。
   */
  uint16_t max;

  /**
   * @property {uint16_t} step
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动的最小单位。
   */
  uint16_t step;

  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 滑块的是否为垂直方向。
   */
  bool_t vertical;

  /*private*/
  bool_t dragging;
} slider_t;

/**
 * @method slider_create
 * 创建slider对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slider_cast
 * 转换为slider对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slider对象。
 *
 * @return {widget_t*} slider对象。
 */
widget_t* slider_cast(widget_t* widget);

/**
 * @method slider_set_value
 * 设置滑块的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint16_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slider_set_value(widget_t* widget, uint16_t value);

/**
 * @method slider_set_min
 * 设置滑块的最小值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint16_t}  min 最小值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slider_set_min(widget_t* widget, uint16_t min);

/**
 * @method slider_set_max
 * 设置滑块的最大值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint16_t}  max 最大值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slider_set_max(widget_t* widget, uint16_t max);

/**
 * @method slider_set_step
 * 设置滑块的拖动的最小单位。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint16_t}  step 拖动的最小单位。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slider_set_step(widget_t* widget, uint16_t step);

/**
 * @method slider_set_vertical
 * 设置滑块的方向。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  vertical 是否为垂直方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slider_set_vertical(widget_t* widget, bool_t vertical);

#define SLIDER(widget) ((slider_t*)(widget))

END_C_DECLS

#endif /*TK_SLIDER_H*/
