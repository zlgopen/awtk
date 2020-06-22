/**
 * File:   native_window_sdl.h
 * Author: AWTK Develop Team
 * Brief:  native window sdl
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <SDL.h>
#include "base/system_info.h"
#include "base/window_manager.h"

#ifdef WITH_NANOVG_GL
#ifndef WITHOUT_GLAD
#include "glad/glad.h"
#define loadGL gladLoadGL
#else
#define loadGL()
#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define GL_ALPHA_TEST 0x0BC0
#else
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/
#endif /*WITHOUT_GLAD*/

#endif /*WITH_NANOVG_GL*/

#include "lcd/lcd_sdl2.h"
#include "lcd/lcd_nanovg.h"
#include "lcd/lcd_sdl2_mono.h"
#include "base/native_window.h"

typedef struct _native_window_sdl_t {
  native_window_t native_window;

  SDL_GLContext context;
  SDL_Renderer* render;
  SDL_Window* window;
  canvas_t canvas;
  SDL_Cursor* cursor;
  SDL_Surface* cursor_surface;
} native_window_sdl_t;

static native_window_t* s_shared_win = NULL;

#define NATIVE_WINDOW_SDL(win) ((native_window_sdl_t*)(win))

static ret_t native_window_sdl_move(native_window_t* win, xy_t x, xy_t y) {
  int oldx = 0;
  int oldy = 0;
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  win->rect.x = x;
  win->rect.y = y;
  SDL_GetWindowPosition(sdl->window, &oldx, &oldy);
  if (oldx != x || oldy != y) {
    SDL_SetWindowPosition(sdl->window, x, y);
  }

  return RET_OK;
}

static ret_t native_window_sdl_resize(native_window_t* win, wh_t w, wh_t h) {
  native_window_info_t info;
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  native_window_get_info(win, &info);

  win->rect.w = w;
  win->rect.h = h;

#if !defined(ANDROID) && !defined(IOS)
  if (system_info()->lcd_orientation == LCD_ORIENTATION_0 && (w != info.w || h != info.h)) {
    SDL_SetWindowSize(sdl->window, w, h);
  }
#endif /*ANDROID*/

  return RET_OK;
}

static ret_t native_window_sdl_minimize(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  SDL_MinimizeWindow(sdl->window);

  return RET_OK;
}

static ret_t native_window_sdl_maximize(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  SDL_MaximizeWindow(sdl->window);

  return RET_OK;
}

static ret_t native_window_sdl_restore(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  SDL_RestoreWindow(sdl->window);

  return RET_OK;
}

static ret_t native_window_sdl_center(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  SDL_SetWindowPosition(sdl->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  return RET_OK;
}

static ret_t native_window_sdl_show_border(native_window_t* win, bool_t show) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  SDL_SetWindowBordered(sdl->window, show);

  return RET_OK;
}

static ret_t native_window_sdl_set_fullscreen(native_window_t* win, bool_t fullscreen) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  if (fullscreen) {
    SDL_SetWindowFullscreen(sdl->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(sdl->window, 0);
  }

  return RET_OK;
}

static ret_t native_window_sdl_close(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);
  lcd_t* lcd = sdl->canvas.lcd;

  canvas_reset(&(sdl->canvas));
  lcd_destroy(lcd);

  if (sdl->render != NULL) {
    SDL_DestroyRenderer(sdl->render);
  }

  if (sdl->window != NULL) {
    SDL_DestroyWindow(sdl->window);
  }

  if (sdl->context != NULL) {
    SDL_GL_DeleteContext(sdl->context);
  }

  sdl->render = NULL;
  sdl->window = NULL;
  sdl->context = NULL;

  return RET_OK;
}

static canvas_t* native_window_sdl_get_canvas(native_window_t* win) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  return &(sdl->canvas);
}

static ret_t native_window_sdl_gl_make_current(native_window_t* win) {
#ifdef WITH_NANOVG_GL
  int fw = 0;
  int fh = 0;
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);
  SDL_Window* window = sdl->window;

  SDL_GL_MakeCurrent(window, sdl->context);
  SDL_GL_GetDrawableSize(window, &fw, &fh);

  glViewport(0, 0, fw, fh);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif /*WITH_NANOVG_GL*/
  return RET_OK;
}

static ret_t native_window_sdl_swap_buffer(native_window_t* win) {
#ifdef WITH_NANOVG_GL
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);
  SDL_GL_SwapWindow(sdl->window);
