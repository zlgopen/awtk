/**
 * File:   style.c
 * Author: AWTK Develop Team
 * Brief:  style interface
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "base/style.h"

static ret_t gradient_str_to_value(value_t* v, const char* value) {
  gradient_t g;
  wbuffer_t wb;
  return_value_if_fail(v != NULL && value != NULL, RET_BAD_PARAMS);

  wbuffer_init_extendable(&wb);
  return_value_if_fail(gradient_init_from_str(&g, value) != NULL, RET_BAD_PARAMS);
  return_value_if_fail(gradient_to_binary(&g, &wb) == RET_OK, RET_OOM);

  value_set_gradient(v, wb.data, wb.cursor);
  v->free_handle = TRUE;

  return RET_OK;
}

ret_t style_notify_widget_state_changed(style_t* s, widget_t* widget) {
  return_value_if_fail(
      s != NULL && s->vt != NULL && s->vt->notify_widget_state_changed != NULL && widget != NULL,
      RET_BAD_PARAMS);
  return s->vt->notify_widget_state_changed(s, widget);
}

ret_t style_update_state(style_t* s, theme_t* theme, const char* widget_type,
                         const char* style_name, const char* widget_state) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->update_state != NULL, RET_BAD_PARAMS);
  return s->vt->update_state(s, theme, widget_type, style_name, widget_state);
}

uint32_t style_get_uint(style_t* s, const char* name, uint32_t defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_int != NULL, defval);

  return s->vt->get_uint(s, name, defval);
}

int32_t style_get_int(style_t* s, const char* name, int32_t defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_int != NULL, defval);

  return s->vt->get_int(s, name, defval);
}

bool_t style_is_valid(style_t* s) {
  if (s != NULL && s->vt != NULL && s->vt->is_valid != NULL) {
    return s->vt->is_valid(s);
  }

  return FALSE;
}

color_t style_get_color(style_t* s, const char* name, color_t defval) {
  gradient_t agradient;
  gradient_t* gradient = NULL;
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_color != NULL, defval);

  gradient = style_get_gradient(s, name, &agradient);
  if (gradient != NULL) {
    return gradient_get_first_color(gradient);
  } else {
    return s->vt->get_color(s, name, defval);
  }
}

gradient_t* style_get_gradient(style_t* s, const char* name, gradient_t* gradient) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_gradient != NULL, NULL);

  return s->vt->get_gradient(s, name, gradient);
}

const char* style_get_str(style_t* s, const char* name, const char* defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_str != NULL, defval);

  return s->vt->get_str(s, name, defval);
}

ret_t style_destroy(style_t* s) {
  if (s != NULL && s->vt != NULL && s->vt->destroy != NULL) {
    return s->vt->destroy(s);
  }

  return RET_FAIL;
}

ret_t style_set(style_t* s, const char* state, const char* name, const value_t* value) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set != NULL, RET_BAD_PARAMS);
  return_value_if_fail(state != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);

  return s->vt->set(s, state, name, value);
}

ret_t style_set_style_data(style_t* s, const uint8_t* data, const char* state) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set_style_data != NULL && data != NULL,
                       RET_BAD_PARAMS);
  return s->vt->set_style_data(s, data, state);
}

const char* style_get_style_state(style_t* s) {
  return_value_if_fail(s != NULL && s->vt != NULL, NULL);
  if (s->vt->get_style_state != NULL) {
    return s->vt->get_style_state(s);
  }
  return NULL;
}

bool_t style_is_mutable(style_t* s) {
  return_value_if_fail(s != NULL && s->vt != NULL, FALSE);

  return s->vt->is_mutable;
}

const char* style_get_style_type(style_t* s) {
  return_value_if_fail(s != NULL && s->vt != NULL, NULL);
  if (s->vt->get_style_type != NULL) {
    return s->vt->get_style_type(s);
  }
  return NULL;
}

#include "base/enums.h"
#include "tkc/color_parser.h"

static uint32_t to_border(const char* value) {
  uint32_t border = tk_atoi(value);
  if (border != 0) return border;

  if (strstr(value, "left")) {
    border |= BORDER_LEFT;
  }
  if (strstr(value, "right")) {
    border |= BORDER_RIGHT;
  }
  if (strstr(value, "top")) {
    border |= BORDER_TOP;
  }
  if (strstr(value, "bottom")) {
    border |= BORDER_BOTTOM;
  }
  if (strstr(value, "all")) {
    border |= BORDER_ALL;
  }

  return border;
}

static uint32_t to_icon_at(const char* value) {
  uint32_t icon_at = ICON_AT_AUTO;

  if (strstr(value, "cent")) {
    icon_at = ICON_AT_CENTRE;
  }
  if (strstr(value, "left")) {
    icon_at = ICON_AT_LEFT;
  }
  if (strstr(value, "right")) {
    icon_at = ICON_AT_RIGHT;
  }
  if (strstr(value, "top")) {
    icon_at = ICON_AT_TOP;
  }
  if (strstr(value, "bottom")) {
    icon_at = ICON_AT_BOTTOM;
  }

  return icon_at;
}

ret_t style_normalize_value(const char* name, const char* value, value_t* out) {
  value_t* v = out;
  const key_type_value_t* dt = NULL;
  return_value_if_fail(name != NULL && value != NULL && out != NULL, RET_BAD_PARAMS);

  value_set_int(v, 0);
  if (strstr(name, "image_draw_type") != NULL) {
    dt = image_draw_type_find(value);
  } else if (strcmp(name, "text_align_h") == 0) {
    dt = align_h_type_find(value);
  } else if (strcmp(name, "text_align_v") == 0) {
    dt = align_v_type_find(value);
  } else if (strcmp(name, "border") == 0) {
    value_set_int(v, to_border(value));
  } else if (strcmp(name, "icon_at") == 0) {
    value_set_int(v, to_icon_at(value));
  } else if (strstr(value, "linear-gradient") != NULL || strstr(value, "radial-gradient") != NULL) {
    return gradient_str_to_value(v, value);
  } else if (strstr(name, "color") != NULL) {
    color_t c = color_parse(value);
    value_set_uint32(v, c.color);
  } else if (strstr(name, "image") != NULL || strstr(name, "name") != NULL ||
             strstr(name, "icon") != NULL) {
    value_dup_str(v, value);
  } else {
    if (tk_isdigit(*value)) {
      value_set_uint32(v, tk_atoi(value));
    } else if (strlen(value) > 1 && *value == '-' && tk_isdigit(*(value + 1))) {
      value_set_int(v, tk_atoi(value));
    } else {
      value_dup_str(v, value);
    }
  }

  if (dt != NULL) {
    value_set_int(v, dt->value);
  }

  return RET_OK;
}
