﻿/**
 * File:   edit.h
 * Author: AWTK Develop Team
 * Brief:  edit
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 * 2018-11-20 Li YaoShen  <liyaoshen@zlg.cn> 1.text selection; 2.cursor movable
 * 2018-06-24 Li XianJing <xianjimli@hotmail.com> rework with stb_text_edit
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "widgets/edit.h"
#include "base/keys.h"
#include "base/idle.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/enums.h"
#include "base/events.h"
#include "base/clip_board.h"
#include "base/widget_vtable.h"
#include "base/window_manager.h"
#include "widgets/edit_ipv4.h"
#include "widgets/edit_date.h"
#include "widgets/edit_time.h"
#include "widgets/edit_time_full.h"

#define ACTION_TEXT_NEXT "next"
#define ACTION_TEXT_DONE "done"

#define PASSWORD_MASK_CHAR '*'

static ret_t edit_auto_fix(widget_t* widget);
static ret_t edit_reset_layout(widget_t* widget);
static ret_t edit_update_status(widget_t* widget);
static ret_t edit_check_valid_value(widget_t* widget);
static ret_t edit_select_all_async(const idle_info_t* info);
static ret_t edit_dispatch_value_change_event(widget_t* widget, uint32_t type);
static ret_t edit_paste(widget_t* widget, const wchar_t* str, uint32_t size);

static ret_t edit_save_text(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->cancelable) {
    wstr_set(&(edit->saved_text), widget->text.str);
  }

  return RET_OK;
}

static ret_t edit_rollback_text(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->cancelable) {
    wstr_set(&(edit->last_changing_text), widget->text.str);
    widget_set_text(widget, edit->saved_text.str);
    edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);
  }

  return RET_OK;
}

static ret_t edit_commit_text(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->cancelable) {
    wstr_set(&(edit->saved_text), widget->text.str);
    edit_update_status(widget);
  }

  return RET_OK;
}

static ret_t edit_dispatch_value_change_event(widget_t* widget, uint32_t type) {
  value_change_event_t evt;
  edit_t* edit = EDIT(widget);
  wstr_t* text = &(widget->text);
  wstr_t* last = &(edit->last_changing_text);

  if (type == EVT_VALUE_CHANGED) {
    last = &(edit->last_changed_text);
  }

  if ((last->size == 0 && text->size == 0) || wstr_equal(last, text)) {
    return RET_NOT_MODIFIED;
  }

  value_change_event_init(&evt, type, widget);
  value_set_wstr(&(evt.old_value), last->str);
  value_set_wstr(&(evt.new_value), text->str);

  widget_dispatch(widget, (event_t*)&evt);

  if (type == EVT_VALUE_CHANGED) {
    wstr_set(last, text->str);
  }

  return RET_OK;
}

static ret_t edit_update_caret(const timer_info_t* timer) {
  edit_t* edit = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(timer != NULL, RET_REMOVE);

  edit = EDIT(timer->ctx);
  widget = WIDGET(timer->ctx);
  return_value_if_fail(edit != NULL && widget != NULL, RET_REMOVE);

  if (edit->readonly) {
    edit->timer_id = TK_INVALID_ID;
    text_edit_set_caret_visible(edit->model, FALSE);
    return RET_REMOVE;
  }

  widget_invalidate_force(widget, NULL);
  text_edit_invert_caret_visible(edit->model);

  if (widget->focused) {
    return RET_REPEAT;
  } else {
    edit->timer_id = TK_INVALID_ID;
    text_edit_set_caret_visible(edit->model, FALSE);
    return RET_REMOVE;
  }
}

static ret_t edit_start_update_caret(edit_t* edit) {
#define UPDATE_CARET_TIME 600
  if (edit->readonly) {
    text_edit_set_caret_visible(edit->model, FALSE);
  } else {
    if (edit->timer_id == TK_INVALID_ID) {
      edit->timer_id = timer_add(edit_update_caret, WIDGET(edit), UPDATE_CARET_TIME);
    } else {
      timer_reset(edit->timer_id);
    }
    text_edit_set_caret_visible(edit->model, TRUE);
  }
  return RET_OK;
}

ret_t edit_on_paint_self(widget_t* widget, canvas_t* c) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->input_type != INPUT_PASSWORD && edit->input_type != INPUT_CUSTOM_PASSWORD) {
    text_edit_set_mask(edit->model, FALSE);
  }

  return text_edit_paint(edit->model, c);
}

static bool_t edit_is_valid_char_default(widget_t* widget, wchar_t c) {
  wstr_t tmp;
  bool_t ret = FALSE;
  wstr_t* text = NULL;
  text_edit_state_t state;
  uint32_t cursor_pos = 0;
  edit_t* edit = EDIT(widget);
  input_type_t input_type = (input_type_t)0;

  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  wstr_init(&tmp, 0);
  text_edit_get_state(edit->model, &state);
  cursor_pos = state.cursor;
  if (state.select_start != state.select_end) {
    uint32_t end = state.select_start > state.select_end ? state.select_start : state.select_end;
    cursor_pos = state.select_start < state.select_end ? state.select_start : state.select_end;
    text = &tmp;
    wstr_set(&tmp, widget->text.str);
    wstr_remove(&tmp, cursor_pos, end - cursor_pos);
  } else {
    text = &(widget->text);
  }
  input_type = edit->input_type;

  switch (input_type) {
    case INPUT_INT:
    case INPUT_UINT: {
      if (text->size >= TK_NUM_MAX_LEN) {
        break;
      } else if (c >= '0' && c <= '9') {
        ret = TRUE;
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_INT)) {
        if (cursor_pos == 0) {
          ret = TRUE;
        }
        break;
      }
      break;
    }
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      if (text->size >= TK_NUM_MAX_LEN) {
        break;
      } else if (c >= '0' && c <= '9') {
        ret = TRUE;
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_FLOAT)) {
        if (cursor_pos == 0) {
          ret = TRUE;
        }
        break;
      } else if (c == '.' || c == 'e') {
        if (cursor_pos > 0 && wcschr(text->str, c) == NULL) {
          ret = TRUE;
        }
      }
      break;
    }
    case INPUT_EMAIL: {
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' ||
          c == '.' || c == '_' || c == ' ') {
        ret = TRUE;
      } else if (c == '@') {
        if (cursor_pos > 0 && wcschr(text->str, c) == NULL) {
          ret = TRUE;
        }
      }
      break;
    }
    case INPUT_HEX: {
      if (text->size > 10) {
        break;
      } else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
        ret = TRUE;
      } else if (c == 'X' || c == 'x') {
        if (cursor_pos == 1 && text->str[0] == '0') {
          ret = TRUE;
        }
      }
      break;
    }
    case INPUT_PHONE: {
      if (c >= '0' && c <= '9') {
        ret = TRUE;
        break;
      } else if (c == '-') {
        if (cursor_pos > 0 && wcschr(text->str, c) == NULL) {
          ret = TRUE;
        }
      }
      break;
    }
    default: {
      ret = TRUE;
      break;
    }
  }

  switch (edit->input_type) {
    case INPUT_ASCII:
    case INPUT_PHONE:
    case INPUT_EMAIL:
    case INPUT_TEXT:
    case INPUT_CUSTOM:
    case INPUT_CUSTOM_PASSWORD:
    case INPUT_PASSWORD: {
      if (text->size >= edit->max) {
        ret = FALSE;
      }
    }
    default:
      break;
  }

  wstr_reset(&tmp);
  return ret;
}

bool_t edit_is_valid_char(widget_t* widget, wchar_t c) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, FALSE);

  if (edit->is_valid_char != NULL) {
    return edit->is_valid_char(widget, c);
  } else {
    return edit_is_valid_char_default(widget, c);
  }
}

ret_t edit_input_char(widget_t* widget, wchar_t c) {
  return edit_paste(widget, &c, 1);
}

static ret_t edit_on_text_edit_text_will_delete(void* ctx, delete_type_t delete_type) {
  widget_t* widget = WIDGET(ctx);
  edit_t* edit = EDIT(widget);
  ret_t ret = RET_OK;
  uint32_t size1 = 0;
  uint32_t size2 = 0;
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  size1 = widget->text.size;
  if (edit->pre_delete != NULL) {
    ret = edit->pre_delete(widget, delete_type);
  }
  size2 = widget->text.size;
  edit->is_text_deleted = size1 != size2;

  return ret;
}

static ret_t edit_on_text_edit_char_will_input(void* ctx, wchar_t c) {
  widget_t* widget = WIDGET(ctx);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit_is_valid_char(widget, c)) {
    if (edit->is_text_deleted) {
      /*selection was cleared by edit->pre_delete, should not overwrite the next char*/
      text_edit_unselect(edit->model);
      edit->is_text_deleted = FALSE;
    }
    return RET_OK;
  }

  return RET_STOP;
}

