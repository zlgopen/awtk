/**
 * File:   main_loop_sdl_fb.c
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented main_loop interface
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "main_loop/main_loop_simple.h"
#include "main_loop/main_loop_sdl_fb.h"
#include "base/window_manager.h"
#include "base/font_manager.h"
#include "lcd/lcd_sdl2.h"
#include "base/idle.h"
#include "base/timer.h"
#include <SDL.h>

#define LOOP_SDL_WINDOW(loop) ((SDL_Window*)(((main_loop_simple_t*)loop)->user1))
#define LOOP_SDL_RENDER(loop) ((SDL_Renderer*)(((main_loop_simple_t*)loop)->user2))
#define LOOP_SDL_WINDOW_SET(loop, v) (((main_loop_simple_t*)loop)->user1 = v)
#define LOOP_SDL_RENDER_SET(loop, v) (((main_loop_simple_t*)loop)->user2 = v)

#include "sdl2_common.inc"

static ret_t main_loop_sdl_fb_destroy(main_loop_t* l) {
  const char* errMsg = SDL_GetError();
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  if (errMsg && strlen(errMsg)) {
    SDL_Log("Error : %s\n", errMsg);
  }

  if (LOOP_SDL_RENDER(loop)) {
    SDL_DestroyRenderer(LOOP_SDL_RENDER(loop));
  }

  if (LOOP_SDL_WINDOW(loop)) {
    SDL_DestroyWindow(LOOP_SDL_WINDOW(loop));
  }

  SDL_Quit();
  main_loop_simple_reset(loop);

  return RET_OK;
}

static ret_t main_loop_sdl_fb_create_window(main_loop_simple_t* l) {
  int win_flags = 0;
  int x = SDL_WINDOWPOS_UNDEFINED;
  int y = SDL_WINDOWPOS_UNDEFINED;
  int flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;

  if (system_info()->app_type == APP_DESKTOP) {
    win_flags |= SDL_WINDOW_RESIZABLE;
  }

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  LOOP_SDL_WINDOW_SET(l, SDL_CreateWindow("AWTK Simulator", x, y, l->w, l->h, win_flags));
  return_value_if_fail(LOOP_SDL_WINDOW(l) != NULL, RET_FAIL);

  LOOP_SDL_RENDER_SET(l, SDL_CreateRenderer(LOOP_SDL_WINDOW(l), -1, flags));
  return_value_if_fail(LOOP_SDL_RENDER(l) != NULL, RET_FAIL);

  l->base.lcd = lcd_sdl2_init(LOOP_SDL_RENDER(l));
  canvas_init(&(l->base.canvas), l->base.lcd, font_manager());

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = main_loop_simple_init(w, h);

  loop->base.destroy = main_loop_sdl_fb_destroy;
  loop->dispatch_input = main_loop_sdl2_dispatch;

  main_loop_sdl_fb_create_window(loop);
  SDL_StopTextInput();

  return (main_loop_t*)loop;
}
