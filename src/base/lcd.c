/**
 * File:   lcd.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lcd interface
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

#include "base/lcd.h"

ret_t lcd_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  return_value_if_fail(lcd != NULL && lcd->begin_frame != NULL, RET_BAD_PARAMS);

  return lcd->begin_frame(lcd, dirty_rect);
}

ret_t lcd_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  return_value_if_fail(lcd != NULL && lcd->draw_vline != NULL, RET_BAD_PARAMS);

  return lcd->draw_vline(lcd, x, y, h);
}

ret_t lcd_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  return_value_if_fail(lcd != NULL && lcd->draw_hline != NULL, RET_BAD_PARAMS);

  return lcd->draw_hline(lcd, x, y, w);
}

ret_t lcd_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(lcd != NULL && lcd->fill_rect != NULL, RET_BAD_PARAMS);

  return lcd->fill_rect(lcd, x, y, w, h);
}

ret_t lcd_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  return_value_if_fail(lcd != NULL && lcd->draw_points != NULL && points != NULL, RET_BAD_PARAMS);

  return lcd->draw_points(lcd, points, nr);
}

ret_t lcd_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  return_value_if_fail(lcd != NULL && lcd->draw_image != NULL && src != NULL && dst != NULL,
                       RET_BAD_PARAMS);

  return lcd->draw_image(lcd, img, src, dst);
}

ret_t lcd_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  return_value_if_fail(lcd != NULL && lcd->draw_glyph != NULL && glyph != NULL && src != NULL,
                       RET_BAD_PARAMS);

  return lcd->draw_glyph(lcd, glyph, src, x, y);
}

ret_t lcd_end_frame(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->end_frame != NULL, RET_BAD_PARAMS);

  return lcd->end_frame(lcd);
}

ret_t lcd_destroy(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->destroy != NULL, RET_BAD_PARAMS);

  return lcd->destroy(lcd);
}

vgcanvas_t* lcd_get_vgcanvas(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->get_vgcanvas != NULL, NULL);

  return lcd->get_vgcanvas(lcd);
}
