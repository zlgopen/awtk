/**
 * File:   lcd_log.h
 * Author: AWTK Develop Team
 * Brief:  log lcd commands.
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

#ifndef LCD_LOG_H
#define LCD_LOG_H

#include <string>
#include "base/lcd.h"

using std::string;

BEGIN_C_DECLS

lcd_t* lcd_log_init(wh_t w, wh_t h);

END_C_DECLS

ret_t lcd_log_reset(lcd_t* lcd);
const string& lcd_log_get_commands(lcd_t* lcd);

#endif /*LCD_LOG_H*/
