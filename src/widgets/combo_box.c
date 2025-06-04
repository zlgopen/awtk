/**
 * File:   combo_box.h
 * Author: AWTK Develop Team
 * Brief:  combo_box
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/window.h"
#include "base/widget_vtable.h"
#include "base/window_manager.h"
#include "widgets/popup.h"
#include "tkc/tokenizer.h"
#include "widgets/button.h"
#include "widgets/combo_box.h"
#include "widgets/combo_box_item.h"

#define COMBO_BOX_DEFAULT_MARGIN 1

static ret_t combo_box_on_button_click(void* ctx, event_t* e);
static ret_t combo_box_sync_index_to_value(widget_t* widget, uint32_t index, bool_t only_set);

const char* const s_combo_box_properties[] = {TK_EDIT_PROPS,
                                              WIDGET_PROP_OPTIONS,
                                              WIDGET_PROP_ITEM_HEIGHT,
                                              WIDGET_PROP_OPEN_WINDOW,
                                              WIDGET_PROP_SELECTED_INDEX,
                                              WIDGET_PROP_THEME_OF_POPUP,
                                              WIDGET_PROP_LOCALIZE_OPTIONS,
                                              NULL};

static ret_t combo_box_active(widget_t* widget);
static ret_t combo_box_add_selected_index(widget_t* widget, int32_t delta);
static ret_t combo_box_set_selected_index_ex(widget_t* widget, uint32_t index, widget_t* item);

static ret_t combo_box_on_copy(widget_t* widget, widget_t* other) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  combo_box_t* combo_box_other = COMBO_BOX(other);
  return_value_if_fail(combo_box != NULL && combo_box_other != NULL, RET_BAD_PARAMS);

  edit_on_copy(widget, other);

  combo_box->item_height = combo_box_other->item_height;
  combo_box->selected_index = combo_box_other->selected_index;
  combo_box->localize_options = combo_box_other->localize_options;

  if (combo_box_other->options != NULL) {
    combo_box_set_options(widget, combo_box_other->options);
  }

  if (combo_box_other->open_window != NULL) {
    combo_box->open_window = tk_str_copy(combo_box->open_window, combo_box_other->open_window);
  }

  if (combo_box_other->theme_of_popup != NULL) {
    combo_box->theme_of_popup =
        tk_str_copy(combo_box->theme_of_popup, combo_box_other->theme_of_popup);
  }

  combo_box->open_popup = combo_box_other->open_popup;
  combo_box->on_item_click = combo_box_other->on_item_click;
  combo_box->on_item_click_ctx = combo_box_other->on_item_click_ctx;
  combo_box->on_layout_combobox_popup = combo_box_other->on_layout_combobox_popup;

  return RET_OK;
}

static ret_t combo_box_on_destroy(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  if (combo_box->init_popup_button_idle_id != TK_INVALID_ID) {
    idle_remove(combo_box->init_popup_button_idle_id);
  }

  str_reset(&(combo_box->text));
  combo_box_reset_options(widget);
  TKMEM_FREE(combo_box->open_window);
  TKMEM_FREE(combo_box->theme_of_popup);

  edit_on_destroy(widget);

  if (combo_box->combobox_popup != NULL) {
    widget_destroy(combo_box->combobox_popup);
  }

  return RET_OK;
}

#define WIDGET_NAME_VALUE "value"

static uint32_t combo_box_get_curr_text_size(widget_t* widget) {
  return_value_if_fail(widget != NULL, 0);
  widget_t* value_widget = widget_lookup(widget, WIDGET_NAME_VALUE, TRUE);

  if (value_widget == NULL) {
    value_widget = widget;
  }

  return value_widget->text.size;
}

static ret_t combo_box_update_status(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  if (combo_box_get_curr_text_size(widget) == 0) {
    if (widget->focused) {
      widget_set_state(widget, WIDGET_STATE_EMPTY_FOCUS);
    } else {
      widget_set_state(widget, WIDGET_STATE_EMPTY);
    }
  } else {
    if (edit != NULL && edit->cancelable) {
      if (!widget->loading) {
        if (!wstr_equal(&(edit->saved_text), &(widget->text))) {
          widget_set_state(widget, WIDGET_STATE_CHANGED);
          return RET_OK;
        }
      }
    }

    if (widget->focused) {
      widget_set_state(widget, WIDGET_STATE_FOCUSED);
    } else {
      widget_set_state(widget, WIDGET_STATE_NORMAL);
    }
  }

  return RET_OK;
}

static ret_t combo_box_set_text_only(widget_t* widget, const char* text, const wchar_t* wtext,
                                     bool_t tr) {
  widget_t* value_widget = widget_lookup(widget, WIDGET_NAME_VALUE, TRUE);

  if (value_widget == NULL) {
    value_widget = widget;
  } else {
    TKMEM_FREE(widget->tr_text);
    wstr_clear(&(widget->text));
    combo_box_update_status(value_widget);
  }

  if (tr) {
    widget_t* win = widget_get_window(value_widget);
    if (win != NULL) {
      text = locale_info_tr(widget_get_locale_info(widget), text);
    }
    wstr_set_utf8(&(value_widget->text), text);
  } else {
    if (wtext != NULL) {
      wstr_set(&(value_widget->text), wtext);
    } else {
      wstr_set_utf8(&(value_widget->text), text);
    }
  }
  combo_box_update_status(value_widget);
  edit_set_cursor(widget, 0);
  return RET_OK;
}

static ret_t combo_box_set_text(widget_t* widget, const char* text, const wchar_t* wtext,
                                bool_t tr) {
  widget_t* value_widget = widget_lookup(widget, WIDGET_NAME_VALUE, TRUE);

  if (value_widget == NULL) {
    value_widget = widget;
  } else {
    TKMEM_FREE(widget->tr_text);
    widget_set_text(widget, L"");
  }

  if (tr) {
    widget_set_tr_text(value_widget, text);
  } else {
    if (wtext != NULL) {
      widget_set_text(value_widget, wtext);
    } else {
      widget_set_text_utf8(value_widget, text);
    }
  }
  edit_set_cursor(widget, 0);
  return RET_OK;
}

static ret_t combo_box_get_prop(widget_t* widget, const char* name, value_t* v) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_OPEN_WINDOW)) {
    value_set_str(v, combo_box->open_window);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME_OF_POPUP)) {
    value_set_str(v, combo_box->theme_of_popup);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    value_set_int(v, combo_box->selected_index);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, combo_box->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALIZE_OPTIONS)) {
    value_set_bool(v, combo_box->localize_options);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPTIONS)) {
    value_set_str(v, combo_box->options);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    value_set_int(v, combo_box->item_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    uint32_t margin = 0;
    if (widget->astyle != NULL) {
      TEXT_EDIT_GET_STYLE_MARGIN(widget->astyle, margin, RIGHT);
    }
    if (margin == 0) {
      edit_t* edit = EDIT(widget);
      margin = edit->right_margin != 0 ? edit->right_margin : edit->margin;
    }
    if (margin == 0 && widget_get_prop_int(widget, WIDGET_PROP_LEFT_MARGIN, 0) == 0) {
      margin = widget->h;
    }
    value_set_int(v, margin);
    return RET_OK;
  } else {
    return edit_get_prop(widget, name, v);
  }
}

static ret_t combo_box_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_OPEN_WINDOW)) {
    value_set_str(v, NULL);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME_OF_POPUP)) {
    value_set_str(v, NULL);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALIZE_OPTIONS)) {
    value_set_bool(v, TRUE);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPTIONS)) {
    value_set_str(v, NULL);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t combo_box_parse_options(widget_t* widget, const char* str) {
  str_t s;
  int32_t i = 0;
  tokenizer_t tokenizer;
  tokenizer_t* t = &tokenizer;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL && str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_init(&s, 100) != NULL, RET_OOM);

  combo_box_reset_options(widget);
  combo_box->options = tk_strdup(str);
  tokenizer_init(t, str, strlen(str), ";");

  while (tokenizer_has_more(t)) {
    int32_t value = 0;
    const char* token = tokenizer_next(t);
    if (*token) {
      const char* text = (*token == '-' || tk_isdigit(*token)) ? strchr(token, ':') : NULL;
      if (text != NULL) {
        text++;
        value = tk_atoi(token);
      } else {
        text = token;
        value = i;
      }

      str_set(&s, text);
      str_unescape(&s);
      combo_box_append_option(widget, value, s.str);
      i++;
    }
  }
  str_reset(&s);
  tokenizer_deinit(t);

  return RET_OK;
}

ret_t combo_box_set_options(widget_t* widget, const char* options) {
  ret_t ret = combo_box_parse_options(widget, options);
  if (!widget->loading) {
    ret = combo_box_sync_index_to_value(widget, 0, FALSE);
  }
  return ret;
}

static ret_t combo_box_text_to_index(widget_t* widget, const char* text) {
  ret_t ret = RET_FAIL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  combo_box_option_t* option = NULL;
  int32_t i = 0;
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  option = combo_box_get_option(widget, combo_box->selected_index);
  if (option != NULL && tk_str_eq(text, option->text)) {
    return RET_OK;
  }

  option = combo_box->option_items;
  for (i = 0; option != NULL; option = option->next, i++) {
    if (tk_str_eq(text, option->text)) {
      combo_box->selected_index = i;
      ret = RET_OK;
      break;
    }
  }

  return ret;
}

static ret_t combo_box_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, WIDGET_PROP_OPEN_WINDOW)) {
    combo_box_set_open_window(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME_OF_POPUP)) {
    combo_box_set_theme_of_popup(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    combo_box_set_selected_index(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    combo_box_set_value(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALIZE_OPTIONS)) {
    combo_box_set_localize_options(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPTIONS)) {
    combo_box_set_options(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    combo_box_set_item_height(widget, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    str_t str;
    str_init(&str, 0);
    edit_set_prop(widget, WIDGET_PROP_TEXT, v);
    str_from_wstr(&str, widget_get_text(widget));
    combo_box_text_to_index(widget, str.str);
    str_reset(&str);
    return RET_OK;
  } else {
    return edit_set_prop(widget, name, v);
  }
}

static ret_t combo_box_on_layout_children_for_combobox_popup(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  if (combo_box->combobox_popup != NULL) {
    point_t p = {0, 0};
    int32_t margin = COMBO_BOX_DEFAULT_MARGIN;
    int32_t item_height = combo_box->item_height;
    int32_t nr = combo_box_count_options(widget);
    int32_t h = nr * item_height + 2 * margin;
    if (combo_box->open_window != NULL) {
      h = combo_box->combobox_popup->h;
    }
    combo_box_combobox_popup_calc_position(widget, widget->w, h, &p);
    widget_move_resize(combo_box->combobox_popup, p.x, p.y, widget->w, h);
  }
  return RET_OK;
}

static ret_t combo_box_on_layout_children(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  widget_t* button = widget_lookup_by_type(widget, WIDGET_TYPE_BUTTON, TRUE);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  widget_layout_children_default(widget);
  if (button != NULL) {
    if (button->auto_created) {
      widget_move_resize(button, widget->w - widget->h, 0, widget->h, widget->h);
    } else {
      widget_layout(button);
    }
  }
  if (combo_box->on_layout_combobox_popup) {
    combo_box->on_layout_combobox_popup(widget);
  } else {
    combo_box_on_layout_children_for_combobox_popup(widget);
  }
  return RET_OK;
}

static uint32_t edit_get_right_margin(widget_t* widget) {
  int32_t right_margin = 0;
  style_t* style = widget != NULL ? widget->astyle : NULL;

  right_margin = widget_get_prop_int(widget, WIDGET_PROP_RIGHT_MARGIN, 0);

  if (right_margin == 0) {
    right_margin = style_get_int(style, STYLE_ID_MARGIN_RIGHT, 0);
  }

  if (right_margin == 0) {
    right_margin = style_get_int(style, STYLE_ID_MARGIN, 0);
  }

  return right_margin;
}

static ret_t combo_box_on_key_event(widget_t* widget, key_event_t* evt) {
  ret_t ret = RET_OK;
  edit_t* edit = EDIT(WIDGET(widget));
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (evt->key == TK_KEY_UP) {
    ret = RET_STOP;
    if (evt->e.type == EVT_KEY_DOWN) {
      combo_box_add_selected_index(widget, -1);
    }
  } else if (evt->key == TK_KEY_DOWN) {
    ret = RET_STOP;
    if (evt->e.type == EVT_KEY_DOWN) {
      combo_box_add_selected_index(widget, 1);
    }
  } else if (widget_is_activate_key(widget, evt) && evt->e.type == EVT_KEY_UP) {
    if (edit->focus_next_when_enter) {
      ret = edit_on_event(widget, (event_t*)evt);
    } else if (edit->readonly) {
      ret = RET_STOP;
      combo_box_active(widget);
    } else if (key_code_is_enter(evt->key)) {
      ret = RET_STOP;
      combo_box_active(widget);
    }
  }

  return ret;
}

static ret_t combo_box_init_popup_button(widget_t* widget) {
  widget_t* popup = button_create(widget, 0, 0, 0, 0);
  ENSURE(popup);
  popup->auto_created = TRUE;
  widget_use_style(popup, "combobox_down");
  widget_invalidate_force(widget, NULL);
  return RET_OK;
}

static ret_t combo_box_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  combo_box_t* combo_box = COMBO_BOX(widget);
  edit_t* edit = EDIT(WIDGET(combo_box));
  return_value_if_fail(combo_box != NULL && edit != NULL && e != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WIDGET_LOAD: {
      /*If there is a value widget, sync the text to value widget*/
      if (widget_lookup(widget, WIDGET_NAME_VALUE, TRUE) != NULL) {
        combo_box_sync_index_to_value(widget, combo_box->selected_index, FALSE);
      }

      if (combo_box_get_curr_text_size(widget) == 0 && widget->tr_text == NULL) {
        combo_box_sync_index_to_value(widget, 0, FALSE);
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      uint32_t right_margin = edit_get_right_margin(widget);
      if (right_margin == 0) {
        edit->right_margin = widget->h;
      } else {
        edit->right_margin = right_margin;
      }
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      if (combo_box_on_key_event(widget, evt) == RET_STOP) {
        return RET_STOP;
      }
      break;
    }
    case EVT_POINTER_DOWN: {
      combo_box->pressed = TRUE;
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      combo_box->pressed = FALSE;
      break;
    }
    case EVT_POINTER_UP: {
      if (widget->target == NULL && edit->readonly && combo_box->pressed) {
        combo_box_active(widget);
        return RET_STOP;
      }
      combo_box->pressed = FALSE;
      break;
    }
    case EVT_POINTER_LEAVE:
      return combo_box_update_status(widget);
    case EVT_POINTER_ENTER:
      if (combo_box_get_curr_text_size(widget) == 0) {
        widget_set_state(widget, WIDGET_STATE_EMPTY_OVER);
      } else {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      return RET_OK;
    case EVT_FOCUS:
      combo_box_update_status(widget);
      break;
    case EVT_BLUR:
      combo_box_update_status(widget);
      break;
    default:
      break;
  }

  ret = edit_on_event(widget, e);

  return ret;
}

