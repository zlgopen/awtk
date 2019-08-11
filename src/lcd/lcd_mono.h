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

lcd_t* lcd_mono_create(wh_t w, wh_t h, lcd_flush_t flush);

#define MONO_ROW_SIZE(w) (((w) + 7) >> 3)

uint8_t* bitmap_mono_create_data(uint32_t w, uint32_t h);
bool_t bitmap_mono_get_pixel(const uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y);
ret_t bitmap_mono_set_pixel(uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y,
                            bool_t pixel);

END_C_DECLS

#endif /*LCD_MONO_H*/
