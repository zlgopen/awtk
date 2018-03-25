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

static ret_t on_paint_9patch(void* ctx, event_t* e) {
  rect_t s;
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

static ret_t on_paint_3patch_h(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 100, 60);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 10, 80, 10, 60);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 10, 160, 300, 50);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 10, 220, 300, 20);
  canvas_draw_image_3patch_h(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_3patch_v(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 60, 100);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 80, 10, 60, 10);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 160, 10, 50, 300);
  canvas_draw_image_3patch_h(c, &img, &r);

  rect_init(r, 220, 10, 20, 300);
  canvas_draw_image_3patch_h(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_tile_h(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, 10, img.h);
  canvas_draw_image_tile_h(c, &img, &r);

  rect_init(r, 10, 60, img.w, img.h);
  canvas_draw_image_tile_h(c, &img, &r);

  rect_init(r, 10, 160, img.w * 2, img.h * 2);
  canvas_draw_image_tile_h(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_tile_v(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w, 10);
  canvas_draw_image_tile_v(c, &img, &r);

  rect_init(r, 90, 10, img.w, img.h);
  canvas_draw_image_tile_v(c, &img, &r);

  rect_init(r, 160, 10, img.w * 2, img.h * 2);
  canvas_draw_image_tile_v(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_tile(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h / 2);
  canvas_draw_image_tile(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_tile(c, &img, &r);

  rect_init(r, 100, 100, img.w * 2 + 10, img.h * 2 + 10);
  canvas_draw_image_tile(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_x(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w, img.h / 2);
  canvas_draw_image_scale_x(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_scale_x(c, &img, &r);

  rect_init(r, 100, 100, img.w * 2, img.h);
  canvas_draw_image_scale_x(c, &img, &r);

  return RET_OK;
}

static ret_t on_paint_scale_y(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  image_manager_load(default_im(), "btn_blue_n", &img);

  rect_init(r, 10, 10, img.w / 2, img.h);
  canvas_draw_image_scale_y(c, &img, &r);

  rect_init(r, 50, 50, img.w, img.h);
  canvas_draw_image_scale_y(c, &img, &r);

  rect_init(r, 100, 100, img.w, img.h * 2);
  canvas_draw_image_scale_y(c, &img, &r);

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

static ret_t on_paint_vg(void* ctx, event_t* e) {
  rect_t r;
  bitmap_t img;
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  vgcanvas_set_fill_color(vg, color_init(0xff, 0xff, 0, 0xff));
  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_clear_rect(vg, 0, 0, vg->w, vg->h, color_init(0xf0, 0xf0, 0xf0, 0xff));

  vgcanvas_set_line_width(vg, 2);
  vgcanvas_round_rect(vg, 10, 10, 100, 20, 5);
  vgcanvas_stroke(vg);

  vgcanvas_rect(vg, 128, 10, 100, 20);
  vgcanvas_fill(vg);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = group_box_create(win, 0, 0, win->w, win->h);

  widget_on(canvas, EVT_PAINT, on_paint_vg, NULL);

  return RET_OK;
}
