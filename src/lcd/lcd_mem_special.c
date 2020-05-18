/**
 * File:   lcd_mem_special.c
 * Author: AWTK Develop Team
 * Brief:  lcd_mem_special
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
 * 2019-06-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "lcd/lcd_mem_special.h"
#include "lcd/lcd_mem_rgb565.h"
#include "lcd/lcd_mem_bgr565.h"
#include "lcd/lcd_mem_rgba8888.h"
#include "lcd/lcd_mem_bgra8888.h"
#include "lcd/lcd_mem_bgr888.h"
#include "lcd/lcd_mem_rgb888.h"

static ret_t lcd_mem_special_begin_frame(lcd_t* lcd, rect_t* dr) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->draw_mode = lcd->draw_mode;
  mem->begin_frame(mem, dr);

  return RET_OK;
}

static ret_t lcd_mem_special_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_hline(mem, x, y, w);
}

static ret_t lcd_mem_special_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_vline(mem, x, y, h);
}

static ret_t lcd_mem_special_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->stroke_color = lcd->stroke_color;

  return lcd_draw_points(mem, points, nr);
}

static color_t lcd_mem_special_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);

  return lcd_get_point_color(mem, x, y);
}

static ret_t lcd_mem_special_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->fill_color = lcd->fill_color;

  return lcd_fill_rect(mem, x, y, w, h);
}

static ret_t lcd_mem_special_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  mem->text_color = lcd->text_color;
  mem->fill_color = lcd->fill_color;

  return lcd_draw_glyph(mem, glyph, src, x, y);
}

static ret_t lcd_mem_special_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);

  return lcd_draw_image_matrix(mem, info);
}

static ret_t lcd_mem_special_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);

  return lcd_draw_image(mem, img, src, dst);
}

static ret_t lcd_mem_special_end_frame(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);

  mem->end_frame(mem);

  if (lcd->draw_mode == LCD_DRAW_OFFLINE) {
    return RET_OK;
  }

  return lcd_flush(lcd);
}

static ret_t lcd_mem_special_flush(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  if (special->on_flush != NULL) {
    special->on_flush(lcd);
  }

  return RET_OK;
}

static ret_t lcd_mem_special_destroy(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  lcd_destroy((lcd_t*)(special->lcd_mem));
  if (special->on_destroy != NULL) {
    special->on_destroy(lcd);
  }

  memset(special, 0x00, sizeof(lcd_mem_special_t));
  TKMEM_FREE(special);

  return RET_OK;
}

static ret_t lcd_mem_special_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  return lcd_take_snapshot((lcd_t*)(special->lcd_mem), img, auto_rotate);
}

static bitmap_format_t lcd_mem_special_get_desired_bitmap_format(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  return lcd_get_desired_bitmap_format((lcd_t*)(special->lcd_mem));
}

static vgcanvas_t* lcd_mem_special_get_vgcanvas(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  return lcd_get_vgcanvas((lcd_t*)(special->lcd_mem));
}

static ret_t lcd_mem_special_set_global_alpha(lcd_t* lcd, uint8_t alpha) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  lcd_t* mem = (lcd_t*)(special->lcd_mem);
  vgcanvas_t* vg = lcd_get_vgcanvas(mem);

  return_value_if_fail(vg != NULL && vg->vt != NULL && vg->vt->set_global_alpha != NULL, RET_FAIL);

  lcd->global_alpha = alpha;
  mem->global_alpha = alpha;

  return vg->vt->set_global_alpha(vg, alpha / 255.0f);
}

static lcd_mem_t* lcd_mem_special_create_lcd_mem(wh_t w, wh_t h, bitmap_format_t fmt) {
  switch (fmt) {
    case BITMAP_FMT_RGBA8888: {
      return (lcd_mem_t*)lcd_mem_rgba8888_create(w, h, TRUE);
    }
    case BITMAP_FMT_BGRA8888: {
      return (lcd_mem_t*)lcd_mem_bgra8888_create(w, h, TRUE);
    }
    case BITMAP_FMT_BGR888: {
      return (lcd_mem_t*)lcd_mem_bgr888_create(w, h, TRUE);
    }
    case BITMAP_FMT_RGB888: {
      return (lcd_mem_t*)lcd_mem_rgb888_create(w, h, TRUE);
    }
    case BITMAP_FMT_BGR565: {
      return (lcd_mem_t*)lcd_mem_bgr565_create(w, h, TRUE);
    }
    case BITMAP_FMT_RGB565: {
      return (lcd_mem_t*)lcd_mem_rgb565_create(w, h, TRUE);
    }
    default: {
      log_debug("not supported: w=%d h=%d fmt=%d\n", w, h, fmt);
      return NULL;
    }
  }
}

static ret_t lcd_mem_special_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  return_value_if_fail(special != NULL, RET_BAD_PARAMS);

  lcd_destroy((lcd_t*)(special->lcd_mem));
  special->lcd_mem = lcd_mem_special_create_lcd_mem(w, h, special->format);

  if (special->on_resize != NULL) {
    special->on_resize(lcd, w, h, line_length);
  }

  return RET_OK;
}

lcd_t* lcd_mem_special_create(wh_t w, wh_t h, bitmap_format_t fmt, lcd_flush_t on_flush,
                              lcd_resize_t on_resize, lcd_destroy_t on_destroy, void* ctx) {
  lcd_mem_special_t* special = TKMEM_ZALLOC(lcd_mem_special_t);
  lcd_t* lcd = (lcd_t*)special;
  return_value_if_fail(special != NULL, NULL);

  memset(special, 0x00, sizeof(lcd_mem_special_t));

  lcd->w = w;
  lcd->h = h;
  lcd->ratio = 1;
  lcd->type = LCD_FRAMEBUFFER;
  lcd->support_dirty_rect = TRUE;

  special->ctx = ctx;
  special->format = fmt;
  special->on_flush = on_flush;
  special->on_resize = on_resize;
  special->on_destroy = on_destroy;
  special->lcd_mem = lcd_mem_special_create_lcd_mem(w, h, fmt);
  ENSURE(special->lcd_mem != NULL);

  special->lcd_mem->base.flush = NULL;
  lcd->begin_frame = lcd_mem_special_begin_frame;
  lcd->draw_vline = lcd_mem_special_draw_vline;
  lcd->draw_hline = lcd_mem_special_draw_hline;
  lcd->fill_rect = lcd_mem_special_fill_rect;
  lcd->draw_image = lcd_mem_special_draw_image;
  lcd->draw_image_matrix = lcd_mem_special_draw_image_matrix;
  lcd->draw_glyph = lcd_mem_special_draw_glyph;
  lcd->draw_points = lcd_mem_special_draw_points;
  lcd->get_point_color = lcd_mem_special_get_point_color;
  lcd->end_frame = lcd_mem_special_end_frame;
  lcd->get_vgcanvas = lcd_mem_special_get_vgcanvas;
  lcd->take_snapshot = lcd_mem_special_take_snapshot;
  lcd->set_global_alpha = lcd_mem_special_set_global_alpha;
  lcd->get_desired_bitmap_format = lcd_mem_special_get_desired_bitmap_format;
  lcd->resize = lcd_mem_special_resize;
  lcd->flush = lcd_mem_special_flush;
  lcd->destroy = lcd_mem_special_destroy;

  return lcd;
}
