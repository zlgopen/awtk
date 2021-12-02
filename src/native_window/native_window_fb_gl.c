/**
 * File:   native_window_fb_gl.c
 * Author: AWTK Develop Team
 * Brief:  native window for egl
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef WITH_GPU_GL
#ifndef WITHOUT_GLAD
#include "glad/glad.h"
#define loadGL gladLoadGL
#else
#define loadGL()
#endif /*WITHOUT_GLAD*/
#endif /*WITH_GPU_GL*/

#include "base/widget.h"
#include "lcd/lcd_nanovg.h"
#include "base/widget_consts.h"
#include "base/window_manager.h"
#include "native_window/native_window_fb_gl.h"

typedef struct _native_window_fb_gl_t {
  native_window_t native_window;
  uint32_t w;
  uint32_t h;
  float_t ratio;
  native_window_swap_buffer_t swap_buffer;
  native_window_gl_make_current_t make_current;
  native_window_destroy_t destroy;
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

ret_t native_window_fb_gl_set_destroy_func(native_window_t* win, native_window_destroy_t destroy) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  return_value_if_fail(fb_gl != NULL && destroy != NULL, RET_BAD_PARAMS);
  fb_gl->destroy = destroy;
  return RET_OK;
}

lcd_t* native_window_get_lcd(native_window_t* win) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  return_value_if_fail(fb_gl != NULL, NULL);
  return fb_gl->canvas.lcd;
}

static ret_t native_window_fb_gl_move(native_window_t* win, xy_t x, xy_t y) {
  return RET_OK;
}

static ret_t native_window_fg_gl_on_resized_timer(const timer_info_t* info) {
  widget_t* wm = window_manager();
  native_window_t* win = NATIVE_WINDOW(info->ctx);
  event_t e = event_init(EVT_NATIVE_WINDOW_RESIZED, NULL);
  window_manager_dispatch_native_window_event(window_manager(), &e, win);
  widget_set_need_relayout_children(wm);
  widget_invalidate_force(wm, NULL);

  log_debug("on_resized_idle\n");
  return RET_REMOVE;
}

static ret_t native_window_fb_gl_resize(native_window_t* win, wh_t w, wh_t h) {
  ret_t ret = RET_OK;
  native_window_info_t info;
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  native_window_get_info(win, &info);

  fb_gl->w = win->rect.w = w;
  fb_gl->h = win->rect.h = h;
  if (w != info.w || h != info.h) {
    ret = lcd_resize(fb_gl->canvas.lcd, w, h, 0);
    return_value_if_fail(ret == RET_OK, ret);
    system_info_set_lcd_w(system_info(), w);
    system_info_set_lcd_h(system_info(), h);
    timer_add(native_window_fg_gl_on_resized_timer, win, 100);
  }
  return RET_OK;
}

static ret_t native_window_fb_gl_set_orientation(native_window_t* win,
                                                 lcd_orientation_t old_orientation,
                                                 lcd_orientation_t new_orientation) {
  wh_t w, h;
  ret_t ret = RET_OK;
  native_window_info_t info;
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(win);
  native_window_get_info(win, &info);
  w = info.w;
  h = info.h;
  if (tk_is_swap_size_by_orientation(old_orientation, new_orientation)) {
    w = info.h;
    h = info.w;
  }

  fb_gl->w = win->rect.w = w;
  fb_gl->h = win->rect.h = h;
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
    .set_orientation = native_window_fb_gl_set_orientation,
    .swap_buffer = native_window_fb_gl_swap_buffer,
    .gl_make_current = native_window_sdl_gl_make_current,
    .get_canvas = native_window_fb_gl_get_canvas};

static ret_t native_window_fb_gl_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_fb_gl_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_fb_gl_on_destroy(tk_object_t* obj) {
  native_window_fb_gl_t* fb_gl = NATIVE_WINDOW_FB_GL(obj);
  lcd_t* lcd = fb_gl->canvas.lcd;

  canvas_reset(&(fb_gl->canvas));
  lcd_destroy(lcd);

  if (fb_gl->destroy != NULL) {
    return fb_gl->destroy(NATIVE_WINDOW(fb_gl));
  }

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
  tk_object_t* obj = tk_object_create(&s_native_window_fb_gl_vtable);
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
#if WITH_NANOVG_GPU
  lcd = lcd_nanovg_init(win);
#endif
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
    tk_object_unref(TK_OBJECT(s_shared_win));
    s_shared_win = NULL;
  }

  return RET_OK;
}
