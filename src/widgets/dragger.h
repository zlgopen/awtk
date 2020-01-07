/**
 * File:   dragger.h
 * Author: AWTK Develop Team
 * Brief:  dragger
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
 * 2018-07-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DRAGGER_H
#define TK_DRAGGER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dragger_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * dragger控件。
 *
 * 目前主要用于scrollbar里的滑块。
 *
 */
typedef struct _dragger_t {
  widget_t widget;
  /**
   * @property {xy_t} x_min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * x坐标的最小值。
   */
  xy_t x_min;
  /**
   * @property {xy_t} y_min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * y坐标的最小值。
   */
  xy_t y_min;
  /**
   * @property {xy_t} x_max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * x坐标的最大值。
   */
  xy_t x_max;
  /**
   * @property {xy_t} y_max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * y坐标的最大值。
   */
  xy_t y_max;

  /**/
  xy_t save_x;
  xy_t save_y;
  xy_t down_x;
  xy_t down_y;
  bool_t dragging;
  bool_t moving;
} dragger_t;

/**
 * @method dragger_create
 * 创建dragger对象。
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* dragger_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dragger_cast
 * 转换为dragger对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget dragger对象。
 *
 * @return {widget_t*} dragger对象。
 */
widget_t* dragger_cast(widget_t* widget);

/**
 * @method dragger_set_range
 * 设置拖动的范围。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget dragger控件。
 * @param {xy_t} x_min x坐标最小值。
 * @param {xy_t} y_min y坐标最小值。
 * @param {xy_t} x_max x坐标最大值。
 * @param {xy_t} y_max y坐标最大值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dragger_set_range(widget_t* widget, xy_t x_min, xy_t y_min, xy_t x_max, xy_t y_max);

#define DRAGGER(widget) ((dragger_t*)(dragger_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(dragger);

END_C_DECLS

#endif /*TK_DRAGGER_H*/
