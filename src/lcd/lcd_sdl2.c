/**
 * File:   lcd_sdl2.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  sdl2 implemented lcd interface/
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "lcd/lcd_sdl2.h"
#include "lcd/lcd_mem.h"

typedef struct _lcd_sdl2_t {
  lcd_t base;
  SDL_Renderer* render;
  lcd_mem_t* lcd_mem;
  SDL_Texture* texture;
} lcd_sdl2_t;

static ret_t lcd_sdl2_begin_frame(lcd_t* lcd, rect_t* dr) {
  int pitch = 0;
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;

  lcd->dirty_rect = dr;
  SDL_LockTexture(sdl->texture, NULL, (void**)&(sdl->lcd_mem->pixels), &pitch);

  return RET_OK;
}

static ret_t lcd_sdl2_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_hline(mem, x, y, w);
}

static ret_t lcd_sdl2_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_vline(mem, x, y, h);
}

static ret_t lcd_sdl2_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_points(mem, points, nr);
}

static ret_t lcd_sdl2_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);
  mem->fill_color = lcd->fill_color;

  return lcd_fill_rect(mem, x, y, w, h);
}

static ret_t lcd_sdl2_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);
  mem->text_color = lcd->text_color;
  mem->fill_color = lcd->fill_color;

  return lcd_draw_glyph(mem, glyph, src, x, y);
}

static ret_t lcd_sdl2_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);

  return lcd_draw_image(mem, img, src, dst);
}

static ret_t lcd_sdl2_end_frame(lcd_t* lcd) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  rect_t* dr = lcd->dirty_rect;

  SDL_UnlockTexture(sdl->texture);
  if (dr->w > 0 && dr->h > 0) {
    SDL_RenderCopy(sdl->render, sdl->texture, NULL, NULL);
  }
  SDL_RenderPresent(sdl->render);

  return RET_OK;
}

static ret_t lcd_sdl2_destroy(lcd_t* lcd) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  SDL_DestroyTexture(sdl->texture);

  return RET_OK;
}

static vgcanvas_t* lcd_sdl2_get_vgcanvas(lcd_t* lcd) {
  lcd_sdl2_t* mem = (lcd_sdl2_t*)lcd;

  return lcd_get_vgcanvas(mem->lcd_mem);
}

lcd_t* lcd_sdl2_init(SDL_Renderer* render) {
  int w = 0;
  int h = 0;
  static lcd_sdl2_t lcd;
  lcd_t* base = &(lcd.base);
  return_value_if_fail(render != NULL, NULL);

  memset(&lcd, 0x00, sizeof(lcd_sdl2_t));

  lcd.render = render;
  base->begin_frame = lcd_sdl2_begin_frame;
  base->draw_vline = lcd_sdl2_draw_vline;
  base->draw_hline = lcd_sdl2_draw_hline;
  base->fill_rect = lcd_sdl2_fill_rect;
  base->draw_image = lcd_sdl2_draw_image;
  base->draw_glyph = lcd_sdl2_draw_glyph;
  base->draw_points = lcd_sdl2_draw_points;
  base->end_frame = lcd_sdl2_end_frame;
  base->get_vgcanvas = lcd_sdl2_get_vgcanvas;
  base->destroy = lcd_sdl2_destroy;

  SDL_GetRendererOutputSize(render, &w, &h);

  base->width = (wh_t)w;
  base->height = (wh_t)h;
  lcd.lcd_mem = (lcd_mem_t*)lcd_mem_create(w, h, FALSE);
  lcd.texture =
      SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
  // SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, w, h);

  return base;
}
