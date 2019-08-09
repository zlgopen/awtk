/**
 * File:   theme.c
 * Author: AWTK Develop Team
 * Brief:  theme manager
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

color_t style_data_get_color(const uint8_t* s, const char* name, color_t defval) {
  defval.color = style_data_get_int(s, name, defval.color);

  return defval;
}

uint32_t style_data_get_int(const uint8_t* s, const char* name, uint32_t defval) {
  uint32_t i = 0;
  uint32_t nr = 0;
  const uint8_t* p = s;

  if (s == NULL) {
    return defval;
  }

  load_uint32(p, nr);
  for (i = 0; i < nr; i++) {
    const style_int_data_t* iter = (const style_int_data_t*)p;

    if (tk_str_eq(iter->name, name)) {
      defval = iter->value;
      break;
    } else {
      p += sizeof(style_int_data_t);
    }
  }

  return defval;
}

const char* style_data_get_str(const uint8_t* s, const char* name, const char* defval) {
  uint32_t i = 0;
  uint32_t nr = 0;
  const uint8_t* p = s;

  if (s == NULL) {
    return defval;
  }

  /*skip int values*/
  load_uint32(p, nr);
  p += nr * sizeof(style_int_data_t);

  load_uint32(p, nr);
  for (i = 0; i < nr; i++) {
    const style_str_data_t* iter = (const style_str_data_t*)p;
    if (tk_str_eq(iter->name, name)) {
      defval = iter->value;
      break;
    } else {
      p += sizeof(style_str_data_t);
    }
  }

  return defval;
}

const uint8_t* theme_find_style(theme_t* t, const char* widget_type, const char* name,
                                const char* widget_state) {
  uint32_t i = 0;
  const theme_item_t* iter = NULL;
  const theme_header_t* header = NULL;
  return_value_if_fail(t != NULL && t->data != NULL, NULL);

  header = (const theme_header_t*)(t->data);
  if (name == NULL) {
    name = TK_DEFAULT_STYLE;
  }

  iter = (const theme_item_t*)(t->data + sizeof(theme_header_t));
  for (i = 0; i < header->nr; i++) {
    if (tk_str_eq(widget_type, iter->widget_type)) {
      if (tk_str_eq(iter->state, widget_state) && tk_str_eq(iter->name, name)) {
        return t->data + iter->offset;
      }
    }

    iter++;
  }

  return NULL;
}

static theme_t* s_theme;

theme_t* theme(void) {
  return s_theme;
}

ret_t theme_set(theme_t* theme) {
  s_theme = theme;

  return RET_OK;
}

theme_t* theme_create(const uint8_t* data) {
  theme_t* theme = TKMEM_ZALLOC(theme_t);
  return_value_if_fail(theme != NULL, NULL);

  return theme_init(theme, data);
}

theme_t* theme_init(theme_t* theme, const uint8_t* data) {
  return_value_if_fail(theme != NULL, NULL);

  theme->data = data;

  return theme;
}

ret_t theme_deinit(theme_t* theme) {
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);

  theme->data = NULL;

  return RET_OK;
}

ret_t theme_destroy(theme_t* theme) {
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);

  theme_deinit(theme);

  TKMEM_FREE(theme);

  return RET_OK;
}
