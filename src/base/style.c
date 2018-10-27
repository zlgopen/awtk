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

const char* style_get_name(style_t* s) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->get_name != NULL, NULL);

  return s->vt->get_name(s);
}

ret_t style_set_name(style_t* s, const char* name) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set_name != NULL && name != NULL,
                       RET_BAD_PARAMS);

  return s->vt->set_name(s, name);
}

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

ret_t style_set_int(style_t* s, widget_state_t state, style_id_t id, uint32_t val) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set_int != NULL, RET_BAD_PARAMS);

  return s->vt->set_int(s, state, id, val);
}

ret_t style_set_color(style_t* s, widget_state_t state, style_id_t id, color_t val) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set_color != NULL, RET_BAD_PARAMS);

  return s->vt->set_color(s, state, id, val);
}

ret_t style_set_str(style_t* s, widget_state_t state, style_id_t id, const char* val) {
  return_value_if_fail(s != NULL && s->vt != NULL && s->vt->set_str != NULL && val != NULL,
                       RET_BAD_PARAMS);

  return s->vt->set_str(s, state, id, val);
}

ret_t style_foreach(style_t* s, tk_on_style_item_t on_style_item, void* ctx) {
  return_value_if_fail(
      s != NULL && s->vt != NULL && s->vt->foreach != NULL && on_style_item != NULL,
      RET_BAD_PARAMS);

  return s->vt->foreach (s, on_style_item, ctx);
}

ret_t style_destroy(style_t* s) {
  if (s != NULL && s->vt != NULL && s->vt->destroy != NULL) {
    return s->vt->destroy(s);
  }

  return RET_FAIL;
}
