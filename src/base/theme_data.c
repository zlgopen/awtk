/**
 * File:   theme_data.c
 * Author: AWTK Develop Team
 * Brief:  theme data
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

/*data related*/
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

gradient_t* style_data_get_gradient(const uint8_t* s, const char* name, gradient_t* gradient) {
  const style_name_value_t* nv = style_data_get(s, name);

  if (nv != NULL) {
    const uint8_t* p = (const uint8_t*)(nv->name) + nv->name_size;
    if (nv->type == VALUE_TYPE_GRADIENT) {
      return gradient_init_from_binary(gradient, p, nv->value_size);
    } else if (nv->type == VALUE_TYPE_UINT32 || nv->type == VALUE_TYPE_INT32) {
      uint32_t value = 0;
      load_uint32(p, value);
      return gradient_init_simple(gradient, value);
    }
  }

  return NULL;
}

const char* style_data_get_str(const uint8_t* s, const char* name, const char* defval) {
  const char* value = defval;
  const style_name_value_t* nv = style_data_get(s, name);

  if (nv != NULL && nv->type == VALUE_TYPE_STRING) {
    value = nv->name + nv->name_size;
  }

  return value;
}
