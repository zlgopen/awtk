/**
 * File:   color_picker.h
 * Author: AWTK Develop Team
 * Brief:  color_picker
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_PICKER_H
#define TK_COLOR_PICKER_H

#include "base/widget.h"
#include "color_picker/color_picker_const.h"

BEGIN_C_DECLS

/**
 * @class color_picker_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 颜色选择器。 其中的控件必须按下列规则命名：
 * COLOR_PICKER_CHILD_R 红色分量。可以是spin_box、edit和slider。
 * COLOR_PICKER_CHILD_G 绿色分量。可以是spin_box、edit和slider。
 * COLOR_PICKER_CHILD_B 蓝色分量。可以是spin_box、edit和slider。
 * COLOR_PICKER_CHILD_H Hue分量。可以是spin_box、edit、slider和color_component。
 * COLOR_PICKER_CHILD_S Saturation分量。可以是spin_box、edit和slider。
 * COLOR_PICKER_CHILD_V Value/Brightness分量。可以是spin_box、edit和slider。
 * COLOR_PICKER_CHILD_SV Saturation和Value/Brightness分量。可以是color_component。
 * COLOR_PICKER_CHILD_OLD 旧的值。可以是spin_box、edit和color_tile。
 * COLOR_PICKER_CHILD_NEW 新的值。可以是spin_box、edit和color_tile。
 */
typedef struct _color_picker_t {
  widget_t widget;

  /**
   * @property {const char*} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 颜色。
   */
  char value[8];

  color_t c;
  color_t init_c;
  widget_t* trigger_child;
} color_picker_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(颜色)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(颜色)改变事件。
 */

/**
 * @method color_picker_create
 * 创建color_picker对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* color_picker_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method color_picker_set_color
 * 设置颜色。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} color 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_picker_set_color(widget_t* widget, const char* color);

/**
 * @method color_picker_cast
 * 转换为color_picker对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget color_picker对象。
 *
 * @return {widget_t*} color_picker对象。
 */
widget_t* color_picker_cast(widget_t* widget);

#define COLOR_PICKER(widget) ((color_picker_t*)(widget))

END_C_DECLS

#endif /*TK_COLOR_PICKER_H*/