static ret_t combo_box_on_add_child(widget_t* widget, widget_t* child) {
  if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_BUTTON)) {
    widget_t* button = widget_lookup_by_type(widget, WIDGET_TYPE_BUTTON, TRUE);

    if (button != NULL && button != child) {
      widget_destroy(button);
    }

    widget_on(child, EVT_CLICK, combo_box_on_button_click, widget);
  }

  return RET_FAIL;
}

static ret_t combo_box_on_idle_init_popup_button(const idle_info_t* idle) {
  widget_t* widget = NULL;
  combo_box_t* combo_box = NULL;
  return_value_if_fail(idle != NULL, RET_BAD_PARAMS);

  widget = WIDGET(idle->ctx);
  combo_box = COMBO_BOX(widget);
  ENSURE(combo_box);

  if (!widget->destroying) {
    widget_t* button = widget_lookup_by_type(widget, WIDGET_TYPE_BUTTON, TRUE);
    if (button == NULL) {
      combo_box_init_popup_button(widget);
    }
  }

  combo_box->init_popup_button_idle_id = TK_INVALID_ID;

  return RET_OK;
}

static ret_t combo_box_idle_init_popup_button(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  ENSURE(combo_box);
  if (combo_box->init_popup_button_idle_id == TK_INVALID_ID) {
    combo_box->init_popup_button_idle_id = idle_add(combo_box_on_idle_init_popup_button, widget);
  }
  return RET_OK;
}

