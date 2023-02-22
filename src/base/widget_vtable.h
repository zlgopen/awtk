/**
 * File:   widget_vtable.h
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_VTABLE_H
#define TK_WIDGET_VTABLE_H

#include "base/widget.h"

BEGIN_C_DECLS

/*widget 虚函数的缺省实现，用于辅助实现子控件*/

/**
 * @method widget_invalidate_default
 * awtk默认的invalidate接口。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {const rect_t*} rect 无效化区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_invalidate_default(widget_t* widget, const rect_t* rect);

/**
 * @method widget_on_event_default
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {event_t*} e 事件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_event_default(widget_t* widget, event_t* e);

/**
 * @method widget_on_paint_null
 * 空的paint函数。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t widget_on_paint_null(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_paint_self_default
 * 绘制控件自身的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_self_default(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_paint_children_clip
 * 绘制子控件带clip功能的实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_children_clip(widget_t* widget, canvas_t* c);

/**
 * @method widget_paint_with_clip
 * 根据当前widget进行clip，然后调用on_paint回调函数进行绘制。
 *
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {rect_t*} clip clip区域(为NULL时使用widget的区域)。
 * @param {canvas_t*} c 画布对象。
 * @param {widget_on_paint_t} on_paint 绘制的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint_with_clip(widget_t* widget, rect_t* clip, canvas_t* c,
                             widget_on_paint_t on_paint);

/**
 * @method widget_on_paint_children_default
 * 绘制子控件的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_keydown_default
 * 控件按键按下事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {key_event_t*} e 按键事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e);

/**
 * @method widget_on_keyup_default
 * 控件按键抬起事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {key_event_t*} e 按键事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e);
ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_down_default
 * 控件指针按下事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_move_default
 * 控件指针移动事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_pointer_up_default
 * 控件指针抬起事件回调的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {pointer_event_t*} e 指针事件。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e);

/**
 * @method widget_on_copy_default
 * 拷贝控件的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 目的控件对象。
 * @param {widget_t*} other 源控件对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_copy_default(widget_t* widget, widget_t* other);

/**
 * @method widget_copy_props
 * 拷贝控件属性。
 * @annotation ["global"]
 * @param {widget_t*} dst 目的控件对象。
 * @param {widget_t*} src 源控件对象。
 * @param {const char* const*} properties 属性列表。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_copy_props(widget_t* dst, widget_t* src, const char* const* properties);

/**
 * @method widget_get_prop_default
 * 获取控件属性的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性名称。
 * @param {value_t*} v 返回的属性值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_set_prop_default
 * 设置控件属性的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性名称。
 * @param {const value_t*} v 设置的属性值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_prop_default(widget_t* widget, const char* name, const value_t* v);

/**
 * @method widget_find_target_default
 * awtk默认的find_target接口。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {xy_t} x 坐标。
 * @param {xy_t} y 坐标。
 *
 * @return {widget_t*} 返回响应的控件。
 */
widget_t* widget_find_target_default(widget_t* widget, xy_t x, xy_t y);

/**
 * @method widget_vtable_init
 * 动态初始化虚表。
 * 
 * 备注：
 * 初始化的时候，虚表会自动继承父类的所有属性。
 * @annotation ["global"]
 *
 * @param {widget_vtable_t*} vt 子类虚表。
 * @param {const widget_vtable_t*} parent 被继承的父类虚表。
 *
 * @return {widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
widget_vtable_t* widget_vtable_init(widget_vtable_t* vt, const widget_vtable_t* parent);

/**
 * @method widget_get_parent_vtable
 * 获取父类的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {const widget_vtable_t*} vt 子类的虚表。
 *
 * @return {const widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
const widget_vtable_t* widget_get_parent_vtable(const widget_vtable_t* vt);

/**
 * @method widget_set_self_vtable
 * 设置控件的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {const widget_vtable_t*} vt 新的虚表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_self_vtable(widget_t* widget, const widget_vtable_t* vt);

/**
 * @method widget_get_self_vtable
 * 获取控件的虚表。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 *
 * @return {const widget_vtable_t*} 成功返回虚表，失败返回 NULL。
 */
const widget_vtable_t* widget_get_self_vtable(widget_t* widget);

ret_t widget_grab_default(widget_t* widget, widget_t* child);
ret_t widget_ungrab_default(widget_t* widget, widget_t* child);
ret_t widget_destroy_default(widget_t* widget);

END_C_DECLS

#endif /*TK_WIDGET_VTABLE_H*/
