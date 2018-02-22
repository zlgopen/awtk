/**
 * File:   lcd_rtthread.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rtthread implemented lcd interface/
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "lcd/lcd_mem.h"
#include "lcd/lcd_rtthread.h"

typedef struct _lcd_rtthread_t {
  lcd_t base;
  lcd_mem_t* lcd_mem;
  struct rtgui_graphic_driver* driver;
} lcd_rtthread_t;

static ret_t lcd_rtthread_begin_frame(lcd_t* lcd, rect_t* dr) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;

  lcd->dirty_rect = dr;
  rtt->lcd_mem->pixels = rtt->driver->framebuffer;

  return RET_OK;
}

static ret_t lcd_rtthread_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_hline(mem, x, y, w);
}

static ret_t lcd_rtthread_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_vline(mem, x, y, h);
}

static ret_t lcd_rtthread_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_points(mem, points, nr);
}

static ret_t lcd_rtthread_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);
  mem->fill_color = lcd->fill_color;

  return lcd_fill_rect(mem, x, y, w, h);
}

static ret_t lcd_rtthread_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);
  mem->text_color = lcd->text_color;
  mem->fill_color = lcd->fill_color;

  return lcd_draw_glyph(mem, glyph, src, x, y);
}

static ret_t lcd_rtthread_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;
  lcd_t* mem = (lcd_t*)(rtt->lcd_mem);

  return lcd_draw_image(mem, img, src, dst);
}

static ret_t lcd_rtthread_end_frame(lcd_t* lcd) {
  rtgui_rect_t rect;
  rect_t* dr = lcd->dirty_rect;
  lcd_rtthread_t* rtt = (lcd_rtthread_t*)lcd;

  rect.x1 = dr->x;
  rect.y1 = dr->y;
  rect.x2 = dr->x + dr->w;
  rect.y2 = dr->y + dr->w;

  rtgui_graphic_driver_screen_update(rtt->driver, &rect);

  return RET_OK;
}

static ret_t lcd_rtthread_destroy(lcd_t* lcd) {
  (void)lcd;
  return RET_OK;
}

lcd_t* lcd_rtthread_init(struct rtgui_graphic_driver* driver) {
  int w = 0;
  int h = 0;
  static lcd_rtthread_t lcd;
  lcd_t* base = &(lcd.base);
  return_value_if_fail(driver != NULL, NULL);

  memset(&lcd, 0x00, sizeof(lcd_rtthread_t));

  base->begin_frame = lcd_rtthread_begin_frame;
  base->draw_vline = lcd_rtthread_draw_vline;
  base->draw_hline = lcd_rtthread_draw_hline;
  base->fill_rect = lcd_rtthread_fill_rect;
  base->draw_image = lcd_rtthread_draw_image;
  base->draw_glyph = lcd_rtthread_draw_glyph;
  base->draw_points = lcd_rtthread_draw_points;
  base->end_frame = lcd_rtthread_end_frame;
  base->destroy = lcd_rtthread_destroy;

  base->width = (wh_t)driver->width;
  base->height = (wh_t)driver->height;
  
  lcd.driver = driver;
  lcd.lcd_mem = (lcd_mem_t*)lcd_mem_create(base->width, base->height, FALSE);

  return base;
}

