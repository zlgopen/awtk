/**
 * File:   clip_board_default.c
 * Author: AWTK Develop Team
 * Brief:  clip_board default
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
#include "clip_board/clip_board_default.h"

typedef struct _clip_board_default_t {
  clip_board_t clip_board;
  str_t str;
} clip_board_default_t;

static ret_t clip_board_default_clear(clip_board_t* cl) {
  clip_board_default_t* clip_board = (clip_board_default_t*)cl;

  return str_set(&(clip_board->str), "");
}

static ret_t clip_board_default_set_data(clip_board_t* cl, clip_board_data_type_t type,
                                         const void* data, uint32_t size) {
  const char* str = (const char*)data;
  clip_board_default_t* clip_board = (clip_board_default_t*)cl;
  return_value_if_fail(type == CLIP_BOARD_DATA_TYPE_TEXT, RET_NOT_IMPL);

  return str_set_with_len(&(clip_board->str), str, size);
}

static ret_t clip_board_default_get_data(clip_board_t* cl, clip_board_data_type_t* type,
                                         const void** data, uint32_t* size) {
  clip_board_default_t* clip_board = (clip_board_default_t*)cl;

  if (clip_board->str.size) {
    if (type != NULL) {
      *type = CLIP_BOARD_DATA_TYPE_TEXT;
    }

    if (data != NULL) {
      *data = clip_board->str.str;
    }

    if (size != NULL) {
      *size = clip_board->str.size;
    }

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t clip_board_default_destroy(clip_board_t* cl) {
  clip_board_default_t* clip_board = (clip_board_default_t*)cl;

  str_reset(&(clip_board->str));
  TKMEM_FREE(clip_board);

  return RET_OK;
}

static const clip_board_vtable_t s_clip_board_default_vtable = {
    .clear = clip_board_default_clear,
    .set_data = clip_board_default_set_data,
    .get_data = clip_board_default_get_data,
    .destroy = clip_board_default_destroy};

clip_board_t* clip_board_default_create(void) {
  clip_board_default_t* cl = TKMEM_ZALLOC(clip_board_default_t);
  return_value_if_fail(cl != NULL, NULL);

  cl->clip_board.vt = &s_clip_board_default_vtable;

  return (clip_board_t*)cl;
}