static ret_t edit_commit_str(widget_t* widget, const char* str) {
  wchar_t wstr[32] = {0};
  tk_utf8_to_utf16(str, wstr, ARRAY_SIZE(wstr));

  return edit_paste(widget, wstr, wcslen(wstr));
}

static bool_t edit_is_number(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  input_type = edit->input_type;

  return input_type == INPUT_UINT || input_type == INPUT_INT || input_type == INPUT_FLOAT ||
         input_type == INPUT_UFLOAT || input_type == INPUT_HEX;
}

static bool_t edit_is_size_valid(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  ENSURE(edit);
  uint32_t size = widget->text.size;
  uint32_t min = (uint32_t)(edit->min);
  uint32_t max = (uint32_t)(edit->max);

  if (min == max && min == 0) {
    return TRUE;
  }

  return (min <= size && size <= max);
}

static bool_t edit_is_valid_value_default(widget_t* widget) {
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  text = &(widget->text);

  switch (edit->input_type) {
    case INPUT_ASCII:
    case INPUT_PHONE:
    case INPUT_EMAIL:
    case INPUT_TEXT:
    case INPUT_PASSWORD: {
      if (edit_is_size_valid(widget)) {
        if (edit->input_type == INPUT_EMAIL) {
          return wstr_count_char(text, '@') == 1;
        }

        return TRUE;
      }
      return FALSE;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int64_t v = 0;
      int64_t min = (int64_t)(edit->min);
      int64_t max = (int64_t)(edit->max);

      if (text->size == 0 || text->size > 11) {
        return FALSE;
      }

      if (min == max) {
        return TRUE;
      }

      wstr_to_int64(text, &v);
      if (text->size >= 10) {
        wstr_t str;
        bool_t result = FALSE;

        wstr_init(&str, 32);
        wstr_from_int64(&str, v);
        result = wstr_equal(&str, text);
        wstr_reset(&str);
        if (!result) {
          return FALSE;
        }
      }

      return min <= v && v <= max;
    }
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      double v = 0;
      double min = edit->min;
      double max = edit->max;

      if (text->size == 0) {
        return FALSE;
      }

      if (min == max) {
        return TRUE;
      }
      wstr_to_float(text, &v);

      return min <= v && v <= max;
    }
    default:
      break;
  }

  return TRUE;
}

static ret_t edit_auto_fix_default(widget_t* widget) {
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);

  switch (edit->input_type) {
    case INPUT_TEXT: {
      uint32_t size = text->size;
      uint32_t max = edit->max;

      if (size > max) {
        text->size = max;
        text->str[max] = 0;
      }

      break;
    }
    case INPUT_INT: {
      int32_t v = 0;
      int32_t min = (int32_t)(edit->min);
      int32_t max = (int32_t)(edit->max);

      wstr_to_int(text, &v);
      if (v < min) {
        v = min;
      }

      if (v > max) {
        v = max;
      }
      wstr_from_int(text, v);
      break;
    }
    case INPUT_UINT: {
      uint64_t v = 0;
      uint32_t min = (uint32_t)(edit->min);
      uint32_t max = (uint32_t)(edit->max);

      wstr_to_int64(text, (int64_t*)&v);
      if (v < min) {
        v = min;
      }

      if (v > max) {
        v = max;
      }
      wstr_from_int64(text, v);
      break;
    }
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      double v = 0;
      double min = edit->min;
      double max = edit->max;

      wstr_to_float(text, &v);
      if (v < min) {
        v = min;
      }

      if (v > max) {
        v = max;
      }
      wstr_from_float(text, v);
      wstr_trim_float_zero(text);
      break;
    }
    default:
      break;
  }

  text_edit_set_cursor(edit->model, 0xffffffff);

  return RET_OK;
}

static ret_t edit_update_status(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  ENSURE(edit);
  if (edit->is_activated) {
    return RET_OK;
  }
  if (widget->text.size == 0) {
    if (widget->focused) {
      widget_set_state(widget, WIDGET_STATE_EMPTY_FOCUS);
    } else {
      widget_set_state(widget, WIDGET_STATE_EMPTY);
    }
  } else {
    if (edit->cancelable) {
      if (widget->loading) {
        edit_save_text(widget);
      } else {
        if (!wstr_equal(&(edit->saved_text), &(widget->text))) {
          widget_set_state(widget, WIDGET_STATE_CHANGED);
          return RET_OK;
        }
      }
    }

    if (widget->focused) {
      widget_set_state(widget, WIDGET_STATE_FOCUSED);
    } else if (edit->is_text_error) {
      widget_set_state(widget, WIDGET_STATE_ERROR);
    } else {
      widget_set_state(widget, WIDGET_STATE_NORMAL);
    }
  }

  return RET_OK;
}

static ret_t edit_do_request_input_method(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_method_t* im = input_method();
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->readonly) {
    return RET_OK;
  }

  input_method_request(im, widget);
  if (edit->action_text != NULL) {
    const char* action_text = locale_info_tr(widget_get_locale_info(widget), edit->action_text);
    input_method_update_action_button_info(im, action_text, TRUE);
  }

  return RET_OK;
}

static ret_t edit_request_input_method_on_window_open(void* ctx, event_t* e) {
  edit_do_request_input_method(WIDGET(ctx));

  return RET_REMOVE;
}

static ret_t edit_request_input_method(widget_t* widget) {
  if (widget_is_window_opened(widget)) {
    edit_do_request_input_method(widget);
  } else {
    widget_t* win = widget_get_window(widget);
    if (win != NULL) {
      widget_on(win, EVT_WINDOW_OPEN, edit_request_input_method_on_window_open, widget);
    }
  }

  return RET_OK;
}

static ret_t edit_on_blur(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->close_im_when_blured) {
    edit->is_key_inputing = FALSE;
    input_method_request(input_method(), NULL);
  }

  if (edit->selected_idle_id != TK_INVALID_ID) {
    widget_remove_idle(widget, edit->selected_idle_id);
    edit->selected_idle_id = TK_INVALID_ID;
  }

  if (edit->selected_idle_id != TK_INVALID_ID) {
    timer_remove(edit->timer_id);
    edit->timer_id = TK_INVALID_ID;
  }
  text_edit_set_caret_visible(edit->model, FALSE);

  edit->is_text_error = FALSE;
  edit_update_status(widget);
  edit_check_valid_value(widget);
  text_edit_preedit_confirm(edit->model);
  text_edit_unselect(edit->model);
  edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGED);
  edit_commit_text(widget);
  return RET_OK;
}

