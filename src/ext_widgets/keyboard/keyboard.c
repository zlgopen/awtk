/**
 * File:   keyboard.c
 * Author: AWTK Develop Team
 * Brief:  keyboard
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/keys.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "widgets/pages.h"
#include "base/locale_info.h"
#include "keyboard/keyboard.h"
#include "base/input_method.h"
#include "base/window_manager.h"

#define WIDGET_PROP_IM_LANG "lang"
#define WIDGET_PROP_PREEDIT_INDEX "preedit_index"

static ret_t keyboard_on_load(void* ctx, event_t* e);
static ret_t keyboard_on_action_info(void* ctx, event_t* e);

static const char* s_keyboard_properties[] = {WIDGET_PROP_ANIM_HINT, WIDGET_PROP_OPEN_ANIM_HINT,
                                              WIDGET_PROP_CLOSE_ANIM_HINT, WIDGET_PROP_THEME, NULL};

static ret_t keyboard_on_destroy(widget_t* widget) {
  keyboard_t* keyboard = KEYBOARD(widget);
  return_value_if_fail(keyboard != NULL, RET_BAD_PARAMS);
  input_method_off(input_method(), keyboard->action_info_id);
  darray_deinit(&(keyboard->action_buttons));
  str_reset(&(keyboard->temp));

  return window_base_on_destroy(widget);
}

static ret_t keyboard_on_event(widget_t* widget, event_t* e) {
  keyboard_t* keyboard = KEYBOARD(widget);
  if (e->type == EVT_KEY_DOWN || e->type == EVT_KEY_UP) {
    key_event_t* evt = (key_event_t*)e;
    /*goto here only when grab_keys=true*/
    if (e->type == EVT_KEY_DOWN) {
      keyboard->key_down = evt->key;
    } else {
      if (keyboard->key_down == evt->key) {
        input_method_dispatch_key(input_method(), evt->key);
        keyboard->key_down = 0;
      }
    }
    return RET_STOP;
  }

  return window_base_on_event(widget, e);
}

TK_DECL_VTABLE(keyboard) = {.size = sizeof(keyboard_t),
                            .type = WIDGET_TYPE_KEYBOARD,
                            .is_window = TRUE,
                            .is_keyboard = TRUE,
                            .clone_properties = s_keyboard_properties,
                            .persistent_properties = s_keyboard_properties,
                            .parent = TK_PARENT_VTABLE(window_base),
                            .create = keyboard_create,
                            .on_event = keyboard_on_event,
                            .on_paint_self = window_base_on_paint_self,
                            .on_paint_begin = window_base_on_paint_begin,
                            .on_paint_end = window_base_on_paint_end,
                            .set_prop = window_base_set_prop,
                            .get_prop = window_base_get_prop,
                            .on_destroy = keyboard_on_destroy};

widget_t* keyboard_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = window_base_create(parent, TK_REF_VTABLE(keyboard), x, y, w, h);
  keyboard_t* keyboard = KEYBOARD(widget);
  return_value_if_fail(keyboard != NULL, NULL);

  str_init(&(keyboard->temp), 0);
  darray_init(&(keyboard->action_buttons), 0, NULL, NULL);
  widget_on(widget, EVT_WINDOW_LOAD, keyboard_on_load, widget);
  keyboard->action_info_id =
      input_method_on(input_method(), EVT_IM_ACTION_INFO, keyboard_on_action_info, widget);

  return widget;
}

static ret_t on_visit_widget_to_focus_first(void* ctx, const void* data) {
  widget_t* w = WIDGET(data);

  if (w->focusable) {
    widget_set_focused(w, TRUE);

    return RET_STOP;
  }

  return RET_OK;
}

static ret_t keyboard_focus_first(widget_t* page) {
  widget_foreach(page, on_visit_widget_to_focus_first, NULL);

  return RET_OK;
}

