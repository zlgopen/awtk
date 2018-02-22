/**
 * file:   main_loop_sdl2.h
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  sdl2 implemented main_loop interface
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
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_MAIN_LOOP_SDL2_H
#define LFTK_MAIN_LOOP_SDL2_H

#include "base/main_loop.h"

BEGIN_C_DECLS

main_loop_t* main_loop_sdl2_init(int w, int h);

END_C_DECLS

#endif/*LFTK_MAIN_LOOP_SDL2_H*/

