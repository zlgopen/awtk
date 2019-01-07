/**
 * File:   guage_pointer.h
 * Author: AWTK Develop Team
 * Brief:  guage_pointer
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
 * 2018-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GUAGE_POINTER_H
#define TK_GUAGE_POINTER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class guage_pointer_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 仪表指针控件。
 */
typedef struct _guage_pointer_t {
  widget_t widget;

  /**
   * @property {int32_t} angle
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
   */
  int32_t angle;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针图片。图片须垂直向上，图片的中心点为旋转方向。
   */
  char* image;

  /*private*/
  const asset_info_t* bsvg_asset;
} guage_pointer_t;

/**
 * @method guage_pointer_create
 * 创建guage_pointer对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* guage_pointer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method guage_pointer_cast
 * 转换为guage_pointer对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget guage_pointer对象。
 *
 * @return {widget_t*} guage_pointer对象。
 */
widget_t* guage_pointer_cast(widget_t* widget);

/**
 * @method guage_pointer_set_angle
 * 设置指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} angle 指针角度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_pointer_set_angle(widget_t* widget, int32_t angle);

/**
 * @method guage_pointer_set_image
 * 设置指针的图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} image 指针的图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_pointer_set_image(widget_t* widget, const char* image);

#define GUAGE_POINTER_PROP_ANGLE "angle"
#define WIDGET_TYPE_GUAGE_POINTER "guage_pointer"
#define GUAGE_POINTER(widget) ((guage_pointer_t*)(widget))

END_C_DECLS

#endif /*TK_GUAGE_POINTER_H*/
