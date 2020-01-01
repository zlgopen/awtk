/**
 * File:   clip_board.h
 * Author: AWTK Develop Team
 * Brief:  clip_board interface
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

#include "base/clip_board.h"

static clip_board_t* s_clip_board = NULL;

clip_board_t* clip_board(void) {
  return s_clip_board;
}

ret_t clip_board_set(clip_board_t* cl) {
  s_clip_board = cl;

  return RET_OK;
}

ret_t clip_board_clear(clip_board_t* cl) {
  return_value_if_fail(cl != NULL && cl->vt->clear != NULL, RET_BAD_PARAMS);

  return cl->vt->clear(cl);
}

ret_t clip_board_set_data(clip_board_t* cl, clip_board_data_type_t type, const void* data,
                          uint32_t size) {
  return_value_if_fail(cl != NULL && cl->vt->set_data != NULL && data != NULL, RET_BAD_PARAMS);

  return cl->vt->set_data(cl, type, data, size);
}

ret_t clip_board_get_data(clip_board_t* cl, clip_board_data_type_t* type, const void** data,
                          uint32_t* size) {
  return_value_if_fail(cl != NULL && cl->vt->get_data != NULL, RET_BAD_PARAMS);

  return cl->vt->get_data(cl, type, data, size);
}

ret_t clip_board_destroy(clip_board_t* cl) {
  if (cl != NULL && cl->vt->destroy != NULL) {
    cl->vt->destroy(cl);
  }

  return RET_OK;
}

ret_t clip_board_set_text(const char* text) {
  return_value_if_fail(text != NULL && s_clip_board != NULL, RET_BAD_PARAMS);

  return clip_board_set_data(s_clip_board, CLIP_BOARD_DATA_TYPE_TEXT, text, strlen(text));
}

const char* clip_board_get_text(void) {
  uint32_t size = 0;
  const void* data = NULL;
  clip_board_data_type_t type = CLIP_BOARD_DATA_TYPE_NONE;

  return_value_if_fail(s_clip_board != NULL, "");
  return_value_if_fail(clip_board_get_data(s_clip_board, &type, &data, &size) == RET_OK, "");

  if (type == CLIP_BOARD_DATA_TYPE_TEXT) {
    return (const char*)data;
  }

  return "";
}
