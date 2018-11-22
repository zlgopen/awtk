/**
 * File:   image.h
 * Author: AWTK Develop Team
 * Brief:  image
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
 * 2018-02-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_H
#define TK_IMAGE_H

#include "base/widget.h"
#include "base/image_base.h"

BEGIN_C_DECLS

/**
 * @class image_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 图片控件。
 */
typedef struct _image_t {
  image_base_t image_base;

  /**
   * @property {image_draw_type_t} draw_type
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片的绘制方式(仅在没有旋转和缩放时生效)。
   */
  image_draw_type_t draw_type;
} image_t;

/**
 * @method image_create
 * 创建image对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method image_set_draw_type
 * 设置图片的显示方式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 显示方式(仅在没有旋转和缩放时生效)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

/**
 * @method image_cast
 * 转换为image对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget image对象。
 *
 * @return {widget_t*} image对象。
 */
widget_t* image_cast(widget_t* widget);

#define IMAGE(widget) ((image_t*)(widget))

END_C_DECLS

#endif /*TK_IMAGE_H*/
