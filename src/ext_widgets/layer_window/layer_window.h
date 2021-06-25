/**
 * File:   layer_window.h
 * Author: AWTK Develop Team
 * Brief:  layer_window
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYER_WINDOW_H
#define TK_LAYER_WINDOW_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class layer_window_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 * layer_window窗口。
 *
 * 用于将窗口显示到指定的层上。
 *
 * layer_window\_t是[window\_base\_t](window_base_t.md)的子类控件，window\_base\_t的函数均适用于layer_window\_t控件。
 *
 * 在xml中使用"layer_window"标签创建窗口。需要指定坐标和大小，可以指定窗体样式和动画名称。如：
 *
 * ```xml
 * <layer_window theme="basic" x="100" y="100" w="200" h="300" layer_name="fast">
 * ...
 * </layer_window>
 * ```
 *
 * >
 * 更多用法请参考：[layer_window.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 在c代码中使用函数layer_window\_create创建窗口。如：
 *
 * ```c
 *  widget_t* layer_window = layer_window_create(NULL, 100, 100, 200, 300);
 * ```
 *
 * > 完整示例请参考：[layer_window
 * demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/)
 *
 * 可用通过style来设置窗口的风格，如背景颜色或图片等。如：
 *
 * ```xml
 * <style name="bricks">
 *  <normal bg_image="bricks"  bg_image_draw_type="repeat"/>
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L0)
 */
typedef struct _layer_window_t {
  window_base_t layer_window;

  /**
   * @property {char*} layer_name
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * layer的名称。
   *
   */
  char* layer_name;
} layer_window_t;

/**
 * @method layer_window_create
 * 创建layer_window对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* layer_window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method layer_window_set_layer_name
 * 设置层的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  layer_name 层的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t layer_window_set_layer_name(widget_t* widget, const char* layer_name);

/**
 * @method layer_window_cast
 * 转换为layer_window对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget layer_window对象。
 *
 * @return {widget_t*} layer_window对象。
 */
widget_t* layer_window_cast(widget_t* widget);

#define LAYER_WINDOW_PROP_LAYER_NAME "layer_name"
#define WIDGET_TYPE_LAYER_WINDOW "layer_window"

#define LAYER_WINDOW(widget) ((layer_window_t*)(layer_window_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(layer_window);

END_C_DECLS

#endif /*TK_LAYER_WINDOW_H*/
