/**
 * File:   lcd_rtthread.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rtthread implemented lcd interface/
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_RTT_H
#define LCD_RTT_H

#include "base/lcd.h"
#include "rtgui/driver.h"

BEGIN_C_DECLS

lcd_t* lcd_rtthread_init(struct rtgui_graphic_driver* driver);

END_C_DECLS

#endif /*LCD_RTT_H*/
