/**
 * File:   edit.h
 * Author: AWTK Develop Team
 * Brief:  edit
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
#include "base/window_manager.h"
#include "widgets/edit_ipv4.h"
#include "widgets/edit_date.h"
#include "widgets/edit_time.h"
#include "widgets/edit_time_full.h"

#define ACTION_TEXT_NEXT "next"
#define ACTION_TEXT_DONE "done"

#define PASSWORD_MASK_CHAR '*'

static ret_t edit_auto_fix(widget_t* widget);
static ret_t edit_update_status(widget_t* widget);
static ret_t edit_select_all_async(const idle_info_t* info);

static ret_t edit_dispatch_event(widget_t* widget, event_type_t type) {
  event_t evt = event_init(type, widget);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t edit_update_caret(const timer_info_t* timer) {
  edit_t* edit = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(timer != NULL, RET_REMOVE);

  edit = EDIT(timer->ctx);
  widget = WIDGET(timer->ctx);
  return_value_if_fail(edit != NULL && widget != NULL, RET_REMOVE);

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

ret_t edit_on_paint_self(widget_t* widget, canvas_t* c) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->model->c = c;
  if (edit->readonly) {
    if (tk_str_eq(widget->vt->type, WIDGET_TYPE_COMBO_BOX))
      text_edit_set_cursor(edit->model, 0);
    else
      text_edit_set_cursor(edit->model, 0xffffffff);
  }

  if (edit->input_type != INPUT_PASSWORD && edit->input_type != INPUT_CUSTOM_PASSWORD) {
    text_edit_set_mask(edit->model, FALSE);
  }

  return text_edit_paint(edit->model, c);
}

static ret_t edit_do_input_char(widget_t* widget, wchar_t c) {
  return text_edit_paste(EDIT(widget)->model, &c, 1);
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
        if (cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
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
        if (cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
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
        if (cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
          ret = TRUE;
        }
      }
      break;
    }
    default: {
      if (text->size < edit->max) {
        ret = TRUE;
      }
    }
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
  ret_t ret = RET_BAD_PARAMS;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (edit_is_valid_char(widget, c)) {
    ret = edit_do_input_char(widget, c);
    edit_dispatch_event(widget, EVT_VALUE_CHANGING);
  }

  return ret;
}

static ret_t edit_commit_str(widget_t* widget, const char* str) {
  uint32_t i = 0;
  wchar_t wstr[32];
  tk_utf8_to_utf16(str, wstr, ARRAY_SIZE(wstr));

  while (wstr[i]) {
    edit_input_char(widget, wstr[i]);
    i++;
  }

  return RET_OK;
}

static bool_t edit_is_number(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  input_type = edit->input_type;

  return input_type == INPUT_UINT || input_type == INPUT_INT || input_type == INPUT_FLOAT ||
         input_type == INPUT_UFLOAT || input_type == INPUT_HEX;
}

static bool_t edit_is_valid_value_default(widget_t* widget) {
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  text = &(widget->text);

  switch (edit->input_type) {
    case INPUT_TEXT: {
      uint32_t size = text->size;
      uint32_t min = (uint32_t)(edit->min);
      uint32_t max = (uint32_t)(edit->max);

      if (min == max && min == 0) {
        return TRUE;
      }

      return min <= size && size <= max;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t v = 0;
      int32_t min = (int32_t)(edit->min);
      int32_t max = (int32_t)(edit->max);

      if (text->size == 0) {
        return FALSE;
      }

      if (min == max) {
        return TRUE;
      }
      wstr_to_int(text, &v);

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
    case INPUT_EMAIL: {
      if (text->size > 0) {
        const wchar_t* p = wcs_chr(text->str, '@');
        return text->size > 0 && p != NULL && p != text->str && wcs_chr(p + 1, '@') == NULL;
      } else {
        return FALSE;
      }
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
      }

      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
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
  if (widget->text.size == 0) {
    if (widget->focused) {
      widget_set_state(widget, WIDGET_STATE_EMPTY_FOCUS);
    } else {
      widget_set_state(widget, WIDGET_STATE_EMPTY);
    }
  } else if (widget->focused) {
    widget_set_state(widget, WIDGET_STATE_FOCUSED);
  } else {
    widget_set_state(widget, WIDGET_STATE_NORMAL);
  }

  return RET_OK;
}

static ret_t edit_do_request_input_method(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_method_t* im = input_method();
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

static ret_t edit_on_focused(widget_t* widget) {
  edit_t* edit = EDIT(widget);

  if (edit->timer_id == TK_INVALID_ID) {
    edit->timer_id = timer_add(edit_update_caret, widget, 600);
  }

  if (widget->target == NULL) {
    edit_request_input_method(widget);

    if (!edit->select_none_when_focused) {
      idle_add(edit_select_all_async, edit);
    }
  }

  return RET_OK;
}

static ret_t edit_pointer_up_cleanup(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  widget_ungrab(widget->parent, widget);
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return RET_OK;
}

static ret_t edit_paste(widget_t* widget) {
  value_t v;
  wstr_t str;
  const char* data = clip_board_get_text();
  if (data != NULL) {
    uint32_t i = 0;
    value_set_str(&v, data);
    wstr_init(&str, 0);
    wstr_from_value(&str, &v);
    wstr_normalize_newline(&str, ' ');

    for (i = 0; i < str.size; i++) {
      edit_input_char(widget, str.str[i]);
    }

    wstr_reset(&str);
  }

  return RET_OK;
}

static ret_t edit_pre_input(widget_t* widget, uint32_t key) {
  edit_t* edit = EDIT(widget);

  if (edit->pre_input != NULL) {
    return edit->pre_input(widget, key);
  }

  return RET_OK;
}

static ret_t edit_on_key_down(widget_t* widget, key_event_t* e) {
  uint32_t key = e->key;
  edit_t* edit = EDIT(widget);

  if (edit_pre_input(widget, key) == RET_STOP) {
    return RET_STOP;
  }

  if (key == TK_KEY_TAB) {
    return RET_OK;
  } else if (key == TK_KEY_LEFT || key == TK_KEY_RIGHT) {
    uint32_t cursor = text_edit_get_cursor(edit->model);
    if (key == TK_KEY_LEFT && cursor == 0) {
      return RET_OK;
    }

    if (key == TK_KEY_RIGHT && cursor == widget->text.size) {
      return RET_OK;
    }

  } else if (key == TK_KEY_DOWN) {
    if (edit_is_number(widget) || edit->inc_value != NULL) {
      edit_dec(edit);
    } else {
      widget_focus_next(widget);
    }
    return RET_STOP;
  } else if (key == TK_KEY_UP) {
    if (edit_is_number(widget) || edit->inc_value != NULL) {
      edit_inc(edit);
    } else {
      widget_focus_prev(widget);
    }
    return RET_STOP;
  }

  if (key == TK_KEY_BACKSPACE || key == TK_KEY_DELETE || key == TK_KEY_LEFT ||
      key == TK_KEY_RIGHT || key == TK_KEY_HOME || key == TK_KEY_END ||
      ((e->ctrl || e->cmd) &&
       (key == TK_KEY_a || key == TK_KEY_y || key == TK_KEY_z || key == TK_KEY_v ||
        key == TK_KEY_x || key == TK_KEY_c || key == TK_KEY_A || key == TK_KEY_Y ||
        key == TK_KEY_Z || key == TK_KEY_V || key == TK_KEY_X || key == TK_KEY_C))) {
    if (key == TK_KEY_v || key == TK_KEY_V) {
      edit_paste(widget);
    } else {
      text_edit_key_down(edit->model, (key_event_t*)e);
    }

    if (key != TK_KEY_LEFT && key != TK_KEY_RIGHT && key != TK_KEY_HOME && key != TK_KEY_END) {
      edit_dispatch_event(widget, EVT_VALUE_CHANGING);
    }
  } else if (key < 128 && isprint(key)) {
    app_type_t app_type = system_info()->app_type;
    if (app_type != APP_DESKTOP && app_type != APP_MOBILE) {
      edit_input_char(widget, (wchar_t)key);
    }
  }

  return RET_STOP;
}

static ret_t edit_on_key_up(widget_t* widget, key_event_t* e) {
  int key = e->key;
  ret_t ret = RET_OK;
  edit_t* edit = EDIT(widget);

  if (key_code_is_enter(key)) {
    if (edit->timer_id == TK_INVALID_ID) {
      edit_on_focused(widget);
    }
    ret = RET_STOP;
  } else {
    ret = text_edit_key_up(edit->model, e);
  }

  return ret;
}

static ret_t edit_select_all_async(const idle_info_t* info) {
  edit_t* edit = EDIT(info->ctx);
  text_edit_select_all(edit->model);

  return RET_REMOVE;
}

ret_t edit_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint32_t type = e->type;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->visible, RET_OK);

  if (edit->readonly) {
    if (type == EVT_RESIZE || type == EVT_MOVE_RESIZE) {
      text_edit_layout(edit->model);
      widget_invalidate(widget, NULL);
    }
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget_find_target(widget, evt.x, evt.y) == NULL) {
        text_edit_click(edit->model, evt.x, evt.y);
        widget_grab(widget->parent, widget);
      }

      if (widget->target == NULL) {
        input_method_request(input_method(), widget);
      }
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      edit->last_user_action_time = e->time;
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

      if (evt.pressed) {
        edit->last_user_action_time = e->time;
      }
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_UP: {
      ret = RET_STOP;
      widget_ungrab(widget->parent, widget);
      widget_invalidate(widget, NULL);
      edit->last_user_action_time = e->time;
      break;
    }
    case EVT_KEY_DOWN: {
      edit->last_user_action_time = e->time;
      ret = edit_on_key_down(widget, (key_event_t*)e);
      edit_update_status(widget);
      break;
    }
    case EVT_KEY_UP: {
      edit->last_user_action_time = e->time;
      ret = edit_on_key_up(widget, (key_event_t*)e);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_COMMIT: {
      text_edit_state_t state;
      text_edit_get_state(edit->model, &state);
      im_commit_event_t* evt = (im_commit_event_t*)e;

      edit->last_user_action_time = e->time;
      if (state.preedit) {
        text_edit_preedit_clear(edit->model);
      }

      if (evt->replace) {
        edit_clear(edit);
      }
      edit_commit_str(widget, evt->text);

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
      text_edit_preedit_abort(edit->model);
      break;
    }
    case EVT_BLUR: {
      input_method_request(input_method(), NULL);

      edit_update_status(widget);
      if (!edit_is_valid_value(widget)) {
        if (edit->auto_fix) {
          edit_auto_fix(widget);
        } else if (widget->text.size > 0) {
          widget_set_state(widget, WIDGET_STATE_ERROR);
        }
      }
      text_edit_unselect(edit->model);
      edit_dispatch_event(widget, EVT_VALUE_CHANGED);
      break;
    }
    case EVT_FOCUS: {
      if (edit->open_im_when_focused) {
        edit_on_focused(widget);
      }
      break;
    }
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;
      int32_t delta = evt->dy;
      if (delta > 0) {
        edit_dec(edit);
      } else if (delta < 0) {
        edit_inc(edit);
      }
      ret = RET_STOP;
      edit->last_user_action_time = e->time;
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      text_edit_layout(edit->model);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_VALUE_CHANGING: {
      edit_update_status(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_ACTION: {
      if (tk_str_eq(edit->action_text, ACTION_TEXT_DONE)) {
        input_method_request(input_method(), NULL);
        ret = RET_STOP;
      } else if (tk_str_eq(edit->action_text, ACTION_TEXT_NEXT)) {
        widget_focus_next(widget);
        ret = RET_STOP;
      }
      log_debug("action button\n");
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t edit_on_re_translate(widget_t* widget) {
  edit_t* edit = EDIT(widget);
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

  return RET_OK;
}

ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max, uint32_t step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->min = min;
  edit->max = max;
  edit->step = step;
  if (edit->input_type != INPUT_UINT) {
    edit->input_type = INPUT_INT;
  }

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

  return RET_OK;
}

ret_t edit_set_readonly(widget_t* widget, bool_t readonly) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->readonly = readonly;

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

ret_t edit_set_input_type(widget_t* widget, input_type_t type) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->input_type = type;
  edit->pre_input = NULL;
  edit->is_valid_char = NULL;
  edit->fix_value = NULL;
  edit->inc_value = NULL;
  edit->dec_value = NULL;
  edit->is_valid_value = NULL;

  if (type == INPUT_INT || type == INPUT_UINT) {
    edit->step = 1;
  } else if (type == INPUT_FLOAT || type == INPUT_UFLOAT) {
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
  } else if (type == INPUT_DATE) {
    edit->fix_value = edit_date_fix;
    edit->inc_value = edit_date_inc_value;
    edit->dec_value = edit_date_dec_value;
    edit->pre_input = edit_date_pre_input;
    edit->is_valid_value = edit_date_is_valid;
    edit->is_valid_char = edit_date_is_valid_char;
  } else if (type == INPUT_TIME) {
    edit->fix_value = edit_time_fix;
    edit->inc_value = edit_time_inc_value;
    edit->dec_value = edit_time_dec_value;
    edit->pre_input = edit_time_pre_input;
    edit->is_valid_value = edit_time_is_valid;
    edit->is_valid_char = edit_time_is_valid_char;
  } else if (type == INPUT_TIME_FULL) {
    edit->fix_value = edit_time_full_fix;
    edit->inc_value = edit_time_full_inc_value;
    edit->dec_value = edit_time_full_dec_value;
    edit->pre_input = edit_time_full_pre_input;
    edit->is_valid_value = edit_time_full_is_valid;
    edit->is_valid_char = edit_time_full_is_valid_char;
  }

  return RET_OK;
}

ret_t edit_set_tips(widget_t* widget, const char* tips) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL && tips != NULL, RET_BAD_PARAMS);

  edit->tips = tk_str_copy(edit->tips, tips);
  text_edit_set_tips(edit->model, edit->tips);

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

  if (edit->tr_tips != NULL) {
    const char* tr_tips = locale_info_tr(widget_get_locale_info(widget), edit->tr_tips);
    edit_set_tips(widget, tr_tips);
  }

  return RET_REMOVE;
}

ret_t edit_set_tr_tips(widget_t* widget, const char* tr_tips) {
  edit_t* edit = EDIT(widget);
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(edit != NULL && tr_tips != NULL, RET_BAD_PARAMS);

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
  return_value_if_fail(edit != NULL && keyboard != NULL, RET_BAD_PARAMS);

  edit->keyboard = tk_str_copy(edit->keyboard, keyboard);

  return RET_OK;
}

ret_t edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->input_type;
  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_int(v, edit->min);
    } else if (input_type == INPUT_TEXT) {
      value_set_uint32(v, edit->min);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_double(v, edit->min);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_int(v, edit->max);
    } else if (input_type == INPUT_TEXT) {
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
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_FIX)) {
    value_set_bool(v, edit->auto_fix);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED)) {
    value_set_bool(v, edit->select_none_when_focused);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED)) {
    value_set_bool(v, edit->open_im_when_focused);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    value_set_int(v, edit->left_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    value_set_int(v, edit->right_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    value_set_int(v, edit->top_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    value_set_int(v, edit->bottom_margin);
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
      default: { value_set_wstr(v, widget->text.str); }
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
  } else if (tk_str_eq(name, WIDGET_PROP_INPUTING)) {
    int64_t delta = (time_now_ms() - edit->last_user_action_time);
    bool_t inputing =
        (delta < TK_INPUTING_TIMEOUT) && (edit->last_user_action_time > 0) && widget->focused;
    value_set_bool(v, inputing);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t edit_set_text(widget_t* widget, const value_t* v) {
  wstr_t str;
  wstr_init(&str, 0);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(wstr_from_value(&str, v) == RET_OK, RET_BAD_PARAMS);

  if (!wstr_equal(&(widget->text), &str)) {
    wstr_set(&(widget->text), str.str);

    text_edit_set_cursor(edit->model, widget->text.size);
    edit_dispatch_event(widget, EVT_VALUE_CHANGED);
    edit_update_status(widget);
  }

  wstr_reset(&str);

  return RET_OK;
}

ret_t edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->input_type;
  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->min = value_int(v);
    } else if (input_type == INPUT_TEXT) {
      edit->min = value_int(v);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->min = value_double(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->max = value_int(v);
    } else if (input_type == INPUT_TEXT) {
      edit->max = value_int(v);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->max = value_double(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->step = value_double(v);
      return RET_OK;
    } else if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->step = value_int(v);
      return RET_OK;
    } else {
      return RET_NOT_FOUND;
    }
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
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_FIX)) {
    edit->auto_fix = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECT_NONE_WHEN_FOCUSED)) {
    edit->select_none_when_focused = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED)) {
    edit->open_im_when_focused = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    int margin = value_int(v);
    edit->left_margin = margin;
    edit->right_margin = margin;
    edit->top_margin = margin;
    edit->bottom_margin = margin;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    edit->left_margin = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    edit->right_margin = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    edit->top_margin = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    edit->bottom_margin = value_int(v);
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
  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

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
  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

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
  edit_set_text(widget, &v);

  return RET_OK;
}

static ret_t edit_inc_default(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->input_type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->step ? edit->step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->min);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->step ? edit->step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->min);
      }
      edit_add_int(edit, step);
      break;
    }
    default:
      break;
  }
  text_edit_select_all(edit->model);

  return widget_invalidate_force(widget, NULL);
}

static ret_t edit_dec_default(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->input_type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->step ? edit->step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->max);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, -step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->step ? edit->step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->max);
      }
      edit_add_int(edit, -step);
      break;
    }
    default:
      break;
  }

  text_edit_select_all(edit->model);
  return widget_invalidate_force(widget, NULL);
}

ret_t edit_clear(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  widget->text.size = 0;
  text_edit_set_cursor(edit->model, 0xffffffff);

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
    const char* name = widget->name;
    if (tk_str_eq(name, "inc")) {
      widget_on(widget, EVT_CLICK, edit_on_inc, edit);
    } else if (tk_str_eq(name, "dec")) {
      widget_on(widget, EVT_CLICK, edit_on_dec, edit);
    } else if (tk_str_eq(name, "clear")) {
      widget_on(widget, EVT_CLICK, edit_on_clear, edit);
    } else if (tk_str_eq(name, "visible")) {
      widget_on(widget, EVT_VALUE_CHANGED, edit_on_password_visible, edit);
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
  TKMEM_FREE(edit->action_text);
  text_edit_destroy(edit->model);

  return RET_OK;
}

static ret_t edit_hook_children_button(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_REMOVE);

  widget_foreach(widget, edit_hook_button, widget);
  edit->idle_id = TK_INVALID_ID;

  return RET_REMOVE;
}

const char* const s_edit_properties[] = {WIDGET_PROP_MIN,
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
                                         WIDGET_PROP_ACTION_TEXT,
                                         WIDGET_PROP_TIPS,
                                         WIDGET_PROP_TR_TIPS,
                                         WIDGET_PROP_KEYBOARD,
                                         WIDGET_PROP_PASSWORD_VISIBLE,
                                         NULL};

ret_t edit_on_copy(widget_t* widget, widget_t* other) {
  edit_t* edit = EDIT(widget);
  edit_t* edit_other = EDIT(other);

  edit->tips = tk_str_copy(edit->tips, edit_other->tips);
  edit->tr_tips = tk_str_copy(edit->tr_tips, edit_other->tr_tips);

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

  return RET_OK;
}

TK_DECL_VTABLE(edit) = {.size = sizeof(edit_t),
                        .type = WIDGET_TYPE_EDIT,
                        .focusable = TRUE,
                        .inputable = TRUE,
                        .pointer_cursor = WIDGET_CURSOR_EDIT,
                        .clone_properties = s_edit_properties,
                        .persistent_properties = s_edit_properties,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = edit_create,
                        .on_re_translate = edit_on_re_translate,
                        .on_paint_self = edit_on_paint_self,
                        .set_prop = edit_set_prop,
                        .get_prop = edit_get_prop,
                        .on_destroy = edit_on_destroy,
                        .on_copy = edit_on_copy,
                        .on_event = edit_on_event};

widget_t* edit_create_ex(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                         wh_t h) {
  widget_t* widget = widget_create(parent, vt, x, y, w, h);
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, NULL);

  edit->left_margin = 2;
  edit->right_margin = 2;
  edit->top_margin = 2;
  edit->bottom_margin = 2;
  edit->open_im_when_focused = TRUE;
  edit_set_text_limit(widget, 0, 1024);

  edit_update_status(widget);
  edit->timer_id = TK_INVALID_ID;
  edit->idle_id = idle_add(edit_hook_children_button, edit);

  edit->model = text_edit_create(widget, TRUE);
  ENSURE(edit->model != NULL);

  widget_set_text(widget, L"");
  edit_set_password_visible(widget, FALSE);
  edit_set_action_text(widget, ACTION_TEXT_DONE);

  return widget;
}

widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return edit_create_ex(parent, TK_REF_VTABLE(edit), x, y, w, h);
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

ret_t edit_pre_input_with_sep(widget_t* widget, uint32_t key, char sep) {
  edit_t* edit = EDIT(widget);
  text_edit_state_t state;
  text_edit_unselect(edit->model);
  text_edit_get_state(edit->model, &state);

  if (key == TK_KEY_BACKSPACE && state.cursor > 0) {
    if (widget->text.str[state.cursor - 1] == sep) {
      text_edit_set_cursor(edit->model, state.cursor - 1);
      return RET_STOP;
    }
  } else if (key == TK_KEY_DELETE) {
    if (widget->text.str[state.cursor] == sep) {
      text_edit_set_cursor(edit->model, state.cursor + 1);
      return RET_STOP;
    }
  }

  return RET_OK;
}

ret_t edit_add_value_with_sep(widget_t* widget, int delta, char sep) {
  char c = 0;
  uint32_t cursor = 0;
  text_edit_state_t state;
  edit_t* edit = EDIT(widget);
  wstr_t* text = &(widget->text);
  text_edit_get_state(edit->model, &state);

  if (text->size == 0) {
    edit->fix_value(widget);
    return RET_OK;
  }

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
  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

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

  if (edit->fix_value != NULL) {
    return edit->fix_value(widget);
  } else {
    return edit_auto_fix_default(widget);
  }
}

bool_t edit_is_valid_value(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  if (edit->is_valid_value != NULL) {
    return edit->is_valid_value(widget);
  } else {
    return edit_is_valid_value_default(widget);
  }
}
