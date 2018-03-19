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
#include "base/image_manager.h"

static ret_t on_paint(void* ctx, event_t* e) {
  rect_t r;
  color_t fill_color = color_init(0xff, 0, 0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;

  bitmap_t img;
  image_manager_load(default_im(), "btn_blue_n", &img);

  canvas_set_fill_color(c, fill_color);

  r.x = 10;
  r.y = 10;
  r.w = 200;
  r.h = 30;
  canvas_draw_image_9patch(c, &img, &r);
 
  r.x = 10;
  r.y = 70;
  r.w = 200;
  r.h = 60;
  canvas_draw_image_9patch(c, &img, &r);

  r.x = 10;
  r.y = 150;
  r.w = 20;
  r.h = 60;
  canvas_draw_image_9patch(c, &img, &r);

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
