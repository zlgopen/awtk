/**
 * file:   lcd_stm32_raw.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  stm32_rawory implemented lcd interface/
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
 * 2018-02-16 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_STM32_RAW_H
#define LCD_STM32_RAW_H

#include "base/lcd.h"

BEGIN_C_DECLS

typedef void (*tft_write_data_t)(uint16_t dat);
typedef void (*tft_set_window_t)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

lcd_t* lcd_stm32_raw_create(wh_t w, wh_t h, tft_write_data_t write_data,
                            tft_set_window_t set_window);

END_C_DECLS

#endif /*LCD_STM32_RAW_H*/
