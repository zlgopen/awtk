/**
 * File:   lcd_mono.h
 * Author: AWTK Develop Team
 * Brief:  mono lcd
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-10 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_MONO_H
#define LCD_MONO_H

#include "base/lcd.h"

BEGIN_C_DECLS

typedef struct _lcd_mono_t {
  lcd_t lcd;
  uint8_t* data;
  void* ctx;
  lcd_destroy_t on_destroy;
} lcd_mono_t;

lcd_t* lcd_mono_create(wh_t w, wh_t h, lcd_flush_t flush, lcd_destroy_t on_destroy, void* ctx);

END_C_DECLS

#endif /*LCD_MONO_H*/
