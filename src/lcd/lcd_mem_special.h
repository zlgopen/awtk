/**
 * File:   lcd_mem_special.h
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

#ifndef LCD_MEM_SPECIAL_H
#define LCD_MEM_SPECIAL_H

#include "base/lcd.h"
#include "lcd/lcd_mem.h"

BEGIN_C_DECLS

typedef struct _lcd_mem_special_t {
  lcd_t base;
  lcd_mem_t* lcd_mem;
  bitmap_format_t format;

  /*custom info*/
  void* ctx;
  lcd_flush_t on_flush;
  lcd_resize_t on_resize;
  lcd_destroy_t on_destroy;
} lcd_mem_special_t;

lcd_t* lcd_mem_special_create(wh_t w, wh_t h, bitmap_format_t fmt, lcd_flush_t on_flush,
                              lcd_resize_t on_resize, lcd_destroy_t on_destroy, void* ctx);

END_C_DECLS

#endif /*LCD_MEM_SPECIAL_H*/
