/**
 * File:   guage_pointer.h
 * Author: AWTK Develop Team
 * Brief:  guage_pointer
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

#ifndef TK_GUAGE_POINTER_H
#define TK_GUAGE_POINTER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class guage_pointer_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 仪表指针控件。
 *
 * 仪表指针就是一张旋转的图片，图片可以是普通图片也可以是SVG图片。
 *
 * 在嵌入式平台上，对于旋转的图片，SVG图片的效率比位图高数倍，所以推荐使用SVG图片。
 *
 * guage\_pointer\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于guage\_pointer\_t控件。
 *
 * 在xml中使用"guage\_pointer"标签创建仪表指针控件。如：
 *
 * ```xml
 * <guage_pointer x="c" y="50" w="24" h="140" value="-128" image="guage_pointer" />
 * ```
 *
 * > 更多用法请参考：
 * [guage.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/guage.xml)
 *
 * 在c代码中使用函数guage\_pointer\_create创建仪表指针控件。如：
 *
 * ```c
 *  widget_t* guage_pointer = guage_pointer_create(guage, 10, 10, 100, 30);
 *  guage_pointer_set_image(guage_pointer, "guage_pointer");
 * ```
 *
 * > 创建之后，需要用guage\_pointer\_set\_image设置仪表指针图片。
 *
 *
 */
typedef struct _guage_pointer_t {
  widget_t widget;

  /**
   * @property {int32_t} angle
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
   */
  int32_t angle;

  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 指针图片。
   *
   * 图片须垂直向上，图片的中心点为旋转方向。
   *
   */
  char* image;

  /**
   * @property {char*} anchor_x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片旋转锚点x坐标。(后面加上px为像素点，不加px为相对百分比坐标0.0f到1.0f)
   */
  char* anchor_x;

  /**
   * @property {char*} anchor_y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 图片旋转锚点x坐标。(后面加上px为像素点，不加px为相对百分比坐标0.0f到1.0f)
   */
  char* anchor_y;

  /*private*/
  const asset_info_t* bsvg_asset;
} guage_pointer_t;

/**
 * @method guage_pointer_create
 * 创建guage_pointer对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* guage_pointer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method guage_pointer_cast
 * 转换为guage_pointer对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget guage_pointer对象。
 *
 * @return {widget_t*} guage_pointer对象。
 */
widget_t* guage_pointer_cast(widget_t* widget);

/**
 * @method guage_pointer_set_angle
 * 设置指针角度。12点钟方向为0度，顺时钟方向为正，单位为度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} angle 指针角度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_pointer_set_angle(widget_t* widget, int32_t angle);

/**
 * @method guage_pointer_set_image
 * 设置指针的图片。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} image 指针的图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_pointer_set_image(widget_t* widget, const char* image);

/**
 * @method guage_pointer_set_anchor
 * 设置指针的旋转锚点。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} anchor_x 指针的锚点坐标x。(后面加上px为像素点，不加px为相对百分比坐标)
 * @param {const char*} anchor_y 指针的锚点坐标y。(后面加上px为像素点，不加px为相对百分比坐标)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t guage_pointer_set_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y);

#define GUAGE_POINTER_PROP_ANGLE "angle"
#define WIDGET_TYPE_GUAGE_POINTER "guage_pointer"
#define GUAGE_POINTER(widget) ((guage_pointer_t*)(guage_pointer_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(guage_pointer);

/*public for test*/
ret_t guage_pointer_set_anchor_for_str(widget_t* widget, const char* anchor, bool_t is_x);

END_C_DECLS

#endif /*TK_GUAGE_POINTER_H*/
