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

#include "base/mem.h"
#include "base/theme.h"
#include "base/utils.h"
#include "base/widget.h"
#include "base/style_const.h"

typedef struct _style_const_t {
  style_t style;
  const uint8_t* data;
} style_const_t;

static const uint8_t* widget_get_const_theme_data(widget_t* widget) {
  value_t v;
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, NULL);

  if (widget_get_prop(win, WIDGET_PROP_THEME_DATA, &v) == RET_OK) {
    return (const uint8_t*)value_pointer(&v);
  }

  return NULL;
}

static const void* widget_get_conststyle_data(widget_t* widget) {
  theme_t t;
  value_t v;
  const void* data = NULL;
  uint16_t state = widget->state;
  const char* type = widget->vt->type;
  char style_name[NAME_LEN + NAME_LEN + 1];
  const char* name = widget->style != NULL ? widget->style : TK_DEFAULT_STYLE;

  if (!widget->enable) {
    state = WIDGET_STATE_DISABLE;
  }

  if (widget_get_prop(widget, WIDGET_PROP_SUB_THEME, &v) == RET_OK && value_str(&v) != NULL) {
    tk_snprintf(style_name, sizeof(style_name) - 1, "%s%s", name, value_str(&v));
  } else {
    tk_strncpy(style_name, name, sizeof(style_name) - 1);
  }

  t.data = widget_get_const_theme_data(widget);

  if (t.data != NULL) {
    data = theme_find_style(&t, type, style_name, state);
  }

  if (data == NULL) {
    data = theme_find_style(theme(), type, style_name, state);
  }

  return data;
}

static ret_t style_const_notify_widget_state_changed(style_t* s, widget_t* widget) {
  style_const_t* style = (style_const_t*)s;

  style->data = widget_get_conststyle_data(widget);

  return RET_OK;
}

bool_t style_const_is_valid(style_t* s) {
  style_const_t* style = (style_const_t*)s;

  return style->data != NULL;
}

int32_t style_const_get_int(style_t* s, style_id_t id, int32_t defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_int(style->data, id, defval);
}

color_t style_const_get_color(style_t* s, style_id_t id, color_t defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_color(style->data, id, defval);
}

const char* style_const_get_str(style_t* s, style_id_t id, const char* defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_str(style->data, id, defval);
}

static ret_t style_const_destroy(style_t* s) {
  memset(s, 0x00, sizeof(style_t));

  TKMEM_FREE(s);

  return RET_OK;
}

static const style_vtable_t style_const_vt = {
    .is_valid = style_const_is_valid,
    .notify_widget_state_changed = style_const_notify_widget_state_changed,
    .get_int = style_const_get_int,
    .get_str = style_const_get_str,
    .get_color = style_const_get_color,
    .destroy = style_const_destroy};

style_t* style_const_create(widget_t* widget) {
  style_const_t* style = TKMEM_ZALLOC(style_const_t);
  return_value_if_fail(style != NULL, NULL);

  style->style.vt = &style_const_vt;

  return (style_t*)style;
}