static ret_t combo_box_on_remove_child(widget_t* widget, widget_t* child) {
  if (!widget->destroying) {
    if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_BUTTON)) {
      widget_t* button = widget_lookup_by_type(widget, WIDGET_TYPE_BUTTON, TRUE);
      if (button == child) {
        widget_off_by_ctx(child, widget);
        combo_box_idle_init_popup_button(widget);
      }
    }
  }

  return RET_FAIL;
}

static ret_t combo_box_init(widget_t* widget) {
  ret_t ret = RET_OK;
  combo_box_t* _combo_box = COMBO_BOX(widget);
  return_value_if_fail(_combo_box != NULL, RET_BAD_PARAMS);

  ret = widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(combo_box));
  return_value_if_fail(ret == RET_OK || ret == RET_NOT_IMPL, ret);

  widget_set_prop_int(widget, WIDGET_PROP_MARGIN, 0);
  widget_set_prop_int(widget, WIDGET_PROP_LEFT_MARGIN, 0);
  widget_set_prop_int(widget, WIDGET_PROP_RIGHT_MARGIN, 0);
  str_init(&(_combo_box->text), 32);
  _combo_box->localize_options = TRUE;

  combo_box_init_popup_button(widget);
  combo_box_set_item_height(widget, 30);
  return RET_OK;
}

