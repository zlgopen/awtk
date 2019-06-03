/**
 * File:   style.c
 * Author: AWTK Develop Team
 * Brief:  style interface
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/style.h"

ret_t style_notify_widget_state_changed(style_t* s, widget_t* widget) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->notify_widget_state_changed != NULL,
                       RET_BAD_PARAMS);

  return s->vt->notify_widget_state_changed(s, widget);
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
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_color != NULL, defval);

  return s->vt->get_color(s, name, defval);
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

bool_t style_is_mutable(style_t* s) {
  return_value_if_fail(s != NULL && s->vt != NULL, FALSE);

  return s->vt->is_mutable;
}