static ret_t keyboard_set_active_page(widget_t* button, const char* name) {
  widget_t* iter = button;
  while (iter != NULL) {
    const char* type = widget_get_type(iter);
    if (tk_str_eq(type, WIDGET_TYPE_PAGES)) {
      break;
    }
    iter = iter->parent;
  }

  return_value_if_fail(iter != NULL, RET_FAIL);
  if (pages_set_active_by_name(iter, name) == RET_OK) {
    value_t v;
    widget_t* active = widget_get_child(iter, widget_get_value(iter));

    if (widget_get_prop(active, WIDGET_PROP_IM_LANG, &v) == RET_OK) {
      input_method_set_lang(input_method(), value_str(&v));
    }

    keyboard_focus_first(widget_get_child(iter, PAGES(iter)->active));
  }

  return RET_OK;
}

#define STR_BACK "back"
#define STR_CLOSE "close"
#define STR_RETURN "return"
#define STR_ACTION "action"
#define STR_KEY_SPACE "space"
#define STR_BACK_TO_HOME "back_to_home"

#define STR_KEY_OPT "opt:"
#define STR_KEY_TAB "tab"
#define STR_KEY_PREFIX "key:"
#define STR_PAGE_PREFIX "page:"
#define STR_HARD_KEY_PREFIX "hard_key:"
#define STR_KEY_BACKSPACE "backspace"
#define STR_KEY_CONFIRM_PREEDIT "confirm_preedit"

#ifndef PREEDIT_TIMER_DURATION
#define PREEDIT_TIMER_DURATION 800
#endif /*PREEDIT_TIMER_DURATION*/

static ret_t keyboard_preedit_confirm(widget_t* widget) {
  input_method_t* im = input_method();
  keyboard_t* keyboard = KEYBOARD(widget);

  input_method_dispatch_preedit_confirm(im);
  widget_set_prop_int(widget, WIDGET_PROP_PREEDIT_INDEX, -1);

  keyboard->timer_id = TK_INVALID_ID;

  return RET_OK;
}

static ret_t keyboard_on_preedit_timeout(const timer_info_t* info) {
  keyboard_preedit_confirm(WIDGET(info->ctx));

  return RET_REMOVE;
}

static ret_t keyboard_on_button_click(void* ctx, event_t* e) {
  input_method_t* im = input_method();
  widget_t* button = WIDGET(e->target);
  widget_t* win = widget_get_window(button);
  keyboard_t* keyboard = KEYBOARD(win);

  const char* name = button->name;
  const char* opt = strstr(name, STR_KEY_OPT);
  const char* key = strstr(name, STR_KEY_PREFIX);
  const char* hard_key = strstr(name, STR_HARD_KEY_PREFIX);
  const char* page_name = strstr(name, STR_PAGE_PREFIX);

  if (keyboard->last_click_button != button) {
    if (keyboard->timer_id != TK_INVALID_ID) {
      timer_remove(keyboard->timer_id);
      keyboard_preedit_confirm(win);
    }
  }

  keyboard->last_click_button = button;
  if (tk_str_eq(name, STR_CLOSE)) {
    input_method_request(im, NULL);

    return RET_OK;
  }

  if (tk_str_eq(name, STR_BACK)) {
    window_manager_back(window_manager());
    return RET_OK;
  }

  if (tk_str_eq(name, STR_BACK_TO_HOME)) {
    window_manager_back_to_home(window_manager());
    return RET_OK;
  }

  if (page_name != NULL) {
    return keyboard_set_active_page(button, page_name + strlen(STR_PAGE_PREFIX));
  } else if (hard_key != NULL) {
    const key_type_value_t* kv = NULL;
    key_event_t key_event;
    hard_key += strlen(STR_HARD_KEY_PREFIX);
    kv = keys_type_find(hard_key);
    return_value_if_fail(kv != NULL, RET_BAD_PARAMS);

    widget_dispatch(window_manager(), key_event_init(&key_event, EVT_KEY_DOWN, NULL, kv->value));
    widget_dispatch(window_manager(), key_event_init(&key_event, EVT_KEY_UP, NULL, kv->value));

    return RET_OK;
  } else if (key != NULL) {
    const key_type_value_t* kv = NULL;

    key += strlen(STR_KEY_PREFIX);
    kv = keys_type_find(key);
    return_value_if_fail(kv != NULL, RET_BAD_PARAMS);

    return input_method_dispatch_key(im, kv->value);
  } else if (opt != NULL) {
    str_t* str = &(keyboard->temp);
    int32_t index = widget_get_prop_int(win, WIDGET_PROP_PREEDIT_INDEX, -1);
    if (index < 0) {
      index = -1;
      input_method_dispatch_preedit(im);
    }

    if (keyboard->timer_id == TK_INVALID_ID) {
      keyboard->timer_id =
          widget_add_timer(win, keyboard_on_preedit_timeout, PREEDIT_TIMER_DURATION);
    } else {
      timer_modify(keyboard->timer_id, PREEDIT_TIMER_DURATION);
    }

    index = (index + 1) % button->text.size;
    str_from_wstr_with_len(str, button->text.str + index, 1);
    widget_set_prop_int(win, WIDGET_PROP_PREEDIT_INDEX, index);

    return input_method_commit_text(input_method(), str->str);
  } else if (tk_str_eq(name, STR_KEY_CONFIRM_PREEDIT)) {
    if (keyboard->timer_id != TK_INVALID_ID) {
      timer_remove(keyboard->timer_id);
      keyboard_preedit_confirm(win);
    }

    return RET_OK;
  } else {
    if (tk_str_eq(name, STR_ACTION)) {
      return input_method_dispatch_action(im);
    } else if (tk_str_eq(name, STR_KEY_SPACE)) {
      name = " ";
    }

    return input_method_commit_text(input_method(), name);
  }
}

