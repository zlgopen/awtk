/**
 * File:   clip_board_default.h
 * Author: AWTK Develop Team
 * Brief:  clip_board on SDL
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
 * 2018-11-21 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CLIP_BOARD_SDL_H
#define TK_CLIP_BOARD_SDL_H

#include "base/clip_board.h"

BEGIN_C_DECLS

clip_board_t* clip_board_sdl_create(void);

END_C_DECLS

#endif /*TK_CLIP_BOARD_SDL_H*/