TK_DECL_VTABLE(combo_box) = {.size = sizeof(combo_box_t),
                             .inputable = TRUE,
                             .type = WIDGET_TYPE_COMBO_BOX,
                             .focusable = TRUE,
                             .pointer_cursor = WIDGET_CURSOR_EDIT,
                             .space_key_to_activate = TRUE,
                             .return_key_to_activate = TRUE,
                             .clone_properties = s_combo_box_properties,
                             .persistent_properties = s_combo_box_properties,
                             .get_parent_vt = TK_GET_PARENT_VTABLE(edit),
                             .create = combo_box_create,
                             .init = combo_box_init,
                             .on_paint_self = edit_on_paint_self,
                             .set_prop = combo_box_set_prop,
                             .get_prop = combo_box_get_prop,
                             .get_prop_default_value = combo_box_get_prop_default_value,
                             .on_add_child = combo_box_on_add_child,
                             .on_remove_child = combo_box_on_remove_child,
                             .on_layout_children = combo_box_on_layout_children,
                             .on_destroy = combo_box_on_destroy,
                             .on_copy = combo_box_on_copy,
                             .on_event = combo_box_on_event};

ret_t combo_box_set_on_item_click(widget_t* widget, event_func_t on_item_click, void* ctx) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->on_item_click = on_item_click;
  combo_box->on_item_click_ctx = ctx;

  return RET_OK;
}

