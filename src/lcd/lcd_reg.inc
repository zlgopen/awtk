/**
 * File:   lcd_reg.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  register based implemented lcd interface
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"
#include "fragment_frame_buffer.h"

#ifndef FRAGMENT_FRAME_BUFFER_SIZE
#define FRAGMENT_FRAME_BUFFER_SIZE 8192
#endif /*FRAGMENT_FRAME_BUFFER_SIZE*/

typedef struct _lcd_reg_t {
  lcd_t lcd;
  fragment_frame_buffer_t affb;
  fragment_frame_buffer_t* ffb;
  pixel_t buff[FRAGMENT_FRAME_BUFFER_SIZE];
} lcd_reg_t;

static inline ret_t lcd_reg_set_window(lcd_t* lcd, int sx, int sy, int ex, int ey) {
  lcd_reg_t* reg = (lcd_reg_t*)lcd;

  if (reg->ffb != NULL) {
    fragment_frame_buffer_set_window(reg->ffb, sx, sy, ex - sx + 1, ey - sy + 1);
  } else {
    set_window_func(sx, sy, ex, ey);
  }

  return RET_OK;
}

static inline ret_t lcd_reg_write_data(lcd_t* lcd, pixel_t data) {
  lcd_reg_t* reg = (lcd_reg_t*)lcd;

  if (reg->ffb != NULL) {
    fragment_frame_buffer_write_data(reg->ffb, data);
  } else {
    write_data_func(data);
  }

  return RET_OK;
}

static ret_t lcd_reg_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd_reg_t* reg = (lcd_reg_t*)lcd;
  lcd->dirty_rect = *dirty_rect;

  reg->ffb = fragment_frame_buffer_begin_frame(&(reg->affb), dirty_rect);

  return RET_OK;
}

