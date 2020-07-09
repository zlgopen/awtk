/**
 * File:   native_window_fb_gl.c
 * Author: AWTK Develop Team
 * Brief:  native window for egl
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
 * 2019-10-31 Lou ZhiMing <luozhiming@zlg.com> created
 *
 */

#ifdef WITH_NANOVG_GL
#ifndef WITHOUT_GLAD
#include "glad/glad.h"
#define loadGL gladLoadGL
#else
#define loadGL()
#endif /*WITHOUT_GLAD*/
#endif /*WITH_NANOVG_GL*/

#include "base/widget.h"
#include "lcd/lcd_nanovg.h"
#include "base/widget_consts.h"
#include "native_window/native_window_fb_gl.h"

typedef struct _native_window_fb_gl_t {
  native_window_t native_window;
  uint32_t w;
  uint32_t h;
  float_t ratio;
  native_window_swap_buffer_t swap_buffer;
  native_window_gl_make_current_t make_current;
  canvas_t canvas;
} native_window_fb_gl_t;

static native_window_t* s_shared_win = NULL;

#define NATIVE_WINDOW_FB_GL(win) ((native_window_fb_gl_t*)(win))

ret_t native_window_fb_gl_set_swap_buffer_func(native_window_t* win,
                                               native_window_swap_buffer_t swap_buffer) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  return_value_if_fail(fb_gl != NULL && swap_buffer != NULL, RET_BAD_PARAMS);
  fb_gl->swap_buffer = swap_buffer;
  return RET_OK;
}

ret_t native_window_fb_gl_set_make_current_func(native_window_t* win,
                                                native_window_gl_make_current_t make_current) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  return_value_if_fail(fb_gl != NULL && make_current != NULL, RET_BAD_PARAMS);
  fb_gl->make_current = make_current;
  return RET_OK;
}

static ret_t native_window_fb_gl_move(native_window_t* win, xy_t x, xy_t y) {
  return RET_OK;
}

static ret_t native_window_fb_gl_resize(native_window_t* win, wh_t w, wh_t h) {
  win->rect.w = w;
  win->rect.h = h;
  return RET_OK;
}

static canvas_t* native_window_fb_gl_get_canvas(native_window_t* win) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);

  return &(fb_gl->canvas);
}

static ret_t native_window_fb_gl_get_info(native_window_t* win, native_window_info_t* info) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);

  info->x = 0;
  info->y = 0;
  info->ratio = fb_gl->ratio;
  info->w = fb_gl->w;
  info->h = fb_gl->h;

  //  log_debug("ratio=%f %d %d\n", info->ratio, info->w, info->h);

  return RET_OK;
}

static ret_t native_window_fb_gl_swap_buffer(native_window_t* win) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  if (fb_gl != NULL && fb_gl->swap_buffer != NULL) {
    return fb_gl->swap_buffer(win);
  }
  return RET_OK;
}

static ret_t native_window_sdl_gl_make_current(native_window_t* win) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  if (fb_gl != NULL && fb_gl->make_current != NULL) {
    return fb_gl->make_current(win);
  }
  return RET_OK;
}

static const native_window_vtable_t s_native_window_vtable = {
    .type = "native_window_fb_gl",
    .move = native_window_fb_gl_move,
    .get_info = native_window_fb_gl_get_info,
    .resize = native_window_fb_gl_resize,
    .swap_buffer = native_window_fb_gl_swap_buffer,
    .gl_make_current = native_window_sdl_gl_make_current,
    .get_canvas = native_window_fb_gl_get_canvas};

static ret_t native_window_fb_gl_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_fb_gl_get_prop(object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_fb_gl_on_destroy(object_t* obj) {
  native_window_fb_gl_t* raw = NATIVE_WINDOW_FB_GL(obj);
  lcd_t* lcd = raw->canvas.lcd;

  canvas_reset(&(raw->canvas));
  lcd_destroy(lcd);

  return RET_OK;
}

static const object_vtable_t s_native_window_fb_gl_vtable = {
    .type = "native_window_fb_gl",
    .desc = "native_window_fb_gl",
    .size = sizeof(native_window_fb_gl_t),
    .get_prop = native_window_fb_gl_get_prop,
    .set_prop = native_window_fb_gl_set_prop,
    .on_destroy = native_window_fb_gl_on_destroy};

static native_window_t* native_window_create_internal(uint32_t w, uint32_t h, float_t ratio) {
  lcd_t* lcd = NULL;
  object_t* obj = object_create(&s_native_window_fb_gl_vtable);
  native_window_t* win = NATIVE_WINDOW(obj);
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  return_value_if_fail(fb_gl != NULL, NULL);
  fb_gl->w = w;
  fb_gl->h = h;
  fb_gl->ratio = ratio;

  win->shared = TRUE;
  win->vt = &s_native_window_vtable;
  win->rect = rect_init(0, 0, w, h);

  loadGL();

  canvas_t* c = &(fb_gl->canvas);
  lcd = lcd_nanovg_init(win);
  canvas_init(c, lcd, font_manager());

  return win;
}

native_window_t* native_window_create(widget_t* widget) {
  native_window_t* nw = s_shared_win;
  return_value_if_fail(nw != NULL, NULL);

  widget_set_prop_pointer(widget, WIDGET_PROP_NATIVE_WINDOW, nw);

  return nw;
}

native_window_t* native_window_fb_gl_init(uint32_t w, uint32_t h, float_t ratio) {
  s_shared_win = native_window_create_internal(w, h, ratio);

  return s_shared_win;
}

ret_t native_window_fb_gl_deinit(void) {
  if (s_shared_win != NULL) {
    object_unref(OBJECT(s_shared_win));
    s_shared_win = NULL;
  }

  return RET_OK;
}
