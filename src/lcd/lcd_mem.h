/**
 * file:   lcd_mem.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  memory implemented lcd interface/
 *
 * copyright (c) 2018 - 2018  li xianjing <xianjimli@hotmail.com>
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
  uint8_t* pixels;
  vgcanvas_t* vgcanvas;
} lcd_mem_t;

lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc);

END_C_DECLS

#endif /*LCD_TKMEM_H*/
