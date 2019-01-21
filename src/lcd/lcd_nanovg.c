/**
 * File:   lcd.c
 * Author: AWTK Develop Team
 * Brief:  lcd interface
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
 * 2018-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/lcd.h"
#include "base/vgcanvas.h"
#include "lcd/lcd_nanovg.h"
#include "lcd_vgcanvas.inc"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

lcd_t* lcd_nanovg_init(SDL_Window* sdl_window) {
  int w = 0;
  int h = 0;
  vgcanvas_t* vg = NULL;
  return_value_if_fail(sdl_window != NULL, NULL);

  SDL_GetWindowSize(sdl_window, &w, &h);
  vg = vgcanvas_create(w, h, 0, 0, sdl_window);
  return_value_if_fail(vg != NULL, NULL);

  return lcd_vgcanvas_init(w, h, vg);
}
