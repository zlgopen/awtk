/**
 * File:   demo2_app.c
 * Author: AWTK Develop Team
 * Brief:  demo paint
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/timer.h"
#include "widgets/window.h"
#include "base/image_manager.h"
#include "canvas_widget/canvas_widget.h"

static ret_t on_paint_rect(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, color_init(0, 0, 0xff, 0));
  canvas_fill_rect(c, 10, 10, 100, 100);

  return RET_OK;
}

static ret_t on_paint_patch9(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;

  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 200, 30);
  canvas_draw_image_patch9(c, &img, &r);

  r = rect_init(10, 70, 200, 60);
  canvas_draw_image_patch9(c, &img, &r);

  r = rect_init(10, 150, 20, 60);
  canvas_draw_image_patch9(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_default(void* ctx, event_t* e) {
  rect_t s;
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "earth", &img);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(10, 220, img.w / 2, img.h / 2);
  canvas_draw_image(c, &img, &s, &r);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(60, 220, img.w, img.h);
  canvas_draw_image(c, &img, &s, &r);

  s = rect_init(0, 0, img.w, img.h);
  r = rect_init(160, 220, img.w * 2, img.h * 2);
  canvas_draw_image(c, &img, &s, &r);

  return RET_OK;
}

static ret_t on_paint_line(void* ctx, event_t* e) {
  color_t fill_color = color_init(0xe0, 0xe0, 0xe0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, fill_color);
  canvas_set_stroke_color(c, stroke_color);

  canvas_set_stroke_color(c, stroke_color);
  canvas_draw_line(c, 10, 10, 100, 300);

  return RET_OK;
}

static ret_t on_paint_points(void* ctx, event_t* e) {
  point_t points[] = {{100, 100}, {100, 101}, {100, 102}, {100, 103}};
  color_t fill_color = color_init(0xe0, 0xe0, 0xe0, 0xff);
  color_t stroke_color = color_init(0x80, 0x80, 0x80, 0xff);
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  canvas_set_fill_color(c, fill_color);

  canvas_set_stroke_color(c, stroke_color);
  canvas_draw_points(c, points, ARRAY_SIZE(points));

  return RET_OK;
}

static ret_t on_paint_patch3_h(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 100, 60);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 80, 10, 60);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 160, 300, 50);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(10, 220, 300, 20);
  canvas_draw_image_patch3_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_patch3_v(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 60, 100);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(80, 10, 60, 10);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(160, 10, 50, 300);
  canvas_draw_image_patch3_x(c, &img, &r);

  r = rect_init(220, 10, 20, 300);
  canvas_draw_image_patch3_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, 10, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  r = rect_init(10, 60, img.w, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  r = rect_init(10, 160, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w, 10);
  canvas_draw_image_repeat_y(c, &img, &r);

  r = rect_init(90, 10, img.w, img.h);
  canvas_draw_image_repeat_y(c, &img, &r);

  r = rect_init(160, 10, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_y(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h / 2);
  canvas_draw_image_repeat(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_repeat(c, &img, &r);

  r = rect_init(100, 100, img.w * 2 + 10, img.h * 2 + 10);
  canvas_draw_image_repeat(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w, img.h / 2);
  canvas_draw_image_scale_w(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  r = rect_init(100, 100, img.w * 2, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_scale_h(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_scale(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_scale(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_scale(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_center(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_get_bitmap(image_manager(), "btn_blue_n", &img);

  r = rect_init(10, 10, img.w / 2, img.h);
  canvas_draw_image_center(c, &img, &r);

  r = rect_init(50, 50, img.w, img.h);
  canvas_draw_image_center(c, &img, &r);

  r = rect_init(100, 100, img.w, img.h * 2);
  canvas_draw_image_center(c, &img, &r);

  return RET_OK;
}

#include "vg_common.inc"

static ret_t on_paint_vg(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  vgcanvas_save(vg);
  vgcanvas_set_line_width(vg, 1);
  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));

  draw_basic_shapes(vg, FALSE);
  vgcanvas_translate(vg, 0, 50);
  draw_basic_shapes(vg, TRUE);
  vgcanvas_translate(vg, 0, 50);
  stroke_lines(vg);
  vgcanvas_translate(vg, 0, 50);
  draw_image(vg);

  vgcanvas_translate(vg, 50, 100);
  draw_matrix(vg);
  vgcanvas_translate(vg, 0, 100);

  draw_text(vg);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_paint_vg_simple(void* ctx, event_t* e) {
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  image_manager_get_bitmap(image_manager(), "rgba", &img);

  canvas_set_fill_color(c, color_init(0x11, 0x22, 0x33, 0xff));
  canvas_fill_rect(c, 0, 0, 10, 10);

  rect_t s = rect_init(0, 0, img.w, img.h);
  rect_t r = rect_init(10, 10, img.w, img.h);
  canvas_draw_image(c, &img, &s, &r);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, 0, 100);

  vgcanvas_set_line_width(vg, 1);
  vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));
  vgcanvas_rect(vg, 5, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_set_fill_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_rect(vg, 110, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_set_fill_color(vg, color_init(0, 0, 0xff, 0xff));
  vgcanvas_rect(vg, 215, 5, 100, 100);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, 0, 105);
  vgcanvas_set_stroke_color(vg, color_init(0xff, 0, 0, 0xff));
  vgcanvas_rect(vg, 5, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_rect(vg, 110, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, color_init(0, 0, 0xff, 0xff));
  vgcanvas_rect(vg, 215, 5, 100, 100);
  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 0, 128);
  image_manager_get_bitmap(image_manager(), "rgb", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);

  image_manager_get_bitmap(image_manager(), "rgba", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 100, 0, img.w, img.h);

  image_manager_get_bitmap(image_manager(), "switch", &img);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 200, 0, img.w, img.h);

  vgcanvas_translate(vg, 0, 60);
  vgcanvas_rounded_rect(vg, 0, 0, img.w, img.h, 5);
  vgcanvas_paint(vg, FALSE, &img);

  vg = canvas_get_vgcanvas(c);
  vgcanvas_translate(vg, 160, 0);
  vgcanvas_rounded_rect(vg, 0, 0, img.w, img.h, 5);
  vgcanvas_paint(vg, FALSE, &img);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_timer(const timer_info_t* timer) {
  widget_t* widget = WIDGET(timer->ctx);

  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = canvas_widget_create(win, 0, 0, win->w, win->h);

  // widget_on(canvas, EVT_PAINT, on_paint_vg_simple, NULL);
  widget_on(canvas, EVT_PAINT, on_paint_vg, NULL);

  timer_add(on_timer, win, 500);

  return RET_OK;
}
