/**
 * File:   lcd.c
 * Author: AWTK Develop Team
 * Brief:  lcd interface
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/time_now.h"
#include "base/system_info.h"

ret_t lcd_begin_frame(lcd_t* lcd, rect_t* dirty_rect, lcd_draw_mode_t draw_mode) {
  return_value_if_fail(lcd != NULL && lcd->begin_frame != NULL, RET_BAD_PARAMS);

  lcd->draw_mode = draw_mode;
  if (dirty_rect == NULL) {
    lcd->dirty_rect.x = 0;
    lcd->dirty_rect.y = 0;
    lcd->dirty_rect.w = lcd->w;
    lcd->dirty_rect.h = lcd->h;
  } else {
    lcd->dirty_rect = *dirty_rect;
    rect_fix(&(lcd->dirty_rect), lcd->w, lcd->h);
  }

  return lcd->begin_frame(lcd, dirty_rect);
}

ret_t lcd_set_clip_rect(lcd_t* lcd, rect_t* rect) {
  return_value_if_fail(lcd != NULL && lcd->set_clip_rect != NULL, RET_BAD_PARAMS);

  return lcd->set_clip_rect(lcd, rect);
}

ret_t lcd_get_clip_rect(lcd_t* lcd, rect_t* rect) {
  return_value_if_fail(lcd != NULL && lcd->get_clip_rect != NULL, RET_BAD_PARAMS);

  return lcd->get_clip_rect(lcd, rect);
}

ret_t lcd_set_global_alpha(lcd_t* lcd, uint8_t alpha) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->global_alpha = alpha;
  if (lcd->set_global_alpha != NULL) {
    lcd->set_global_alpha(lcd, alpha);
  }

  return RET_OK;
}

ret_t lcd_set_text_color(lcd_t* lcd, color_t color) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->text_color = color;
  if (lcd->set_text_color != NULL) {
    lcd->set_text_color(lcd, color);
  }

  return RET_OK;
}

ret_t lcd_set_stroke_color(lcd_t* lcd, color_t color) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->stroke_color = color;
  if (lcd->set_stroke_color != NULL) {
    lcd->set_stroke_color(lcd, color);
  }

  return RET_OK;
}

ret_t lcd_set_fill_color(lcd_t* lcd, color_t color) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->fill_color = color;
  if (lcd->set_fill_color != NULL) {
    lcd->set_fill_color(lcd, color);
  }

  return RET_OK;
}

ret_t lcd_set_font_name(lcd_t* lcd, const char* name) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->font_name = name;
  if (lcd->set_font_name != NULL) {
    lcd->set_font_name(lcd, name);
  }

  return RET_OK;
}

ret_t lcd_set_font_size(lcd_t* lcd, uint32_t size) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd->font_size = size;
  if (lcd->set_font_size != NULL) {
    lcd->set_font_size(lcd, size);
  }

  return RET_OK;
}

ret_t lcd_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  return_value_if_fail(lcd != NULL && lcd->draw_vline != NULL, RET_BAD_PARAMS);

  if (h == 0) {
    return RET_BAD_PARAMS;
  }

  return lcd->draw_vline(lcd, x, y, h);
}

ret_t lcd_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  return_value_if_fail(lcd != NULL && lcd->draw_hline != NULL, RET_BAD_PARAMS);

  if (w == 0) {
    return RET_BAD_PARAMS;
  }

  return lcd->draw_hline(lcd, x, y, w);
}

ret_t lcd_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(lcd != NULL && lcd->fill_rect != NULL, RET_BAD_PARAMS);
  if (w == 0 || h == 0) {
    return RET_BAD_PARAMS;
  }

  return lcd->fill_rect(lcd, x, y, w, h);
}

ret_t lcd_stroke_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(lcd != NULL && lcd->stroke_rect != NULL, RET_BAD_PARAMS);

  if (w == 0 || h == 0) {
    return RET_BAD_PARAMS;
  }

  return lcd->stroke_rect(lcd, x, y, w, h);
}

ret_t lcd_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  return_value_if_fail(lcd != NULL && lcd->draw_points != NULL && points != NULL, RET_BAD_PARAMS);

  return lcd->draw_points(lcd, points, nr);
}

color_t lcd_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  color_t c = color_init(0, 0, 0, 0);
  return_value_if_fail(lcd != NULL && x >= 0 && y >= 0 && x < lcd->w && y < lcd->h, c);

  if (lcd->get_point_color != NULL) {
    return lcd->get_point_color(lcd, x, y);
  } else {
    return lcd->fill_color;
  }
}

ret_t lcd_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  return_value_if_fail(lcd != NULL && lcd->draw_image != NULL && src != NULL && dst != NULL,
                       RET_BAD_PARAMS);

  return lcd->draw_image(lcd, img, src, dst);
}

ret_t lcd_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  return_value_if_fail(lcd != NULL && info != NULL, RET_BAD_PARAMS);

  if (lcd->draw_image_matrix != NULL) {
    return lcd->draw_image_matrix(lcd, info);
  }

  return RET_NOT_IMPL;
}

ret_t lcd_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  return_value_if_fail(lcd != NULL && lcd->draw_glyph != NULL && glyph != NULL && src != NULL,
                       RET_BAD_PARAMS);

  return lcd->draw_glyph(lcd, glyph, src, x, y);
}

float_t lcd_measure_text(lcd_t* lcd, const wchar_t* str, uint32_t nr) {
  return_value_if_fail(nr < 10240, 0.0f);
  return_value_if_fail(lcd != NULL && lcd->measure_text != NULL && str != NULL, 0.0f);

  return lcd->measure_text(lcd, str, nr);
}

ret_t lcd_draw_text(lcd_t* lcd, const wchar_t* str, uint32_t nr, xy_t x, xy_t y) {
  return_value_if_fail(nr < 10240, RET_BAD_PARAMS);
  return_value_if_fail(lcd != NULL && lcd->draw_text != NULL && str != NULL, RET_OK);

  return lcd->draw_text(lcd, str, nr, x, y);
}

ret_t lcd_end_frame(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->end_frame != NULL, RET_BAD_PARAMS);

  return_value_if_fail(lcd->end_frame(lcd) == RET_OK, RET_FAIL);
  lcd->last_update_time = time_now_ms();

  return RET_OK;
}

ret_t lcd_swap(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  if (lcd->swap != NULL) {
    return lcd->swap(lcd);
  }

  return RET_OK;
}

ret_t lcd_flush(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  if (lcd->flush != NULL) {
    return lcd->flush(lcd);
  }

  return RET_OK;
}

ret_t lcd_sync(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  if (lcd->sync != NULL) {
    return lcd->sync(lcd);
  }

  return RET_OK;
}

bool_t lcd_is_swappable(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, FALSE);

  if (lcd->swap != NULL) {
    return system_info()->lcd_orientation == LCD_ORIENTATION_0;
  }

  return FALSE;
}

ret_t lcd_destroy(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->destroy != NULL, RET_BAD_PARAMS);

  return lcd->destroy(lcd);
}

vgcanvas_t* lcd_get_vgcanvas(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, NULL);

  if (lcd->get_vgcanvas != NULL) {
    return lcd->get_vgcanvas(lcd);
  }

  return NULL;
}

ret_t lcd_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  return_value_if_fail(lcd != NULL && lcd->take_snapshot != NULL, RET_BAD_PARAMS);

  return lcd->take_snapshot(lcd, img, auto_rotate);
}

bitmap_format_t lcd_get_desired_bitmap_format(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL && lcd->get_desired_bitmap_format != NULL, BITMAP_FMT_BGR565);

  return lcd->get_desired_bitmap_format(lcd);
}

ret_t lcd_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);
  lcd->w = w;
  lcd->h = h;

  if (lcd->resize != NULL) {
    lcd->resize(lcd, w, h, line_length);
  }

  return RET_OK;
}

wh_t lcd_get_width(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, 0);

  if (lcd->get_width != NULL) {
    return lcd->get_width(lcd);
  } else {
    return lcd->w;
  }
}

wh_t lcd_get_height(lcd_t* lcd) {
  return_value_if_fail(lcd != NULL, 0);

  if (lcd->get_height != NULL) {
    return lcd->get_height(lcd);
  } else {
    return lcd->h;
  }
}

ret_t lcd_get_text_metrics(lcd_t* lcd, float_t* ascent, float_t* descent, float_t* line_hight) {
  return_value_if_fail(lcd != NULL && lcd->get_text_metrics != NULL, RET_BAD_PARAMS);

  return lcd->get_text_metrics(lcd, ascent, descent, line_hight);
}
