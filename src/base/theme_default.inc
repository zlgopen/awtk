/**
 * File:   theme_default.c
 * Author: AWTK Develop Team
 * Brief:  theme default impl
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/theme.h"
#include "tkc/buffer.h"

static const uint8_t* theme_default_find_style(theme_t* theme, const char* widget_type,
                                               const char* name, const char* widget_state) {
  uint32_t i = 0;
  const theme_item_t* iter = NULL;
  const theme_header_t* header = NULL;
  return_value_if_fail(theme != NULL, NULL);
  return_value_if_fail(theme->data != NULL, NULL);

  header = (const theme_header_t*)(theme->data);
  if (name == NULL) {
    name = TK_DEFAULT_STYLE;
  }

  iter = (const theme_item_t*)(theme->data + sizeof(theme_header_t));
  for (i = 0; i < header->nr; i++) {
    if (tk_str_eq(widget_type, iter->widget_type)) {
      if (tk_str_eq(iter->state, widget_state) && tk_str_eq(iter->name, name)) {
        return theme->data + iter->offset;
      }
    }
    iter++;
  }

  return NULL;
}

static ret_t theme_default_foreach(theme_t* theme, theme_on_data_t on_data, void* ctx) {
  uint32_t i = 0;
  const theme_item_t* iter = NULL;
  const theme_header_t* header = NULL;
  return_value_if_fail(theme != NULL, RET_OK);
  return_value_if_fail(theme->data != NULL, RET_OK);

  header = (const theme_header_t*)(theme->data);
  iter = (const theme_item_t*)(theme->data + sizeof(theme_header_t));

  for (i = 0; i < header->nr; i++) {
    const uint8_t* data = theme->data + iter->offset;
    if (on_data(ctx, iter->widget_type, iter->name, iter->state, data) == RET_STOP) {
      break;
    }
    iter++;
  }

  return RET_OK;
}

theme_t* theme_default_create_ex(const uint8_t* data, bool_t need_free_data) {
  theme_t* theme = TKMEM_ZALLOC(theme_t);
  return_value_if_fail(theme != NULL, NULL);

  theme->data = data;
  theme->foreach = theme_default_foreach;
  theme->need_free_data = need_free_data;
  theme->find_style = theme_default_find_style;

  return theme;
}

theme_t* theme_default_create(const uint8_t* data) {
  return theme_default_create_ex(data, FALSE);
}
