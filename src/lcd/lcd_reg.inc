/**
 * File:   lcd_reg.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  register based implemented lcd interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/system_info.h"

static ret_t lcd_reg_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = *dirty_rect;

  return RET_OK;
}

static ret_t lcd_reg_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  set_window_func(x, y, x + w, y);
  for (i = 0; i < w; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  set_window_func(x, y, x, y + h);
  for (i = 0; i < h; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    set_window_func(point->x, point->y, point->x, point->y);
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  uint32_t i = 0;
  uint32_t size = w * h;
  pixel_t color = color_to_pixel(lcd->fill_color);

  set_window_func(x, y, x + w - 1, y + h - 1);

  for (i = 0; i < size; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
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
        write_data_func(color_to_pixel(fill_color));
      }
    }
    src_p += glyph->w;
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t i = 0;
  wh_t j = 0;
  wh_t dw = dst->w;
  wh_t dh = dst->h;
  color_t fill_color = lcd->fill_color;
  pixel_t fill_pixel = color_to_pixel(fill_color);
  const color_t* data = (color_t*)img->data;

  if (src->w == dst->w && src->h == dst->h) {
    x = dst->x;
    y = dst->y;
    const color_t* src_p = data + img->w * src->y + src->x;

    set_window_func(x, y, x + dst->w - 1, y + dst->h - 1);
    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        color_t src_color = src_p[i];
        if (src_color.rgba.a > 7) {
          pixel_t color = src_color.rgba.a < 0xfe
                              ? blend_color(fill_color, src_color, src_color.rgba.a)
                              : color_to_pixel(src_color);
          write_data_func(color);
        } else {
          write_data_func(fill_pixel);
        }
      }
      src_p += img->w;
    }
  } else {
    xy_t sx = src->x;
    xy_t sy = src->y;
    wh_t sw = src->w;
    wh_t sh = src->h;
    wh_t iw = img->w;
    xy_t r = dst->x + dst->w;
    xy_t b = dst->y + dst->h;

    for (j = 0, y = dst->y; y < b; j++, y++) {
      const color_t* src_p = data + iw * (sy + (j * sh / dh)) + sx;
      for (i = 0, x = dst->x; x < r; i++, x++) {
        uint32_t s = (i * sw) / dw;
        color_t src_color = src_p[s];
        if (src_color.rgba.a > 7) {
          pixel_t color = src_color.rgba.a < 0xfe
                              ? blend_color(fill_color, src_color, src_color.rgba.a)
                              : color_to_pixel(src_color);

          set_window_func(x, y, x, y);
          write_data_func(color);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t lcd_reg_end_frame(lcd_t* lcd) { return RET_OK; }

static ret_t lcd_reg_destroy(lcd_t* lcd) {
  TKMEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_reg_create(wh_t w, wh_t h) {
  lcd_t* lcd = TKMEM_ZALLOC(lcd_t);
  system_info_t* info = system_info();
  return_value_if_fail(lcd != NULL, NULL);

  lcd->w = w;
  lcd->h = h;
  lcd->ratio = 1;
  lcd->type = LCD_REGISTER;
  info->lcd_w = lcd->w;
  info->lcd_h = lcd->h;
  info->lcd_type = lcd->type;
  info->device_pixel_ratio = 1;

  lcd->begin_frame = lcd_reg_begin_frame;
  lcd->draw_vline = lcd_reg_draw_vline;
  lcd->draw_hline = lcd_reg_draw_hline;
  lcd->fill_rect = lcd_reg_fill_rect;
  lcd->draw_image = lcd_reg_draw_image;
  lcd->draw_glyph = lcd_reg_draw_glyph;
  lcd->draw_points = lcd_reg_draw_points;
  lcd->end_frame = lcd_reg_end_frame;
  lcd->destroy = lcd_reg_destroy;

  return lcd;
}
