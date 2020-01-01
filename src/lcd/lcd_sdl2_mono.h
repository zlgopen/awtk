/**
 * File:   lcd_sdl2_mono.h
 * Author: AWTK Develop Team
 * Brief:  mono lcd on sdl2
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
 * 2010-08-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_SDL_MONO_H
#define LCD_SDL_MONO_H

#include <SDL.h>
#include "base/lcd.h"

BEGIN_C_DECLS

lcd_t* lcd_sdl2_mono_init(SDL_Renderer* render);

END_C_DECLS

#endif /*LCD_SDL_MONO_H*/
