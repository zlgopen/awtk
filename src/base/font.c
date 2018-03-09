/**
 * File:   font.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

ret_t font_find_glyph(font_t* f, wchar_t chr, glyph_t* g) {
  return_value_if_fail(f != NULL && f->find_glyph != NULL && g != NULL, RET_BAD_PARAMS);

  return f->find_glyph(f, chr, g);
}

ret_t font_destroy(font_t* f) {
  return_value_if_fail(f != NULL && f->destroy != NULL, RET_BAD_PARAMS);

  return f->destroy(f);
}
