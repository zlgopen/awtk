/**
 * File:   lcd.h
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

#ifndef LFTK_LCD_H
#define LFTK_LCD_H

#include "base/rect.h"
#include "base/font.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

struct _lcd_t;
typedef struct _lcd_t lcd_t;

typedef ret_t (*lcd_begin_frame_t)(lcd_t* lcd, rect_t* dirty_rect);

typedef ret_t (*lcd_draw_vline_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t w);
typedef ret_t (*lcd_draw_hline_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t h);
typedef ret_t (*lcd_draw_points_t)(lcd_t* lcd, point_t* points, uint32_t nr);
typedef ret_t (*lcd_fill_rect_t)(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);
typedef ret_t (*lcd_draw_glyph_t)(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y);
typedef ret_t (*lcd_draw_image_t)(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst);

typedef ret_t (*lcd_end_frame_t)(lcd_t* lcd);
typedef ret_t (*lcd_destroy_t)(lcd_t* lcd);

struct _lcd_t {
    lcd_begin_frame_t  begin_frame;
    lcd_draw_vline_t   draw_vline;
    lcd_draw_hline_t   draw_hline;
    lcd_fill_rect_t    fill_rect;
    lcd_draw_image_t   draw_image;
    lcd_draw_glyph_t   draw_glyph;
    lcd_draw_points_t  draw_points;
    lcd_end_frame_t    end_frame;
    lcd_destroy_t      destroy; 

/*properties:*/
    uint8_t global_alpha;
    wh_t width;  /*readonly*/
    wh_t height; /*readonly*/
    rect_t* dirty_rect;
    color_t text_color;
    color_t fill_color;
    color_t stroke_color;
};

ret_t lcd_begin_frame(lcd_t* lcd, rect_t* dirty_rect);

ret_t lcd_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t w);
ret_t lcd_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t h);
ret_t lcd_draw_points(lcd_t* lcd, point_t* points, uint32_t nr);
ret_t lcd_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);
ret_t lcd_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y);
ret_t lcd_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst);

ret_t lcd_end_frame(lcd_t* lcd);

ret_t lcd_destroy(lcd_t* lcd);

END_C_DECLS

#endif/*LFTK_LCD_H*/


