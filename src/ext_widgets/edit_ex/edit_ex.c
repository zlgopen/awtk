﻿/**
 * File:   edit_ex.h
 * Author: AWTK Develop Team
 * Brief:  扩展edit控件
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-02-13 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/object_default.h"
#include "base/widget_vtable.h"
#include "ext_widgets/edit_ex/edit_ex.h"
#include "ext_widgets/edit_ex/edit_ex_suggest_words_helper.inc"

ret_t edit_ex_set_suggest_words(widget_t* widget, tk_object_t* suggest_words) {
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(edit_ex->suggest_words);
  edit_ex->suggest_words = TK_OBJECT_REF(suggest_words);

  return RET_OK;
}

static ret_t edit_ex_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, EDIT_EX_PROP_SUGGEST_WORDS)) {
    return edit_ex_set_suggest_words(widget, value_object(v));
  } else if (tk_str_start_with(name, EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS)) {
    ret_t ret = RET_OK;
    str_t sub_prop;
    str_init(&sub_prop, 0);
    str_set(&sub_prop, name);
    ret = str_remove(&sub_prop, 0, sizeof(EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS_PREFIX) - 1);
    return_value_if_fail(RET_OK == ret, (str_reset(&sub_prop), ret));

    ret = tk_object_set_prop(edit_ex->suggest_words_ui_props, sub_prop.str, v);

    str_reset(&sub_prop);
    return ret;
  }

  return widget_vtable_set_prop_by_parent(widget, name, v, WIDGET_VTABLE_GET_VTABLE(edit_ex));
}

static ret_t edit_ex_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, EDIT_EX_PROP_SUGGEST_WORDS)) {
    value_set_object(v, edit_ex->suggest_words);
    return RET_OK;
  } else if (tk_str_start_with(name, EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS)) {
    ret_t ret = RET_OK;
    str_t sub_prop;
    str_init(&sub_prop, 0);
    str_set(&sub_prop, name);
    ret = str_remove(&sub_prop, 0, sizeof(EDIT_EX_PROP_SUGGEST_WORDS_UI_PROPS_PREFIX) - 1);
    return_value_if_fail(RET_OK == ret, (str_reset(&sub_prop), ret));

    ret = tk_object_get_prop(edit_ex->suggest_words_ui_props, sub_prop.str, v);

    str_reset(&sub_prop);
    return ret;
  }

  return widget_vtable_get_prop_by_parent(widget, name, v, WIDGET_VTABLE_GET_VTABLE(edit_ex));
}

static ret_t edit_ex_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint32_t type = e->type;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (!widget->visible) {
    return RET_OK;
  }

  switch (type) {
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      int32_t key = evt->key;
      if (RET_OK == edit_ex_suggest_words_focus_by_key(widget, key)) {
        return RET_STOP;
      }
      break;
    }
    case EVT_FOCUS: {
      ret = widget_vtable_on_event_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(edit_ex));
      edit_ex_show_suggest_words(widget);
      return ret;
    }
    case EVT_VALUE_CHANGING: {
      edit_ex_show_suggest_words(widget);
      break;
    }
    default: {
    } break;
  }

  return widget_vtable_on_event_by_parent(widget, e, WIDGET_VTABLE_GET_VTABLE(edit_ex));
}

static ret_t edit_ex_on_destroy(widget_t* widget) {
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (edit_ex->suggest_words_popup != NULL) {
    widget_destroy(edit_ex->suggest_words_popup);
  }
  TK_OBJECT_UNREF(edit_ex->suggest_words_ui_props);
  TK_OBJECT_UNREF(edit_ex->suggest_words);

  return widget_vtable_on_destroy_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(edit_ex));
}

static ret_t edit_ex_init(widget_t* widget) {
  uint32_t i = 0;
  const char* suggest_words_ui_prop_names[] = {
      EDIT_EX_SUGGEST_WORDS_UI_NAME_POPUP,
      EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW,
      EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR,
      EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW,
  };
  edit_ex_t* edit_ex = EDIT_EX(widget);
  ret_t ret = widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(edit_ex));
  return_value_if_fail(ret == RET_OK || ret == RET_NOT_IMPL, ret);

  edit_ex->suggest_words_ui_props = object_default_create();
  ENSURE(edit_ex->suggest_words_ui_props != NULL);
  return_value_if_fail(edit_ex->suggest_words_ui_props != NULL, RET_OOM);

  for (i = 0; i < ARRAY_SIZE(suggest_words_ui_prop_names); i++) {
    tk_object_t* ui_props = object_default_create_ex(FALSE);
    ENSURE(ui_props != NULL);
    return_value_if_fail(ui_props != NULL, RET_OOM);

    tk_object_set_prop_object(edit_ex->suggest_words_ui_props, suggest_words_ui_prop_names[i],
                              ui_props);
    tk_object_unref(ui_props);
  }

  return RET_OK;
}

TK_DECL_VTABLE(edit_ex) = {
    .size = sizeof(edit_ex_t),
    .type = WIDGET_TYPE_EDIT_EX,
    .focusable = TRUE,
    .inputable = TRUE,
    .get_parent_vt = TK_GET_PARENT_VTABLE(edit),
    .init = edit_ex_init,
    .create = edit_ex_create,
    .on_destroy = edit_ex_on_destroy,
    .on_event = edit_ex_on_event,
    .set_prop = edit_ex_set_prop,
    .get_prop = edit_ex_get_prop,
};

widget_t* edit_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(edit_ex), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  edit_ex_init(widget);
  return widget;
}

widget_t* edit_ex_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, edit_ex), NULL);

  return widget;
}