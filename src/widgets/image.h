/**
 * File:   image.h
 * Author: AWTK Develop Team
 * Brief:  image
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
 * @parent image_base_t
 * @annotation ["scriptable","design","widget"]
 * 图片控件。
 *
 * 用来显示一张静态图片，目前支持bmp/png/jpg等格式。
 *
 * 如果要显示gif文件，请用[gif\_image](gif_image_t.md)。
 *
 * 如果要显示svg文件，请用[svg\_image](svg_image_t.md)。
 *
 * 如果需要支持勾选效果，请设置**selectable**属性。
 *
 * 如果需要支持点击效果，请设置**clickable**属性。
 *
 * image\_t是[image\_base\_t](image_base_t.md)的子类控件，image\_base\_t的函数均适用于image\_t控件。
 *
 * 在xml中使用"image"标签创建图片控件。如：
 *
 * ```xml
 * <image style="border" image="earth" draw_type="icon" />
 * ```
 *
 * > 更多用法请参考：
 * [image.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/images.xml)
 *
 * 在c代码中使用函数image\_create创建图片控件。如：
 *
 * ```c
 *  widget_t* image = image_create(win, 10, 10, 128, 30);
 *  image_set_image(image, "earth");
 * ```
 *
 * > 创建之后:
 * >
 * > 需要用widget\_set\_image设置图片名称。
 * >
 * > 可以用image\_set\_draw\_type设置图片的绘制方式。
 *
 * > 绘制方式请参考[image\_draw\_type\_t](image_draw_type_t.md)
 *
 * > 绘制方式的属性值和枚举值:
 * [image\_draw\_type\_name\_value](https://github.com/zlgopen/awtk/blob/master/src/base/enums.c#L98)
 *
 * > 完整示例请参考：
 * [image demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/image.c)
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <image>
 *  <style name="border">
 *   <normal border_color="#000000" bg_color="#e0e0e0" text_color="black"/>
 *  </style>
 * </image>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L313)
 *
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
 * 设置图片的绘制方式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 绘制方式(仅在没有旋转和缩放时生效)。
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

#define IMAGE(widget) ((image_t*)(image_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(image);

END_C_DECLS

#endif /*TK_IMAGE_H*/
