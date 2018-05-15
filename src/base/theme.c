/**
 * File:   theme.c
 * Author: AWTK Develop Team
 * Brief:  theme manager
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/theme.h"

color_t style_get_color(style_t* s, uint32_t name, color_t defval) {
  defval.color = style_get_int(s, name, defval.color);

  return defval;
}

uint32_t style_get_int(style_t* s, uint32_t name, uint32_t defval) {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t iter = 0;
  uint32_t value = 0;
  const uint8_t* p = NULL;
  return_value_if_fail(s != NULL, defval);

  p = s->data;
  if (p == NULL) {
    return defval;
  }

  load_uint32(p, nr);

  for (i = 0; i < nr; i++) {
    load_uint32(p, iter);
    if (iter == name) {
      load_uint32(p, value);
      defval = value;
      break;
    } else {
      p += 4;
    }
  }

  return defval;
}

const char* style_get_str(style_t* s, uint32_t name, const char* defval) {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t iter = 0;
  const uint8_t* p = NULL;
  return_value_if_fail(s != NULL && s->data != NULL, defval);

  /*skip int values*/
  p = s->data;
  load_uint32(p, nr);
  p += nr * 8;

  load_uint32(p, nr);
  for (i = 0; i < nr; i++) {
    load_uint32(p, iter);
    if (iter == name) {
      defval = (const char*)p;
      break;
    } else {
      p += strlen((const char*)p) + 1;
    }
  }

  return defval;
}

const uint8_t* theme_find_style(theme_t* t, uint16_t widget_type, uint8_t style_type,
                                uint8_t state) {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t iter = 0;
  uint32_t magic = 0;
  uint32_t version = 0;
  uint32_t offset = 0;
  const uint8_t* p = NULL;
  uint32_t name = (widget_type << 16) | (style_type << 8) | state;

  return_value_if_fail(t != NULL && t->data != NULL, NULL);

  p = t->data;
  load_uint32(p, magic);
  return_value_if_fail(magic == THEME_MAGIC, NULL);

  load_uint32(p, version);
  load_uint32(p, nr);
  return_value_if_fail(version == 0, NULL);

  for (i = 0; i < nr; i++) {
    load_uint32(p, iter);
    if (iter == name) {
      load_uint32(p, offset);

      return t->data + offset;
    } else {
      p += 4;
    }
  }

  return NULL;
}

static theme_t s_theme;
theme_t* theme() { return &s_theme; }

theme_t* theme_init(const uint8_t* data) {
  return_value_if_fail(data != NULL, NULL);

  s_theme.data = data;

  return theme();
}