static ret_t edit_focus_request_input_method(const idle_info_t* info) {
  edit_t* edit = EDIT(info->ctx);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->open_im_when_focused && edit->widget.target == NULL) {
    edit_request_input_method(WIDGET(edit));
  }

  return RET_REMOVE;
}

static ret_t edit_on_focused(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  edit_start_update_caret(edit);

  if (widget->target == NULL) {
    widget_add_idle(widget, edit_focus_request_input_method);

    if (!edit->select_none_when_focused && !edit->readonly) {
      edit->selected_idle_id = widget_add_idle(widget, edit_select_all_async);
    }
  }
  edit_update_status(widget);

  return RET_OK;
}

static ret_t edit_pointer_up_cleanup(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  widget_ungrab(widget->parent, widget);
  edit_update_status(widget);

  return RET_OK;
}

static ret_t edit_paste(widget_t* widget, const wchar_t* str, uint32_t size) {
  edit_t* edit = EDIT(widget);
  ret_t ret = RET_BAD_PARAMS;
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  wstr_set(&(edit->last_changing_text), widget->text.str);
  ret = text_edit_paste(edit->model, str, size);
  if (ret == RET_OK) {
    edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);
  }
  return ret;
}

static ret_t edit_pre_input(widget_t* widget, uint32_t key) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->pre_input != NULL) {
    return edit->pre_input(widget, key);
  }

  return RET_OK;
}

static ret_t edit_on_key_down(widget_t* widget, key_event_t* e) {
  uint32_t key = e->key;
  edit_t* edit = EDIT(widget);
#ifdef MACOS
  bool_t is_control = e->cmd;
#else
  bool_t is_control = e->ctrl;
#endif
  bool_t is_print = key < 128 && tk_isprint(key);
  keyboard_type_t keyboard_type = system_info()->keyboard_type;
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (!is_control) {
    if (edit_pre_input(widget, key) == RET_STOP) {
      return RET_STOP;
    }
  }

  if (key == TK_KEY_TAB || key == TK_KEY_ESCAPE || (key >= TK_KEY_F1 && key <= TK_KEY_F12)) {
    return RET_OK;
  } else if (key == TK_KEY_DOWN && keyboard_type != KEYBOARD_3KEYS &&
             keyboard_type != KEYBOARD_5KEYS) {
    if (widget_is_change_focus_key(widget, e)) {
      return RET_OK;
    }

    if (edit_is_number(widget) || edit->inc_value != NULL) {
      edit_dec(edit);
    } else {
      widget_focus_next(widget);
    }
    return RET_STOP;
  } else if (key == TK_KEY_UP && keyboard_type != KEYBOARD_3KEYS &&
             keyboard_type != KEYBOARD_5KEYS) {
    if (widget_is_change_focus_key(widget, e)) {
      return RET_OK;
    }

    if (edit_is_number(widget) || edit->inc_value != NULL) {
      edit_inc(edit);
    } else {
      widget_focus_prev(widget);
    }
    return RET_STOP;
  } else if (key == TK_KEY_LEFT || key == TK_KEY_RIGHT || key == TK_KEY_HOME || key == TK_KEY_END) {
    text_edit_key_down(edit->model, e);
    return RET_STOP;
  } else if (is_print || key == TK_KEY_BACKSPACE || key == TK_KEY_DELETE) {
    wstr_set(&(edit->last_changing_text), widget->text.str);
    text_edit_key_down(edit->model, e);
    edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);
    return RET_STOP;
  } else {
    if (widget->emitter != NULL) {
      ret_t ret = RET_OK;
      void* saved_target = e->e.target;

      e->e.target = widget;
      ret = emitter_dispatch(widget->emitter, (event_t*)e);
      e->e.target = saved_target;
      return ret;
    }
  }

  return RET_OK;
}

static ret_t edit_on_key_up(widget_t* widget, key_event_t* e) {
  int key = e->key;
  ret_t ret = RET_OK;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (key_code_is_enter(key)) {
    if (edit->timer_id == TK_INVALID_ID) {
      edit_on_focused(widget);
    } else {
      keyboard_type_t keyboard_type = system_info()->keyboard_type;
      if (edit->focus_next_when_enter && keyboard_type != KEYBOARD_3KEYS &&
          keyboard_type != KEYBOARD_5KEYS) {
        widget_focus_next(widget);
        widget_set_focused(widget, FALSE);
      }
    }
    ret = RET_STOP;
  } else {
    ret = text_edit_key_up(edit->model, e);
  }

  return ret;
}

static ret_t edit_select_all_async(const idle_info_t* info) {
  edit_t* edit = EDIT(info->ctx);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  text_edit_select_all(edit->model);

  if (edit->fix_value != NULL) {
    text_edit_set_cursor(edit->model, 0);
  }

  return RET_REMOVE;
}

static ret_t edit_check_valid_value(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);
  edit->is_text_error = FALSE;
  if (!edit_is_valid_value(widget)) {
    if (edit->auto_fix) {
      edit_auto_fix(widget);
      edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGED);
      if (widget->text.size > 0 && !edit_is_valid_value(widget)) {
        edit->is_text_error = TRUE;
        widget_set_state(widget, WIDGET_STATE_ERROR);
      }
    } else if (widget->text.size > 0) {
      edit->is_text_error = TRUE;
      widget_set_state(widget, WIDGET_STATE_ERROR);
    }
  }
  return RET_OK;
}

