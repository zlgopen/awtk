/**
 * file:   lcd_reg.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  register based implemented lcd interface/
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

#ifndef LCD_REG_H
#define LCD_REG_H

#include "base/lcd.h"

BEGIN_C_DECLS

lcd_t* lcd_reg_create(wh_t w, wh_t h);

END_C_DECLS

#endif /*LCD_REG_H*/
