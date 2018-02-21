/**
 * File:   lcd_stm32_raw.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stm32_raw implemented lcd interface/
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/color.h"
#include "lcd/rgb565.h"
#include "lcd/lcd_stm32_raw.h"

static tft_set_window_t set_window_func;
static tft_write_data_t write_data_func;

static ret_t lcd_stm32_raw_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = dirty_rect;

  return RET_OK;
}

static ret_t lcd_stm32_raw_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

  set_window_func(x, y, x + w, y);
  for (i = 0; i < w; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

  set_window_func(x, y, x, y + h);
  for (i = 0; i < h; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    set_window_func(point->x, point->y, point->x, point->y);
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  uint32_t i = 0;
  uint32_t size = w * h;
  pixel_t color = to_pixel(lcd->fill_color);

  set_window_func(x, y, x + w - 1, y + h - 1);

  for (i = 0; i < size; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  color_t text_color = lcd->text_color;
  color_t fill_color = lcd->fill_color;
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;

  set_window_func(x, y, x + sw - 1, y + sh - 1);
  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      uint8_t alpha = src_p[i];
      if (alpha) {
        pixel_t color = blend_color(fill_color, text_color, alpha);
        write_data_func(color);
      } else {
        write_data_func(to_pixel(fill_color));
      }
    }
    src_p += glyph->w;
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  wh_t i = 0;
  wh_t j = 0;
  xy_t x = dst->x;
  xy_t y = dst->y;
  xy_t sx = src->x;
  xy_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  color_t fill_color = lcd->fill_color;
  const color_t* data = (color_t*)img->data;
  const color_t* src_p = data + img->w * sy + sx;

  set_window_func(x, y, x + sw - 1, y + sh - 1);
  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      color_t src_color = src_p[i];
      pixel_t color = blend_color(fill_color, src_color, src_color.rgba.a);

      write_data_func(color);
    }
    src_p += img->w;
  }

  return RET_OK;
}

static ret_t lcd_stm32_raw_end_frame(lcd_t* lcd) { return RET_OK; }

static ret_t lcd_stm32_raw_destroy(lcd_t* lcd) {
  MEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_stm32_raw_create(wh_t w, wh_t h, tft_write_data_t write_data,
                            tft_set_window_t set_window) {
  lcd_t* lcd = NULL;
  return_value_if_fail(write_data != NULL && set_window != NULL, NULL);

  lcd = MEM_ZALLOC(lcd_t);
  return_value_if_fail(lcd != NULL, NULL);

  lcd->width = w;
  lcd->height = h;
  write_data_func = write_data;
  set_window_func = set_window;

  lcd->begin_frame = lcd_stm32_raw_begin_frame;
  lcd->draw_vline = lcd_stm32_raw_draw_vline;
  lcd->draw_hline = lcd_stm32_raw_draw_hline;
  lcd->fill_rect = lcd_stm32_raw_fill_rect;
  lcd->draw_image = lcd_stm32_raw_draw_image;
  lcd->draw_glyph = lcd_stm32_raw_draw_glyph;
  lcd->draw_points = lcd_stm32_raw_draw_points;
  lcd->end_frame = lcd_stm32_raw_end_frame;
  lcd->destroy = lcd_stm32_raw_destroy;

  return lcd;
}
