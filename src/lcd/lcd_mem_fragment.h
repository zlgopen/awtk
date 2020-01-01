/**
 * File:   lcd_mem_fragment.h
 * Author: AWTK Develop Team
 * Brief:  mem fragment based implemented lcd interface
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
 * 2019-11-14 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_MEM_FRAGMENT_H
#define LCD_MEM_FRAGMENT_H

#include "base/lcd.h"

BEGIN_C_DECLS

lcd_t* lcd_mem_fragment_create(wh_t w, wh_t h);

END_C_DECLS

#endif /*LCD_MEM_FRAGMENT_H*/
