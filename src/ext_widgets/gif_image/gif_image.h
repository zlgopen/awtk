/**
 * File:   gif_image.h
 * Author: AWTK Develop Team
 * Brief:  gif_image
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
 * 2018-11-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GIF_IMAGE_H
#define TK_GIF_IMAGE_H

#include "base/widget.h"
#include "base/image_base.h"

BEGIN_C_DECLS

/**
 * @class gif_image_t
 * @parent image_base_t
 * @annotation ["scriptable"]
 * GIF图片控件。
 */
typedef struct _gif_image_t {
  image_base_t image_base;

  /*private*/
  int* delays;
  uint32_t index;
  uint32_t timer_id;
  uint32_t frames_nr;
} gif_image_t;

/**
 * @method gif_image_create
 * 创建gif_image对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* gif_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method gif_image_cast
 * 转换为gif_image对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget gif_image对象。
 *
 * @return {widget_t*} gif_image对象。
 */
widget_t* gif_image_cast(widget_t* widget);

#define WIDGET_TYPE_GIF_IMAGE "gif"

#define GIF_IMAGE(widget) ((gif_image_t*)(widget))

END_C_DECLS

#endif /*TK_GIF_IMAGE_H*/
