/**
 * File:   window_manager.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_WINDOW_MANAGER_H
#define LFTK_WINDOW_MANAGER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class window_manager_t
 * @parent widget_t
 * 窗口管理器。
 */
typedef struct _window_manager_t {
  widget_t widget;

  array_t graps; 
  rect_t dirty_rect;
  rect_t last_dirty_rect;
}window_manager_t;

widget_t* default_wm(void);

widget_t* window_manager_init(window_manager_t* wm);

ret_t window_manager_resize(widget_t* wm, wh_t w, wh_t h);
ret_t window_manager_add_child(widget_t* wm, widget_t* window);
ret_t window_manager_remove_child(widget_t* wm, widget_t* window);
ret_t window_manager_paint(widget_t* wm, canvas_t* c);

#define WINDOW_MANAGER(widget) (window_manager_t*)(widget)

END_C_DECLS

#endif/*LFTK_WINDOW_MANAGER_H*/

