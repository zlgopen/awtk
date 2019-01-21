/**
 * File:   input_method_sdl.c
 * Author: AWTK Develop Team
 * Brief:  input method sdl
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <SDL.h>
#include "tkc/mem.h"
#include "base/input_method.h"
#include "main_loop/main_loop_simple.h"

static SDL_Rect* to_sdl_global(SDL_Rect* r) {
  int x = 0;
  int y = 0;
  main_loop_simple_t* loop = (main_loop_simple_t*)(main_loop());
  SDL_Window* win = (SDL_Window*)(loop->user1);

  SDL_GetWindowPosition(win, &x, &y);
  r->x += x;
  r->y += y;

  return r;
}

static ret_t input_method_sdl_request(input_method_t* im, widget_t* widget) {
  im->widget = widget;

  if (widget != NULL) {
    SDL_Rect r;
    point_t p = {0, 0};
    widget_to_screen(widget, &p);

    r.x = p.x;
    r.y = p.y;
    r.w = widget->w;
    r.h = widget->h;

    SDL_StartTextInput();
    SDL_SetTextInputRect(to_sdl_global(&r));
  } else {
    SDL_StopTextInput();
  }

  return RET_OK;
}

static ret_t input_method_sdl_destroy(input_method_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  emitter_deinit(&(im->emitter));
  TKMEM_FREE(im);

  return RET_OK;
}

input_method_t* input_method_sdl_create(void) {
  input_method_t* im = TKMEM_ZALLOC(input_method_t);
  return_value_if_fail(im != NULL, NULL);

  im->request = input_method_sdl_request;
  im->destroy = input_method_sdl_destroy;

  emitter_init(&(im->emitter));

  return im;
}
