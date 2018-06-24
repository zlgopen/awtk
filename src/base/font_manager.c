/**
 * File:   font_manager.c
 * Author: AWTK Develop Team
 * Brief:  font manager
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

#include "base/mem.h"
#include "base/font_manager.h"

static font_manager_t* s_font_manager = NULL;

font_manager_t* font_manager(void) {
  return s_font_manager;
}

ret_t font_manager_set(font_manager_t* fm) {
  s_font_manager = fm;

  return RET_OK;
}

font_manager_t* font_manager_create(void) {
  font_manager_t* fm = TKMEM_ZALLOC(font_manager_t);

  return font_manager_init(fm);
}

font_manager_t* font_manager_init(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, NULL);
  array_init(&(fm->fonts), 2);

  return fm;
}

ret_t font_manager_add(font_manager_t* fm, font_t* font) {
  return_value_if_fail(fm != NULL && font != NULL, RET_BAD_PARAMS);

  return array_push(&(fm->fonts), font);
}

font_t* font_manager_find(font_manager_t* fm, const char* name, uint16_t size) {
  uint32_t i = 0;
  uint32_t nr = 0;
  font_t** fonts = NULL;
  return_value_if_fail(fm != NULL, NULL);

  nr = fm->fonts.size;
  fonts = (font_t**)fm->fonts.elms;
  return_value_if_fail(nr > 0, NULL);

  if (name == NULL) {
    name = TK_DEFAULT_FONT;
  }

  for (i = 0; i < nr; i++) {
    font_t* iter = fonts[i];
    if (font_match(iter, name, size)) {
      return iter;
    }
  }

  return fonts[0];
}

ret_t font_manager_deinit(font_manager_t* fm) {
  uint32_t i = 0;
  uint32_t nr = 0;
  font_t** fonts = NULL;
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  nr = fm->fonts.size;
  fonts = (font_t**)fm->fonts.elms;
  for (i = 0; i < nr; i++) {
    font_t* iter = fonts[i];
    font_destroy(iter);
  }

  array_deinit(&(fm->fonts));

  return RET_OK;
}

ret_t font_manager_destroy(font_manager_t* fm) {
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);
  font_manager_deinit(fm);
  TKMEM_FREE(fm);

  return RET_OK;
}
