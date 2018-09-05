/**
 * File:   guage.h
 * Author: AWTK Develop Team
 * Brief:  guage
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
 * 2018-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GUAGE_H
#define TK_GUAGE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class guage_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 仪表控件。
 */
typedef struct _guage_t {
  widget_t widget;

  /**
   * @property {int32_t} angle
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
   */
  int32_t angle;

  /**
   * @property {char*} bg_image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 背景图片。
   */
  char* bg_image;

  /**
   * @property {char*} pointer_image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针图片。图片须垂直向上，图片的中心点为旋转方向。
   */
  char* pointer_image;
} guage_t;

/**
 * @method guage_create
 * 创建guage对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* guage_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method guage_cast
 * 转换为guage对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget guage对象。
 *
 * @return {widget_t*} guage对象。
 */
widget_t* guage_cast(widget_t* widget);

/**
 * @method guage_set_angle
 * 设置指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} angle 指针角度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_angle(widget_t* widget, int32_t angle);

/**
 * @method guage_set_pointer_image
 * 设置指针的图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} angle 指针的图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_pointer_image(widget_t* widget, const char* angle);

/**
 * @method guage_set_bg_image
 * 设置背景图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} bg_image 背景图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_bg_image(widget_t* widget, const char* bg_image);

#define GUAGE_PROP_ANGLE "angle"
#define GUAGE_PROP_BG_IMAGE "bg_image"
#define GUAGE_PROP_POINTER_IMAGE "pointer_image"

#define WIDGET_TYPE_GUAGE "guage"

#define GUAGE(widget) ((guage_t*)(widget))

END_C_DECLS

#endif /*TK_GUAGE_H*/
