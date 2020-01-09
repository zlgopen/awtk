/**
 * File:   color_component.h
 * Author: AWTK Develop Team
 * Brief:  color_component
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_COMPONENT_H
#define TK_COLOR_COMPONENT_H

#include "base/widget.h"
#include "base/bitmap.h"
#include "color_picker/color_picker_const.h"

BEGIN_C_DECLS

typedef ret_t (*color_component_update_t)(widget_t* widget);

/**
 * @class color_component_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 颜色选择器的颜色分量。
 * 控件的名称有严格规定：
 * COLOR_PICKER_CHILD_SV: 水平为Value/Brightness(递增)，垂直为Saturation(递减)。
 * COLOR_PICKER_CHILD_H: 水平为同色，垂直为Hue(递减)。
 */
typedef struct _color_component_t {
  widget_t widget;

  xy_t color_x;
  xy_t color_y;

  color_t c;
  bitmap_t* image;
  int32_t last_hue;
  bool_t need_update;
  color_component_update_t update;

  bool_t pressed;
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
 * @param {color_t}  c 颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_component_set_color(widget_t* widget, color_t c);

/**
 * @method color_component_set_hsv
 * 设置颜色。
 * @param {widget_t*} widget 控件对象。
 * @param {float}  h 色调。
 * @param {float}  s 饱和度。
 * @param {float}  v 明度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_component_set_hsv(widget_t* widget, float h, float s, float v);

/**
 * @method color_component_get_h
 * 获取h分量。
 * @param {widget_t*} widget color_component对象。
 *
 * @return {float} 返回h分量。
 */
float color_component_get_h(widget_t* widget);

/**
 * @method color_component_get_s
 * 获取s分量。
 * @param {widget_t*} widget color_component对象。
 *
 * @return {float} 返回s分量。
 */
float color_component_get_s(widget_t* widget);

/**
 * @method color_component_get_v
 * 获取v分量。
 * @param {widget_t*} widget color_component对象。
 *
 * @return {float} 返回v分量。
 */
float color_component_get_v(widget_t* widget);

/**
 * @method color_component_cast
 * 转换为color_component对象(供脚本语言使用)。
 *
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget color_component对象。
 *
 * @return {widget_t*} color_component对象。
 */
widget_t* color_component_cast(widget_t* widget);

#define COLOR_COMPONENT(widget) ((color_component_t*)(color_component_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(color_component);

END_C_DECLS

#endif /*TK_COLOR_COMPONENT_H*/
