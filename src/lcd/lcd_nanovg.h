/**
 * File:   lcd_nanovg.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  nanovg implemented lcd interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
#include <SDL2/SDL.h>

BEGIN_C_DECLS

lcd_t* lcd_nanovg_init(SDL_Window* window);

END_C_DECLS

#endif /*LCD_NANOVG_H*/