static ret_t combo_box_on_item_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* item = WIDGET(e->target);
  combo_box_t* combo_box = COMBO_BOX(ctx);
  ENSURE(combo_box);
  return_value_if_fail(widget != NULL && item != NULL, RET_BAD_PARAMS);

  if (combo_box->on_item_click != NULL) {
    if (combo_box->on_item_click(combo_box->on_item_click_ctx, e) == RET_OK) {
      return RET_OK;
    }
  }

  combo_box_set_selected_index_ex(widget, widget_index_of(item), item);

  widget->target = NULL;
  widget->key_target = NULL;
  window_close(widget_get_window(item));
  widget_set_focused_internal(widget, TRUE);

  return RET_OK;
}

static ret_t combo_box_visit_item(void* ctx, const void* data) {
  widget_t* iter = WIDGET(data);
  combo_box_t* combo_box = COMBO_BOX(ctx);
  return_value_if_fail(combo_box != NULL && iter != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(widget_get_type(iter), WIDGET_TYPE_COMBO_BOX_ITEM)) {
    int32_t index = widget_index_of(iter);

    widget_on(iter, EVT_CLICK, combo_box_on_item_click, combo_box);

    if (index == combo_box->selected_index) {
      COMBO_BOX_ITEM(iter)->checked = TRUE;
      widget_set_need_update_style(iter);
      widget_set_focused(iter, TRUE);
    }
  }

  return RET_OK;
}

static ret_t combo_box_hook_items(combo_box_t* combo_box, widget_t* popup) {
  return widget_foreach(popup, combo_box_visit_item, combo_box);
}

static ret_t combo_box_create_popup_items(combo_box_t* combo_box, widget_t* parent) {
  combo_box_option_t* iter = NULL;
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  while (iter != NULL) {
    widget_t* item = combo_box_item_create(parent, 0, 0, 0, 0);

    widget_set_value(item, iter->value);
    if (combo_box->localize_options) {
      widget_set_tr_text(item, iter->text);
    } else {
      widget_set_text_utf8(item, iter->text);
    }

    iter = iter->next;
  }

  return RET_OK;
}

