/**
 * File:   mutable_image.h
 * Author: AWTK Develop Team
 * Brief:  mutable_image
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
 * 2019-02-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MUTABLE_IMAGE_H
#define TK_MUTABLE_IMAGE_H

#include "base/widget.h"
#include "base/image_base.h"

BEGIN_C_DECLS

typedef ret_t (*mutable_image_prepare_image_t)(void* ctx, bitmap_t* image);
typedef bitmap_t* (*mutable_image_create_image_t)(void* ctx, bitmap_format_t format,
                                                  bitmap_t* old_image);

/**
 * @class mutable_image_t
 * @parent image_base_t
 * @annotation ["scriptable","design","widget"]
 *
 * mutable图片控件。
 *
 * 像摄像头和视频的图像是变化的，每一帧都不同，我们把这类图片称为mutable image。
 *
 * 本控件辅助实现摄像头和视频的显示功能。
 *
 * mutable\_image\_t是[image\_base\_t](image_base_t.md)的子类控件，image\_base\_t的函数均适用于mutable\_image\_t控件。
 *
 * 在xml中使用"mutable\_image"标签创建mutable图片控件。如：
 *
 * ```xml
 * <mutable_image w="100%" h="100%"/>
 * ```
 *
 * >更多用法请参考：
 * [mutable
 * image](https://github.com/zlgopen/awtk/blob/master/design/default/ui/mutable_image.xml)
 *
 * 在c代码中使用函数mutable\_image\_create创建mutable图片控件。如：
 *
 * ```c
 *  widget_t* image = mutable_image_create(win, 10, 10, 200, 200);
 *  mutable_image_set_prepare_image(image, get_camera_image, camera);
 * ```
 *
 * > 创建之后:
 * >
 * > 需要用mutable\_image\_set\_create\_image设置创建图片的回调函数。
 * > 需要用mutable\_image\_set\_prepare\_image设置准备图片的回调函数。
 *
 * > 完整示例请参考：[mutable image demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/mutable_image.c)
 *
 * 一般不需通过style来设置控件的显示风格，如果在特殊情况下需要，可以参考其它控件。
 *
 */
typedef struct _mutable_image_t {
  image_base_t image_base;

  /*private*/
  void* prepare_image_ctx;
  mutable_image_prepare_image_t prepare_image;

  void* create_image_ctx;
  mutable_image_create_image_t create_image;

  bitmap_t* fb;
  bitmap_t* image;
  canvas_t* canvas;
} mutable_image_t;

/**
 * @method mutable_image_create
 * 创建mutable_image对象
 * @annotation ["constructor"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* mutable_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method mutable_image_set_prepare_image
 * 设置prepare_image回调函数。
 *
 * prepare_image回调函数在每次绘制之前被调用，用于准备下一帧要显示的图片。
 * 比如获取摄像头的预览图片，将其设置到image参数中。
 *
 * 注意：在回调函数中，只能修改图片的内容，不用修改图片的大小和格式，如果不匹配请先转换。
 *
 * @param {widget_t*} widget mutable_image对象。
 * @param {mutable_image_prepare_image_t} prepare_image 准备图片的回调函数。
 * @param {void*} prepare_image_ctx prepare_image回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutable_image_set_prepare_image(widget_t* widget, mutable_image_prepare_image_t prepare_image,
                                      void* prepare_image_ctx);

/**
 * @method mutable_image_set_create_image
 * 设置create_image回调函数。
 * 
 * @param {widget_t*} widget mutable_image对象。
 * @param {mutable_image_create_image_t} create_image 创建图片的回调函数。
 * @param {void*} create_image_ctx create_image回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutable_image_set_create_image(widget_t* widget, mutable_image_create_image_t create_image,
                                     void* create_image_ctx);

/**
 * @method mutable_image_set_framebuffer
 * 设置framebuffer(当硬件支持多层合成时才用)。
 *
 * 有的硬件支持多层framebuffer，一层用于视图/摄像头，一层用于GUI，由硬件合成最终图像。
 * 此时可以设置用于摄像头的framebuffer，图像直接绘制到该framebuffer上。
 *
 * @param {widget_t*} widget mutable_image对象。
 * @param {uint32_t} w framebuffer宽度。
 * @param {uint32_t} h framebuffer高度。
 * @param {bitmap_format_t} format framebuffer的格式。
 * @param {uint8_t*} buff framebuffer内存。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutable_image_set_framebuffer(widget_t* widget, uint32_t w, uint32_t h,
                                    bitmap_format_t format, uint8_t* buff);

/**
 * @method mutable_image_cast
 * 转换为mutable_image对象(供脚本语言使用)。
 *
 * @annotation ["cast"]
 * @param {widget_t*} widget mutable_image对象。
 *
 * @return {widget_t*} mutable_image对象。
 */
widget_t* mutable_image_cast(widget_t* widget);

/**
 * @method mutable_image_init
 * 初始化 mutable_image （提供给继承的子类使用的）
 *
 * @annotation ["cast"]
 * @param {widget_t*} widget mutable_image对象。
 *
 * @return {widget_t*} mutable_image对象。
 */
widget_t* mutable_image_init(widget_t* widget);

/**
 * @method mutable_image_on_destroy
 * 释放 mutable_image （提供给继承的子类使用的）
 *
 * @annotation ["cast"]
 * @param {widget_t*} widget mutable_image对象。
 *
 * @return {widget_t*} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutable_image_on_destroy(widget_t* widget);

/**
 * @method mutable_image_on_paint_self
 * mutable_image 的绘制函数 （提供给继承的子类使用的）
 *
 * @annotation ["cast"]
 * @param {widget_t*} widget mutable_image对象。
 * @param {canvas_t*} canvas 画布对象。
 *
 * @return {widget_t*} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutable_image_on_paint_self(widget_t* widget, canvas_t* canvas);

#define WIDGET_TYPE_MUTABLE_IMAGE "mutable_image"

#define MUTABLE_IMAGE(widget) ((mutable_image_t*)(mutable_image_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(mutable_image);

END_C_DECLS

#endif /*TK_MUTABLE_IMAGE_H*/
