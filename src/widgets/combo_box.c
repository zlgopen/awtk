/**
 * File:   combo_box.h
 * Author: AWTK Develop Team
 * Brief:  combo_box
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/window.h"
#include "widgets/button.h"
#include "widgets/popup.h"
#include "widgets/combo_box.h"
#include "tkc/tokenizer.h"
#include "widgets/combo_box_item.h"

static ret_t combo_box_on_button_click(void* ctx, event_t* e);
static ret_t combo_box_sync_index_to_value(widget_t* widget, uint32_t index);

const char* const s_combo_box_properties[] = {WIDGET_PROP_MIN,
                                              WIDGET_PROP_MAX,
                                              WIDGET_PROP_STEP,
                                              WIDGET_PROP_INPUT_TYPE,
                                              WIDGET_PROP_READONLY,
                                              WIDGET_PROP_AUTO_FIX,
                                              WIDGET_PROP_MARGIN,
                                              WIDGET_PROP_LEFT_MARGIN,
                                              WIDGET_PROP_RIGHT_MARGIN,
                                              WIDGET_PROP_TOP_MARGIN,
                                              WIDGET_PROP_BOTTOM_MARGIN,
                                              WIDGET_PROP_TIPS,
                                              WIDGET_PROP_OPTIONS,
                                              WIDGET_PROP_ITEM_HEIGHT,
                                              WIDGET_PROP_OPEN_WINDOW,
                                              WIDGET_PROP_SELECTED_INDEX,
                                              WIDGET_PROP_LOCALIZE_OPTIONS,
                                              NULL};

static ret_t combo_box_active(widget_t* widget);
static ret_t combo_box_add_selected_index(widget_t* widget, int32_t delta);
static widget_t* combo_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
static ret_t combo_box_set_selected_index_ex(widget_t* widget, uint32_t index, widget_t* item);

static ret_t combo_box_on_copy(widget_t* widget, widget_t* other) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  combo_box_t* combo_box_other = COMBO_BOX(other);

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

  return RET_OK;
}

static ret_t combo_box_on_destroy(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  str_reset(&(combo_box->text));
  combo_box_reset_options(widget);
  TKMEM_FREE(combo_box->open_window);

  edit_on_destroy(widget);

  return RET_OK;
}

#define WIDGET_NAME_VALUE "value"

static ret_t combo_box_set_text(widget_t* widget, const char* text, const wchar_t* wtext,
                                bool_t tr) {
  widget_t* value_widget = widget_lookup(widget, WIDGET_NAME_VALUE, TRUE);

  if (value_widget == NULL) {
    value_widget = widget;
  } else {
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

  return RET_OK;
}

static ret_t combo_box_get_prop(widget_t* widget, const char* name, value_t* v) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_OPEN_WINDOW)) {
    value_set_str(v, combo_box->open_window);
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
  } else {
    return edit_get_prop(widget, name, v);
  }
}

ret_t combo_box_parse_options(widget_t* widget, const char* str) {
  int32_t i = 0;
  tokenizer_t tokenizer;
  tokenizer_t* t = &tokenizer;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL && str != NULL, RET_BAD_PARAMS);

  combo_box_reset_options(widget);
  combo_box->options = tk_strdup(str);
  tokenizer_init(t, str, strlen(str), ";");

  while (tokenizer_has_more(t)) {
    int32_t value = 0;
    const char* token = tokenizer_next(t);
    if (*token) {
      const char* text = strchr(token, ':');
      if (text != NULL) {
        text++;
        value = tk_atoi(token);
      } else {
        text = token;
        value = i;
      }

      combo_box_append_option(widget, value, text);
      i++;
    }
  }
  tokenizer_deinit(t);

  return RET_OK;
}

ret_t combo_box_set_options(widget_t* widget, const char* options) {
  return combo_box_parse_options(widget, options);
}

static ret_t combo_box_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, WIDGET_PROP_OPEN_WINDOW)) {
    combo_box_set_open_window(widget, value_str(v));
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
    combo_box_parse_options(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    combo_box_set_item_height(widget, value_uint32(v));
    return RET_OK;
  } else {
    return edit_set_prop(widget, name, v);
  }
}

static ret_t combo_box_on_layout_children(widget_t* widget) {
  widget_t* button = widget_lookup_by_type(widget, WIDGET_TYPE_BUTTON, TRUE);

  widget_layout_children_default(widget);
  if (button != NULL) {
    if (button->auto_created) {
      widget_move_resize(button, widget->w - widget->h, 0, widget->h, widget->h);
    } else {
      widget_layout(button);
    }
  }

  return RET_OK;
}

static uint32_t edit_get_right_margin(widget_t* widget) {
  int32_t right_margin = 0;
  style_t* style = widget->astyle;

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
    if (edit->readonly) {
      ret = RET_STOP;
      combo_box_active(widget);
    } else if (key_code_is_enter(evt->key)) {
      ret = RET_STOP;
      combo_box_active(widget);
    }
  }

  return ret;
}

static ret_t combo_box_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  combo_box_t* combo_box = COMBO_BOX(widget);
  edit_t* edit = EDIT(WIDGET(combo_box));
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WIDGET_LOAD: {
      /*If there is a value widget, sync the text to value widget*/
      if (widget_lookup(widget, WIDGET_NAME_VALUE, TRUE) != NULL) {
        combo_box_sync_index_to_value(widget, combo_box->selected_index);
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      if (edit_get_right_margin(widget) == 0) {
        edit->right_margin = widget->h;
        edit->left_margin = 4;
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
    }

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

TK_DECL_VTABLE(combo_box) = {.size = sizeof(combo_box_t),
                             .inputable = TRUE,
                             .type = WIDGET_TYPE_COMBO_BOX,
                             .focusable = TRUE,
                             .pointer_cursor = WIDGET_CURSOR_EDIT,
                             .space_key_to_activate = TRUE,
                             .return_key_to_activate = TRUE,
                             .clone_properties = s_combo_box_properties,
                             .persistent_properties = s_combo_box_properties,
                             .parent = TK_PARENT_VTABLE(edit),
                             .create = combo_box_create_self,
                             .on_paint_self = edit_on_paint_self,
                             .set_prop = combo_box_set_prop,
                             .get_prop = combo_box_get_prop,
                             .on_add_child = combo_box_on_add_child,
                             .on_layout_children = combo_box_on_layout_children,
                             .on_destroy = combo_box_on_destroy,
                             .on_copy = combo_box_on_copy,
                             .on_event = combo_box_on_event};

widget_t* combo_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = edit_create_ex(parent, TK_REF_VTABLE(combo_box), x, y, w, h);
  combo_box_t* combo_box = COMBO_BOX(widget);
  edit_t* edit = EDIT(WIDGET(combo_box));
  return_value_if_fail(combo_box != NULL, NULL);

  edit->left_margin = 0;
  edit->right_margin = 0;
  str_init(&(combo_box->text), 32);
  combo_box->localize_options = TRUE;

  return widget;
}