ret_t edit_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint32_t type = e->type;
  edit_t* edit = EDIT(widget);
  keyboard_type_t keyboard_type = system_info()->keyboard_type;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (!widget->visible) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget_find_target(widget, evt.x, evt.y) == NULL) {
        text_edit_click(edit->model, evt.x, evt.y);
        widget_grab(widget->parent, widget);
      }

      if (widget->target == NULL && !edit->readonly) {
        input_method_request(input_method(), widget);
      }
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      edit_start_update_caret(edit);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      edit_pointer_up_cleanup(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget->parent && widget->parent->grab_widget == widget) {
        text_edit_drag(edit->model, evt.x, evt.y);
        ret = RET_STOP;
      }

      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_UP: {
      widget_ungrab(widget->parent, widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_DOUBLE_CLICK: {
      uint32_t len = 0;
      int32_t left = 0;
      int32_t right = 0;
      uint32_t cursor = 0;
      wchar_t* text = NULL;
      pointer_event_t evt = *(pointer_event_t*)e;

      if (widget_find_target(widget, evt.x, evt.y) == NULL && widget->focused) {
        cursor = edit_get_cursor(widget);
        len = edit->model->widget->text.size;
        text = edit->model->widget->text.str;

        if (tk_wstr_select_word(text, len, cursor, &left, &right) == RET_OK) {
          edit_set_select(widget, left, right);
        }
      }
      break;
    }
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      int32_t key = evt->key;
#ifdef MACOS
      bool_t is_control = evt->cmd;
#else
      bool_t is_control = evt->ctrl;
#endif
      if ((!edit->is_activated || key == TK_KEY_RETURN) &&
          (keyboard_type == KEYBOARD_3KEYS || keyboard_type == KEYBOARD_5KEYS)) {
        break;
      }
      if (edit->readonly) {
        if (is_control && (key == TK_KEY_C || key == TK_KEY_c)) {
          text_edit_copy(edit->model);
        } else if (key == TK_KEY_DOWN || key == TK_KEY_UP) {
          log_debug("key down or key up\n");
        }
        break;
      }

      edit->is_key_inputing = TRUE;
      ret = edit_on_key_down(widget, evt);
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      edit_start_update_caret(edit);
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      if (!edit->is_activated &&
          (keyboard_type == KEYBOARD_3KEYS || keyboard_type == KEYBOARD_5KEYS)) {
        break;
      }
      edit->is_key_inputing = TRUE;
      ret = edit_on_key_up(widget, evt);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_COMMIT: {
      text_edit_state_t state;
      text_edit_get_state(edit->model, &state);
      im_commit_event_t* evt = (im_commit_event_t*)e;

      if (edit->readonly) {
        break;
      }
#ifndef AWTK_WEB
      if (!widget->focused) {
        break;
      }
#endif /*AWTK_WEB*/
      if (state.preedit) {
        text_edit_preedit_clear(edit->model);
      }

      if (evt->replace) {
        edit_clear(edit);
      }
      edit_commit_str(widget, evt->text);
      log_debug("EVT_IM_COMMIT:%s\n", evt->text);
      widget_invalidate(widget, NULL);

      break;
    }
    case EVT_IM_PREEDIT: {
      text_edit_preedit(edit->model);
      break;
    }
    case EVT_IM_PREEDIT_CONFIRM: {
      text_edit_preedit_confirm(edit->model);
      break;
    }
    case EVT_IM_PREEDIT_ABORT: {
      wstr_set(&(edit->last_changing_text), widget->text.str);
      text_edit_preedit_abort(edit->model);
      edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);
      break;
    }
    case EVT_UNACTIVATED: {
      edit->is_activated = FALSE;
      input_method_request(input_method(), NULL);
      if (edit->timer_id != TK_INVALID_ID) {
        timer_remove(edit->timer_id);
        edit->timer_id = TK_INVALID_ID;
        text_edit_set_caret_visible(edit->model, FALSE);
      }
      break;
    }
    case EVT_ACTIVATED: {
      edit->is_activated = TRUE;
      edit_do_request_input_method(widget);
      edit_save_text(widget);
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      edit_start_update_caret(edit);
      break;
    }
    case EVT_BLUR: {
      edit_on_blur(widget);
      break;
    }
    case EVT_FOCUS: {
      if (keyboard_type != KEYBOARD_3KEYS && keyboard_type != KEYBOARD_5KEYS &&
          edit->open_im_when_focused) {
        edit_on_focused(widget);
      }
      edit_save_text(widget);
      break;
    }
    case EVT_WHEEL: {
      if (edit->readonly) {
        break;
      }
      if (widget->focused) {
        wheel_event_t* evt = (wheel_event_t*)e;
        int32_t delta = evt->dy;
        if (delta > 0) {
          edit_dec(edit);
        } else if (delta < 0) {
          edit_inc(edit);
        }
        ret = RET_STOP;
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      edit_reset_layout(widget);
      break;
    }
    case EVT_VALUE_CHANGING: {
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_CLEAR: {
      edit_clear(edit);
      break;
    }
    case EVT_IM_CANCEL: {
      edit_rollback_text(widget);
      break;
    }
    case EVT_IM_ACTION: {
      if (tk_str_eq(edit->action_text, ACTION_TEXT_DONE)) {
        edit->is_key_inputing = FALSE;
        edit_check_valid_value(widget);
        input_method_request(input_method(), NULL);
        text_edit_preedit_confirm(edit->model);
        text_edit_unselect(edit->model);
        edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGED);
        edit_commit_text(widget);
      } else if (tk_str_eq(edit->action_text, ACTION_TEXT_NEXT)) {
        widget_focus_next(widget);
      }
      log_debug("action button:%s\n", edit->action_text);
      break;
    }
    case EVT_POINTER_LEAVE: {
      edit_update_status(widget);
      break;
    }
    case EVT_POINTER_ENTER: {
      if (widget->text.size == 0) {
        widget_set_state(widget, WIDGET_STATE_EMPTY_OVER);
      } else {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      break;
    }
    case EVT_CONTEXT_MENU: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};
      widget_to_local(widget, &p);
      widget_to_screen(widget, &p);
      text_edit_show_context_menu(edit->model, p.x, p.y);
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t edit_on_re_translate(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->tr_tips != NULL) {
    const char* tr_tips = locale_info_tr(widget_get_locale_info(widget), edit->tr_tips);
    edit_set_tips(widget, tr_tips);
  }

  return RET_OK;
}

ret_t edit_set_text_limit(widget_t* widget, uint32_t min, uint32_t max) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->min = min;
  edit->max = max;
  edit->input_type = INPUT_TEXT;
  edit_check_valid_value(widget);
  edit_update_status(widget);

  return RET_OK;
}

ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max, uint32_t step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (edit->input_type != INPUT_UINT) {
    edit->min = min;
    edit->max = max;
    edit->input_type = INPUT_INT;
  } else {
    edit->min = (uint32_t)min;
    edit->max = (uint32_t)max;
  }
  edit->step = step;
  edit_check_valid_value(widget);
  edit_update_status(widget);
  return RET_OK;
}

ret_t edit_set_float_limit(widget_t* widget, double min, double max, double step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->min = min;
  edit->max = max;
  edit->step = step;

  if (edit->input_type != INPUT_UFLOAT) {
    edit->input_type = INPUT_FLOAT;
  }
  edit_check_valid_value(widget);
  edit_update_status(widget);
  return RET_OK;
}

ret_t edit_set_readonly(widget_t* widget, bool_t readonly) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->readonly = readonly;

  return RET_OK;
}

ret_t edit_set_cancelable(widget_t* widget, bool_t cancelable) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->cancelable = cancelable;

  return RET_OK;
}

ret_t edit_set_auto_fix(widget_t* widget, bool_t auto_fix) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->auto_fix = auto_fix;

  return RET_OK;
}

ret_t edit_set_select_none_when_focused(widget_t* widget, bool_t select_none_when_focused) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->select_none_when_focused = select_none_when_focused;

  return RET_OK;
}

ret_t edit_set_open_im_when_focused(widget_t* widget, bool_t open_im_when_focused) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->open_im_when_focused = open_im_when_focused;

  return RET_OK;
}

ret_t edit_set_close_im_when_blured(widget_t* widget, bool_t close_im_when_blured) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->close_im_when_blured = close_im_when_blured;

  return RET_OK;
}

ret_t edit_set_input_type(widget_t* widget, input_type_t type) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->input_type = type;
  edit->pre_input = NULL;
  edit->pre_delete = NULL;
  edit->is_valid_char = NULL;
  edit->fix_value = NULL;
  edit->inc_value = NULL;
  edit->dec_value = NULL;
  edit->is_valid_value = NULL;

  if (edit->step == 0.0 && (type == INPUT_INT || type == INPUT_UINT)) {
    edit->step = 1;
  } else if (edit->step == 0.0 && (type == INPUT_FLOAT || type == INPUT_UFLOAT)) {
    edit->step = 1.0f;
  } else if (type == INPUT_PASSWORD) {
    edit_set_password_visible(widget, edit->password_visible);
  } else if (type == INPUT_IPV4) {
    edit->fix_value = edit_ipv4_fix;
    edit->inc_value = edit_ipv4_inc_value;
    edit->dec_value = edit_ipv4_dec_value;
    edit->pre_input = edit_ipv4_pre_input;
    edit->is_valid_value = edit_ipv4_is_valid;
    edit->is_valid_char = edit_ipv4_is_valid_char;
    edit->pre_delete = edit_ipv4_pre_delete;
  } else if (type == INPUT_DATE) {
    edit->fix_value = edit_date_fix;
    edit->inc_value = edit_date_inc_value;
    edit->dec_value = edit_date_dec_value;
    edit->pre_input = edit_date_pre_input;
    edit->is_valid_value = edit_date_is_valid;
    edit->is_valid_char = edit_date_is_valid_char;
    edit->pre_delete = edit_date_pre_delete;
  } else if (type == INPUT_TIME) {
    edit->fix_value = edit_time_fix;
    edit->inc_value = edit_time_inc_value;
    edit->dec_value = edit_time_dec_value;
    edit->pre_input = edit_time_pre_input;
    edit->is_valid_value = edit_time_is_valid;
    edit->is_valid_char = edit_time_is_valid_char;
    edit->pre_delete = edit_time_pre_delete;
  } else if (type == INPUT_TIME_FULL) {
    edit->fix_value = edit_time_full_fix;
    edit->inc_value = edit_time_full_inc_value;
    edit->dec_value = edit_time_full_dec_value;
    edit->pre_input = edit_time_full_pre_input;
    edit->is_valid_value = edit_time_full_is_valid;
    edit->is_valid_char = edit_time_full_is_valid_char;
    edit->pre_delete = edit_time_full_pre_delete;
  }

  edit_check_valid_value(widget);
  return RET_OK;
}

