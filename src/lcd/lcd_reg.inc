/**
 * File:   lcd_reg.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  register based implemented lcd interface/
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

static ret_t lcd_reg_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = dirty_rect;

  return RET_OK;
}

static ret_t lcd_reg_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

  set_window_func(x, y, x + w, y);
  for (i = 0; i < w; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

  set_window_func(x, y, x, y + h);
  for (i = 0; i < h; i++) {
    write_data_func(color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  pixel_t color = to_pixel(lcd->stroke_color);

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
  pixel_t color = to_pixel(lcd->fill_color);

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
        write_data_func(to_pixel(fill_color));
      }
    }
    src_p += glyph->w;
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {

  wh_t i = 0;
  wh_t j = 0;
  wh_t dw = dst->w;
  wh_t dh = dst->h;
  wh_t width = lcd->width;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  color_t fill_color = lcd->fill_color;
  pixel_t* pixels = (pixel_t*)mem->pixels;
  pixel_t* dst_p = pixels + dst->y * width + dst->x;
  const color_t* data = (color_t*)img->data;

  if (src->w == dst->w && src->h == dst->h) {
    set_window_func(x, y, x + dw - 1, y + dh - 1);
    const color_t* src_p = data + img->w * src->y + src->x;
    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        color_t src_color = src_p[i];
        pixel_t color = blend_color(fill_color, src_color, src_color.rgba.a);
        write_data_func(color);
      }
      src_p += img->w;
      dst_p += width;
    }
  } else {
    xy_t sx = src->x;
    xy_t sy = src->y;
    xy_t sw = src->w;
    xy_t sh = src->h;
    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        const color_t* src_p = data + img->w * (sy + (j * sh / dh)) + sx + i * sw / dw;
        color_t src_color = src_p[i];
        pixel_t color = blend_color(fill_color, src_color, src_color.rgba.a);
        write_data_func(color);
      }
      dst_p += width;
    }
  }

  return RET_OK;

  return RET_OK;
}

static ret_t lcd_reg_end_frame(lcd_t* lcd) { return RET_OK; }

static ret_t lcd_reg_destroy(lcd_t* lcd) {
  MEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_reg_create(wh_t w, wh_t h) {
  lcd_t* lcd = NULL;
  lcd = MEM_ZALLOC(lcd_t);
  return_value_if_fail(lcd != NULL, NULL);

  lcd->width = w;
  lcd->height = h;

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
