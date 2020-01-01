/**
 * File:   lcd_sdl2.h
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_SDL_H
#define LCD_SDL_H

#include "base/lcd.h"
#include <SDL.h>

BEGIN_C_DECLS

lcd_t* lcd_sdl2_init(SDL_Renderer* render);

END_C_DECLS

#endif /*LCD_SDL_H*/
