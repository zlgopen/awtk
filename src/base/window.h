/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_H
#define TK_WINDOW_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class window_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 * 窗口。
 *
 * 缺省的应用程序窗口，占用除system\_bar\_t之外的整个区域，请不要修改它的位置和大小(除非你清楚后果)。
 *
 * window\_t是[window\_base\_t](window_base_t.md)的子类控件，window\_base\_t的函数均适用于window\_t控件。
 *
 * 在xml中使用"window"标签创建窗口。无需指定坐标和大小，可以指定主题和动画名称。如：
 *
 * ```xml
 * <window theme="basic" anim_hint="htranslate">
 * ...
 * </window>
 * ```
 *
 * >
 * 更多用法请参考：[window.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 在c代码中使用函数window\_create创建窗口。如：
 *
 * ```c
 *  widget_t* window = window_create(NULL, 0, 0, 0, 0);
 * ```
 *
 * > 无需指定父控件、坐标和大小，使用0即可。
 *
 * > 完整示例请参考：[window
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
typedef struct _window_t {
  window_base_t window;

  /**
   * @property {bool_t} fullscreen
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否全屏。
   *
   *>这里全屏是指与LCD相同大小，而非让SDL窗口全屏。
   */
  bool_t fullscreen;
} window_t;

/**
 * @method window_create
 * 创建window对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method window_create_default
 * 以缺省的方式创建window对象。
 * @annotation ["constructor", "scriptable"]
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_create_default(void);

/**
 * @method window_set_fullscreen
 * 设置为全屏窗口。
 *
 *>这里全屏是指与LCD相同大小，而非让SDL窗口全屏。
 *
 * @annotation ["deconstructor", "scriptable"]
 * @param {widget_t*} widget window对象。
 * @param {bool_t} fullscreen 是否全屏。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_set_fullscreen(widget_t* widget, bool_t fullscreen);

/**
 * @method window_open
 * @annotation ["constructor", "scriptable"]
 * 从资源文件中加载并创建window_base对象。本函数在ui_loader/ui_builder_default里实现。
 *
 * @param {const char*} name window的名称。
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_open(const char* name);

/**
 * @method window_open_and_close
 * @annotation ["constructor", "scriptable"]
 * 从资源文件中加载并创建window对象。本函数在ui_loader/ui_builder_default里实现。
 * @param {const char*} name window的名称。
 * @param {widget_t*} to_close 关闭该窗口。
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_open_and_close(const char* name, widget_t* to_close);

/**
 * @method window_close
 * 关闭窗口。
 * @annotation ["deconstructor", "scriptable"]
 * @param {widget_t*} widget window对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_close(widget_t* widget);

/**
 * @method window_close_force
 * 立即无条件关闭窗口(无动画)。
 * @annotation ["deconstructor", "scriptable"]
 * @param {widget_t*} widget window对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_close_force(widget_t* widget);

/**
 * @method window_cast
 * 转换为window对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget window对象。
 *
 * @return {widget_t*} window对象。
 */
widget_t* window_cast(widget_t* widget);

#define WINDOW(widget) ((window_t*)(window_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(window);

END_C_DECLS

#endif /*TK_WINDOW_H*/
