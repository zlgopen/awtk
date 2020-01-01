/**
 * File:   lcd_nanovg.h
 * Author: AWTK Develop Team
 * Brief:  nanovg implemented lcd interface
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
 * 2018-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_NANOVG_H
#define LCD_NANOVG_H

#include "base/lcd.h"
#include "base/native_window.h"

BEGIN_C_DECLS

lcd_t* lcd_nanovg_init(native_window_t* window);

END_C_DECLS

#endif /*LCD_NANOVG_H*/
