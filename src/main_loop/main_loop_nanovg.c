/**
 * file:   main_loop_nanovg.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  nanovg implemented main_loop interface
 *
 * copyright (c) 2018 - 2018  li xianjing <xianjimli@hotmail.com>
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-04-11 li xianjing <xianjimli@hotmail.com> created
 *
 */

#define GL_GLEXT_PROTOTYPES

#include "main_loop/main_loop_nanovg.h"
#include "base/font_manager.h"
#include "base/window_manager.h"
#include "lcd/lcd_nanovg.h"
#include "base/timer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

typedef struct _main_loop_nanovg_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;
  uint16_t pressed : 1;
  SDL_Window* sdl_window;
  SDL_GLContext gl_context;
} main_loop_nanovg_t;

static ret_t main_loop_nanovg_dispatch_key_event(main_loop_nanovg_t* loop, SDL_Event* sdl_event) {
  key_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->wm;

  event.key = sdl_event->key.keysym.sym;
  switch (type) {
    case SDL_KEYDOWN: {
      event.e.type = EVT_KEY_DOWN;
      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    case SDL_KEYUP: {
      event.e.type = EVT_KEY_UP;
      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t main_loop_nanovg_dispatch_mouse_event(main_loop_nanovg_t* loop, SDL_Event* sdl_event) {
  pointer_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->wm;

  switch (type) {
    case SDL_MOUSEBUTTONDOWN: {
      loop->pressed = 1;
      event.e.type = EVT_POINTER_DOWN;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      event.e.type = EVT_POINTER_UP;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      loop->pressed = 0;
      break;
    }
    case SDL_MOUSEMOTION: {
      event.e.type = EVT_POINTER_MOVE;
      event.x = sdl_event->motion.x;
      event.y = sdl_event->motion.y;
      event.button = 0;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t main_loop_nanovg_dispatch(main_loop_nanovg_t* loop) {
  SDL_Event event;
  ret_t ret = RET_OK;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        ret = main_loop_nanovg_dispatch_key_event(loop, &event);
        break;
      }
      case SDL_MOUSEMOTION:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        ret = main_loop_nanovg_dispatch_mouse_event(loop, &event);
        break;
      }
      case SDL_QUIT: {
        ret = main_loop_quit(&(loop->base));
        break;
      }
    }
  }

  return ret;
}

static ret_t main_loop_nanovg_paint(main_loop_nanovg_t* loop) {
  ret_t ret = window_manager_paint(loop->wm, &(loop->canvas));

  return ret;
}

static ret_t main_loop_nanovg_run(main_loop_t* l) {
  main_loop_nanovg_t* loop = (main_loop_nanovg_t*)l;

  while (l->running) {
    timer_check();
    main_loop_nanovg_dispatch(loop);
    main_loop_nanovg_paint(loop);
    SDL_Delay(30);
  }

  return RET_OK;
}

static ret_t main_loop_nanovg_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_nanovg_destroy(main_loop_t* l) {
  const char* errMsg = SDL_GetError();
  main_loop_nanovg_t* loop = (main_loop_nanovg_t*)l;

  if (errMsg && strlen(errMsg)) {
    SDL_Log("Error : %s\n", errMsg);
  }

  if (loop->sdl_window) {
    SDL_DestroyWindow(loop->sdl_window);
    SDL_GL_DeleteContext(loop->gl_context);
    loop->sdl_window = NULL;
  }

  SDL_Quit();

  return RET_OK;
}

static ret_t main_loop_nanovg_create_window(main_loop_nanovg_t* l, font_manager_t* fm, int w,
                                            int h) {
  int32_t x = SDL_WINDOWPOS_UNDEFINED;
  int32_t y = SDL_WINDOWPOS_UNDEFINED;
  uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

  l->w = w;
  l->h = h;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return RET_FAIL;
  }

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  l->sdl_window = SDL_CreateWindow("LFTK Simulator", x, y, w, h, flags);
  return_value_if_fail(l->sdl_window != NULL, RET_FAIL);

  l->gl_context = SDL_GL_CreateContext(l->sdl_window);
  SDL_GL_MakeCurrent(l->sdl_window, l->gl_context);
  SDL_GL_SetSwapInterval(0);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  canvas_init(&(l->canvas), lcd_nanovg_init(l->sdl_window), fm);

  return RET_OK;
}

static main_loop_nanovg_t loop;
main_loop_t* main_loop_init(int w, int h) {
  widget_t* wm = default_wm();
  font_manager_t* fm = default_fm();
  main_loop_t* base = &(loop.base);

  memset(&loop, 0x00, sizeof(loop));

  base->run = main_loop_nanovg_run;
  base->quit = main_loop_nanovg_quit;
  base->destroy = main_loop_nanovg_destroy;

  loop.wm = wm;
  window_manager_resize(wm, w, h);
  main_loop_nanovg_create_window(&loop, fm, w, h);
  main_loop_set_default(base);

  log_debug("%s:%s\n", __FILE__, __func__);

  return base;
}
