/**
 * File:   lcd_mem.c
 * Author: AWTK Develop Team
 * Brief:  mem implemented lcd interface/
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

#define LCD_FORMAT BITMAP_FMT_RGBA

#include "base/mem.h"
#include "lcd/lcd_mem.h"

typedef uint32_t pixel_t;
#include "blend/rgba8888.inc"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_mem.inc"

lcd_t* lcd_mem_rgba8888_create(wh_t w, wh_t h, bool_t alloc) {
  return lcd_mem_create(w, h, alloc);
}

lcd_t* lcd_mem_rgba8888_create_single_fb(wh_t w, wh_t h, uint8_t* fbuff) {
  return lcd_mem_create_single_fb(w, h, fbuff);
}

lcd_t* lcd_mem_rgba8888_create_double_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb) {
  return lcd_mem_create_double_fb(w, h, online_fb, offline_fb);
}

lcd_t* lcd_mem_rgba8888_create_three_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb, uint8_t* next_fb) {
  return lcd_mem_create_three_fb(w, h, online_fb, offline_fb, next_fb);
}
