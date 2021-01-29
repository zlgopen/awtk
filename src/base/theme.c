/**
 * File:   theme.c
 * Author: AWTK Develop Team
 * Brief:  theme manager
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  defval.color = style_data_get_uint(s, name, defval.color);

  return defval;
}

static const style_name_value_t* style_data_get(const uint8_t* s, const char* name) {
  uint32_t i = 0;
  uint32_t nr = 0;
  const uint8_t* p = s;

  if (s == NULL) {
    return NULL;
  }

  load_uint32(p, nr);
  for (i = 0; i < nr; i++) {
    const style_name_value_t* iter = (const style_name_value_t*)p;

    if (tk_str_eq(iter->name, name)) {
      return iter;
    } else {
      p += sizeof(style_name_value_header_t) + iter->name_size + iter->value_size;
    }
  }

  return NULL;
}

int32_t style_data_get_int(const uint8_t* s, const char* name, int32_t defval) {
  uint32_t value = defval;
  const style_name_value_t* nv = style_data_get(s, name);

  if (nv != NULL && (nv->type == VALUE_TYPE_UINT32 || nv->type == VALUE_TYPE_INT32)) {
    const uint8_t* p = (const uint8_t*)(nv->name) + nv->name_size;
    if (nv->type == VALUE_TYPE_UINT32) {
      load_uint32(p, value);
    } else {
      load_int32(p, value);
    }
  }

  return value;
}

uint32_t style_data_get_uint(const uint8_t* s, const char* name, uint32_t defval) {
  uint32_t value = defval;
  const style_name_value_t* nv = style_data_get(s, name);

  if (nv != NULL && nv->type == VALUE_TYPE_UINT32) {
    const uint8_t* p = (const uint8_t*)(nv->name) + nv->name_size;
    load_uint32(p, value);
  }

  return value;
}

const char* style_data_get_str(const uint8_t* s, const char* name, const char* defval) {
  const char* value = defval;
  const style_name_value_t* nv = style_data_get(s, name);

  if (nv != NULL && nv->type == VALUE_TYPE_STRING) {
    value = nv->name + nv->name_size;
  }

  return value;
}

const uint8_t* theme_find_style(theme_t* theme, const char* widget_type, const char* name,
                                const char* widget_state) {
  return_value_if_fail(theme != NULL, NULL);
  if (theme->find_style != NULL) {
    return theme->find_style(theme, widget_type, name, widget_state);
  } else {
    uint32_t i = 0;
    const theme_item_t* iter = NULL;
    const theme_header_t* header = NULL;
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
}

const char* theme_get_style_type(theme_t* theme) {
  return_value_if_fail(theme != NULL, NULL);
  if (theme->get_style_type != NULL) {
    return theme->get_style_type(theme);
  } else {
    return THEME_DEFAULT_STYLE_TYPE;
  }
}

ret_t theme_set_theme_data(theme_t* theme, const uint8_t* data) {
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);
  if (theme->set_style_data != NULL) {
    return theme->set_style_data(theme, data);
  } else {
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
    theme->data = data;
    return RET_OK;
  }
}

static theme_t* s_theme;

theme_t* theme(void) {
  return s_theme;
}

ret_t theme_set(theme_t* theme) {
  s_theme = theme;

  return RET_OK;
}

theme_t* theme_default_create(const uint8_t* data) {
  theme_t* theme = TKMEM_ZALLOC(theme_t);
  return_value_if_fail(theme != NULL, NULL);
  theme->data = data;
  return theme;
}

ret_t theme_destroy(theme_t* theme) {
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);
  if (theme->theme_destroy != NULL) {
    return theme->theme_destroy(theme);
  } else {
    theme->data = NULL;
    TKMEM_FREE(theme);
    return RET_OK;
  }
}
