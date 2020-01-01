/**
 * File:   lcd_log.cc
 * Author: AWTK Develop Team
 * Brief:  log lcd commands
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
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "lcd_log.h"

typedef struct _lcd_log_t {
  lcd_t base;
  string str;
} lcd_log_t;

static string itos(int v) {
  char str[32];
  snprintf(str, sizeof(str), "%d", v);

  return string(str);
}

static ret_t lcd_log_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str += "bf();";

  return RET_OK;
}

static ret_t lcd_log_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str += "dhl(" + (itos(x)) + string(",") + (itos(y)) + string(",") + (itos(w)) + string(");");

  return RET_OK;
}

static ret_t lcd_log_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str += "dvl(" + (itos(x)) + string(",") + (itos(y)) + string(",") + (itos(h)) + string(");");

  return RET_OK;
}

static ret_t lcd_log_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  uint32_t i = 0;
  lcd_log_t* log = (lcd_log_t*)lcd;

  log->str += "dps(";
  for (i = 0; i < nr; i++) {
    point_t* p = points + i;
    log->str += string("(") + (itos(p->x)) + string(",") + (itos(p->y)) + string(")");
  }
  log->str += ");";

  return RET_OK;
}

static ret_t lcd_log_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str += "fr(" + (itos(x)) + string(",") + (itos(y)) + string(",") + (itos(w)) + string(",") +
              (itos(h)) + string(");");

  return RET_OK;
}

static ret_t lcd_log_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  lcd_log_t* log = (lcd_log_t*)lcd;

  log->str += "dg(" + (itos(src->x)) + string(",") + (itos(src->y)) + string(",") + (itos(src->w)) +
              string(",") + (itos(src->h)) + string(",") + (itos(x)) + string(",") + (itos(y)) +
              string(");");

  return RET_OK;
}

static ret_t lcd_log_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  lcd_log_t* log = (lcd_log_t*)lcd;

  log->str += "dg(" + (itos(src->x)) + string(",") + (itos(src->y)) + string(",") + (itos(src->w)) +
              string(",") + (itos(src->h)) + string(",") + (itos(dst->x)) + string(",") +
              (itos(dst->y)) + string(",") + (itos(dst->w)) + string(",") + (itos(dst->h)) +
              string(");");

  return RET_OK;
}

static ret_t lcd_log_end_frame(lcd_t* lcd) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str += "ef();";

  return RET_OK;
}

static ret_t lcd_log_destroy(lcd_t* lcd) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  delete log;
  return RET_OK;
}

lcd_t* lcd_log_init(wh_t w, wh_t h) {
  lcd_log_t* lcd = new lcd_log_t();
  lcd_t* base = &(lcd->base);

  memset(lcd, 0x00, sizeof(lcd_log_t));

  base->begin_frame = lcd_log_begin_frame;
  base->draw_vline = lcd_log_draw_vline;
  base->draw_hline = lcd_log_draw_hline;
  base->fill_rect = lcd_log_fill_rect;
  base->draw_image = lcd_log_draw_image;
  base->draw_glyph = lcd_log_draw_glyph;
  base->draw_points = lcd_log_draw_points;
  base->end_frame = lcd_log_end_frame;
  base->destroy = lcd_log_destroy;

  base->w = w;
  base->h = h;
  base->support_dirty_rect = TRUE;

  return base;
}

ret_t lcd_log_reset(lcd_t* lcd) {
  lcd_log_t* log = (lcd_log_t*)lcd;
  log->str = "";

  return RET_OK;
}

const string& lcd_log_get_commands(lcd_t* lcd) {
  lcd_log_t* log = (lcd_log_t*)lcd;

  return log->str;
}