static ret_t keyboard_update_action_buton_info(widget_t* button, const char* text, bool_t enable) {
  text = locale_info_tr(locale_info(), ((text && *text) ? text : STR_RETURN));

  widget_set_text_utf8(button, text);
  widget_set_enable(button, enable);

  return RET_OK;
}

static ret_t keyboard_on_action_info(void* ctx, event_t* e) {
  uint32_t i = 0;
  uint32_t nr = 0;
  widget_t** buttons = NULL;
  input_method_t* im = input_method();
  keyboard_t* keyboard = KEYBOARD(ctx);
  return_value_if_fail(keyboard != NULL && im != NULL, RET_BAD_PARAMS);

  nr = keyboard->action_buttons.size;
  buttons = (widget_t**)keyboard->action_buttons.elms;

  for (i = 0; i < nr; i++) {
    keyboard_update_action_buton_info(buttons[i], im->action_buton_text, im->action_button_enable);
  }

  return RET_OK;
}

static ret_t keyboard_hook_buttons(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  input_method_t* im = input_method();
  keyboard_t* keyboard = KEYBOARD(ctx);
  const char* name = widget->name;
  const char* type = widget_get_type(widget);

  if (tk_str_eq(type, WIDGET_TYPE_BUTTON) && name != NULL) {
    widget_on(widget, EVT_CLICK, keyboard_on_button_click, keyboard);

    if (tk_str_eq(name, STR_ACTION)) {
      darray_push(&(keyboard->action_buttons), widget);
      keyboard_update_action_buton_info(widget, im->action_buton_text, im->action_button_enable);
    }
  }

  return RET_OK;
}

static ret_t keyboard_on_load(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* pages = widget_lookup_by_type(widget, WIDGET_TYPE_PAGES, TRUE);

  (void)e;
  widget_foreach(widget, keyboard_hook_buttons, widget);
  if (pages != NULL) {
    keyboard_focus_first(widget_get_child(pages, PAGES(pages)->active));
  } else {
    keyboard_focus_first(widget);
  }

  return RET_OK;
}

ret_t keyboard_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

widget_t* keyboard_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, keyboard), NULL);

  return widget;
}
