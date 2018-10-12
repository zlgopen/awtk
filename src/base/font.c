/**
 * File:   font.c
 * Author: AWTK Develop Team
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/font.h"

ret_t font_find_glyph(font_t* f, wchar_t chr, glyph_t* g, uint16_t font_size) {
  return_value_if_fail(f != NULL && f->find_glyph != NULL && g != NULL, RET_BAD_PARAMS);

  return f->find_glyph(f, chr, g, font_size);
}

int32_t font_get_baseline(font_t* f, uint16_t font_size) {
  if (f != NULL && f->get_baseline != NULL) {
    return f->get_baseline(f, font_size);
  } else {
    return font_size;
  }
}

bool_t font_match(font_t* f, const char* name, uint16_t font_size) {
  return_value_if_fail(f != NULL && f->match != NULL, FALSE);

  return f->match(f, name, font_size);
}

ret_t font_destroy(font_t* f) {
  return_value_if_fail(f != NULL && f->destroy != NULL, RET_BAD_PARAMS);

  return f->destroy(f);
}
