/**
 * File:   lcd_mono.h
 * Author: AWTK Develop Team
 * Brief:  mono lcd
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-10 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/pixel.h"
#include "lcd/lcd_mono.h"
#include "base/system_info.h"

#undef color_to_pixel
typedef uint8_t pixel_t;
#define color_to_pixel(c) color_to_mono(c)
#define color_from_pixel(p) color_from_mono(p)

static ret_t inline lcd_mono_set_pixel(lcd_t* lcd, uint16_t x, uint16_t y, bool_t pixel) {
  lcd_mono_t* mono = (lcd_mono_t*)(lcd);

  return bitmap_mono_set_pixel(mono->data, lcd->w, lcd->h, x, y, pixel);
}

static color_t lcd_mono_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  color_t c;
  lcd_mono_t* mono = (lcd_mono_t*)(lcd);
  pixel_t pixel = bitmap_mono_get_pixel(mono->data, lcd->w, lcd->h, x, y);

  c = color_from_pixel(pixel);

  return c;
}

static ret_t lcd_mono_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd->dirty_rect = *dirty_rect;

  return RET_OK;
}

static ret_t lcd_mono_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  pixel_t pixel = color_to_pixel(lcd->stroke_color);

  for (i = 0; i < w; i++) {
    lcd_mono_set_pixel(lcd, x + i, y, pixel);
  }

  return RET_OK;
}

static ret_t lcd_mono_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t pixel = color_to_pixel(lcd->stroke_color);

  for (i = 0; i < h; i++) {
    lcd_mono_set_pixel(lcd, x, y + i, pixel);
  }

  return RET_OK;
}

static ret_t lcd_mono_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  pixel_t pixel = color_to_pixel(lcd->stroke_color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    lcd_mono_set_pixel(lcd, point->x, point->y, pixel);
  }

  return RET_OK;
}

static ret_t lcd_mono_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  uint32_t i = 0;
  uint32_t j = 0;
  pixel_t pixel = color_to_pixel(lcd->fill_color);

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      lcd_mono_set_pixel(lcd, x + i, y + j, pixel);
    }
  }

  return RET_OK;
}

static ret_t lcd_mono_draw_data(lcd_t* lcd, const uint8_t* buff, uint32_t w, uint32_t h,
                                rect_t* src, xy_t x, xy_t y, bool_t revert_pixel) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;

  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      pixel_t pixel = bitmap_mono_get_pixel(buff, w, h, sx + i, sy + j);
      if (revert_pixel) {
        pixel = !pixel;
      }
      lcd_mono_set_pixel(lcd, x + i, y + j, pixel);
    }
  }

  return RET_OK;
}

static ret_t lcd_mono_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  pixel_t pixel = color_to_pixel(lcd->text_color);

  return lcd_mono_draw_data(lcd, glyph->data, glyph->w, glyph->h, src, x, y, !pixel);
}

static ret_t lcd_mono_draw_image_mono(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  ret_t ret = RET_OK;
  const uint8_t* data = NULL;
  return_value_if_fail(src->w == dst->w && src->h == dst->h, RET_OK);

  data = bitmap_lock_buffer_for_read(img);
  ret = lcd_mono_draw_data(lcd, data, img->w, img->h, src, dst->x, dst->y, FALSE);
  bitmap_unlock_buffer(img);

  return ret;
}

static ret_t lcd_mono_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  return_value_if_fail(img->format == BITMAP_FMT_MONO, RET_NOT_IMPL);
  return_value_if_fail(src->w == dst->w && src->h == dst->h, RET_NOT_IMPL);

  return lcd_mono_draw_image_mono(lcd, img, src, dst);
}

static ret_t lcd_mono_end_frame(lcd_t* lcd) {
  if (lcd->flush != NULL) {
    lcd->flush(lcd);
  }

  return RET_OK;
}

static ret_t lcd_mono_destroy(lcd_t* lcd) {
  lcd_mono_t* mono = (lcd_mono_t*)(lcd);
  if (mono->on_destroy) {
    mono->on_destroy(lcd);
  }
  TKMEM_FREE(mono->data);
  TKMEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_mono_create(wh_t w, wh_t h, lcd_flush_t flush, lcd_destroy_t on_destroy, void* ctx) {
  lcd_mono_t* mono = TKMEM_ZALLOC(lcd_mono_t);
  system_info_t* info = system_info();
  lcd_t* lcd = (lcd_t*)mono;
  return_value_if_fail(lcd != NULL, NULL);

  lcd->w = w;
  lcd->h = h;
  lcd->ratio = 1;
  lcd->type = LCD_MONO;
  mono->data = bitmap_mono_create_data(w, h);
  mono->on_destroy = on_destroy;
  mono->ctx = ctx;

  ENSURE(mono->data != NULL);

  system_info_set_lcd_w(info, lcd->w);
  system_info_set_lcd_h(info, lcd->h);
  system_info_set_lcd_type(info, lcd->type);
  system_info_set_device_pixel_ratio(info, 1);

  lcd->begin_frame = lcd_mono_begin_frame;
  lcd->draw_vline = lcd_mono_draw_vline;
  lcd->draw_hline = lcd_mono_draw_hline;
  lcd->fill_rect = lcd_mono_fill_rect;
  lcd->draw_image = lcd_mono_draw_image;
  lcd->draw_glyph = lcd_mono_draw_glyph;
  lcd->draw_points = lcd_mono_draw_points;
  lcd->get_point_color = lcd_mono_get_point_color;
  lcd->end_frame = lcd_mono_end_frame;
  lcd->destroy = lcd_mono_destroy;
  lcd->support_dirty_rect = TRUE;
  lcd->flush = flush;

  return lcd;
}
