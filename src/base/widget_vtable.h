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

ret_t widget_invalidate_default(widget_t* widget, rect_t* r);
ret_t widget_on_event_default(widget_t* widget, event_t* e);
ret_t widget_on_paint_self_default(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c);
ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e);
ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e);
ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e);
ret_t widget_on_copy_default(widget_t* widget, widget_t* other);
ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v);
ret_t widget_set_prop_default(widget_t* widget, const char* name, const value_t* v);
widget_t* widget_find_target_default(widget_t* widget, xy_t x, xy_t y);

const widget_vtable_t* widget_vtable_default(void);
ret_t widget_grab_default(widget_t* widget, widget_t* child);
ret_t widget_ungrab_default(widget_t* widget, widget_t* child);
ret_t widget_destroy_default(widget_t* widget);

ret_t widget_on_paint_null(widget_t* widget, canvas_t* c);

END_C_DECLS

#endif /*TK_WIDGET_VTABLE_H*/