ret_t edit_set_tips(widget_t* widget, const char* tips) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->tips = tk_str_copy(edit->tips, tips);
  text_edit_set_tips(edit->model, edit->tips, FALSE);

  return RET_OK;
}

ret_t edit_set_action_text(widget_t* widget, const char* action_text) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->action_text = tk_str_copy(edit->action_text, action_text);

  return RET_OK;
}

static ret_t edit_apply_tr_text_before_paint(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->tr_tips != NULL) {
    const char* tr_tips = locale_info_tr(widget_get_locale_info(widget), edit->tr_tips);
    edit_set_tips(widget, tr_tips);
  }

  return RET_REMOVE;
}

static ret_t edit_reset_layout(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  text_edit_layout(edit->model);
  widget_invalidate(widget, NULL);
  return RET_OK;
}

ret_t edit_set_tr_tips(widget_t* widget, const char* tr_tips) {
  edit_t* edit = EDIT(widget);
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  if (TK_STR_IS_EMPTY(tr_tips)) {
    TKMEM_FREE(edit->tr_tips);
    return RET_OK;
  }

  edit->tr_tips = tk_str_copy(edit->tr_tips, tr_tips);
  if (win != NULL) {
    tr_tips = locale_info_tr(widget_get_locale_info(widget), tr_tips);
    edit_set_tips(widget, tr_tips);
  } else {
    widget_on(widget, EVT_BEFORE_PAINT, edit_apply_tr_text_before_paint, widget);
  }

  return RET_OK;
}

ret_t edit_set_keyboard(widget_t* widget, const char* keyboard) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->keyboard = tk_str_copy(edit->keyboard, keyboard);

  return RET_OK;
}

static bool_t widget_has_uint_min_max(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL, FALSE);

  input_type = edit->input_type;

  return input_type == INPUT_TEXT || input_type == INPUT_ASCII || input_type == INPUT_PASSWORD ||
         input_type == INPUT_EMAIL || input_type == INPUT_UINT;
}

ret_t edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->input_type;
  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_INT) {
      value_set_int(v, edit->min);
    } else if (widget_has_uint_min_max(widget)) {
      value_set_uint32(v, edit->min);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_double(v, edit->min);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_INT) {
      value_set_int(v, edit->max);
    } else if (widget_has_uint_min_max(widget)) {
      value_set_uint32(v, edit->max);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_double(v, edit->max);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_float(v, edit->step);
      return RET_OK;
    } else if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_double(v, edit->step);
      return RET_OK;
    } else {
      return RET_NOT_FOUND;
    }
  } else if (tk_str_eq(name, WIDGET_PROP_INPUT_TYPE)) {
    value_set_uint32(v, input_type);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_READONLY)) {
    value_set_bool(v, edit->readonly);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CANCELABLE)) {
    value_set_bool(v, edit->cancelable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_FIX)) {
    value_set_bool(v, edit->auto_fix);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED)) {
    value_set_bool(v, edit->select_none_when_focused);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED)) {
    value_set_bool(v, edit->open_im_when_focused);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_IM_WHEN_BLURED)) {
    value_set_bool(v, edit->close_im_when_blured);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    uint32_t margin = 0;
    if (widget->astyle != NULL) {
      TEXT_EDIT_GET_STYLE_MARGIN(widget->astyle, margin, LEFT);
    }
    if (margin == 0) {
      margin = edit->left_margin != 0 ? edit->left_margin : edit->margin;
    }
    value_set_int(v, margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    uint32_t margin = 0;
    if (widget->astyle != NULL) {
      TEXT_EDIT_GET_STYLE_MARGIN(widget->astyle, margin, RIGHT);
    }
    if (margin == 0) {
      margin = edit->right_margin != 0 ? edit->right_margin : edit->margin;
    }
    value_set_int(v, margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    uint32_t margin = 0;
    if (widget->astyle != NULL) {
      TEXT_EDIT_GET_STYLE_MARGIN(widget->astyle, margin, TOP);
    }
    if (margin == 0) {
      margin = edit->top_margin != 0 ? edit->top_margin : edit->margin;
    }
    value_set_int(v, margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    uint32_t margin = 0;
    if (widget->astyle != NULL) {
      TEXT_EDIT_GET_STYLE_MARGIN(widget->astyle, margin, BOTTOM);
    }
    if (margin == 0) {
      margin = edit->bottom_margin != 0 ? edit->bottom_margin : edit->margin;
    }
    value_set_int(v, margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_PASSWORD_VISIBLE)) {
    value_set_bool(v, edit->password_visible);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ACTION_TEXT)) {
    value_set_str(v, edit->action_text);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    value_set_str(v, edit->tips);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TIPS)) {
    value_set_str(v, edit->tr_tips);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_KEYBOARD)) {
    value_set_str(v, edit->keyboard);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    value_set_bool(v, !(edit->readonly));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    switch (edit->input_type) {
      case INPUT_INT: {
        int32_t n = edit_get_int(widget);
        value_set_int32(v, n);
        break;
      }
      case INPUT_UINT: {
        uint32_t n = (uint32_t)edit_get_int(widget);
        value_set_uint32(v, n);
        break;
      }
      case INPUT_FLOAT:
      case INPUT_UFLOAT: {
        double d = edit_get_double(widget);
        value_set_double(v, d);
        break;
      }
      default: {
        value_set_wstr(v, widget->text.str);
      }
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CARET_X)) {
    text_edit_state_t state;
    text_edit_get_state(edit->model, &state);
    value_set_int(v, state.caret.x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CARET_Y)) {
    text_edit_state_t state;
    text_edit_get_state(edit->model, &state);
    value_set_int(v, state.caret.y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LINE_HEIGHT)) {
    text_edit_state_t state;
    text_edit_get_state(edit->model, &state);
    value_set_int(v, state.line_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INPUTING)) {
    input_method_t* im = input_method();
    bool_t inputing = (im != NULL && im->widget == widget) || edit->is_key_inputing;
    /* 当控件没有父集窗口或者父集窗口没有打开的时候，通过 focused 来判断是否正在输入 */
    if (!inputing && !widget_is_window_opened(widget)) {
      inputing = widget->focused;
    }
    value_set_bool(v, inputing);

    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALIDATOR)) {
    value_set_str(v, edit->validator);

    return RET_OK;
  } else if (tk_str_eq(name, EDIT_PROP_FOCUS_NEXT_WHEN_ENTER)) {
    value_set_bool(v, edit->focus_next_when_enter);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t edit_set_text(widget_t* widget, const value_t* v) {
  wstr_t str;
  wstr_init(&str, 0);
  edit_t* edit = EDIT(widget);
  ENSURE(edit);
  return_value_if_fail(wstr_from_value(&str, v) == RET_OK, RET_BAD_PARAMS);

  if (!wstr_equal(&(widget->text), &str)) {
    uint32_t len = str.size;
    input_type_t input_type = edit->input_type;
    if (input_type == INPUT_INT || input_type == INPUT_UINT || input_type == INPUT_FLOAT ||
        input_type == INPUT_UFLOAT) {
      len = tk_min(str.size, 64);
    } else {
      len = edit->max > 0 ? tk_min(str.size, edit->max) : str.size;
    }
    wstr_set_with_len(&(widget->text), str.str, len);

    text_edit_set_cursor(edit->model, 0);
    text_edit_layout(edit->model);
    edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGED);
    edit->is_text_error = FALSE;
    edit_update_status(widget);
    edit_check_valid_value(widget);
  }

  wstr_reset(&str);

  return widget_invalidate_force(widget, NULL);
}

