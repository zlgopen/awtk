/**
 * File:   style.c
 * Author: AWTK Develop Team
 * Brief:  style interface
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/theme.h"
#include "tkc/utils.h"
#include "base/widget.h"
#include "base/style_const.h"

static ret_t widget_get_window_theme(widget_t* widget, theme_t** win_theme,
                                     theme_t** default_theme) {
  value_t v;
  widget_t* win = widget_get_window(widget);

  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  if (widget_get_prop(win, WIDGET_PROP_THEME_OBJ, &v) == RET_OK) {
    *win_theme = (theme_t*)value_pointer(&v);
  }

  if (widget_get_prop(win, WIDGET_PROP_DEFAULT_THEME_OBJ, &v) == RET_OK) {
    *default_theme = (theme_t*)value_pointer(&v);
  }

  return RET_OK;
}

static bool_t is_valid_style_name(const char* str) {
  return str != NULL && *str;
}

static const void* widget_get_const_style_data_for_state_impl(widget_t* widget,
                                                              const char* style_name,
                                                              const char* state) {
  const void* data = NULL;
  theme_t* win_theme = NULL;
  theme_t* default_theme = NULL;
  const char* type = widget_get_type(widget);

  if (tk_str_eq(type, WIDGET_TYPE_WINDOW_MANAGER)) {
    return theme_find_style(theme(), type, style_name, state);
  }

  return_value_if_fail(widget_get_window_theme(widget, &win_theme, &default_theme) == RET_OK, NULL);
  if (win_theme != NULL) {
    data = theme_find_style(win_theme, type, style_name, state);
  }

  if (data == NULL) {
    data = theme_find_style(default_theme, type, style_name, state);
  }

  return data;
}

static const void* widget_get_const_style_data_for_style(widget_t* widget, const char* style_name) {
  const void* data = NULL;
  const char* state = widget_get_prop_str(widget, WIDGET_PROP_STATE_FOR_STYLE, widget->state);

  data = widget_get_const_style_data_for_state_impl(widget, style_name, state);
  if (data == NULL && !tk_str_eq(state, WIDGET_STATE_NORMAL)) {
    data = widget_get_const_style_data_for_state_impl(widget, style_name, WIDGET_STATE_NORMAL);
  }

  return data;
}

static const void* widget_get_const_style_data(widget_t* widget) {
  const char* style_name = is_valid_style_name(widget->style) ? widget->style : TK_DEFAULT_STYLE;
  const void* data = widget_get_const_style_data_for_style(widget, style_name);

  if (data == NULL && !tk_str_eq(style_name, TK_DEFAULT_STYLE)) {
    data = widget_get_const_style_data_for_style(widget, TK_DEFAULT_STYLE);
  }

  return data;
}

static ret_t style_const_apply_layout(style_t* s, widget_t* widget) {
  const char* self_layout = style_get_str(s, STYLE_ID_SELF_LAYOUT, NULL);
  const char* children_layout = style_get_str(s, STYLE_ID_CHILDREN_LAYOUT, NULL);

  if (self_layout != NULL || children_layout != NULL) {
    if (self_layout != NULL) {
      widget_set_self_layout(widget, self_layout);
      widget_set_need_relayout_children(widget->parent);
    }

    if (children_layout != NULL) {
      widget_set_children_layout(widget, children_layout);
      widget_set_need_relayout_children(widget);
    }
  }

  return RET_OK;
}

static ret_t style_const_notify_widget_state_changed(style_t* s, widget_t* widget) {
  style_const_t* style = (style_const_t*)s;
  const void* old_data = style->data;
  style->data = widget_get_const_style_data(widget);

  if (old_data != style->data) {
    style_const_apply_layout(s, widget);
  }

  return RET_OK;
}

bool_t style_const_is_valid(style_t* s) {
  style_const_t* style = (style_const_t*)s;

  return style->data != NULL;
}

int32_t style_const_get_int(style_t* s, const char* name, int32_t defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_int(style->data, name, defval);
}

color_t style_const_get_color(style_t* s, const char* name, color_t defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_color(style->data, name, defval);
}

const char* style_const_get_str(style_t* s, const char* name, const char* defval) {
  style_const_t* style = (style_const_t*)s;

  return style_data_get_str(style->data, name, defval);
}

static ret_t style_const_destroy(style_t* s) {
  memset(s, 0x00, sizeof(style_t));

  TKMEM_FREE(s);

  return RET_OK;
}

static const style_vtable_t style_const_vt = {
    .is_mutable = FALSE,
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
