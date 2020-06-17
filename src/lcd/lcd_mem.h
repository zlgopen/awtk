/**
 * File:   lcd_mem.h
 * Author: AWTK Develop Team
 * Brief:  memory implemented lcd interface
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

#ifndef TK_LCD_MEM_H
#define TK_LCD_MEM_H

#include "base/lcd.h"
#include "base/graphic_buffer.h"

BEGIN_C_DECLS

typedef struct _lcd_mem_t {
  lcd_t base;
  uint8_t* offline_fb;
  uint8_t* online_fb;
  uint8_t* next_fb;
  vgcanvas_t* vgcanvas;

  uint32_t online_line_length;
  uint32_t line_length;
  bitmap_format_t format;
  bool_t own_offline_fb;

  graphic_buffer_t* online_gb;
  graphic_buffer_t* offline_gb;

} lcd_mem_t;

#define lcd_mem_set_line_length(lcd, value) \
  ((lcd_mem_t*)lcd)->line_length = ((lcd_mem_t*)lcd)->online_line_length = value;

END_C_DECLS

#endif /*TK_LCD_MEM_H*/
