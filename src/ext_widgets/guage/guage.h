/**
 * File:   guage.h
 * Author: AWTK Develop Team
 * Brief:  guage
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

#ifndef TK_GUAGE_H
#define TK_GUAGE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class guage_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 表盘控件。
 */
typedef struct _guage_t {
  widget_t widget;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 背景图片。
   */
  char* image;

  /**
   * @property {image_draw_type_t} draw_type
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的绘制方式。
   */
  image_draw_type_t draw_type;
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
 * @method guage_set_image
 * 设置背景图片的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_image(widget_t* widget, const char* name);

/**
 * @method guage_set_draw_type
 * 设置图片的显示方式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 显示方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

#define WIDGET_TYPE_GUAGE "guage"
#define GUAGE(widget) ((guage_t*)(widget))

END_C_DECLS

#endif /*TK_GUAGE_H*/