static ret_t combo_box_on_item_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* item = WIDGET(e->target);

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

static widget_t* combo_box_create_popup(combo_box_t* combo_box) {
  value_t v;
  char params[128];
  int32_t margin = 2;
  widget_t* widget = WIDGET(combo_box);
  int32_t item_height = combo_box->item_height;
  int32_t nr = combo_box_count_options(widget);
  int32_t w = widget->w;
  int32_t h = nr * item_height + 2 * margin;
  widget_t* win = popup_create(NULL, 0, 0, w, h);

  value_set_bool(&v, TRUE);
  widget_set_prop(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, &v);

  tk_snprintf(params, sizeof(params) - 1, "default(m=%d, r=%d c=%d)", margin, nr, 1);
  widget_set_children_layout(win, params);

  widget_use_style(win, "combobox_popup");
  combo_box_create_popup_items(combo_box, win);
  widget_layout(win);

  return win;
}

static ret_t combo_box_active(widget_t* widget) {
  point_t p = {0, 0};
  widget_t* wm = NULL;
  widget_t* win = NULL;
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);
  if (combo_box->open_window) {
    win = window_open(combo_box->open_window);
  }

  if (win != NULL) {
    widget_resize(win, widget->w, win->h);
    widget_layout_children(win);
  } else {
    if (combo_box->open_popup) {
      win = combo_box->open_popup(widget);
      return_value_if_fail(win != NULL, RET_FAIL);

      widget_resize(win, widget->w, win->h);
      widget_layout_children(win);
    } else {
      win = combo_box_create_popup(combo_box);
      return_value_if_fail(win != NULL, RET_FAIL);
    }
  }
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_PREV_KEY, "up");
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY, "down");
  combo_box_hook_items(combo_box, win);
  widget_to_screen(widget, &p);

  wm = win->parent;
  if ((p.y + widget->h + win->h) < wm->h) {
    p.y += widget->h;
  } else if (p.y >= win->h) {
    p.y -= win->h;
  } else {
    p.y = 0;
  }

  widget_move(win, p.x, p.y);

  return RET_OK;
}

static ret_t combo_box_on_button_click(void* ctx, event_t* e) {
  return combo_box_active(WIDGET(ctx));
}

widget_t* combo_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* popup = NULL;
  widget_t* widget = combo_box_create_self(parent, x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  popup = button_create(widget, 0, 0, 0, 0);
  popup->auto_created = TRUE;
  combo_box_set_item_height(widget, 30);
  widget_use_style(popup, "combobox_down");

  return widget;
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
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(combo_box->open_window);
  combo_box->open_window = tk_strdup(open_window);

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

static ret_t combo_box_sync_index_to_value(widget_t* widget, uint32_t index) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(widget != NULL && combo_box != NULL, RET_BAD_PARAMS);

  if (combo_box->option_items != NULL) {
    combo_box_option_t* option = combo_box_get_option(widget, index);

    if (option != NULL) {
      combo_box->value = option->value;
      combo_box_set_text(widget, option->text, NULL, combo_box->localize_options);
    }
  }

  return RET_OK;
}

static ret_t combo_box_set_selected_index_ex(widget_t* widget, uint32_t index, widget_t* item) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_OK);

  if (combo_box->selected_index != index) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);

    emitter_disable(widget->emitter);
    combo_box->selected_index = index;
    if (item != NULL) {
      combo_box->value = COMBO_BOX_ITEM(item)->value;
      if (item->tr_text != NULL) {
        combo_box_set_text(widget, item->tr_text, NULL, TRUE);
      } else {
        combo_box_set_text(widget, NULL, item->text.str, FALSE);
      }
    } else {
      combo_box_sync_index_to_value(widget, index);
    }
    emitter_enable(widget->emitter);

    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
  } else {
    combo_box_sync_index_to_value(widget, index);
  }

  return widget_invalidate_force(widget, NULL);
}

static ret_t combo_box_add_selected_index(widget_t* widget, int32_t delta) {
  combo_box_t* combo_box = COMBO_BOX(widget);
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

widget_t* combo_box_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, combo_box), NULL);

  return widget;
}

ret_t combo_box_set_custom_open_popup(widget_t* widget, combo_box_custom_open_popup_t open_popup) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  combo_box->open_popup = open_popup;

  return RET_OK;
}