ret_t combo_box_combobox_popup_on_close_func(void* ctx, event_t* e) {
  combo_box_t* combo_box = COMBO_BOX(ctx);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->combobox_popup = NULL;

  return RET_OK;
}

ret_t combo_box_combobox_popup_calc_position(widget_t* widget, wh_t popup_w, wh_t popup_h,
                                             point_t* p) {
  widget_t* wm = window_manager();
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL && p != NULL, RET_BAD_PARAMS);

  memset(p, 0x00, sizeof(point_t));
  widget_to_screen(widget, p);

  if ((p->y + widget->h + popup_h) < wm->h) {
    p->y += widget->h;
  } else if (p->y >= popup_h) {
    p->y -= combo_box->combobox_popup->h;
  } else {
    p->y = 0;
  }
  return RET_OK;
}

static widget_t* combo_box_create_popup(combo_box_t* combo_box) {
  value_t v;
  int32_t w = 0;
  int32_t h = 0;
  int32_t nr = 0;
  char params[128];
  widget_t* win = NULL;
  int32_t item_height = 0;
  widget_t* combo_box_win = NULL;
  const char* applet_name = NULL;
  widget_t* widget = WIDGET(combo_box);
  int32_t margin = COMBO_BOX_DEFAULT_MARGIN;
  return_value_if_fail(combo_box != NULL && widget != NULL, NULL);
  item_height = combo_box->item_height;
  nr = combo_box_count_options(widget);
  w = widget->w;
  h = nr * item_height + 2 * margin;
  win = popup_create(NULL, 0, 0, w, h);
  combo_box_win = widget_get_window(widget);
  applet_name = widget_get_prop_str(combo_box_win, WIDGET_PROP_APPLET_NAME, NULL);
  widget_set_prop_str(win, WIDGET_PROP_APPLET_NAME, applet_name);

  value_set_bool(&v, TRUE);
  widget_set_prop(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, &v);
  widget_set_prop_str(win, WIDGET_PROP_THEME, "combobox_popup");

  tk_snprintf(params, sizeof(params) - 1, "default(m=%d, r=%d c=%d)", margin, nr, 1);
  widget_set_children_layout(win, params);

  widget_use_style(win, "combobox_popup");
  combo_box_create_popup_items(combo_box, win);
  widget_layout(win);

  combo_box->combobox_popup = win;

  widget_on(win, EVT_WINDOW_CLOSE, combo_box_combobox_popup_on_close_func, widget);
  return win;
}

static ret_t combo_box_active(widget_t* widget) {
  point_t p = {0, 0};
  widget_t* win = NULL;
  widget_t* combo_box_win = NULL;
  const char* applet_name = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);
  if (combo_box->open_window) {
    win = window_open(combo_box->open_window);
    combo_box_win = widget_get_window(widget);
    applet_name = widget_get_prop_str(combo_box_win, WIDGET_PROP_APPLET_NAME, NULL);
    widget_set_prop_str(win, WIDGET_PROP_APPLET_NAME, applet_name);

    combo_box->combobox_popup = win;
    widget_on(win, EVT_WINDOW_CLOSE, combo_box_combobox_popup_on_close_func, widget);
  }

  if (win != NULL) {
    widget_resize(win, widget->w, win->h);
    widget_layout_children(win);
  } else {
    if (combo_box->open_popup) {
      win = combo_box->open_popup(widget);
      return_value_if_fail(win != NULL, RET_FAIL);

      widget_resize(win, tk_max_int(win->w, widget->w), win->h);
      widget_layout_children(win);
    } else {
      win = combo_box_create_popup(combo_box);
      return_value_if_fail(win != NULL, RET_FAIL);
    }
  }

  if (combo_box->theme_of_popup != NULL) {
    widget_set_prop_str(win, WIDGET_PROP_THEME, combo_box->theme_of_popup);
  }
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_PREV_KEY, "up");
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY, "down");
  combo_box_hook_items(combo_box, win);

  combo_box_combobox_popup_calc_position(widget, win->w, win->h, &p);
  widget_move(win, p.x, p.y);

  return RET_OK;
}

static ret_t combo_box_on_button_click(void* ctx, event_t* e) {
  return combo_box_active(WIDGET(ctx));
}

widget_t* combo_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(combo_box), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  combo_box_init(widget);
  return widget;
}

