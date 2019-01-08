/**
 * File:   canvas.h
 * Author: AWTK Develop Team
 * Brief:  canvas provides basic drawings functions.
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
 * @annotation ["scriptable"]
 * canvas类。
 */
struct _canvas_t {
  xy_t ox;
  xy_t oy;
  xy_t clip_left;
  xy_t clip_top;
  xy_t clip_right;
  xy_t clip_bottom;
  uint32_t fps;
  bool_t show_fps;

  lcd_t* lcd;
  font_t* font;
  font_size_t font_size;
  const char* font_name;

  align_v_t text_align_v;
  align_h_t text_align_h;
  font_manager_t* font_manager;
};

canvas_t* canvas_init(canvas_t* c, lcd_t* lcd, font_manager_t* font_manager);

wh_t canvas_get_width(canvas_t* c);
wh_t canvas_get_height(canvas_t* c);

ret_t canvas_set_font_manager(canvas_t* c, font_manager_t* font_manager);

ret_t canvas_get_clip_rect(canvas_t* c, rect_t* r);
ret_t canvas_set_clip_rect(canvas_t* c, const rect_t* r);
ret_t canvas_set_clip_rect_ex(canvas_t* c, const rect_t* r, bool_t translate);
ret_t canvas_set_fill_color(canvas_t* c, color_t color);
ret_t canvas_set_text_color(canvas_t* c, color_t color);
ret_t canvas_set_stroke_color(canvas_t* c, color_t color);
ret_t canvas_set_global_alpha(canvas_t* c, uint8_t alpha);
ret_t canvas_set_font(canvas_t* c, const char* name, font_size_t size);
ret_t canvas_set_text_align(canvas_t* c, align_h_t align_h, align_v_t align_v);

float_t canvas_measure_text(canvas_t* c, wchar_t* str, uint32_t nr);

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
ret_t canvas_draw_text(canvas_t* c, wchar_t* str, uint32_t nr, xy_t x, xy_t y);
ret_t canvas_draw_text_in_rect(canvas_t* c, wchar_t* str, uint32_t nr, const rect_t* r);
ret_t canvas_draw_image(canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst);

ret_t canvas_draw_image_at(canvas_t* c, bitmap_t* img, xy_t x, xy_t y);
ret_t canvas_draw_icon(canvas_t* c, bitmap_t* img, xy_t cx, xy_t cy);
ret_t canvas_draw_icon_in_rect(canvas_t* c, bitmap_t* img, rect_t* r);

ret_t canvas_draw_image_center(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_patch3_x(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_patch3_x_scale_y(canvas_t* c, bitmap_t* img, rect_t* dst);

ret_t canvas_draw_image_patch3_y(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_patch3_y_scale_x(canvas_t* c, bitmap_t* img, rect_t* dst);

ret_t canvas_draw_image_patch9(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat_x(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_repeat_y(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale_w(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale_h(canvas_t* c, bitmap_t* img, rect_t* dst);
ret_t canvas_draw_image_scale_down(canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst);

ret_t canvas_draw_image_matrix(canvas_t* c, bitmap_t* img, matrix_t* matrix);
ret_t canvas_draw_image_ex(canvas_t* c, bitmap_t* img, image_draw_type_t draw_type,
                           const rect_t* dst);

vgcanvas_t* canvas_get_vgcanvas(canvas_t* c);

ret_t canvas_test_paint(canvas_t* c, bool_t pressed, xy_t x, xy_t y);

ret_t canvas_set_fps(canvas_t* c, bool_t show_fps, uint32_t fps);

ret_t canvas_end_frame(canvas_t* c);

END_C_DECLS

#endif /*TK_CANVAS_H*/
