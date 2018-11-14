/**
 * File:   style.c
 * Author: AWTK Develop Team
 * Brief:  style interface
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/style.h"

ret_t style_notify_widget_state_changed(style_t* s, widget_t* widget) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->notify_widget_state_changed != NULL,
                       RET_BAD_PARAMS);

  return s->vt->notify_widget_state_changed(s, widget);
}

int32_t style_get_int(style_t* s, style_id_t id, int32_t defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_int != NULL, defval);

  return s->vt->get_int(s, id, defval);
}

bool_t style_is_valid(style_t* s) {
  if (s != NULL && s->vt != NULL && s->vt->is_valid != NULL) {
    return s->vt->is_valid(s);
  }

  return FALSE;
}

color_t style_get_color(style_t* s, style_id_t id, color_t defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_color != NULL, defval);

  return s->vt->get_color(s, id, defval);
}

const char* style_get_str(style_t* s, style_id_t id, const char* defval) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_str != NULL, defval);

  return s->vt->get_str(s, id, defval);
}

ret_t style_destroy(style_t* s) {
  if (s != NULL && s->vt != NULL && s->vt->destroy != NULL) {
    return s->vt->destroy(s);
  }

  return RET_FAIL;
}