ret_t combo_box_remove_option_by_index(widget_t* widget, uint32_t index) {
  uint32_t i = 0;
  combo_box_option_t* iter = NULL;
  combo_box_option_t* prev = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  prev = combo_box->option_items;

  while (iter != NULL) {
    if (i == index) {
      if (iter == combo_box->option_items) {
        combo_box->option_items = iter->next;
      } else {
        prev->next = iter->next;
      }

      TKMEM_FREE(iter->text);
      TKMEM_FREE(iter);
      return RET_OK;
    }
    i++;
    prev = iter;
    iter = iter->next;
  }

  return RET_NOT_FOUND;
}

ret_t combo_box_remove_option(widget_t* widget, int32_t value) {
  combo_box_option_t* iter = NULL;
  combo_box_option_t* prev = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  prev = combo_box->option_items;

  while (iter != NULL) {
    if (iter->value == value) {
      if (iter == combo_box->option_items) {
        combo_box->option_items = iter->next;
      } else {
        prev->next = iter->next;
      }

      TKMEM_FREE(iter->text);
      TKMEM_FREE(iter);
      return RET_OK;
    }
    prev = iter;
    iter = iter->next;
  }

  return RET_NOT_FOUND;
}

ret_t combo_box_reset_options(widget_t* widget) {
  combo_box_option_t* iter = NULL;
  combo_box_option_t* next = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  while (iter != NULL) {
    next = iter->next;
    TKMEM_FREE(iter->text);
    TKMEM_FREE(iter);
    iter = next;
  }
  combo_box->option_items = NULL;
  TKMEM_FREE(combo_box->options);

  return RET_OK;
}

ret_t combo_box_set_open_window(widget_t* widget, const char* open_window) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  ENSURE(combo_box);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(combo_box->open_window);
  if (TK_STR_IS_NOT_EMPTY(open_window)) {
    combo_box->open_window = tk_strdup(open_window);
  }

  return RET_OK;
}

ret_t combo_box_set_theme_of_popup(widget_t* widget, const char* theme_of_popup) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  ENSURE(combo_box);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(combo_box->theme_of_popup);
  if (TK_STR_IS_NOT_EMPTY(theme_of_popup)) {
    combo_box->theme_of_popup = tk_strdup(theme_of_popup);
  }

  return RET_OK;
}

ret_t combo_box_append_option(widget_t* widget, int32_t value, const char* text) {
  combo_box_option_t* iter = NULL;
  combo_box_option_t* option = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL && text != NULL, RET_BAD_PARAMS);

  option = TKMEM_ZALLOC(combo_box_option_t);
  return_value_if_fail(option != NULL, RET_OOM);

  option->value = value;
  option->text = tk_strdup(text);

  if (option->text == NULL) {
    TKMEM_FREE(option);
    return RET_OOM;
  }

  if (combo_box->option_items != NULL) {
    iter = combo_box->option_items;
    while (iter->next != NULL) iter = iter->next;
    iter->next = option;
  } else {
    combo_box->option_items = option;
  }

  return RET_OK;
}

int32_t combo_box_count_options(widget_t* widget) {
  int32_t nr = 0;
  combo_box_option_t* iter = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, nr);

  iter = combo_box->option_items;
  while (iter != NULL) {
    nr++;
    iter = iter->next;
  }

  return nr;
}

combo_box_option_t* combo_box_get_option(widget_t* widget, uint32_t index) {
  uint32_t i = 0;
  combo_box_option_t* iter = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, NULL);

  iter = combo_box->option_items;
  while (iter != NULL) {
    if (i == index) {
      return iter;
    }

    i++;
    iter = iter->next;
  }

  return NULL;
}

bool_t combo_box_has_option_text(widget_t* widget, const char* text) {
  combo_box_option_t* iter = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, FALSE);

  iter = combo_box->option_items;
  while (iter != NULL) {
    if (tk_str_eq(iter->text, text)) {
      return TRUE;
    }

    iter = iter->next;
  }

  return FALSE;
}

int32_t combo_box_find_option(widget_t* widget, int32_t value) {
  uint32_t i = 0;
  combo_box_option_t* iter = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, 0);

  iter = combo_box->option_items;
  while (iter != NULL) {
    if (iter->value == value) {
      return i;
    }

    i++;
    iter = iter->next;
  }

  return 0;
}

