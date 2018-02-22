/**
 * File:   lcd_mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  mem implemented lcd interface/
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

#include "lcd/lcd_mem.h"
#include "base/mem.h"
#include "lcd/rgb565.h"

static ret_t lcd_mem_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = dirty_rect;

  return RET_OK;
}

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  wh_t width = lcd->width;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t stroke_color = lcd->stroke_color;
  pixel_t color = to_pixel(stroke_color);
  pixel_t* p = pixels + y * width + x;

  for (i = 0; i < w; i++) {
    p[i] = color;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  wh_t width = lcd->width;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t stroke_color = lcd->stroke_color;
  pixel_t color = to_pixel(stroke_color);
  pixel_t* p = pixels + y * width + x;

  for (i = 0; i < h; i++) {
    *p = color;
    p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  wh_t width = lcd->width;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  color_t stroke_color = lcd->stroke_color;
  pixel_t color = to_pixel(stroke_color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    pixel_t* p = pixels + point->y * width + point->x;
    p[i] = color;
  }

  return RET_OK;
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  wh_t i = 0;

  color_t c = lcd->stroke_color;
  lcd->stroke_color = lcd->fill_color;
  for (i = 0; i < h; i++) {
    lcd_mem_draw_hline(lcd, x, y + i, w);
  }
  lcd->stroke_color = c;

  return RET_OK;
}

static ret_t lcd_mem_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t width = lcd->width;
  color_t color = lcd->text_color;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t* dst_p = pixels + y * width + x;
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;
  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      uint8_t alpha = src_p[i];
      if (alpha) {
        color.rgba.a = alpha;
        dst_p[i] = blend_pixel(dst_p[i], color);
      }
    }
    src_p += glyph->w;
    dst_p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t width = lcd->width;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t* dst_p = pixels + dst->y * width + dst->x;
  const color_t* data = (color_t*)img->data;
  const color_t* src_p = data + img->w * sy + sx;

  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      dst_p[i] = blend_pixel(dst_p[i], src_p[i]);
    }
    src_p += img->w;
    dst_p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) { return RET_OK; }

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  MEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  lcd_mem_t* lcd = MEM_ZALLOC(lcd_mem_t);
  lcd_t* base = &(lcd->base);

  if (alloc) {
    lcd->pixels = (uint8_t*)MEM_ALLOC(w * h * sizeof(pixel_t));
    return_value_if_fail(lcd->pixels != NULL, NULL);
  }

  base->begin_frame = lcd_mem_begin_frame;
  base->draw_vline = lcd_mem_draw_vline;
  base->draw_hline = lcd_mem_draw_hline;
  base->fill_rect = lcd_mem_fill_rect;
  base->draw_image = lcd_mem_draw_image;
  base->draw_glyph = lcd_mem_draw_glyph;
  base->draw_points = lcd_mem_draw_points;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = lcd_mem_destroy;

  base->width = w;
  base->height = h;

  return base;
}
