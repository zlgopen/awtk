/**
 * File:   lcd_mem.h
 * Author: AWTK Develop Team
 * Brief:  memory implemented lcd interface/
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef LCD_TKMEM_H
#define LCD_TKMEM_H

#include "base/lcd.h"

BEGIN_C_DECLS

typedef struct _lcd_mem_t {
  lcd_t base;
  uint8_t* fbuff;
  uint8_t* online_fbuff;
  vgcanvas_t* vgcanvas;
  bitmap_format_t format;
} lcd_mem_t;

lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc);
lcd_t* lcd_mem_create_double_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb);

END_C_DECLS

#endif /*LCD_TKMEM_H*/