static int32_t combo_box_find_option_of_text(widget_t* widget, const char* text) {
  uint32_t i = 0;
  combo_box_option_t* iter = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, 0);

  iter = combo_box->option_items;
  while (iter != NULL) {
    if (tk_str_eq(iter->text, text)) {
      return i;
    }

    i++;
    iter = iter->next;
  }

  return 0;
}

static ret_t combo_box_sync_index_to_value(widget_t* widget, uint32_t index, bool_t only_set) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  if (combo_box->option_items != NULL) {
    combo_box_option_t* option = combo_box_get_option(widget, index);

    if (option != NULL) {
      combo_box->value = option->value;
      if (only_set) {
        combo_box_set_text_only(widget, option->text, NULL, combo_box->localize_options);
      } else {
        combo_box_set_text(widget, option->text, NULL, combo_box->localize_options);
      }
    }
  }

  return RET_OK;
}

static ret_t combo_box_set_selected_index_ex(widget_t* widget, uint32_t index, widget_t* item) {
  edit_t* edit = EDIT(widget);
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_OK);

  if (combo_box->selected_index != index) {
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_uint32(&(evt.old_value), combo_box->selected_index);
    value_set_uint32(&(evt.new_value), index);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      if (widget->emitter != NULL) {
        emitter_disable(widget->emitter);
      }
      combo_box->selected_index = index;
      if (item != NULL) {
        combo_box->value = COMBO_BOX_ITEM(item)->value;
        if (item->tr_text != NULL) {
          combo_box_set_text(widget, item->tr_text, NULL, TRUE);
        } else {
          combo_box_set_text(widget, NULL, item->text.str, FALSE);
        }
      } else {
        combo_box_sync_index_to_value(widget, index, FALSE);
      }
      if (widget->emitter != NULL) {
        emitter_enable(widget->emitter);
      }

      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  } else {
    combo_box_sync_index_to_value(widget, index, FALSE);
  }

  if (edit->select_none_when_focused || edit->readonly || combo_box->combobox_popup == NULL) {
    text_edit_unselect(edit->model);
  } else {
    text_edit_select_all(edit->model);
  }
  return widget_invalidate_force(widget, NULL);
}

static ret_t combo_box_add_selected_index(widget_t* widget, int32_t delta) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  ENSURE(combo_box);
  uint32_t nr = combo_box_count_options(widget);

  if (nr > 0) {
    uint32_t selected_index = (combo_box->selected_index + delta + nr) % nr;
    combo_box_set_selected_index(widget, selected_index);
  }

  return RET_OK;
}

ret_t combo_box_set_selected_index(widget_t* widget, uint32_t index) {
  return combo_box_set_selected_index_ex(widget, index, NULL);
}

ret_t combo_box_set_value(widget_t* widget, int32_t value) {
  int32_t index = combo_box_find_option(widget, value);

  return combo_box_set_selected_index(widget, index);
}

int32_t combo_box_get_value(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, 0);

  return combo_box->value;
}

ret_t combo_box_set_item_height(widget_t* widget, uint32_t item_height) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->item_height = item_height;

  return RET_OK;
}

ret_t combo_box_set_localize_options(widget_t* widget, bool_t localize_options) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->localize_options = localize_options;

  return RET_OK;
}

const char* combo_box_get_text(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, NULL);

  if (widget->tr_text != NULL && EDIT(widget)->readonly) {
    return widget->tr_text;
  } else {
    str_from_wstr(&(combo_box->text), widget->text.str);
    return combo_box->text.str;
  }
}

const char* combo_box_get_text_of_selected(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  combo_box_option_t* option = NULL;
  return_value_if_fail(combo_box != NULL, NULL);

  option = combo_box_get_option(widget, combo_box->selected_index);
  return_value_if_fail(option != NULL, NULL);

  return option->text;
}

ret_t combo_box_set_selected_index_by_text(widget_t* widget, const char* text) {
  return combo_box_set_selected_index(widget, combo_box_find_option_of_text(widget, text));
}

widget_t* combo_box_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, combo_box), NULL);

  return widget;
}

ret_t combo_box_set_custom_open_popup(
    widget_t* widget, combo_box_custom_open_popup_t open_popup,
    combo_box_custom_on_layout_combobox_popup_t on_layout_combobox_popup) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->open_popup = open_popup;
  combo_box->on_layout_combobox_popup = on_layout_combobox_popup;

  return RET_OK;
}
