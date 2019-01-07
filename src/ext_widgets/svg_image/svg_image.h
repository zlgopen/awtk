/**
 * File:   svg_image.h
 * Author: AWTK Develop Team
 * Brief:  svg_image
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
 * 2018-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_IMAGE_H
#define TK_SVG_IMAGE_H

#include "base/widget.h"
#include "base/image_base.h"

BEGIN_C_DECLS

/**
 * @class svg_image_t
 * @parent image_base_t
 * @annotation ["scriptable"]
 * SVG图片控件。
 */
typedef struct _svg_image_t {
  image_base_t image_base;

  /*private*/
  const asset_info_t* bsvg_asset;
} svg_image_t;

/**
 * @method svg_image_create
 * 创建svg_image对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* svg_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method svg_image_cast
 * 转换为svg_image对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget svg_image对象。
 *
 * @return {widget_t*} svg_image对象。
 */
widget_t* svg_image_cast(widget_t* widget);

#define WIDGET_TYPE_SVG_IMAGE "svg"

#define SVG_IMAGE(widget) ((svg_image_t*)(widget))

END_C_DECLS

#endif /*TK_SVG_IMAGE_H*/
