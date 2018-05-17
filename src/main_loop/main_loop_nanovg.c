/**
 * File:   main_loop_nanovg.c
 * Author: AWTK Develop Team
 * Brief:  nanovg implemented main_loop interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "glad/glad.h"
#include "lcd/lcd_nanovg.h"
#include "main_loop/main_loop_nanovg.h"
#include "main_loop/main_loop_simple.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#define LOOP_SDL_WINDOW(loop) ((SDL_Window*)(((main_loop_simple_t*)loop)->user1))
#define LOOP_SDL_GLCONTEXT(loop) ((SDL_Window*)(((main_loop_simple_t*)loop)->user2))
#define LOOP_SDL_WINDOW_SET(loop, v) (((main_loop_simple_t*)loop)->user1 = v)
#define LOOP_SDL_GLCONTEXT_SET(loop, v) (((main_loop_simple_t*)loop)->user2 = v)

#include "sdl2_common.inc"

static ret_t main_loop_nanovg_destroy(main_loop_t* l) {
  const char* errMsg = SDL_GetError();
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  if (errMsg && strlen(errMsg)) {
    SDL_Log("Error : %s\n", errMsg);
  }

  if (LOOP_SDL_WINDOW(loop)) {
    SDL_DestroyWindow(LOOP_SDL_WINDOW(loop));
    SDL_GL_DeleteContext(LOOP_SDL_GLCONTEXT(loop));
  }

  SDL_Quit();
  main_loop_simple_reset(loop);

  return RET_OK;
}

static ret_t main_loop_nanovg_create_window(main_loop_simple_t* l) {
  int32_t x = SDL_WINDOWPOS_UNDEFINED;
  int32_t y = SDL_WINDOWPOS_UNDEFINED;
  uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return RET_FAIL;
  }

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

#ifdef WITH_GL2
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(WITH_GL3)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

  LOOP_SDL_WINDOW_SET(l,  SDL_CreateWindow("AWTK Simulator", x, y, l->w, l->h, flags));
  return_value_if_fail(LOOP_SDL_WINDOW(l) != NULL, RET_FAIL);

  LOOP_SDL_GLCONTEXT_SET(l, SDL_GL_CreateContext(LOOP_SDL_WINDOW(l)));
  SDL_GL_MakeCurrent(LOOP_SDL_WINDOW(l), LOOP_SDL_GLCONTEXT(l));
  SDL_GL_SetSwapInterval(1);

  gladLoadGL();
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  canvas_init(&(l->canvas), lcd_nanovg_init(LOOP_SDL_WINDOW(l)), font_manager());

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = main_loop_simple_init(w, h);

  loop->base.destroy = main_loop_nanovg_destroy;
  loop->dispatch_input = main_loop_sdl2_dispatch;

  main_loop_nanovg_create_window(loop);

  return (main_loop_t*)loop;
}

