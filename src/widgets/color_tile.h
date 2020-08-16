/**
 * File:   color_tile.h
 * Author: AWTK Develop Team
 * Brief:  color_tile
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
 * 2018-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_TILE_H
#define TK_COLOR_TILE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class color_tile_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 色块控件。
 *
 * 用来显示一个颜色块，它通过属性而不是主题来设置颜色，方便在运行时动态改变颜色。
 *
 * 可以使用value属性访问背景颜色的颜色值。
 *
 * color\_tile\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于color\_tile\_t控件。
 *
 * 在xml中使用"color_tile"标签创建色块控件。如：
 *
 * ```xml
 * <color_tile x="c" y="m" w="80" h="30" bg_color="green" />
 * ```
 *
 * > 更多用法请参考：
 * [color_tile](https://github.com/zlgopen/awtk/blob/master/design/default/ui/color_picker_rgb.xml)
 *
 * 在c代码中使用函数color_tile\_create创建色块控件。如：
 *
 * ```c
 *  widget_t* color_tile = color_tile_create(win, 10, 10, 128, 30);
 *  color_tile_set_bg_color(color_tile, "red");
 * ```
 * > 创建之后，用color\_tile\_set\_bg\_color设置背景颜色。
 *
 */
typedef struct _color_tile_t {
  widget_t widget;

  /**
   * @property {const char*} bg_color
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 背景颜色。
   */
  char bg_color[TK_COLOR_HEX_LEN + 1];

  /**
   * @property {const char*} border_color
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 边框颜色。
   */
  char border_color[TK_COLOR_HEX_LEN + 1];

  color_t bg;
  color_t border;
} color_tile_t;

/**
 * @method color_tile_create
 * 创建color_tile对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* color_tile_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method color_tile_cast
 * 转换为color_tile对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget color_tile对象。
 *
 * @return {widget_t*} color_tile对象。
 */
widget_t* color_tile_cast(widget_t* widget);

/**
 * @method color_tile_set_bg_color
 * 设置背景颜色。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  color 背景颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_tile_set_bg_color(widget_t* widget, const char* color);

/**
 * @method color_tile_set_border_color
 * 设置边框颜色。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  color 边框颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_tile_set_border_color(widget_t* widget, const char* color);

/**
 * @method color_tile_set_value
 * 设置背景颜色。
 * @param {widget_t*} widget 控件对象。
 * @param {color_t}  color 背景颜色。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_tile_set_value(widget_t* widget, color_t color);

#define COLOR_TILE(widget) ((color_tile_t*)(color_tile_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(color_tile);

END_C_DECLS

#endif /*TK_COLOR_TILE_H*/