ret_t edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->input_type;
  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->min = value_double(v);
    } else {
      edit->min = value_int(v);
    }
    edit_check_valid_value(widget);
    edit_update_status(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->max = value_double(v);
    } else {
      edit->max = value_int64(v);
    }
    edit_check_valid_value(widget);
    edit_update_status(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    edit->step = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INPUT_TYPE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = input_type_find(value_str(v));
      if (kv != NULL) {
        input_type = (input_type_t)(kv->value);
      }
    } else {
      input_type = (input_type_t)value_int(v);
    }
    edit_set_input_type(widget, input_type);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_READONLY)) {
    edit->readonly = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CANCELABLE)) {
    edit->cancelable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_FIX)) {
    edit->auto_fix = value_bool(v);
    if (edit->auto_fix) {
      edit_check_valid_value(widget);
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED)) {
    edit->select_none_when_focused = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED)) {
    edit->open_im_when_focused = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_IM_WHEN_BLURED)) {
    edit->close_im_when_blured = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    edit->margin = value_int(v);
    edit_reset_layout(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    edit->left_margin = value_int(v);
    edit_reset_layout(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    edit->right_margin = value_int(v);
    edit_reset_layout(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    edit->top_margin = value_int(v);
    edit_reset_layout(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    edit->bottom_margin = value_int(v);
    edit_reset_layout(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_PASSWORD_VISIBLE)) {
    edit_set_password_visible(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUS) || tk_str_eq(name, WIDGET_PROP_FOCUSED)) {
    edit_set_focus(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ACTION_TEXT)) {
    edit_set_action_text(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    edit_set_tips(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TIPS)) {
    edit_set_tr_tips(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_KEYBOARD)) {
    edit_set_keyboard(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_TEXT)) {
    edit_set_text(widget, v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALIDATOR)) {
    edit_set_validator(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, EDIT_PROP_FOCUS_NEXT_WHEN_ENTER)) {
    return edit_set_focus_next_when_enter(widget, value_bool(v));
  }

  edit_update_status(widget);

  return RET_NOT_FOUND;
}

ret_t edit_set_password_visible(widget_t* widget, bool_t password_visible) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->password_visible = password_visible;
  text_edit_set_mask(edit->model, !password_visible);
  text_edit_set_mask_char(edit->model, PASSWORD_MASK_CHAR);
  text_edit_set_cursor(edit->model, 0xffffffff);
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t edit_set_focus(widget_t* widget, bool_t focus) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  widget_set_focused(widget, focus);
  edit_update_status(widget);

  return RET_OK;
}

ret_t edit_set_cursor(widget_t* widget, uint32_t cursor) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  return text_edit_set_cursor(edit->model, cursor);
}

uint32_t edit_get_cursor(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && edit->model != NULL, 0);

  return text_edit_get_cursor(edit->model);
}

static ret_t edit_add_float(edit_t* edit, double delta) {
  double v = 0;
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  return_value_if_fail(wstr_to_float(text, &v) == RET_OK, RET_FAIL);

  v += delta;
  if (edit->min < edit->max) {
    if (v < edit->min) {
      wstr_from_float(text, edit->min);
    } else if (v > edit->max) {
      wstr_from_float(text, edit->max);
    } else {
      wstr_add_float(text, delta);
    }
  } else {
    wstr_add_float(text, delta);
  }

  wstr_trim_float_zero(text);

  return RET_OK;
}

static ret_t edit_add_int(edit_t* edit, int delta) {
  int32_t v = 0;
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  return_value_if_fail(wstr_to_int(text, &v) == RET_OK, RET_FAIL);

  v += delta;
  if (edit->auto_fix && (edit->min < edit->max)) {
    if (v < edit->min) {
      v = (int32_t)(edit->min);
    }

    if (v > edit->max) {
      v = (int32_t)(edit->max);
    }
  }

  wstr_from_int(text, v);

  return RET_OK;
}

static ret_t edit_add_hex(edit_t* edit, int delta) {
  int32_t v = 0;
  uint32_t size = 0;
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  char buff[TK_NUM_MAX_LEN + 1] = {0};
  wchar_t wbuff[TK_NUM_MAX_LEN] = {0};
  char hex_buff[TK_NUM_MAX_LEN + 1] = {0};
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  if (text->size > 0) {
    size = tk_min(text->size, TK_NUM_MAX_LEN) * sizeof(wchar_t);
    tk_memcpy(wbuff, text->str, size);
    tk_utf8_from_utf16_ex(wbuff, ARRAY_SIZE(wbuff), buff, ARRAY_SIZE(buff));

    tk_sscanf(buff, "%x", &v);

  } else {
    v = 0;
  }

  v += delta;
  if (edit->auto_fix && (edit->min < edit->max)) {
    if (v < edit->min) {
      v = (int32_t)(edit->min);
    }

    if (v > edit->max) {
      v = (int32_t)(edit->max);
    }
  }

  tk_snprintf(hex_buff, ARRAY_SIZE(hex_buff), "%x", v);
  tk_str_toupper(hex_buff);

  wstr_set_utf8(text, hex_buff);

  return RET_OK;
}

int32_t edit_get_int(widget_t* widget) {
  int32_t v = 0;
  return_value_if_fail(widget != NULL, 0);

  wstr_to_int(&(widget->text), &v);

  return v;
}

double edit_get_double(widget_t* widget) {
  double v = 0;
  return_value_if_fail(widget != NULL, 0);

  wstr_to_float(&(widget->text), &v);

  return v;
}

ret_t edit_set_int(widget_t* widget, int32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  value_set_int32(&v, value);
  edit_set_text(widget, &v);

  return RET_OK;
}

ret_t edit_set_double(widget_t* widget, double value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  value_set_double(&v, value);
  return edit_set_text(widget, &v);
}

ret_t edit_set_double_ex(widget_t* widget, const char* format, double value) {
  value_t v;
  char text[64];
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (format == NULL) {
    format = "%2.2lf";
  }

  tk_snprintf(text, sizeof(text) - 1, format, value);
  value_set_str(&v, text);

  return edit_set_text(widget, &v);
}

static ret_t edit_inc_default(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->input_type;

  wstr_set(&(edit->last_changing_text), text->str);

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->step != 0.0 ? edit->step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->min);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->step != 0.0 ? edit->step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->min);
      }
      edit_add_int(edit, step);
      break;
    }
    case INPUT_HEX: {
      int32_t step = edit->step != 0.0 ? edit->step : 1;
      if (text->size == 0) {
        if (edit->min < 0) {
          wstr_from_int(text, 0);
        } else {
          wstr_from_int(text, edit->min);
        }
      }
      edit_add_hex(edit, step);
      break;
    }
    default:
      break;
  }
  if (!edit->readonly) {
    text_edit_select_all(edit->model);
  }
  text_edit_set_cursor(edit->model, text->size);
  edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);

  return widget_invalidate_force(widget, NULL);
}

