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
#include "base/view.h"
#include "base/image_manager.h"

static ret_t on_paint_9patch(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;

  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 200, 30);
  canvas_draw_image_9patch(c, &img, &r);

  rect_init(r, 10, 70, 200, 60);
  canvas_draw_image_9patch(c, &img, &r);

  rect_init(r, 10, 150, 20, 60);
  canvas_draw_image_9patch(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_default(void* ctx, event_t* e) {
  rect_t s;
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "earth", &img);

  rect_init(s, 0, 0, img.w, img.h);
  rect_init(r, 10, 220, img.w / 2, img.h / 2);
  canvas_draw_image(c, &img, &s, &r);

  rect_init(s, 0, 0, img.w, img.h);
  rect_init(r, 60, 220, img.w, img.h);
  canvas_draw_image(c, &img, &s, &r);

  rect_init(s, 0, 0, img.w, img.h);
  rect_init(r, 160, 220, img.w * 2, img.h * 2);
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

static ret_t on_paint_3patch_h(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 100, 60);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 10, 80, 10, 60);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 10, 160, 300, 50);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 10, 220, 300, 20);
  canvas_draw_image_3patch_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_3patch_v(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 60, 100);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 80, 10, 60, 10);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 160, 10, 50, 300);
  canvas_draw_image_3patch_x(c, &img, &r);

  rect_init(r, 220, 10, 20, 300);
  canvas_draw_image_3patch_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 10, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  rect_init(r, 10, 60, img.w, img.h);
  canvas_draw_image_repeat_x(c, &img, &r);

  rect_init(r, 10, 160, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w, 10);
  canvas_draw_image_repeat_y(c, &img, &r);

  rect_init(r, 90, 10, img.w, img.h);
  canvas_draw_image_repeat_y(c, &img, &r);

  rect_init(r, 160, 10, img.w * 2, img.h * 2);
  canvas_draw_image_repeat_y(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_repeat(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h / 2);
  canvas_draw_image_repeat(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_repeat(c, &img, &r);

  rect_init(r, 100, 100, img.w * 2 + 10, img.h * 2 + 10);
  canvas_draw_image_repeat(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w, img.h / 2);
  canvas_draw_image_scale_w(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  rect_init(r, 100, 100, img.w * 2, img.h);
  canvas_draw_image_scale_w(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_scale_h(c, &img, &r);

  rect_init(r, 100, 100, img.w, img.h * 2);
  canvas_draw_image_scale_h(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h);
  canvas_draw_image_scale(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_scale(c, &img, &r);

  rect_init(r, 100, 100, img.w, img.h * 2);
  canvas_draw_image_scale(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_center(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h);
  canvas_draw_image_center(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_center(c, &img, &r);

  rect_init(r, 100, 100, img.w, img.h * 2);
  canvas_draw_image_center(c, &img, &r);

  return RET_OK;
}

static void draw_basic_shapes(vgcanvas_t* vg, bool_t stroke) {
  vgcanvas_save(vg);

  vgcanvas_translate(vg, 5, 5);
  vgcanvas_rounded_rect(vg, 0, 0, 60, 40, 5);
  if(stroke) vgcanvas_stroke(vg); else vgcanvas_fill(vg);
  
  vgcanvas_translate(vg, 65, 0);
  vgcanvas_rounded_rect(vg, 0, 0, 60, 40, 1);
  if(stroke) vgcanvas_stroke(vg); else vgcanvas_fill(vg);
  
  vgcanvas_translate(vg, 65, 0);
  vgcanvas_ellipse(vg, 30, 20, 30, 20);
  if(stroke) vgcanvas_stroke(vg); else vgcanvas_fill(vg);

  vgcanvas_translate(vg, 65, 0);
  vgcanvas_arc(vg, 20, 20, 20, 0, 2*3.15, FALSE);
  if(stroke) vgcanvas_stroke(vg); else vgcanvas_fill(vg);
  
  vgcanvas_translate(vg, 50, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_line_to(vg, 40, 0);
  vgcanvas_line_to(vg, 40, 40);
  vgcanvas_close_path(vg);
  if(stroke) vgcanvas_stroke(vg); else vgcanvas_fill(vg);

  vgcanvas_restore(vg);
}

static void stroke_lines(vgcanvas_t* vg) {
  vgcanvas_save(vg);

  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_line_to(vg, 40, 40);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_quad_to(vg, 40, 0, 40, 40);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_bezier_to(vg, 20, 0, 20, 40, 40, 40);

  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_arc(vg, 20, 20, 20, 0, 3.14, TRUE);
  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, 40, 0);
  vgcanvas_set_line_width(vg, 5);
  vgcanvas_set_line_cap(vg, "round");
  vgcanvas_arc(vg, 20, 20, 20, 0, 3.14/2, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);
}

static void draw_image(vgcanvas_t* vg) {
  bitmap_t img;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, 10, 0);
  image_manager_load(default_im(), "earth", &img);
  vgcanvas_draw_image(vg, &img, 5, 5, img.w-10, img.h-10, 0, 0, img.w * 2, img.h * 2);
  
  vgcanvas_translate(vg, 100, 0);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);

  image_manager_load(default_im(), "bricks", &img);

  vgcanvas_translate(vg, 50, 0);

  vgcanvas_translate(vg, img.w >> 1, img.h >> 1);
  vgcanvas_rotate(vg, 3.14/4);
  vgcanvas_translate(vg, -img.w >> 1, -img.h >> 1);
  
  vgcanvas_scale(vg, 1.5, 1.5);
  vgcanvas_draw_image(vg, &img, 0, 0, img.w, img.h, 0, 0, img.w, img.h);
  vgcanvas_restore(vg);

  return;
}

static void draw_matrix(vgcanvas_t* vg) {
  float_t w = 50;
  float_t h = 50;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, w/2, h/2);
  vgcanvas_rotate(vg, 3.14/4);
  vgcanvas_translate(vg, -w/2, -h/2);

  vgcanvas_rect(vg, 0, 0, w, h);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);
}

static void draw_text(vgcanvas_t* vg) {
  float_t h = 20;
  const char* text = "Hello World";
  float_t w = 100;

  vgcanvas_set_font_size(vg, 20);
  vgcanvas_set_font(vg, STR_DEFAULT_FONT);
  w = vgcanvas_measure_text(vg, text);
  
  vgcanvas_save(vg);
  vgcanvas_translate(vg, w/2, h/2);
  vgcanvas_rotate(vg, 3.14/4);
  vgcanvas_translate(vg, -w/2, -h/2);


  vgcanvas_fill_text(vg, text, 10, 10, 100);
  log_debug("text=%s w=%f\n", text, w);
  vgcanvas_restore(vg);
}

static ret_t on_paint_vg(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

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

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = view_create(win, 0, 0, win->w, win->h);

  widget_on(canvas, EVT_PAINT, on_paint_vg, NULL);

  return RET_OK;
}
