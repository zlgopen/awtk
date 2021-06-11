/**
 * File:   widget_vtable.h
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
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
 * @param {canvas_t*} c 画布对象。
 * @param (widget_on_paint_t} on_paint 绘制的回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_paint_with_clip(widget_t* widget, canvas_t* c, widget_on_paint_t on_paint);

/**
 * @method widget_on_paint_children_default
 * 绘制子控件的缺省实现。
 * @annotation ["global"]
 * @param {widget_t*} widget 控件对象。
 * @param {canvas_t*} c 画布对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c);

/**
 * @method widget_on_keydown_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e);
/**
 * @method widget_on_keyup_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e);
ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_down_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_move_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_up_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_copy_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_on_copy_default(widget_t* widget, widget_t* other);
/**
 * @method widget_get_prop_default
 * @annotation ["global"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v);
/**
 * @method widget_set_prop_default
 * @annotation ["global"]
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

const widget_vtable_t* widget_vtable_default(void);
ret_t widget_grab_default(widget_t* widget, widget_t* child);
ret_t widget_ungrab_default(widget_t* widget, widget_t* child);
ret_t widget_destroy_default(widget_t* widget);

END_C_DECLS

#endif /*TK_WIDGET_VTABLE_H*/
