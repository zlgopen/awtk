/**
 * File:   gif_image.h
 * Author: AWTK Develop Team
 * Brief:  gif_image
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
 * @annotation ["scriptable","design","widget"]
 * GIF图片控件。
 *
 * > 注意：GIF图片的尺寸大于控件大小时会自动缩小图片，但一般的嵌入式系统的硬件加速都不支持图片缩放，
 * 所以缩放图片会导致性能明显下降。如果性能不满意时，请确认一下GIF图片的尺寸是否小余控件大小。
 *
 * gif\_image\_t是[image\_base\_t](image_base_t.md)的子类控件，image\_base\_t的函数均适用于gif\_image\_t控件。
 *
 * 在xml中使用"gif"标签创建GIF图片控件。如：
 *
 * ```xml
 * <gif image="bee"/>
 * ```
 *
 * >更多用法请参考：
 * [gif
 * image](https://github.com/zlgopen/awtk/blob/master/design/default/ui/gif_image.xml)
 *
 * 在c代码中使用函数gif\_image\_create创建GIF图片控件。如：
 *
 * ```c
 *  widget_t* image = gif_image_create(win, 10, 10, 200, 200);
 *  image_set_image(image, "bee");
 * ```
 *
 * > 创建之后:
 * >
 * > 需要用widget\_set\_image设置图片名称。
 *
 * > 完整示例请参考：[gif image demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/gif_image.c)
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <gif>
 *  <style name="border">
 *   <normal border_color="#000000" bg_color="#e0e0e0" text_color="black"/>
 *  </style>
 * </gif>
 * ```
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
 *
 */
typedef struct _gif_image_t {
  image_base_t image_base;

  /*private*/
  uint32_t index;
  uint32_t delay;
  uint32_t timer_id;
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

#define GIF_IMAGE(widget) ((gif_image_t*)(gif_image_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(gif_image);

END_C_DECLS

#endif /*TK_GIF_IMAGE_H*/
