/**
 * File:   overlay.h
 * Author: AWTK Develop Team
 * Brief:  overlay
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
 * 2019-05-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OVERLAY_H
#define TK_OVERLAY_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class overlay_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 * overlay窗口。
 *
 * overlay窗口有点类似于非模态的dialog，但是它位置和大小是完全自由的，窗口管理器不会对它做任何限制。
 *
 * 如果overlay窗口有透明或半透效果，则不支持窗口动画，但可以通过移动窗口位置来实现类似动画的效果。
 *
 * overlay\_t是[window\_base\_t](window_base_t.md)的子类控件，window\_base\_t的函数均适用于overlay\_t控件。
 *
 * 在xml中使用"overlay"标签创建窗口。需要指定坐标和大小，可以指定主题和动画名称。如：
 *
 * ```xml
 * <overlay theme="basic" x="100" y="100" w="200" h="300">
 * ...
 * </overlay>
 * ```
 *
 * >
 * 更多用法请参考：[overlay.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 * 在c代码中使用函数overlay\_create创建窗口。如：
 *
 * ```c
 *  widget_t* overlay = overlay_create(NULL, 100, 100, 200, 300);
 * ```
 *
 * > 完整示例请参考：[overlay
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
typedef struct _overlay_t {
  window_base_t overlay;

} overlay_t;

/**
 * @method overlay_create
 * 创建overlay对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* overlay_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method overlay_cast
 * 转换为overlay对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget overlay对象。
 *
 * @return {widget_t*} overlay对象。
 */
widget_t* overlay_cast(widget_t* widget);

#define OVERLAY(widget) ((overlay_t*)(overlay_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(overlay);

END_C_DECLS

#endif /*TK_OVERLAY_H*/
