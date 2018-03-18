/**
 * File:   demo2_app.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  demo paint
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
 * 2018-03-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window.h"
#include "base/group_box.h"

static ret_t on_paint(void* ctx, event_t* e) {
  rect_t r;
  color_t fill_color = color_init(0xff, 0, 0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;

  canvas_set_fill_color(c, fill_color);

  canvas_set_stroke_color(c, stroke_color);
  canvas_draw_line(c, 10, 10, 100, 300);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = group_box_create(win, 0, 0, win->w, win->h);

  widget_on(canvas, EVT_PAINT, on_paint, NULL);

  return RET_OK;
}
