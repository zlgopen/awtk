﻿/**
 * File:   main_loop_sdl.h
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented main_loop interface
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_MAIN_LOOP_SDL_H
#define TK_MAIN_LOOP_SDL_H

#include "base/main_loop.h"

BEGIN_C_DECLS

main_loop_t* main_loop_init(int w, int h);

/**
 * @method main_loop_sdl2_dispatch
 * dispatch events.
 * @annotation ["global"]
 * @param {main_loop_simple_t*} loop the main loop.
 *
 * @return {ret_t} return RET_OK if successful, otherwise return RET_FAIL.
 */
ret_t main_loop_sdl2_dispatch(main_loop_simple_t* loop);

END_C_DECLS

#endif /*TK_MAIN_LOOP_SDL_H*/
