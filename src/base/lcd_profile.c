/**
 * File:   lcd_profile.c
 * Author: AWTK Develop Team
 * Brief:  wrap lcd for performance profile.
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
 * 2018-03-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "base/lcd_profile.h"

#ifdef ENABLE_PERFORMANCE_PROFILE
static ret_t lcd_profile_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  ret_t ret = RET_OK;
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  profile->draw_image_times = 0;
  profile->draw_image_cost = 0;
  profile->draw_image_pixels = 0;
  profile->draw_text_times = 0;
  profile->draw_text_cost = 0;
  profile->draw_text_chars = 0;
  profile->fill_times = 0;
  profile->fill_cost = 0;
  profile->fill_pixels = 0;
  profile->stroke_times = 0;
  profile->stroke_cost = 0;
  profile->stroke_pixels = 0;

  profile->begin_frame_time = time_now_ms();
  ret = lcd_begin_frame(profile->impl, dirty_rect, lcd->draw_mode);

  return ret;
}

static ret_t lcd_profile_set_clip_rect(lcd_t* lcd, rect_t* rect) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_clip_rect(profile->impl, rect);
}

static ret_t lcd_profile_get_clip_rect(lcd_t* lcd, rect_t* rect) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_get_clip_rect(profile->impl, rect);
}

static ret_t lcd_profile_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_resize(profile->impl, w, h, line_length);
}

static ret_t lcd_profile_set_global_alpha(lcd_t* lcd, uint8_t alpha) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_global_alpha(profile->impl, alpha);
}

static ret_t lcd_profile_set_text_color(lcd_t* lcd, color_t color) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_text_color(profile->impl, color);
}

static ret_t lcd_profile_set_stroke_color(lcd_t* lcd, color_t color) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_stroke_color(profile->impl, color);
}

static ret_t lcd_profile_set_fill_color(lcd_t* lcd, color_t color) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_fill_color(profile->impl, color);
}

static ret_t lcd_profile_set_font_name(lcd_t* lcd, const char* name) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_font_name(profile->impl, name);
}

static ret_t lcd_profile_set_font_size(lcd_t* lcd, uint32_t font_size) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_set_font_size(profile->impl, font_size);
}

static ret_t lcd_profile_draw_vline(lcd_t* lcd, xy_t x, xy_t y, xy_t h) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_vline(profile->impl, x, y, h);
  cost = time_now_ms() - start;

  profile->stroke_times++;
  profile->stroke_cost += cost;
  profile->stroke_pixels += h;

  return ret;
}

static ret_t lcd_profile_draw_hline(lcd_t* lcd, xy_t x, xy_t y, xy_t w) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_hline(profile->impl, x, y, w);
  cost = time_now_ms() - start;

  profile->stroke_times++;
  profile->stroke_cost += cost;
  profile->stroke_pixels += w;

  return ret;
}

static ret_t lcd_profile_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_points(profile->impl, points, nr);
  cost = time_now_ms() - start;

  profile->stroke_times++;
  profile->stroke_cost += cost;
  profile->stroke_pixels += nr;

  return ret;
}

static color_t lcd_profile_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_get_point_color(profile->impl, x, y);
}

static ret_t lcd_profile_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_fill_rect(profile->impl, x, y, w, h);
  cost = time_now_ms() - start;

  profile->fill_times++;
  profile->fill_cost += cost;
  profile->fill_pixels += w * h;

  return ret;
}

static ret_t lcd_profile_stroke_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_stroke_rect(profile->impl, x, y, w, h);
  cost = time_now_ms() - start;

  profile->stroke_times++;
  profile->stroke_cost += cost;
  profile->stroke_pixels += w * h;

  return ret;
}

static ret_t lcd_profile_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_glyph(profile->impl, glyph, src, x, y);
  cost = time_now_ms() - start;

  profile->draw_text_times++;
  profile->draw_text_cost += cost;
  profile->draw_text_chars++;

  return ret;
}

static float_t lcd_profile_measure_text(lcd_t* lcd, const wchar_t* str, uint32_t nr) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_measure_text(profile->impl, str, nr);
}

static ret_t lcd_profile_draw_text(lcd_t* lcd, const wchar_t* str, uint32_t nr, xy_t x, xy_t y) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_text(profile->impl, str, nr, x, y);
  cost = time_now_ms() - start;

  profile->draw_text_times++;
  profile->draw_text_cost += cost;
  profile->draw_text_chars += nr;

  return ret;
}

static ret_t lcd_profile_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_image(profile->impl, img, src, dst);
  cost = time_now_ms() - start;

  profile->draw_image_times++;
  profile->draw_image_cost += cost;
  profile->draw_image_pixels += dst->w * dst->h;

  return ret;
}

static ret_t lcd_profile_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_draw_image_matrix(profile->impl, info);
  cost = time_now_ms() - start;

  profile->draw_image_times++;
  profile->draw_image_cost += cost;

  return ret;
}

static vgcanvas_t* lcd_profile_get_vgcanvas(lcd_t* lcd) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_get_vgcanvas(profile->impl);
}

static ret_t lcd_profile_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_take_snapshot(profile->impl, img, auto_rotate);
  cost = time_now_ms() - start;

  return ret;
}

static bitmap_format_t lcd_profile_get_desired_bitmap_format(lcd_t* lcd) {
  lcd_profile_t* profile = LCD_PROFILE(lcd);

  return lcd_get_desired_bitmap_format(profile->impl);
}

static ret_t lcd_profile_swap(lcd_t* lcd) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_swap(profile->impl);
  cost = time_now_ms() - start;

  profile->swap_cost = cost;

  return ret;
}

static ret_t lcd_profile_flush(lcd_t* lcd) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_flush(profile->impl);
  cost = time_now_ms() - start;

  profile->flush_cost = cost;

  return ret;
}

static ret_t lcd_profile_end_frame(lcd_t* lcd) {
  ret_t ret = RET_OK;

  uint32_t cost = 0;
  uint32_t end = 0;
  uint64_t start = time_now_ms();
  lcd_profile_t* profile = LCD_PROFILE(lcd);
  ret = lcd_end_frame(profile->impl);
  end = time_now_ms();
  cost = end - start;

  profile->total_cost = time_now_ms() - profile->begin_frame_time;

  if (profile->total_cost > 25) {
    log_debug("#####################################\n");
    log_debug("%s:%d\n", __FILE__, __LINE__);
    log_debug("-------------------------------------\n");
    log_debug("  total_cost=%u \n", profile->total_cost);
    log_debug("  draw_image_cost=%u times=%u\n", profile->draw_image_cost,
              profile->draw_image_times);
    log_debug("  draw_text_cost=%u times=%u\n", profile->draw_text_cost, profile->draw_text_times);
    log_debug("  fill_cost=%u times=%u\n", profile->fill_cost, profile->fill_times);
    log_debug("  stroke_cost=%u times=%u\n", profile->stroke_cost, profile->stroke_times);

    if (cost) {
      log_debug("  end_frame_cost=%u\n", cost);
    }
    if (profile->flush_cost) {
      log_debug("  flush_cost=%u\n", profile->flush_cost);
    }
    if (profile->swap_cost) {
      log_debug("  swap_cost=%u\n", profile->swap_cost);
    }
    log_debug("-------------------------------------\n");
  }

  return ret;
}

lcd_t* lcd_profile_create(lcd_t* impl) {
  lcd_t* lcd = NULL;

  return_value_if_fail(impl != NULL, NULL);
  lcd = (lcd_t*)TKMEM_ZALLOC(lcd_profile_t);
  return_value_if_fail(lcd != NULL, NULL);

  lcd->w = impl->w;
  lcd->h = impl->h;
  lcd->type = impl->type;
  lcd->ratio = impl->ratio;
  LCD_PROFILE(lcd)->impl = impl;
  lcd->support_dirty_rect = impl->support_dirty_rect;

  lcd->begin_frame = lcd_profile_begin_frame;
  lcd->set_global_alpha = lcd_profile_set_global_alpha;
  lcd->set_text_color = lcd_profile_set_text_color;
  lcd->set_stroke_color = lcd_profile_set_stroke_color;
  lcd->set_fill_color = lcd_profile_set_fill_color;

  if (impl->set_font_name != NULL) {
    lcd->set_font_name = lcd_profile_set_font_name;
    lcd->set_font_size = lcd_profile_set_font_size;
  }

  if (impl->set_clip_rect != NULL) {
    lcd->set_clip_rect = lcd_profile_set_clip_rect;
  }

  if (impl->get_clip_rect != NULL) {
    lcd->get_clip_rect = lcd_profile_get_clip_rect;
  }

  if (impl->resize != NULL) {
    lcd->resize = lcd_profile_resize;
  }

  if (impl->draw_vline != NULL) {
    lcd->draw_vline = lcd_profile_draw_vline;
  }

  if (impl->draw_hline != NULL) {
    lcd->draw_hline = lcd_profile_draw_hline;
  }

  if (impl->draw_points != NULL) {
    lcd->draw_points = lcd_profile_draw_points;
  }

  if (impl->get_point_color != NULL) {
    lcd->get_point_color = lcd_profile_get_point_color;
  }

  if (impl->fill_rect != NULL) {
    lcd->fill_rect = lcd_profile_fill_rect;
  }

  if (impl->stroke_rect != NULL) {
    lcd->stroke_rect = lcd_profile_stroke_rect;
  }

  if (impl->draw_glyph != NULL) {
    lcd->draw_glyph = lcd_profile_draw_glyph;
  }

  if (impl->measure_text != NULL) {
    lcd->measure_text = lcd_profile_measure_text;
  }

  if (impl->draw_text != NULL) {
    lcd->draw_text = lcd_profile_draw_text;
  }

  if (impl->draw_image != NULL) {
    lcd->draw_image = lcd_profile_draw_image;
  }

  if (impl->draw_image_matrix != NULL) {
    lcd->draw_image_matrix = lcd_profile_draw_image_matrix;
  }

  if (impl->get_vgcanvas != NULL) {
    lcd->get_vgcanvas = lcd_profile_get_vgcanvas;
  }

  if (impl->take_snapshot != NULL) {
    lcd->take_snapshot = lcd_profile_take_snapshot;
  }

  if (impl->get_desired_bitmap_format != NULL) {
    lcd->get_desired_bitmap_format = lcd_profile_get_desired_bitmap_format;
  }

  if (impl->swap != NULL) {
    lcd->swap = lcd_profile_swap;
  }

  if (impl->flush != NULL) {
    lcd->flush = lcd_profile_flush;
  }

  if (impl->end_frame != NULL) {
    lcd->end_frame = lcd_profile_end_frame;
  }

  return lcd;
}
#else
lcd_t* lcd_profile_create(lcd_t* impl) {
  return impl;
}
#endif /*ENABLE_PERFORMANCE_PROFILE*/