static ret_t edit_dec_default(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->input_type;

  wstr_set(&(edit->last_changing_text), text->str);

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->step != 0.0 ? edit->step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->max);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, -step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->step != 0.0 ? edit->step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->max);
      }
      edit_add_int(edit, -step);
      break;
    }
    case INPUT_HEX: {
      int32_t step = edit->step != 0.0 ? edit->step : 1;
      if (text->size == 0) {
        if (edit->max < 0) {
          wstr_from_int(text, 0);
        } else {
          wstr_from_int(text, edit->max);
        }
      }
      edit_add_hex(edit, -step);
      break;
    }
    default:
      break;
  }
  if (!edit->readonly) {
    text_edit_select_all(edit->model);
  }
  text_edit_set_cursor(edit->model, text->size);
  edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);

  return widget_invalidate_force(widget, NULL);
}

ret_t edit_clear(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  wstr_set(&(widget->text), L"");
  text_edit_set_cursor(edit->model, 0xffffffff);
  edit_update_status(widget);

  if (edit->fix_value != NULL) {
    edit->fix_value(widget);
  }
  return widget_invalidate_force(widget, NULL);
}

static ret_t edit_on_inc(void* ctx, event_t* e) {
  (void)e;
  return edit_inc(EDIT(ctx));
}

static ret_t edit_on_dec(void* ctx, event_t* e) {
  (void)e;
  return edit_dec(EDIT(ctx));
}

static ret_t edit_on_clear(void* ctx, event_t* e) {
  (void)e;
  return edit_clear(EDIT(ctx));
}

static ret_t edit_on_password_visible(void* ctx, event_t* e) {
  edit_t* edit = EDIT(ctx);
  bool_t password_visible = FALSE;
  widget_t* widget = WIDGET(e->target);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  password_visible = widget_get_prop_bool(widget, WIDGET_PROP_VALUE, edit->password_visible);
  edit_set_password_visible(WIDGET(edit), password_visible);

  return RET_OK;
}

static ret_t edit_hook_button(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* edit = WIDGET(ctx);
  return_value_if_fail(widget != NULL && edit != NULL, RET_REMOVE);

  if (widget->name && widget != edit) {
    uint32_t etype = EVT_CLICK;
    event_func_t handler = NULL;
    const char* name = widget->name;
    emitter_t* emitter = widget->emitter;
    const char* type = widget_get_type(widget);

    if (tk_str_eq(type, WIDGET_TYPE_BUTTON)) {
      if (tk_str_eq(name, STR_EDIT_INC_NAME)) {
        handler = edit_on_inc;
      } else if (tk_str_eq(name, STR_EDIT_DEC_NAME)) {
        handler = edit_on_dec;
      } else if (tk_str_eq(name, STR_EDIT_CLEAR_NAME)) {
        handler = edit_on_clear;
      }
    } else if (tk_str_eq(type, WIDGET_TYPE_CHECK_BUTTON)) {
      if (tk_str_eq(name, STR_EDIT_VISIBLE_NAME)) {
        etype = EVT_VALUE_CHANGED;
        handler = edit_on_password_visible;
      }
    }

    if (handler != NULL && (emitter == NULL || !emitter_exist(emitter, etype, handler, edit))) {
      widget_on(widget, etype, handler, edit);
    }
  }

  return RET_OK;
}

ret_t edit_on_destroy(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit->timer_id != TK_INVALID_ID) {
    timer_remove(edit->timer_id);
    edit->timer_id = TK_INVALID_ID;
  }

  if (edit->idle_id != TK_INVALID_ID) {
    idle_remove(edit->idle_id);
    edit->idle_id = TK_INVALID_ID;
  }

  TKMEM_FREE(edit->tips);
  TKMEM_FREE(edit->tr_tips);
  TKMEM_FREE(edit->keyboard);
  TKMEM_FREE(edit->validator);
  TKMEM_FREE(edit->action_text);
  wstr_reset(&(edit->saved_text));
  wstr_reset(&(edit->last_changing_text));
  wstr_reset(&(edit->last_changed_text));
  text_edit_destroy(edit->model);

  return RET_OK;
}

static ret_t edit_hook_children_button_and_check_valid_value(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_REMOVE);

  widget_foreach(widget, edit_hook_button, widget);
  edit->idle_id = TK_INVALID_ID;

  edit_check_valid_value(widget);

  return RET_REMOVE;
}

ret_t edit_on_add_child(widget_t* widget, widget_t* child) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_FAIL);

  if (edit->idle_id == TK_INVALID_ID) {
    edit->idle_id = widget_add_idle(widget, edit_hook_children_button_and_check_valid_value);
  }

  return RET_CONTINUE;
}

const char* const s_edit_properties[] = {TK_EDIT_PROPS, NULL};

ret_t edit_on_copy(widget_t* widget, widget_t* other) {
  edit_t* edit = EDIT(widget);
  edit_t* edit_other = EDIT(other);
  return_value_if_fail(edit != NULL && edit_other != NULL, RET_BAD_PARAMS);

  widget_on_copy_recursive(widget, other);

  edit->min = edit_other->min;
  edit->max = edit_other->max;
  edit->step = edit_other->step;
  edit->readonly = edit_other->readonly;
  edit->auto_fix = edit_other->auto_fix;
  edit->left_margin = edit_other->left_margin;
  edit->right_margin = edit_other->right_margin;
  edit->top_margin = edit_other->top_margin;
  edit->bottom_margin = edit_other->bottom_margin;
  edit->password_visible = edit_other->password_visible;

  edit_set_input_type(widget, edit_other->input_type);

  if (edit_other->tips != NULL) {
    edit_set_tips(widget, edit_other->tips);
  }
  if (edit_other->tr_tips != NULL) {
    edit_set_tr_tips(widget, edit_other->tr_tips);
  }

  return RET_OK;
}

static ret_t edit_init(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->margin = 2;
  edit->top_margin = 0;
  edit->left_margin = 0;
  edit->right_margin = 0;
  edit->bottom_margin = 0;
  edit->close_im_when_blured = TRUE;
  edit->open_im_when_focused = TRUE;
  edit->focus_next_when_enter = FALSE;
  edit_set_text_limit(widget, 0, 1024);

  edit_update_status(widget);
  edit->timer_id = TK_INVALID_ID;
  edit->selected_idle_id = TK_INVALID_ID;

  edit->model = text_edit_create(widget, TRUE);
  ENSURE(edit->model != NULL);
  text_edit_set_on_text_will_delete(edit->model, edit_on_text_edit_text_will_delete, widget);
  text_edit_set_on_char_will_input(edit->model, edit_on_text_edit_char_will_input, widget);

  wstr_init(&(edit->last_changing_text), 0);
  wstr_init(&(edit->last_changed_text), 0);
  widget_set_text(widget, L"");
  wstr_init(&(edit->saved_text), 0);

  edit->model->ignore_layout = TRUE;
  edit_set_password_visible(widget, FALSE);
  edit->model->ignore_layout = FALSE;

  edit_set_action_text(widget, ACTION_TEXT_DONE);

  return RET_OK;
}

TK_DECL_VTABLE(edit) = {.size = sizeof(edit_t),
                        .type = WIDGET_TYPE_EDIT,
                        .focusable = TRUE,
                        .inputable = TRUE,
                        .pointer_cursor = WIDGET_CURSOR_EDIT,
                        .clone_properties = s_edit_properties,
                        .persistent_properties = s_edit_properties,
                        .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                        .create = edit_create,
                        .init = edit_init,
                        .on_re_translate = edit_on_re_translate,
                        .on_paint_self = edit_on_paint_self,
                        .set_prop = edit_set_prop,
                        .get_prop = edit_get_prop,
                        .on_destroy = edit_on_destroy,
                        .on_copy = edit_on_copy,
                        .on_event = edit_on_event,
                        .on_add_child = edit_on_add_child};

widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(edit), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  edit_init(widget);
  return widget;
}

widget_t* edit_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, edit), NULL);

  return widget;
}

ret_t edit_set_is_valid_char(widget_t* widget, edit_is_valid_char_t is_valid_char) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->is_valid_char = is_valid_char;

  return RET_OK;
}

ret_t edit_set_is_valid_value(widget_t* widget, edit_is_valid_value_t is_valid_value) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->is_valid_value = is_valid_value;

  return RET_OK;
}

ret_t edit_set_fix_value(widget_t* widget, edit_fix_value_t fix_value) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->fix_value = fix_value;

  return RET_OK;
}

ret_t edit_set_inc_value(widget_t* widget, edit_inc_value_t inc_value) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->inc_value = inc_value;

  return RET_OK;
}

ret_t edit_set_dec_value(widget_t* widget, edit_dec_value_t dec_value) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->dec_value = dec_value;

  return RET_OK;
}

ret_t edit_set_pre_input(widget_t* widget, edit_pre_input_t pre_input) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->pre_input = pre_input;

  return RET_OK;
}

ret_t edit_set_pre_delete(widget_t* widget, edit_pre_delete_t pre_delete) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->pre_delete = pre_delete;

  return RET_OK;
}

ret_t edit_set_select(widget_t* widget, uint32_t start, uint32_t end) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && edit->model != NULL, RET_BAD_PARAMS);

  return text_edit_set_select(edit->model, start, end);
}

char* edit_get_selected_text(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && edit->model != NULL, NULL);

  return text_edit_get_selected_text(edit->model);
}

ret_t edit_pre_input_with_sep(widget_t* widget, uint32_t key, char sep) {
  text_edit_state_t state;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  if (key == TK_KEY_LEFT || key == TK_KEY_RIGHT) {
    wstr_t* text = &(widget->text);
    text_edit_get_state(edit->model, &state);
    if (state.select_start == state.select_end && text->size > 1) {
      int32_t i = 0;
      if (key == TK_KEY_LEFT) {
        if (state.cursor > 1 && text->str[state.cursor - 1] == sep) {
          /*select prev part*/
          for (i = state.cursor - 2; i >= 0; i--) {
            if (text->str[i] == sep) {
              break;
            }
          }
          text_edit_set_cursor(edit->model, state.cursor - 1);
          text_edit_set_select(edit->model, i + 1, state.cursor - 1);
          return RET_STOP;
        }
      } else if (key == TK_KEY_RIGHT) {
        if (text->str[state.cursor] == sep) {
          /*select next part*/
          for (i = state.cursor + 1; i < text->size; i++) {
            if (text->str[i] == sep) {
              break;
            }
          }
          text_edit_set_cursor(edit->model, state.cursor + 1);
          text_edit_set_select(edit->model, state.cursor + 1, i);
          return RET_STOP;
        }
      }
    }
  }

  return RET_OK;
}

#define EDIT_DEFAULT_CHAR '0'

ret_t edit_pre_delete_with_sep(widget_t* widget, delete_type_t delete_type, char sep) {
  text_edit_state_t state;
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  text_edit_get_state(edit->model, &state);
  if (state.select_start < state.select_end) {
    uint32_t i = 0;
    wchar_t* s = text->str + state.select_start;
    wchar_t* d = text->str + state.select_start;
    for (i = state.select_start; i < state.select_end; i++, s++) {
      if (*s == sep) {
        *d++ = sep;
      } else if (d[-1] == sep) {
        /*避免连续分隔符*/
        *d++ = EDIT_DEFAULT_CHAR;
      }
    }

    for (; i < text->size; i++) {
      *d++ = *s++;
    }
    text->size = d - text->str;
    *d = 0;

    text_edit_unselect(edit->model);
    text_edit_set_cursor(edit->model, state.select_start);
    if (delete_type == DELETE_BY_KEY_BACKSPACE || delete_type == DELETE_BY_KEY_DELETE) {
      return RET_STOP;
    }
  } else {
    if (delete_type == DELETE_BY_KEY_BACKSPACE) {
      if (state.cursor > 0 && text->str[state.cursor - 1] == sep) {
        /*不允许删除分隔符*/
        text_edit_set_cursor(edit->model, state.cursor - 1);
        return RET_STOP;
      } else if (state.cursor > 1 && text->str[state.cursor - 2] == sep) {
        /*分隔符之间保留一个字符*/
        text->str[state.cursor - 1] = EDIT_DEFAULT_CHAR;
        text_edit_set_cursor(edit->model, state.cursor - 1);
        return RET_STOP;
      } else if (state.cursor == 1) {
        /*不允许删除首字符*/
        text->str[0] = EDIT_DEFAULT_CHAR;
        text_edit_set_cursor(edit->model, state.cursor - 1);
        return RET_STOP;
      }
    } else if (delete_type == DELETE_BY_KEY_DELETE) {
      if (text->str[state.cursor] == sep) {
        text_edit_set_cursor(edit->model, state.cursor + 1);
        return RET_STOP;
      }
    }
  }
  return RET_OK;
}

ret_t edit_add_value_with_sep(widget_t* widget, int delta, char sep) {
  char c = 0;
  uint32_t cursor = 0;
  wstr_t* text = NULL;
  text_edit_state_t state;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  text_edit_get_state(edit->model, &state);

  if (text->size == 0) {
    edit->fix_value(widget);
    return RET_OK;
  }

  wstr_set(&(edit->last_changing_text), text->str);

  cursor = state.cursor < text->size ? state.cursor : text->size - 1;
  if (text->str[cursor] == sep && cursor > 0) {
    cursor--;
  }

  c = text->str[cursor];
  if (c >= '0' && c <= '9') {
    c += delta;

    if (c < '0') {
      c = '9';
    }

    if (c > '9') {
      c = '0';
    }

    text->str[cursor] = c;
  }

  text_edit_set_select(edit->model, cursor, cursor + 1);
  edit_dispatch_value_change_event(widget, EVT_VALUE_CHANGING);

  return widget_invalidate_force(widget, NULL);
}

ret_t edit_inc(edit_t* edit) {
  if (edit->inc_value != NULL) {
    return edit->inc_value(WIDGET(edit));
  } else {
    return edit_inc_default(edit);
  }
}

ret_t edit_dec(edit_t* edit) {
  if (edit->dec_value != NULL) {
    return edit->dec_value(WIDGET(edit));
  } else {
    return edit_dec_default(edit);
  }
}

static ret_t edit_auto_fix(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->fix_value != NULL) {
    return edit->fix_value(widget);
  } else {
    return edit_auto_fix_default(widget);
  }
}

#include "tkc/fscript.h"
#include "base/object_widget.h"

ret_t edit_set_validator(widget_t* widget, const char* validator) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->validator = tk_str_copy(edit->validator, validator);

  return RET_OK;
}

bool_t edit_is_valid_value(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  if (edit->is_valid_value != NULL) {
    return edit->is_valid_value(widget);
  } else {
#ifndef WITHOUT_FSCRIPT
    if (edit->validator != NULL) {
      value_t v;
      tk_object_t* obj = object_widget_create(widget);
      tk_object_set_prop_pointer(obj, STR_PROP_SELF, widget);
      if (fscript_eval(obj, edit->validator, &v) == RET_OK) {
        TK_OBJECT_UNREF(obj);
        return value_bool(&v);
      }
      TK_OBJECT_UNREF(obj);
    }
#endif /*WITHOUT_FSCRIPT*/

    return edit_is_valid_value_default(widget);
  }
}

ret_t edit_set_focus_next_when_enter(widget_t* widget, bool_t focus_next_when_enter) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);
  edit->focus_next_when_enter = focus_next_when_enter;
  return RET_OK;
}
