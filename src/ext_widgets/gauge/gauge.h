/**
 * File:   gauge.h
 * Author: AWTK Develop Team
 * Brief:  gauge
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_GAUGE_H
#define TK_GAUGE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class gauge_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 表盘控件。
 *
 * 表盘控件就是一张图片。
 *
 * gauge\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于gauge\_t控件。
 *
 * 在xml中使用"gauge"标签创建表盘控件。如：
 *
 * ```xml
 * <gauge x="c" y="10" w="240" h="240" image="gauge_bg" >
 * ```
 *
 * > 更多用法请参考：
 * [gauge.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/gauge.xml)
 *
 * 在c代码中使用函数gauge\_create创建表盘控件。如：
 *
 * ```c
 *  widget_t* gauge = gauge_create(win, 10, 10, 200, 200);
 *  gauge_set_image(gauge, "gauge_bg");
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <gauge>
 *  <style name="border">
 *   <normal border_color="#000000" bg_color="#e0e0e0" text_color="black"/>
 *  </style>
 * </gauge>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
 *
 */
typedef struct _gauge_t {
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
} gauge_t;

/**
 * @method gauge_create
 * 创建gauge对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* gauge_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method gauge_cast
 * 转换为gauge对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget gauge对象。
 *
 * @return {widget_t*} gauge对象。
 */
widget_t* gauge_cast(widget_t* widget);

/**
 * @method gauge_set_image
 * 设置背景图片的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget image对象。
 * @param {char*}  name 图片名称，该图片必须存在于资源管理器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gauge_set_image(widget_t* widget, const char* name);

/**
 * @method gauge_set_draw_type
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
ret_t gauge_set_draw_type(widget_t* widget, image_draw_type_t draw_type);

#define WIDGET_TYPE_GAUGE "gauge"
#define GAUGE(widget) ((gauge_t*)(gauge_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(gauge);

#define GUAGE GAUGE
#define guage_set_image gauge_set_image
#define guage_set_draw_type gauge_set_draw_type

END_C_DECLS

#endif /*TK_GAUGE_H*/
