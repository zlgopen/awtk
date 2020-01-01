/**
 * File:   native_window_sdl.h
 * Author: AWTK Develop Team
 * Brief:  native window sdl
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NATIVE_WINDOW_SDL_H
#define TK_NATIVE_WINDOW_SDL_H

#include "base/native_window.h"

BEGIN_C_DECLS

ret_t native_window_sdl_deinit(void);
ret_t native_window_sdl_init(bool_t shared, uint32_t w, uint32_t h);

END_C_DECLS

#endif /*TK_NATIVE_WINDOW_SDL_H*/
