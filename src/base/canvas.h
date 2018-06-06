/**
 * File:   canvas.h
 * Author: AWTK Develop Team
 * Brief:  canvas provides basic drawings functions.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANVAS_H
#define TK_CANVAS_H

#include "base/lcd.h"
#include "base/font_manager.h"

BEGIN_C_DECLS

struct _canvas_t;
typedef struct _canvas_t canvas_t;

/**
 * @class canvas_t
 * @scriptable
 * canvas类。
 */
struct _canvas_t {
  xy_t ox;
  xy_t oy;
  xy_t clip_left;
  xy_t clip_top;
  xy_t clip_right;
  xy_t clip_bottom;

  lcd_t* lcd;
  font_t* font;
  uint16_t font_size;
  const char* font_name;
  font_manager_t* font_manager;
};

canvas_t* canvas_init(canvas_t* c, lcd_t* lcd, font_manager_t* font_manager);

wh_t canvas_get_width(canvas_t* c);
wh_t canvas_get_height(canvas_t* c);

ret_t canvas_get_clip_rect(canvas_t* c, rect_t* r);
ret_t canvas_set_clip_rect(canvas_t* c, const rect_t* r);
ret_t canvas_set_fill_color(canvas_t* c, color_t color);
ret_t canvas_set_text_color(canvas_t* c, color_t color);
ret_t canvas_set_stroke_color(canvas_t* c, color_t color);
ret_t canvas_set_global_alpha(canvas_t* c, uint8_t alpha);
ret_t canvas_set_font(canvas_t* c, const char* name, uint16_t size);

wh_t canvas_measure_text(canvas_t* c, wchar_t* str, int32_t nr);

ret_t canvas_begin_frame(canvas_t* c, rect_t* dirty_rect, lcd_draw_mode_t draw_mode);

ret_t canvas_translate(canvas_t* c, xy_t dx, xy_t dy);
ret_t canvas_untranslate(canvas_t* c, xy_t dx, xy_t dy);

ret_t canvas_draw_vline(canvas_t* c, xy_t x, xy_t y, wh_t h);
ret_t canvas_draw_hline(canvas_t* c, xy_t x, xy_t y, wh_t w);
ret_t canvas_draw_line(canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2);
ret_t canvas_draw_points(canvas_t* c, point_t* points, uint32_t nr);

ret_t canvas_fill_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h);
ret_t canvas_stroke_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t canvas_draw_char(canvas_t* c, wchar_t chr, xy_t x, xy_t y);
ret_t canvas_draw_text(canvas_t* c, wchar_t* str, int32_t nr, xy_t x, xy_t y);
ret_t canvas_draw_image(canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst);

ret_t canvas_draw_image_at(canvas_t* c, bitmap_t* img, xy_t x, xy_t y);
ret_t canvas_draw_icon(canvas_t* c, bitmap_t* img, xy_t cx, xy_t cy);

ret_t canvas_draw_image_center(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_3patch_x(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_3patch_x_scale_y(canvas_t* c, bitmap_t* img, rect_t* dst);

ret_t canvas_draw_image_3patch_y(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_3patch_y_scale_x(canvas_t* c, bitmap_t* img, rect_t* dst);

ret_t canvas_draw_image_9patch(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat_x(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat_y(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale_w(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale_h(canvas_t* c, bitmap_t* img, rect_t* dst);

ret_t canvas_draw_image_ex(canvas_t* c, bitmap_t* img, image_draw_type_t draw_type, rect_t* dst);

ret_t canvas_test_paint(canvas_t* c, bool_t pressed, xy_t x, xy_t y);

ret_t canvas_end_frame(canvas_t* c);

END_C_DECLS

#endif /*TK_CANVAS_H*/
