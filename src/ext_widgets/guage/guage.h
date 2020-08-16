/**
 * File:   guage.h
 * Author: AWTK Develop Team
 * Brief:  guage
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
 * @annotation ["scriptable","design","widget"]
 * 表盘控件。
 *
 * 表盘控件就是一张图片。
 *
 * guage\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于guage\_t控件。
 *
 * 在xml中使用"guage"标签创建表盘控件。如：
 *
 * ```xml
 * <guage x="c" y="10" w="240" h="240" image="guage_bg" >
 * ```
 *
 * > 更多用法请参考：
 * [guage.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/guage.xml)
 *
 * 在c代码中使用函数guage\_create创建表盘控件。如：
 *
 * ```c
 *  widget_t* guage = guage_create(win, 10, 10, 200, 200);
 *  guage_set_image(guage, "guage_bg");
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <guage>
 *  <style name="border">
 *   <normal border_color="#000000" bg_color="#e0e0e0" text_color="black"/>
 *  </style>
 * </guage>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
 *
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
 *
 * > 绘制方式的属性值和枚举值:
 * [image\_draw\_type\_name\_value](https://github.com/zlgopen/awtk/blob/master/src/base/enums.c#L98)
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {image_draw_type_t}  draw_type 显示方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

#define WIDGET_TYPE_GUAGE "guage"
#define GUAGE(widget) ((guage_t*)(guage_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(guage);

END_C_DECLS

#endif /*TK_GUAGE_H*/
