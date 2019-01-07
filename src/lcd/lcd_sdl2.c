/**
 * File:   lcd_sdl2.c
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "lcd/lcd_sdl2.h"
#include "lcd/lcd_mem_bgr565.h"
#include "lcd/lcd_mem_bgr888.h"
#include "lcd/lcd_mem_bgra8888.h"

typedef struct _lcd_sdl2_t {
  lcd_t base;
  SDL_Renderer* render;
  lcd_mem_t* lcd_mem;
  SDL_Texture* texture;
} lcd_sdl2_t;

static ret_t lcd_sdl2_begin_frame(lcd_t* lcd, rect_t* dr) {
  int pitch = 0;
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;

  SDL_LockTexture(sdl->texture, NULL, (void**)&(sdl->lcd_mem->offline_fb), &pitch);
  lcd_mem_set_line_length(sdl->lcd_mem, pitch);

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

static color_t lcd_sdl2_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);

  return lcd_get_point_color(mem, x, y);
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

static ret_t lcd_sdl2_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);

  return lcd_draw_image_matrix(mem, info);
}

static ret_t lcd_sdl2_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);

  return lcd_draw_image(mem, img, src, dst);
}

static ret_t lcd_sdl2_end_frame(lcd_t* lcd) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  rect_t* dr = &(lcd->dirty_rect);
  rect_t* fps_r = &(lcd->fps_rect);

  SDL_UnlockTexture(sdl->texture);
  if ((dr->w > 0 && dr->h > 0) || (fps_r->w > 0 && fps_r->h > 0)) {
    SDL_Rect sr = {0, 0, lcd->w, lcd->h};

    SDL_RenderCopy(sdl->render, sdl->texture, &sr, &sr);
  }

  if (lcd->draw_mode != LCD_DRAW_OFFLINE) {
    SDL_RenderPresent(sdl->render);
  }

  return RET_OK;
}

static ret_t lcd_sdl2_destroy(lcd_t* lcd) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  SDL_DestroyTexture(sdl->texture);

  return RET_OK;
}

static ret_t lcd_sdl2_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;

  return lcd_take_snapshot((lcd_t*)(sdl->lcd_mem), img, auto_rotate);
}

static vgcanvas_t* lcd_sdl2_get_vgcanvas(lcd_t* lcd) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;

  return lcd_get_vgcanvas((lcd_t*)(sdl->lcd_mem));
}

static ret_t lcd_sdl2_set_global_alpha(lcd_t* lcd, uint8_t alpha) {
  lcd_sdl2_t* sdl = (lcd_sdl2_t*)lcd;
  lcd_t* mem = (lcd_t*)(sdl->lcd_mem);

  lcd->global_alpha = alpha;
  mem->global_alpha = alpha;

  return RET_OK;
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
  base->draw_image_matrix = lcd_sdl2_draw_image_matrix;
  base->draw_glyph = lcd_sdl2_draw_glyph;
  base->draw_points = lcd_sdl2_draw_points;
  base->get_point_color = lcd_sdl2_get_point_color;
  base->end_frame = lcd_sdl2_end_frame;
  base->get_vgcanvas = lcd_sdl2_get_vgcanvas;
  base->take_snapshot = lcd_sdl2_take_snapshot;
  base->set_global_alpha = lcd_sdl2_set_global_alpha;
  base->destroy = lcd_sdl2_destroy;

  SDL_GetRendererOutputSize(render, &w, &h);

  base->ratio = 1;
  base->w = (wh_t)w;
  base->h = (wh_t)h;

#ifdef WITH_FB_BGRA8888
  /*SDL ABGR is rgba from low address to high address*/
  lcd.lcd_mem = (lcd_mem_t*)lcd_mem_bgra8888_create(w, h, FALSE);
  lcd.texture =
      SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
  log_debug("WITH_FB_BGRA8888\n");
#elif defined(WITH_FB_BGR888)
  /*SDL ABGR is rgba from low address to high address*/
  lcd.lcd_mem = (lcd_mem_t*)lcd_mem_bgr888_create(w, h, FALSE);
  lcd.texture =
      SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);
  log_debug("WITH_FB_BGR888\n");
#else
  /*SDL ABGR is rgba from low address to high address*/
  lcd.lcd_mem = (lcd_mem_t*)lcd_mem_bgr565_create(w, h, FALSE);
  lcd.texture =
      SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, w, h);
  log_debug("WITH_FB_BGR565\n");
#endif

  base->type = lcd.lcd_mem->base.type;
  base->support_dirty_rect = FALSE;

  return base;
}