#else
#endif /*WITH_NANOVG_GL*/

  return RET_OK;
}

extern ret_t tk_quit();

static ret_t native_window_sdl_preprocess_event(native_window_t* win, event_t* e) {
#if defined(ANDROID)
  if (e->type == EVT_POINTER_DOWN || e->type == EVT_POINTER_MOVE || e->type == EVT_CLICK ||
      e->type == EVT_POINTER_UP || e->type == EVT_CONTEXT_MENU) {
    pointer_event_t* evt = pointer_event_cast(e);
    evt->x /= win->ratio;
    evt->y /= win->ratio;
  } else if (e->type == EVT_KEY_DOWN) {
    key_event_t* evt = key_event_cast(e);
    if (evt->key == TK_KEY_AC_BACK) {
      window_manager_back(window_manager());
      if (widget_count_children(window_manager()) == 0) {
        tk_quit();
      }
    }
  }
#endif /*ANDROID*/

  return RET_OK;
}

static ret_t native_window_sdl_get_info(native_window_t* win, native_window_info_t* info) {
  int ww = 0;
  int wh = 0;
  int fw = 0;
  int fh = 0;
  int x = 0;
  int y = 0;
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);
  SDL_Window* window = sdl->window;

  SDL_GetWindowPosition(window, &x, &y);
  SDL_GetWindowSize(window, &ww, &wh);
  SDL_GL_GetDrawableSize(window, &fw, &fh);

  memset(info, 0x00, sizeof(*info));
  info->x = x;
  info->y = y;

#if defined(ANDROID)
  float dpi = 1;
  SDL_GetDisplayDPI(0, &dpi, NULL, NULL);
  float_t ratio = dpi / 160;

  info->w = ww / ratio;
  info->h = wh / ratio;
  info->ratio = ratio;
#elif defined(IOS)
  info->w = ww;
  info->h = wh;
  info->ratio = (float_t)fw / (float_t)ww;
#else
  info->w = ww;
  info->h = wh;
  info->ratio = (float_t)fw / (float_t)ww;
#endif /**/

  win->rect.x = info->x;
  win->rect.y = info->y;
  win->rect.w = info->w;
  win->rect.h = info->h;
  win->ratio = info->ratio;

  return RET_OK;
}

static ret_t native_window_sdl_cursor_from_bitmap(native_window_t* win, bitmap_t* img) {
  Uint32 depth = 32;
  uint8_t* data = NULL;
  uint32_t w = img->w;
  uint32_t h = img->h;
  uint32_t rmask = 0;
  uint32_t gmask = 0;
  uint32_t bmask = 0;
  uint32_t amask = 0;
  uint32_t pitch = 4 * w;
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  if (img->format == BITMAP_FMT_BGRA8888) {
    bmask = 0x000000ff;
    gmask = 0x0000ff00;
    rmask = 0x00ff0000;
    amask = 0xff000000;
  } else if (img->format == BITMAP_FMT_RGBA8888) {
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  } else {
    /*
     *assert(!"not supported format!");
     */
    return RET_FAIL;
  }

  if (sdl->cursor_surface != NULL) {
    SDL_FreeSurface(sdl->cursor_surface);
    sdl->cursor_surface = NULL;
  }

  data = bitmap_lock_buffer_for_read(img);
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);
  sdl->cursor_surface =
      SDL_CreateRGBSurfaceFrom(data, w, h, depth, pitch, rmask, gmask, bmask, amask);
  bitmap_unlock_buffer(img);
  return_value_if_fail(sdl->cursor_surface != NULL, RET_OOM);

  sdl->cursor = SDL_CreateColorCursor(sdl->cursor_surface, 0, 0);
  SDL_SetCursor(sdl->cursor);

  return RET_OK;
}

