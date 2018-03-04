/**
 * File:   window.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window
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

#include "base/window.h"
#include "base/mem.h"
#include "base/window_manager.h"

static ret_t window_on_paint_self(widget_t* widget, canvas_t* c) {
  style_t* style = &(widget->style);
  color_t color = color_init(0xe9, 0xe9, 0xe9, 0xff);

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_BG_COLOR, color));
  canvas_fill_rect(c, widget->x, widget->y, widget->w, widget->h);

  return RET_OK;
}

static const widget_vtable_t s_window_vtable = {.on_paint_self = window_on_paint_self};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  window_t* win = MEM_ZALLOC(window_t);
  return_value_if_fail(win != NULL, NULL);

  widget = WIDGETP(win);
  widget_init(widget, NULL, WIDGET_NORMAL_WINDOW);
  widget->vt = &s_window_vtable;

  if (parent == NULL) {
    parent = default_wm();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  return widget;
}
