/**
 * File:   widget_vtable.h
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
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
 * 2018-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_VTABLE_H
#define TK_WIDGET_VTABLE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @method widget_invalidate_default
 * awtk默认的invalidate接口。
 * 
 * @annotation ["global"]
 *
 * @param {widget_t*} widget 控件。
 * @param {rect_t*} r 无效化区域。
 *
 * @return {ret_t} 返回。
 */
ret_t widget_invalidate_default(widget_t* widget, rect_t* r);
/**
 * @method widget_on_event_default
 * @annotation ["global"]
 */
ret_t widget_on_event_default(widget_t* widget, event_t* e);
/**
 * @method widget_on_paint_self_default
 * @annotation ["global"]
 */
ret_t widget_on_paint_self_default(widget_t* widget, canvas_t* c);
/**
 * @method widget_on_paint_children_default
 * @annotation ["global"]
 */
ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c);
/**
 * @method widget_on_keydown_default
 * @annotation ["global"]
 */
ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e);
/**
 * @method widget_on_keyup_default
 * @annotation ["global"]
 */
ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e);
ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_down_default
 * @annotation ["global"]
 */
ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_move_default
 * @annotation ["global"]
 */
ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_pointer_up_default
 * @annotation ["global"]
 */
ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e);
/**
 * @method widget_on_copy_default
 * @annotation ["global"]
 */
ret_t widget_on_copy_default(widget_t* widget, widget_t* other);
/**
 * @method widget_get_prop_default
 * @annotation ["global"]
 */
ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v);
/**
 * @method widget_set_prop_default
 * @annotation ["global"]
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

/**
 * @method widget_on_paint_null
 * 
 * @annotation ["global"]
 *
 */
ret_t widget_on_paint_null(widget_t* widget, canvas_t* c);

END_C_DECLS

#endif /*TK_WIDGET_VTABLE_H*/