static int map_to_sdl_cursor(const char* name) {
  if (tk_str_eq(WIDGET_CURSOR_DEFAULT, name)) {
    return SDL_SYSTEM_CURSOR_ARROW;
  } else if (tk_str_eq(WIDGET_CURSOR_EDIT, name)) {
    return SDL_SYSTEM_CURSOR_IBEAM;
  } else if (tk_str_eq(WIDGET_CURSOR_HAND, name)) {
    return SDL_SYSTEM_CURSOR_HAND;
  } else if (tk_str_eq(WIDGET_CURSOR_WAIT, name)) {
    return SDL_SYSTEM_CURSOR_WAIT;
  } else if (tk_str_eq(WIDGET_CURSOR_CROSS, name)) {
    return SDL_SYSTEM_CURSOR_CROSSHAIR;
  } else if (tk_str_eq(WIDGET_CURSOR_NO, name)) {
    return SDL_SYSTEM_CURSOR_NO;
  } else if (tk_str_eq(WIDGET_CURSOR_SIZENWSE, name)) {
    return SDL_SYSTEM_CURSOR_SIZENWSE;
  } else if (tk_str_eq(WIDGET_CURSOR_SIZENESW, name)) {
    return SDL_SYSTEM_CURSOR_SIZENESW;
  } else if (tk_str_eq(WIDGET_CURSOR_SIZEWE, name)) {
    return SDL_SYSTEM_CURSOR_SIZEWE;
  } else if (tk_str_eq(WIDGET_CURSOR_SIZENS, name)) {
    return SDL_SYSTEM_CURSOR_SIZENS;
  } else if (tk_str_eq(WIDGET_CURSOR_SIZEALL, name)) {
    return SDL_SYSTEM_CURSOR_SIZEALL;
  }

  return -1;
}

static ret_t native_window_sdl_set_cursor(native_window_t* win, const char* name, bitmap_t* img) {
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);

  if (sdl->cursor != NULL) {
    SDL_FreeCursor(sdl->cursor);
    sdl->cursor = NULL;
  }

  if (system_info()->app_type == APP_DESKTOP) {
    int system_cursor = map_to_sdl_cursor(name);
    if (system_cursor >= 0) {
      sdl->cursor = SDL_CreateSystemCursor((SDL_SystemCursor)system_cursor);
      SDL_SetCursor(sdl->cursor);

      return RET_OK;
    }
  } else if (img != NULL) {
    return native_window_sdl_cursor_from_bitmap(win, img);
  }

  return RET_FAIL;
}

static const native_window_vtable_t s_native_window_vtable = {
    .type = "native_window_sdl",
    .move = native_window_sdl_move,
    .resize = native_window_sdl_resize,
    .minimize = native_window_sdl_minimize,
    .maximize = native_window_sdl_maximize,
    .restore = native_window_sdl_restore,
    .center = native_window_sdl_center,
    .show_border = native_window_sdl_show_border,
    .set_fullscreen = native_window_sdl_set_fullscreen,
    .get_info = native_window_sdl_get_info,
    .preprocess_event = native_window_sdl_preprocess_event,
    .swap_buffer = native_window_sdl_swap_buffer,
    .gl_make_current = native_window_sdl_gl_make_current,
    .set_cursor = native_window_sdl_set_cursor,
    .get_canvas = native_window_sdl_get_canvas};

