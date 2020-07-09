/**
 * File:   native_window_raw.h
 * Author: AWTK Develop Team
 * Brief:  native window raw
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"
#include "native_window/native_window_raw.h"

typedef struct _native_window_raw_t {
  native_window_t native_window;

  canvas_t canvas;
} native_window_raw_t;

static native_window_t* s_shared_win = NULL;

#define NATIVE_WINDOW_RAW(win) ((native_window_raw_t*)(win))

static ret_t native_window_raw_move(native_window_t* win, xy_t x, xy_t y) {
  return RET_OK;
}

static ret_t native_window_raw_resize(native_window_t* win, wh_t w, wh_t h) {
  win->rect.w = w;
  win->rect.h = h;
  return RET_OK;
}

static canvas_t* native_window_raw_get_canvas(native_window_t* win) {
  native_window_raw_t* raw = NATIVE_WINDOW_RAW(win);

  return &(raw->canvas);
}

static ret_t native_window_raw_get_info(native_window_t* win, native_window_info_t* info) {
  native_window_raw_t* raw = NATIVE_WINDOW_RAW(win);

  info->x = 0;
  info->y = 0;
  info->ratio = raw->canvas.lcd->ratio;
  info->w = lcd_get_width(raw->canvas.lcd);
  info->h = lcd_get_height(raw->canvas.lcd);

  log_debug("ratio=%f %d %d\n", info->ratio, info->w, info->h);

  return RET_OK;
}

static const native_window_vtable_t s_native_window_vtable = {
    .type = "native_window_raw",
    .move = native_window_raw_move,
    .get_info = native_window_raw_get_info,
    .resize = native_window_raw_resize,
    .get_canvas = native_window_raw_get_canvas};

static ret_t native_window_raw_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_raw_get_prop(object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_raw_on_destroy(object_t* obj) {
  native_window_raw_t* raw = NATIVE_WINDOW_RAW(obj);
  lcd_t* lcd = raw->canvas.lcd;

  canvas_reset(&(raw->canvas));
  lcd_destroy(lcd);

  return RET_OK;
}

static const object_vtable_t s_native_window_raw_vtable = {
    .type = "native_window_raw",
    .desc = "native_window_raw",
    .size = sizeof(native_window_raw_t),
    .get_prop = native_window_raw_get_prop,
    .set_prop = native_window_raw_set_prop,
    .on_destroy = native_window_raw_on_destroy};

static native_window_t* native_window_create_internal(lcd_t* lcd) {
  object_t* obj = object_create(&s_native_window_raw_vtable);
  native_window_t* win = NATIVE_WINDOW(obj);
  native_window_raw_t* raw = NATIVE_WINDOW_RAW(win);
  return_value_if_fail(raw != NULL, NULL);

  canvas_t* c = &(raw->canvas);
  canvas_init(c, lcd, font_manager());

  win->shared = TRUE;
  win->vt = &s_native_window_vtable;
  win->rect = rect_init(0, 0, lcd_get_width(lcd), lcd_get_height(lcd));

  return win;
}

native_window_t* native_window_create(widget_t* widget) {
  native_window_t* nw = s_shared_win;
  return_value_if_fail(nw != NULL, NULL);

  widget_set_prop_pointer(widget, WIDGET_PROP_NATIVE_WINDOW, nw);

  return nw;
}

ret_t native_window_raw_init(lcd_t* lcd) {
  s_shared_win = native_window_create_internal(lcd);

  return RET_OK;
}

ret_t native_window_raw_deinit(void) {
  return RET_OK;
}
