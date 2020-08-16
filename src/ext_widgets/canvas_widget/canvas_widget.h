/**
 * File:   canvas_widget.h
 * Author: AWTK Develop Team
 * Brief:  canvas_widget
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANVAS_WIDGET_H
#define TK_CANVAS_WIDGET_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class canvas_widget_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 画布控件。
 *
 * 画布控件让开发者可以自己在控件上绘制需要的内容。
 *
 * canvas\_widget\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于canvas\_widget\_t控件。
 *
 * 在xml中使用"canvas"标签创建画布控件。如：
 *
 * ```xml
 * <canvas name="paint_vgcanvas" x="0" y="0" w="100%" h="100%" />
 * ```
 *
 * > 更多用法请参考：
 * [canvas_widget.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/vgcanvas.xml)
 *
 * 在c代码中使用函数canvas\_widget\_create创建画布控件。如：
 *
 * ```c
 * widget_t* canvas = canvas_widget_create(win, 0, 0, win->w, win->h);
 * ```
 *
 * > 创建之后，需要用widget\_on注册EVT\_PAINT事件，并在EVT\_PAINT事件处理函数中绘制。
 *
 * ```c
 * widget_on(canvas, EVT_PAINT, on_paint_event, canvas);
 * ```
 *
 * 绘制时，可以通过canvas接口去绘制，也可以通过vgcanvas接口去绘制。
 * 先从evt获取canvas对象，再通过canvas\_get\_vgcanvas从canvas中获取vgcanvas对象。
 *
 * ```c
 *
 * static ret_t on_paint_event(void* ctx, event_t* evt) {
 *  widget_t* canvas_widget = WIDGET(ctx);
 *  canvas_t* c = paint_event_cast(evt)->c;
 *  vgcanvas_t* vg = canvas_get_vgcanvas(c);
 *  color_t bg = color_init(0xe0, 0xe0, 0xe0, 0xff);
 *  color_t tc = color_init(0, 0, 0, 0xff);
 *  rect_t r = rect_init(canvas_widget->x, canvas_widget->y, canvas_widget->w, canvas_widget->h);
 *
 *  vgcanvas_save(vg);
 *  vgcanvas_clip_rect(vg, r.x, r.y, r.w, r.h);
 *  vgcanvas_translate(vg, r.x, r.y);
 *
 * ...
 *
 *  vgcanvas_restore(vg);
 *
 *  return RET_OK;
 * }
 * ```
 *
 * > 完整示例请参考：
 * [canvas demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/canvas.c)
 *
 * 参考：
 *
 * * [canvas接口描述](canvas_t.md)
 * * [vgcanvas接口描述](vgcanvas_t.md)
 *
 */
typedef struct _canvas_widget_t {
  widget_t widget;
} canvas_widget_t;

/**
 * @method canvas_widget_create
 * 创建canvas_widget对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* canvas_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method canvas_widget_cast
 * 转换为canvas_widget对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget canvas_widget对象。
 *
 * @return {widget_t*} canvas_widget对象。
 */
widget_t* canvas_widget_cast(widget_t* widget);

#define WIDGET_TYPE_CANVAS_WIDGET "canvas"

#define CANVAS_WIDGET(widget) ((canvas_widget_t*)(canvas_widget_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(canvas_widget);

END_C_DECLS

#endif /*TK_CANVAS_WIDGET_H*/
