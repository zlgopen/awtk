/**
 * File:   demo_rounded_rect.c
 * Author: AWTK Develop Team
 * Brief:  rounded rect demo
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-06-30 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */
#include "awtk.h"

static uint32_t s_border_width = 30;
static uint32_t s_radius_tl = 20;
static uint32_t s_radius_tr = 0;
static uint32_t s_radius_bl = 0;
static uint32_t s_radius_br = 20;

static darray_t s_rounded_rects;

static ret_t on_value_changed(void* ctx, event_t* e) {
  uint32_t i = 0;
  widget_t* edit = WIDGET(e->target);
  uint32_t* value = (uint32_t*)ctx;
  *value = widget_get_value_int(edit);

  for (i = 0; i < s_rounded_rects.size; i++) {
    widget_t* view = (widget_t*)darray_get(&s_rounded_rects, i);
    widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_TOP_LEFT, s_radius_tl);
    widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_TOP_RIGHT, s_radius_tr);
    widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_BOTTOM_LEFT, s_radius_bl);
    widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT, s_radius_br);
    widget_set_style_int(view, STYLE_ID_BORDER_WIDTH, s_border_width);
  }

  return RET_OK;
}

ret_t create_input_widget(widget_t* win, xy_t y) {
  xy_t x = 0;
  widget_t* radius_tl_edit = NULL;
  widget_t* radius_tl_label = NULL;
  widget_t* radius_tr_edit = NULL;
  widget_t* radius_tr_label = NULL;
  widget_t* radius_bl_edit = NULL;
  widget_t* radius_bl_label = NULL;
  widget_t* radius_br_edit = NULL;
  widget_t* radius_br_label = NULL;
  widget_t* border_width_edit = NULL;
  widget_t* border_width_label = NULL;

  radius_tl_label = label_create(win, x, y, 70, 30);
  widget_set_text_utf8(radius_tl_label, "radius_tl:");
  x += radius_tl_label->w;
  radius_tl_edit = edit_create(win, x, y, 70, 30);
  edit_set_keyboard(radius_tl_edit, "");
  widget_set_value_int(radius_tl_edit, s_radius_tl);

  x += radius_tl_edit->w;
  radius_tr_label = label_create(win, radius_tl_edit->x + radius_tl_edit->w, y, 70, 30);
  widget_set_text_utf8(radius_tr_label, "radius_tr:");
  x += radius_tr_label->w;
  radius_tr_edit = edit_create(win, x, y, 70, 30);
  edit_set_keyboard(radius_tr_edit, "");
  widget_set_value_int(radius_tr_edit, s_radius_tr);

  x += radius_tr_edit->w;
  radius_bl_label = label_create(win, radius_tr_edit->x + radius_tr_edit->w, y, 70, 30);
  widget_set_text_utf8(radius_bl_label, "radius_bl:");
  x += radius_bl_label->w;
  radius_bl_edit = edit_create(win, x, y, 70, 30);
  edit_set_keyboard(radius_bl_edit, "");
  widget_set_value_int(radius_bl_edit, s_radius_bl);

  x += radius_bl_edit->w;
  radius_br_label = label_create(win, radius_bl_edit->x + radius_bl_edit->w, y, 70, 30);
  widget_set_text_utf8(radius_br_label, "radius_br:");
  x += radius_br_label->w;
  radius_br_edit = edit_create(win, x, y, 70, 30);
  edit_set_keyboard(radius_br_edit, "");
  widget_set_value_int(radius_br_edit, s_radius_br);

  x += radius_br_edit->w;
  border_width_label = label_create(win, radius_br_edit->x + radius_br_edit->w, y, 100, 30);
  widget_set_text_utf8(border_width_label, "border_width:");
  x += border_width_label->w;
  border_width_edit = edit_create(win, x, y, 70, 30);
  edit_set_keyboard(border_width_edit, "");
  widget_set_value_int(border_width_edit, s_border_width);

  widget_on(radius_tl_edit, EVT_VALUE_CHANGED, on_value_changed, &s_radius_tl);
  widget_on(radius_tr_edit, EVT_VALUE_CHANGED, on_value_changed, &s_radius_tr);
  widget_on(radius_bl_edit, EVT_VALUE_CHANGED, on_value_changed, &s_radius_bl);
  widget_on(radius_br_edit, EVT_VALUE_CHANGED, on_value_changed, &s_radius_br);
  widget_on(border_width_edit, EVT_VALUE_CHANGED, on_value_changed, &s_border_width);

  return RET_OK;
}

widget_t* create_test_view(widget_t* win, const rect_t* r, int32_t border) {
  widget_t* view = view_create(win, r->x, r->y, r->w, r->h);
  widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_TOP_LEFT, s_radius_tl);
  widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_TOP_RIGHT, s_radius_tr);
  widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_BOTTOM_LEFT, s_radius_bl);
  widget_set_style_int(view, STYLE_ID_ROUND_RADIUS_BOTTOM_RIGHT, s_radius_br);
  widget_set_style_int(view, STYLE_ID_BORDER_WIDTH, s_border_width);
  widget_set_style_int(view, STYLE_ID_BORDER, border);

  widget_set_style_color(view, STYLE_ID_BORDER_COLOR, color_init(0xc5, 0xc5, 0xc5, 0xff).color);
  widget_set_style_color(view, STYLE_ID_BG_COLOR, color_init(0xff, 0, 0, 0xff).color);

  darray_push(&s_rounded_rects, view);
  return view;
}

ret_t application_init() {
  uint32_t w = 100;
  uint32_t h = 100;
  rect_t r = rect_init(0, 0, w, h);
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  darray_init(&s_rounded_rects, 16, NULL, NULL);

  create_test_view(win, &r, 0);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_RIGHT | BORDER_TOP);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_RIGHT | BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM);

  r.x = 0;
  r.y += h + 30;

  create_test_view(win, &r, BORDER_LEFT | BORDER_RIGHT);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_TOP);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_LEFT | BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_RIGHT | BORDER_TOP);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_RIGHT | BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_TOP | BORDER_BOTTOM);

  r.x = 0;
  r.y += h + 30;

  create_test_view(win, &r, BORDER_LEFT);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_TOP);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_BOTTOM);
  r.x += w + 30;
  create_test_view(win, &r, BORDER_RIGHT);

  create_input_widget(win, r.y + h + 30);
  return RET_OK;
}

ret_t application_exit() {
  darray_deinit(&s_rounded_rects);
  log_debug("application_exit\n");
  return RET_OK;
}

#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#include "awtk_main.inc"
