/**
 * File:   color_component.h
 * Author: AWTK Develop Team
 * Brief:  color_component
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_COMPONENT_H
#define TK_COLOR_COMPONENT_H

#include "base/widget.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

typedef ret_t (*color_component_update_t)(widget_t* widget);

/**
 * @class color_component_t
 * @parent widget_t
 * 颜色选择器的颜色分量。 
 * 控件的名称有严格规定：
 * sv: 水平为Value/Brightness(递增)，垂直为Saturation(递减)。
 * h: 水平为同色，垂直为Hue(递减)。 
 */
typedef struct _color_component_t {
  widget_t widget;

  color_t c;
  bitmap_t image;
  bool_t need_update;
  color_component_update_t update;
} color_component_t;

/**
 * @method color_component_create
 * 创建color_component对象
 * @annotation ["constructor"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* color_component_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method color_component_set_color
 * 设置颜色。
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  r 红色分量。
 * @param {uint8_t}  g 绿色分量。
 * @param {uint8_t}  b 蓝色分量。
 * @param {uint8_t}  a alpha分量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_component_set_color(widget_t* widget, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * @method color_component_cast
 * 转换为color_component对象(供脚本语言使用)。
 * @annotation ["cast"]
 * @param {widget_t*} widget color_component对象。
 *
 * @return {widget_t*} color_component对象。
 */
widget_t* color_component_cast(widget_t* widget);

#define COLOR_COMPONENT(widget) ((color_component_t*)(widget))

END_C_DECLS

#endif /*TK_COLOR_COMPONENT_H*/