static ret_t lcd_reg_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  lcd_reg_set_window(lcd, x, y, x + w, y);
  for (i = 0; i < w; i++) {
    lcd_reg_write_data(lcd, color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  lcd_reg_set_window(lcd, x, y, x, y + h);
  for (i = 0; i < h; i++) {
    lcd_reg_write_data(lcd, color);
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  pixel_t color = color_to_pixel(lcd->stroke_color);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    lcd_reg_set_window(lcd, point->x, point->y, point->x, point->y);
    lcd_reg_write_data(lcd, color);
  }

  return RET_OK;
}

static ret_t lcd_reg_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  uint32_t i = 0;
  uint32_t size = w * h;
  pixel_t color = color_to_pixel(lcd->fill_color);

  lcd_reg_set_window(lcd, x, y, x + w - 1, y + h - 1);

  for (i = 0; i < size; i++) {
    lcd_reg_write_data(lcd, color);
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
  pixel_t fill_pixel = color_to_pixel(fill_color);
  pixel_t text_pixel = color_to_pixel(text_color);
  lcd_reg_set_window(lcd, x, y, x + sw - 1, y + sh - 1);
  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      uint8_t a = src_p[i];

      if (a >= TK_OPACITY_ALPHA) {
        lcd_reg_write_data(lcd, text_pixel);
      } else if (a >= TK_TRANSPARENT_ALPHA) {
        pixel_t color = blend_color(fill_color, text_color, a);
        lcd_reg_write_data(lcd, color);
      } else {
        lcd_reg_write_data(lcd, fill_pixel);
      }
    }
    src_p += glyph->w;
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_image_bgr565(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  wh_t i = 0;
  wh_t j = 0;
  xy_t x = dst->x;
  xy_t y = dst->y;
  wh_t dw = dst->w;
  wh_t dh = dst->h;
  const uint16_t* data = (uint16_t*)img->data;

  lcd_reg_set_window(lcd, x, y, x + dw - 1, y + dh - 1);
  if (src->w == dst->w && src->h == dst->h) {
    const uint16_t* src_p = data + img->w * src->y + src->x;

    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        lcd_reg_write_data(lcd, src_p[i]);
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
    uint32_t scale_x = (((sw) << 16) / dw);
    uint32_t scale_y = (((sh) << 16) / dh);

    for (j = 0, y = dst->y; y < b; j++, y++) {
      const uint16_t* src_p = data + iw * (sy + ((j * scale_y) >> 16)) + sx;
      for (i = 0, x = dst->x; x < r; i++, x++) {
        uint32_t s = (i * scale_x) >> 16;
        lcd_reg_write_data(lcd, src_p[s]);
      }
    }
  }

  return RET_OK;
}

static ret_t lcd_reg_8888(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  wh_t i = 0;
  wh_t j = 0;
  uint8_t a = 0;
  xy_t x = dst->x;
  xy_t y = dst->y;
  wh_t dw = dst->w;
  wh_t dh = dst->h;
  color_t src_color;
  uint32_t format = img->format;
  color_t fill_color = lcd->fill_color;
  pixel_t fill_pixel = color_to_pixel(fill_color);
  const color_t* data = (color_t*)img->data;

  lcd_reg_set_window(lcd, x, y, x + dw - 1, y + dh - 1);
  if (src->w == dst->w && src->h == dst->h) {
    const color_t* src_p = data + img->w * src->y + src->x;

    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        src_color = src_p[i];
        if (format == BITMAP_FMT_BGRA8888) {
          uint8_t r = src_color.rgba.r;
          src_color.rgba.r = src_color.rgba.b;
          src_color.rgba.b = r;
        }
        a = src_color.rgba.a;

        if (a >= TK_OPACITY_ALPHA) {
          lcd_reg_write_data(lcd, color_to_pixel(src_color));
        } else if (a >= TK_TRANSPARENT_ALPHA) {
          lcd_reg_write_data(lcd, blend_color(fill_color, src_color, a));
        } else {
          lcd_reg_write_data(lcd, fill_pixel);
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
    uint32_t scale_x = (((sw) << 16) / dw);
    uint32_t scale_y = (((sh) << 16) / dh);

    for (j = 0, y = dst->y; y < b; j++, y++) {
      const color_t* src_p = data + iw * (sy + ((j * scale_y) >> 16)) + sx;
      for (i = 0, x = dst->x; x < r; i++, x++) {
        uint32_t s = (i * scale_x) >> 16;
        src_color = src_p[s];
        if (format == BITMAP_FMT_BGRA8888) {
          uint8_t r = src_color.rgba.r;
          src_color.rgba.r = src_color.rgba.b;
          src_color.rgba.b = r;
        }
        a = src_color.rgba.a;

        if (a >= TK_OPACITY_ALPHA) {
          lcd_reg_write_data(lcd, color_to_pixel(src_color));
        } else if (a >= TK_TRANSPARENT_ALPHA) {
          lcd_reg_write_data(lcd, blend_color(fill_color, src_color, a));
        } else {
          lcd_reg_write_data(lcd, fill_pixel);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t lcd_reg_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  if (img->format == BITMAP_FMT_BGR565) {
    return lcd_reg_draw_image_bgr565(lcd, img, src, dst);
  } else if (img->format == BITMAP_FMT_RGBA8888 || img->format == BITMAP_FMT_BGRA8888) {
    return lcd_reg_8888(lcd, img, src, dst);
  } else {
    assert(!"not supported format");
    return RET_FAIL;
  }
}

static ret_t lcd_reg_end_frame(lcd_t* lcd) {
  lcd_reg_t* reg = (lcd_reg_t*)lcd;

  if (reg->ffb != NULL) {
    fragment_frame_buffer_end_frame(reg->ffb);
  }

  return RET_OK;
}

static ret_t lcd_reg_destroy(lcd_t* lcd) {
  return RET_OK;
}

static bitmap_format_t lcd_reg_get_desired_bitmap_format(lcd_t* lcd) {
  return LCD_FORMAT;
}

lcd_t* lcd_reg_create(wh_t w, wh_t h) {
  static lcd_reg_t reg;

  lcd_t* lcd = (lcd_t*)(&reg);
  uint32_t size = sizeof(reg);
  system_info_t* info = system_info();
  return_value_if_fail(lcd != NULL, NULL);

  memset(&reg, 0x00, size);

  lcd->w = w;
  lcd->h = h;
  lcd->ratio = 1;
  lcd->type = LCD_REGISTER;

  system_info_set_lcd_w(info, lcd->w);
  system_info_set_lcd_h(info, lcd->h);
  system_info_set_lcd_type(info, lcd->type);
  system_info_set_device_pixel_ratio(info, 1);

  lcd->begin_frame = lcd_reg_begin_frame;
  lcd->draw_vline = lcd_reg_draw_vline;
  lcd->draw_hline = lcd_reg_draw_hline;
  lcd->fill_rect = lcd_reg_fill_rect;
  lcd->draw_image = lcd_reg_draw_image;
  lcd->draw_glyph = lcd_reg_draw_glyph;
  lcd->draw_points = lcd_reg_draw_points;
  lcd->end_frame = lcd_reg_end_frame;
  lcd->destroy = lcd_reg_destroy;
  lcd->support_dirty_rect = TRUE;
  lcd->get_desired_bitmap_format = lcd_reg_get_desired_bitmap_format;

  fragment_frame_buffer_init(&(reg.affb), reg.buff, FRAGMENT_FRAME_BUFFER_SIZE);

  return lcd;
}
