/**
 * File:   clip_board_sdl.c
 * Author: AWTK Develop Team
 * Brief:  clip_board on SDL
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
 * 2018-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/str.h"

#ifdef WITH_SDL
#include "SDL_clipboard.h"
#include "clip_board/clip_board_sdl.h"

typedef struct _clip_board_sdl_t {
  clip_board_t clip_board;
  str_t str;
} clip_board_sdl_t;

static ret_t clip_board_sdl_clear(clip_board_t* cl) {
  SDL_SetClipboardText("");
  (void)cl;

  return RET_OK;
}

static ret_t clip_board_sdl_set_data(clip_board_t* cl, clip_board_data_type_t type,
                                     const void* data, uint32_t size) {
  const char* str = (const char*)data;
  return_value_if_fail(type == CLIP_BOARD_DATA_TYPE_TEXT, RET_NOT_IMPL);

  SDL_SetClipboardText(str);
  (void)cl;

  return RET_OK;
}

static ret_t clip_board_sdl_get_data(clip_board_t* cl, clip_board_data_type_t* type,
                                     const void** data, uint32_t* size) {
  clip_board_sdl_t* clip_board = (clip_board_sdl_t*)cl;

  if (SDL_HasClipboardText()) {
    char* str = SDL_GetClipboardText();
    return_value_if_fail(str != NULL, RET_FAIL);

    return_value_if_fail(str_set(&(clip_board->str), str) == RET_OK, RET_OOM);

    if (type != NULL) {
      *type = CLIP_BOARD_DATA_TYPE_TEXT;
    }

    if (data != NULL) {
      *data = clip_board->str.str;
    }

    if (size != NULL) {
      *size = clip_board->str.size;
    }

    SDL_free(str);

    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t clip_board_sdl_destroy(clip_board_t* cl) {
  clip_board_sdl_t* clip_board = (clip_board_sdl_t*)cl;

  str_reset(&(clip_board->str));
  TKMEM_FREE(clip_board);

  return RET_OK;
}

static const clip_board_vtable_t s_clip_board_sdl_vtable = {.clear = clip_board_sdl_clear,
                                                            .set_data = clip_board_sdl_set_data,
                                                            .get_data = clip_board_sdl_get_data,
                                                            .destroy = clip_board_sdl_destroy};

clip_board_t* clip_board_sdl_create(void) {
  clip_board_sdl_t* cl = TKMEM_ZALLOC(clip_board_sdl_t);
  return_value_if_fail(cl != NULL, NULL);

  cl->clip_board.vt = &s_clip_board_sdl_vtable;

  return (clip_board_t*)cl;
}

#endif /*WITH_SDL*/
