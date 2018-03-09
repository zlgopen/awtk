/**
 * File:   image.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  image
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_IMAGE_H
#define LFTK_IMAGE_H

#include "base/wstr.h"
#include "base/image.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class image_t 
 * @parent widget_t
 * @scriptable
 * 图片控件。
 */
typedef struct _image_t {
  widget_t widget;
  bitmap_t bitmap;
}image_t;

/**
 * @method image_create
 * @constructor
 * 创建image对象
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
 * @method image_set_image_name
 * 设置控件的图片名称。
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_set_image_name(widget_t* widget, const char* name);

ret_t image_set_image(widget_t* widget, bitmap_t* image);

#define IMAGE(widget) (image_t*)(widget)

END_C_DECLS

#endif/*LFTK_IMAGE_H*/

