/**
 * File:   demo_vg_text_app.c
 * Author: AWTK Develop Team
 * Brief:  demo for vgcanvas text rendering
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

#include "base/window.h"
#include "base/vgcanvas.h"
#include "canvas_widget/canvas_widget.h"

static ret_t on_paint_vg_text(void* ctx, event_t* e) {
  paint_event_t* evt = (paint_event_t*)e;
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t bg = color_init(0xff, 0xf8, 0xf8, 0xf8);
  color_t black = color_init(0x20, 0x20, 0x20, 0xff);
  color_t red = color_init(0xff, 0x40, 0x40, 0xff);
  color_t green = color_init(0x20, 0xb0, 0x20, 0xff);
  color_t blue = color_init(0x30, 0x60, 0xff, 0xff);
  float_t x = 20;
  float_t y = 20;

  (void)ctx;

  vgcanvas_clear_rect(vg, 0, 0, vg->w, vg->h, bg);
  vgcanvas_set_font_size(vg, 26);
  vgcanvas_set_font(vg, NULL);

  vgcanvas_set_fill_color(vg, black);
  vgcanvas_set_text_align(vg, "left");
  vgcanvas_set_text_baseline(vg, "top");
  vgcanvas_fill_text(vg, "VG Text Demo", x, y, 1000);
  y += 50;

  vgcanvas_set_fill_color(vg, red);
  vgcanvas_fill_text(vg, "left/top", x, y, 1000);
  vgcanvas_set_fill_color(vg, green);
  vgcanvas_set_text_align(vg, "center");
  vgcanvas_fill_text(vg, "center/top", x + 260, y, 1000);
  vgcanvas_set_fill_color(vg, blue);
  vgcanvas_set_text_align(vg, "right");
  vgcanvas_fill_text(vg, "right/top", x + 520, y, 1000);
  y += 70;

  vgcanvas_set_fill_color(vg, black);
  vgcanvas_set_text_align(vg, "left");
  vgcanvas_set_text_baseline(vg, "middle");
  vgcanvas_fill_text(vg, "baseline: middle", x, y, 1000);
  vgcanvas_set_text_baseline(vg, "bottom");
  vgcanvas_fill_text(vg, "baseline: bottom", x + 280, y, 1000);
  y += 80;

  vgcanvas_set_text_baseline(vg, "top");
  vgcanvas_set_fill_color(vg, red);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, x + 120, y + 60);
  vgcanvas_rotate(vg, TK_D2R(-20));
  vgcanvas_fill_text(vg, "rotate -20deg", 0, 0, 1000);
  vgcanvas_restore(vg);

  vgcanvas_set_fill_color(vg, blue);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, x + 360, y + 40);
  vgcanvas_scale(vg, 1.6f, 1.2f);
  vgcanvas_fill_text(vg, "scale 1.6x,1.2x", 0, 0, 1000);
  vgcanvas_restore(vg);
  y += 130;

  vgcanvas_set_fill_color(vg, green);
  vgcanvas_save(vg);
  vgcanvas_clip_rect(vg, x, y, 240, 40);
  vgcanvas_fill_text(vg, "clip_rect -> this long text will be clipped here", x, y, 1000);
  vgcanvas_restore(vg);

  vgcanvas_set_fill_color(vg, black);
  vgcanvas_set_global_alpha(vg, 0.35f);
  vgcanvas_fill_text(vg, "global alpha 0.35", x + 280, y, 1000);
  vgcanvas_set_global_alpha(vg, 1.0f);

  return RET_OK;
}

ret_t application_init(void) {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = canvas_widget_create(win, 0, 0, win->w, win->h);

  widget_on(canvas, EVT_PAINT, on_paint_vg_text, NULL);

  return RET_OK;
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
