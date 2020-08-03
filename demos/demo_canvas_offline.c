/**
 * File:   demo_canvas_offline.c
 * Author: AWTK Develop Team
 * Brief:  demo_canvas_offline
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
 * 2020-07-10 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "awtk.h"
#include "ext_widgets.h"
#include "base/canvas_offline.h"

static canvas_t* canvas_offline = NULL;

#include "vg_common.inc"

static ret_t on_paint_vgcanvas(void* ctx, event_t* e) {
#ifndef AWTK_WEB
  paint_event_t* evt = paint_event_cast(e);
  /* 设置离线的 canvas 为当前绘图的画布 */
  canvas_t* c = canvas_offline;
  /* 获取离线的 canvas 对应的离线 bitmap */
  bitmap_t* offline_bitmap = canvas_offline_get_bitmap(canvas_offline);

  /* 开始离线 canvas 绘图 */
  canvas_offline_begin_draw(canvas_offline);

  /* 
   * 下面绘图的内容是 demoui 中的 vgcanvas 的内容，
   * 只不过这一部分内容绘图到离线的 canvas 中
   */
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  vgcanvas_save(vg);
  vgcanvas_set_fill_color(vg, color_init(0xf0, 0xf0, 0xf0, 0xff));
  vgcanvas_rounded_rect(vg, 0, 0, canvas_offline->lcd->w, canvas_offline->lcd->h, 0);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, c->ox, c->oy);
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

  /* 结束离线 canvas 绘图 */
  canvas_offline_end_draw(canvas_offline);

  /* 把显存中的 bitmap 显示到在线的 canvas 中 */
  rect_t r = rect_init(0, 0, offline_bitmap->w, offline_bitmap->h);
  canvas_draw_image_ex(evt->c, offline_bitmap, IMAGE_DRAW_DEFAULT, &r);
#endif /*AWTK_WEB*/

  return RET_OK;
}

static ret_t on_snapshot(void* ctx, event_t* e) {
  bitmap_t* offline_bitmap = NULL;
#ifndef AWTK_WEB
  /* 开始离线 canvas 绘图 */
  canvas_offline_begin_draw(canvas_offline);
  /* 把整个窗口绘制到离线的 canvas 中 */
  widget_paint(window_manager(), canvas_offline);
  /* 结束离线 canvas 绘图 */
  canvas_offline_end_draw(canvas_offline);

  /* 由于需要把 bitmap 保存为 png 文件，所以需要把显存中的数据回传到内存中 */
  canvas_offline_flush_bitmap(canvas_offline);

  /* 获取离线的 canvas 对应的离线 bitmap */
  offline_bitmap = canvas_offline_get_bitmap(canvas_offline);

  /* 把 bitmap 保存到 png 文件 */
  bitmap_save_png(offline_bitmap, "test.png");

#endif /*AWTK_WEB*/

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas_widget = canvas_widget_create(win, 0, 0, win->w, win->h);
  widget_t* button = button_create(win, 0, 0, 0, 0);
  widget_set_self_layout_params(button, "c", "b", "100", "40");

  /* 创建离线的 canvas */
  canvas_offline = canvas_offline_create(320, 480, BITMAP_FMT_RGBA8888);

  widget_set_style_color(win, "bg_color", 0xff00ff00);

  widget_on(canvas_widget, EVT_PAINT, on_paint_vgcanvas, NULL);
  widget_on(button, EVT_CLICK, on_snapshot, NULL);
  widget_set_text_utf8(button, "snapshot");

  return RET_OK;
}

ret_t application_exit() {
  /* 释放离线 canvas */
  canvas_offline_destroy(canvas_offline);

  log_debug("application_exit\n");
  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"