static ret_t native_window_sdl_set_prop(object_t* obj, const char* name, const value_t* v) {
  native_window_t* win = NATIVE_WINDOW(obj);

  if (tk_str_eq(NATIVE_WINDOW_PROP_SIZE, name)) {
    rect_t* r = (rect_t*)value_pointer(v);
    native_window_sdl_resize(win, r->w, r->h);

    return RET_OK;
  } else if (tk_str_eq(NATIVE_WINDOW_PROP_POSITION, name)) {
    rect_t* r = (rect_t*)value_pointer(v);
    native_window_sdl_move(win, r->x, r->y);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t native_window_sdl_get_prop(object_t* obj, const char* name, value_t* v) {
  native_window_t* win = NATIVE_WINDOW(obj);

  if (tk_str_eq(NATIVE_WINDOW_PROP_SIZE, name) || tk_str_eq(NATIVE_WINDOW_PROP_POSITION, name)) {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    SDL_Window* sdlwin = (SDL_Window*)(win->handle);

    SDL_GetWindowSize(sdlwin, &w, &h);
    SDL_GetWindowPosition(sdlwin, &x, &y);
    win->rect = rect_init(x, y, w, h);
    value_set_pointer(v, &(win->rect));

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t native_window_sdl_on_destroy(object_t* obj) {
  log_debug("Close native window.\n");
  native_window_sdl_close(NATIVE_WINDOW(obj));

  return RET_OK;
}

static ret_t native_window_sdl_exec(object_t* obj, const char* cmd, const char* args) {
#ifdef WITH_NANOVG_GPU
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(obj);
  if (tk_str_eq(cmd, "reset_canvas")) {
    canvas_t* c = &(sdl->canvas);
    vgcanvas_t* vg = canvas_get_vgcanvas(c);

    vgcanvas_reset(vg);

    return RET_OK;
  }
#endif /*WITH_NANOVG_GPU*/

  return RET_NOT_FOUND;
}

static const object_vtable_t s_native_window_sdl_vtable = {
    .type = "native_window_sdl",
    .desc = "native_window_sdl",
    .size = sizeof(native_window_sdl_t),
    .get_prop = native_window_sdl_get_prop,
    .set_prop = native_window_sdl_set_prop,
    .exec = native_window_sdl_exec,
    .on_destroy = native_window_sdl_on_destroy};

static native_window_t* native_window_create_internal(const char* title, uint32_t flags, int32_t x,
                                                      int32_t y, uint32_t w, uint32_t h) {
  lcd_t* lcd = NULL;
  native_window_info_t info;
  object_t* obj = object_create(&s_native_window_sdl_vtable);
  native_window_t* win = NATIVE_WINDOW(obj);
  native_window_sdl_t* sdl = NATIVE_WINDOW_SDL(win);
  canvas_t* c = &(sdl->canvas);

  if (system_info()->app_type == APP_DESKTOP) {
    flags |= SDL_WINDOW_RESIZABLE;
  }

#ifndef WITH_NANOVG_SOFT
  flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
#endif /*WITH_NANOVG_SOFT*/

#ifdef NATIVE_WINDOW_BORDERLESS
  flags |= SDL_WINDOW_BORDERLESS;
#endif /*NATIVE_WINDOW_BORDERLESS*/

  sdl->window = SDL_CreateWindow(title, x, y, w, h, flags);

#ifdef WITH_NANOVG_SOFT
  sdl->render =
      SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
#endif /*WITH_NANOVG_SOFT*/

  win->handle = sdl->window;
  win->vt = &s_native_window_vtable;

#ifdef WITH_NANOVG_GL
  sdl->context = SDL_GL_CreateContext(sdl->window);
  SDL_GL_SetSwapInterval(1);

  loadGL();
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
#endif /*WITH_NANOVG_GL*/

  if (native_window_get_info(win, &info) == RET_OK) {
    w = info.w;
    h = info.h;
  }
  win->rect = rect_init(x, y, w, h);

#ifdef WITH_LCD_MONO
  lcd = lcd_sdl2_mono_init(sdl->render);
#else
#ifdef WITH_NANOVG_SOFT
  lcd = lcd_sdl2_init(sdl->render);
#else
  lcd = lcd_nanovg_init(win);
#endif /*WITH_NANOVG_SOFT*/
#endif /*WITH_LCD_MONO*/

  canvas_init(c, lcd, font_manager());

  return win;
}

native_window_t* native_window_create(widget_t* widget) {
  int32_t x = widget->x;
  int32_t y = widget->y;
  int32_t w = widget->w;
  int32_t h = widget->h;
  native_window_t* nw = NULL;

  if (s_shared_win != NULL) {
    object_ref(OBJECT(s_shared_win));

    nw = s_shared_win;
  } else {
    str_t str;

    str_init(&str, 0);
    str_from_wstr(&str, widget->text.str);
    nw = native_window_create_internal(str.str, 0, x, y, w, h);
    str_reset(&str);
  }

  widget_set_prop_pointer(widget, WIDGET_PROP_NATIVE_WINDOW, nw);

  return nw;
}

#ifdef WITH_NANOVG_GL
static ret_t sdl_init_gl(void) {
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

#ifdef WITH_NANOVG_GL2
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(WITH_NANOVG_GL3)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

  log_debug("Init opengl done.\n");
  return RET_OK;
}
#endif /*WITH_NANOVG_GL*/

ret_t native_window_sdl_init(bool_t shared, uint32_t w, uint32_t h) {
  const char* title = system_info()->app_name;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0) {
    log_debug("Failed to initialize SDL: %s", SDL_GetError());
    exit(0);
    return RET_FAIL;
  }

#ifdef WITH_NANOVG_GL
  sdl_init_gl();
#endif /*WITH_NANOVG_GL*/

  SDL_StopTextInput();
  if (shared) {
    int32_t x = SDL_WINDOWPOS_UNDEFINED;
    int32_t y = SDL_WINDOWPOS_UNDEFINED;
    s_shared_win = native_window_create_internal(title, 0, x, y, w, h);
    s_shared_win->shared = TRUE;
  }

  return RET_OK;
}

ret_t native_window_sdl_deinit(void) {
  if (s_shared_win != NULL) {
    object_unref(OBJECT(s_shared_win));
    s_shared_win = NULL;
  }

  SDL_Quit();

  return RET_OK;
}
