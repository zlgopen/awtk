/**
 * File:   lcd_mem_special.c
 * Author: AWTK Develop Team
 * Brief:  lcd_mem_special
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t lcd_mem_special_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length);

static ret_t lcd_mem_special_flush(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  if (special->on_flush != NULL) {
    special->on_flush(lcd);
  }

  return RET_OK;
}

static ret_t lcd_mem_special_destroy(lcd_t* lcd) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;

  lcd_mem_deinit((lcd_mem_t*)lcd);
  if (special->on_destroy != NULL) {
    special->on_destroy(lcd);
  }

  memset(special, 0x00, sizeof(lcd_mem_special_t));
  TKMEM_FREE(special);

  return RET_OK;
}

static lcd_t* lcd_mem_special_init_lcd_mem(lcd_t* lcd_mem, wh_t w, wh_t h, bitmap_format_t fmt) {
  lcd_t* lcd = NULL;
  switch (fmt) {
    case BITMAP_FMT_RGBA8888: {
      lcd = lcd_mem_rgba8888_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      lcd = lcd_mem_bgra8888_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    case BITMAP_FMT_BGR888: {
      lcd = lcd_mem_bgr888_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    case BITMAP_FMT_RGB888: {
      lcd = lcd_mem_rgb888_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    case BITMAP_FMT_BGR565: {
      lcd = lcd_mem_bgr565_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    case BITMAP_FMT_RGB565: {
      lcd = lcd_mem_rgb565_init((lcd_mem_t*)lcd_mem, w, h, TRUE);
      break;
    }
    default: {
      log_debug("not supported: w=%d h=%d fmt=%d\n", w, h, fmt);
      return NULL;
    }
  }
  if (lcd != NULL) {
    lcd->flush = lcd_mem_special_flush;
    lcd->resize = lcd_mem_special_resize;
    lcd->destroy = lcd_mem_special_destroy;
  }
  return lcd;
}

static ret_t lcd_mem_special_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  lcd_mem_special_t* special = (lcd_mem_special_t*)lcd;
  return_value_if_fail(special != NULL, RET_BAD_PARAMS);

  lcd_mem_deinit((lcd_mem_t*)lcd);
  lcd_mem_special_init_lcd_mem(lcd, w, h, special->format);

  if (special->on_resize != NULL) {
    special->on_resize(lcd, w, h, line_length);
  }

  return RET_OK;
}

lcd_t* lcd_mem_special_create(wh_t w, wh_t h, bitmap_format_t fmt, lcd_flush_t on_flush,
                              lcd_resize_t on_resize, lcd_destroy_t on_destroy, void* ctx) {
  lcd_t* lcd = NULL;
  lcd_mem_special_t* special = TKMEM_ZALLOC(lcd_mem_special_t);
  return_value_if_fail(special != NULL, NULL);

  memset(special, 0x00, sizeof(lcd_mem_special_t));

  special->ctx = ctx;
  special->format = fmt;
  special->on_flush = on_flush;
  special->on_resize = on_resize;
  special->on_destroy = on_destroy;
  lcd = lcd_mem_special_init_lcd_mem((lcd_t*)special, w, h, fmt);
  return_value_if_fail(lcd != NULL, NULL);
  return lcd;
}
