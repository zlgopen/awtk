/**
 * File:   mledit.h
 * Author: AWTK Develop Team
 * Brief:  mledit
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
 * 2019-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/events.h"
#include "mledit/mledit.h"
#include "mledit/line_number.h"
#include "base/input_method.h"
#include "scroll_view/scroll_bar.h"

static ret_t mledit_update_status(widget_t* widget);

static ret_t mledit_dispatch_event(widget_t* widget, event_type_t type) {
  event_t evt = event_init(type, widget);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

ret_t mledit_set_tips(widget_t* widget, const char* tips) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL && tips != NULL, RET_BAD_PARAMS);

  mledit->tips = tk_str_copy(mledit->tips, tips);
  text_edit_set_tips(mledit->model, mledit->tips);

  return RET_OK;
}

static ret_t mledit_apply_tr_text_before_paint(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  mledit_t* mledit = MLEDIT(widget);

  if (mledit->tr_tips != NULL) {
    const char* tr_tips = locale_info_tr(widget_get_locale_info(widget), mledit->tr_tips);
    mledit_set_tips(widget, tr_tips);
  }

  return RET_REMOVE;
}

ret_t mledit_set_tr_tips(widget_t* widget, const char* tr_tips) {
  mledit_t* mledit = MLEDIT(widget);
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(mledit != NULL && tr_tips != NULL, RET_BAD_PARAMS);

  mledit->tr_tips = tk_str_copy(mledit->tr_tips, tr_tips);
  if (win != NULL) {
    tr_tips = locale_info_tr(widget_get_locale_info(widget), tr_tips);
    mledit_set_tips(widget, tr_tips);
  } else {
    widget_on(widget, EVT_BEFORE_PAINT, mledit_apply_tr_text_before_paint, widget);
  }

  return RET_OK;
}

ret_t mledit_set_keyboard(widget_t* widget, const char* keyboard) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL && keyboard != NULL, RET_BAD_PARAMS);

  mledit->keyboard = tk_str_copy(mledit->keyboard, keyboard);

  return RET_OK;
}

ret_t mledit_set_focus(widget_t* widget, bool_t focus) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL, RET_BAD_PARAMS);

  widget_set_focused(widget, focus);
  mledit_update_status(widget);

  return RET_OK;
}

ret_t mledit_set_readonly(widget_t* widget, bool_t readonly) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL, RET_BAD_PARAMS);

  mledit->readonly = readonly;

  return RET_OK;
}

ret_t mledit_set_wrap_word(widget_t* widget, bool_t wrap_word) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL, RET_BAD_PARAMS);

  mledit->wrap_word = wrap_word;
  text_edit_set_wrap_word(mledit->model, wrap_word);

  return RET_OK;
}

ret_t mledit_set_max_lines(widget_t* widget, uint32_t max_lines) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL, RET_BAD_PARAMS);

  mledit->max_lines = max_lines;
  text_edit_set_max_rows(mledit->model, max_lines);
  text_edit_layout(mledit->model);

  return RET_OK;
}

static ret_t mledit_get_prop(widget_t* widget, const char* name, value_t* v) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, WIDGET_PROP_READONLY)) {
    value_set_bool(v, mledit->readonly);
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_WRAP_WORD)) {
    value_set_bool(v, mledit->wrap_word);
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_MAX_LINES)) {
    value_set_int(v, mledit->max_lines);
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_SCROLL_LINE)) {
    value_set_int(v, mledit->scroll_line);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    value_set_int(v, mledit->left_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    value_set_int(v, mledit->right_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    value_set_int(v, mledit->top_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    value_set_int(v, mledit->bottom_margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    value_set_str(v, mledit->tips);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TIPS)) {
    value_set_str(v, mledit->tr_tips);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_KEYBOARD)) {
    value_set_str(v, mledit->keyboard);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    value_set_bool(v, !(mledit->readonly));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_wstr(v, widget->text.str);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CARET_X)) {
    text_edit_state_t state;
    text_edit_get_state(mledit->model, &state);
    value_set_int(v, state.caret.x - state.ox);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CARET_Y)) {
    text_edit_state_t state;
    text_edit_get_state(mledit->model, &state);
    value_set_int(v, state.caret.y - state.oy);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INPUTING)) {
    int64_t delta = (time_now_ms() - mledit->last_user_action_time);
    bool_t inputing =
        (delta < TK_INPUTING_TIMEOUT) && (mledit->last_user_action_time > 0) && widget->focused;
    value_set_bool(v, inputing);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static inline void mledit_reset_text_edit_layout(text_edit_t* text_edit) {
  text_edit_set_offset(text_edit, 0, 0);
  text_edit_set_select(text_edit, 0, 0);
  text_edit_set_cursor(text_edit, text_edit_get_cursor(text_edit));
}

static ret_t mledit_set_text(widget_t* widget, const value_t* v) {
  wstr_t str;
  wstr_init(&str, 0);
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(wstr_from_value(&str, v) == RET_OK, RET_BAD_PARAMS);

  if (!wstr_equal(&(widget->text), &str)) {
    wstr_set(&(widget->text), str.str);
    mledit_reset_text_edit_layout(mledit->model);
    mledit_dispatch_event(widget, EVT_VALUE_CHANGED);
  }

  wstr_reset(&str);

  return RET_OK;
}
static ret_t mledit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_TEXT) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    mledit_set_text(widget, v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_READONLY)) {
    mledit->readonly = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_WRAP_WORD)) {
    mledit_set_wrap_word(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_MAX_LINES)) {
    mledit_set_max_lines(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, MLEDIT_PROP_SCROLL_LINE)) {
    mledit_set_scroll_line(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    int margin = value_int(v);
    mledit->left_margin = margin;
    mledit->right_margin = margin;
    mledit->top_margin = margin;
    mledit->bottom_margin = margin;
    mledit_reset_text_edit_layout(mledit->model);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LEFT_MARGIN)) {
    mledit->left_margin = value_int(v);
    mledit_reset_text_edit_layout(mledit->model);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_RIGHT_MARGIN)) {
    mledit->right_margin = value_int(v);
    mledit_reset_text_edit_layout(mledit->model);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TOP_MARGIN)) {
    mledit->top_margin = value_int(v);
    mledit_reset_text_edit_layout(mledit->model);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BOTTOM_MARGIN)) {
    mledit->bottom_margin = value_int(v);
    mledit_reset_text_edit_layout(mledit->model);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUS) || tk_str_eq(name, WIDGET_PROP_FOCUSED)) {
    mledit_set_focus(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    mledit_set_tips(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TIPS)) {
    mledit_set_tr_tips(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_KEYBOARD)) {
    mledit_set_keyboard(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    wstr_from_value(&(widget->text), v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t mledit_on_destroy(widget_t* widget) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(widget != NULL && mledit != NULL, RET_BAD_PARAMS);

  if (mledit->timer_id != TK_INVALID_ID) {
    timer_remove(mledit->timer_id);
  }

  wstr_reset(&(mledit->temp));
  TKMEM_FREE(mledit->tips);
  TKMEM_FREE(mledit->tr_tips);
  TKMEM_FREE(mledit->keyboard);
  text_edit_destroy(mledit->model);

  return RET_OK;
}

static ret_t mledit_on_paint_self(widget_t* widget, canvas_t* c) {
  mledit_t* mledit = MLEDIT(widget);

  text_edit_paint(mledit->model, c);

  return RET_OK;
}

static ret_t mledit_commit_str(widget_t* widget, const char* str) {
  mledit_t* mledit = MLEDIT(widget);
  wstr_set_utf8(&(mledit->temp), str);

  text_edit_paste(mledit->model, mledit->temp.str, mledit->temp.size);
  mledit_dispatch_event(widget, EVT_VALUE_CHANGING);

  return RET_OK;
}

static ret_t mledit_request_input_method_on_window_open(void* ctx, event_t* e) {
  mledit_t* mledit = MLEDIT(ctx);

  if (!mledit->readonly) {
    input_method_request(input_method(), WIDGET(ctx));
  }

  return RET_REMOVE;
}

static ret_t mledit_request_input_method(widget_t* widget) {
  mledit_t* mledit = MLEDIT(widget);

  if (mledit->readonly) {
    return RET_OK;
  }

  if (widget_is_window_opened(widget)) {
    input_method_request(input_method(), widget);
  } else {
    widget_t* win = widget_get_window(widget);
    if (win != NULL) {
      widget_on(win, EVT_WINDOW_OPEN, mledit_request_input_method_on_window_open, widget);
    }
  }

  return RET_OK;
}

static ret_t mledit_update_caret(const timer_info_t* timer) {
  mledit_t* mledit = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(timer != NULL, RET_REMOVE);

  mledit = MLEDIT(timer->ctx);
  widget = WIDGET(timer->ctx);
  return_value_if_fail(mledit != NULL && widget != NULL, RET_REMOVE);

  if (mledit->readonly) {
    return RET_REMOVE;
  }

  widget_invalidate_force(widget, NULL);
  text_edit_invert_caret_visible(mledit->model);

  if (widget->focused) {
    return RET_REPEAT;
  } else {
    mledit->timer_id = TK_INVALID_ID;
    text_edit_set_caret_visible(mledit->model, FALSE);
    return RET_REMOVE;
  }
}

static ret_t mledit_update_status(widget_t* widget) {
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

static ret_t mledit_pointer_up_cleanup(widget_t* widget) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL && widget != NULL, RET_BAD_PARAMS);

  widget_ungrab(widget->parent, widget);
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return RET_OK;
}

ret_t mledit_clear(mledit_t* mledit) {
  widget_t* widget = WIDGET(mledit);
  return_value_if_fail(widget != NULL && mledit != NULL, RET_BAD_PARAMS);

  widget->text.size = 0;
  text_edit_set_cursor(mledit->model, 0);

  return widget_invalidate_force(widget, NULL);
}

ret_t mledit_set_cursor(widget_t* widget, uint32_t cursor) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(widget != NULL && mledit != NULL, RET_BAD_PARAMS);

  return text_edit_set_cursor(mledit->model, cursor);
}

ret_t mledit_set_scroll_line(widget_t* widget, uint32_t scroll_line) {
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(widget != NULL && mledit != NULL, RET_BAD_PARAMS);
  mledit->scroll_line = scroll_line;
  return RET_OK;
}

static ret_t mledit_focus_set_cursor(const idle_info_t* info) {
  mledit_t* mledit = MLEDIT(info->ctx);
  mledit_set_cursor(WIDGET(mledit), text_edit_get_cursor(mledit->model));

  return RET_REMOVE;
}

static ret_t mledit_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint32_t type = e->type;
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(widget != NULL && mledit != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->visible, RET_OK);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget_find_target(widget, evt.x, evt.y) == NULL) {
        text_edit_click(mledit->model, evt.x, evt.y);
        widget_grab(widget->parent, widget);
      }

      if (widget->target == NULL) {
        mledit_request_input_method(widget);
      }
      mledit->last_user_action_time = e->time;
      mledit_update_status(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      mledit_pointer_up_cleanup(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget->parent && widget->parent->grab_widget == widget) {
        if (widget->target == NULL) {
          text_edit_drag(mledit->model, evt.x, evt.y);
          ret = RET_STOP;
        }
      }

      if (evt.pressed) {
        mledit->last_user_action_time = e->time;
      }
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_UP: {
      widget_ungrab(widget->parent, widget);
      mledit->last_user_action_time = e->time;
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      int32_t key = evt->key;
      if (mledit->readonly) {
        if (evt->ctrl && (key == TK_KEY_C || key == TK_KEY_c)) {
          log_debug("copy\n");
        } else {
          break;
        }
      }
      text_edit_key_down(mledit->model, (key_event_t*)e);
      if ((key < 128 && isprint(key)) || key == TK_KEY_BACKSPACE || key == TK_KEY_DELETE) {
        mledit_dispatch_event(widget, EVT_VALUE_CHANGING);
      }

      mledit_update_status(widget);
      ret = RET_STOP;
      mledit->last_user_action_time = e->time;
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_COMMIT: {
      text_edit_state_t state;
      text_edit_get_state(mledit->model, &state);
      im_commit_event_t* evt = (im_commit_event_t*)e;

      if (mledit->readonly) {
        break;
      }

      if (state.preedit) {
        text_edit_preedit_clear(mledit->model);
      }

      if (evt->replace) {
        mledit_clear(mledit);
      }
      mledit_commit_str(widget, evt->text);
      mledit_update_status(widget);
      mledit->last_user_action_time = e->time;
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_IM_PREEDIT: {
      mledit->last_user_action_time = e->time;
      text_edit_preedit(mledit->model);
      break;
    }
    case EVT_IM_PREEDIT_CONFIRM: {
      mledit->last_user_action_time = e->time;
      text_edit_preedit_confirm(mledit->model);
      break;
    }
    case EVT_IM_PREEDIT_ABORT: {
      mledit->last_user_action_time = e->time;
      text_edit_preedit_abort(mledit->model);
      break;
    }
    case EVT_IM_ACTION: {
      mledit_commit_str(widget, "\n");
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* key_event = key_event_cast(e);
      if (key_code_is_enter(key_event->key)) {
        ret = RET_STOP;
      } else {
        ret = text_edit_key_up(mledit->model, key_event);
      }
      mledit->last_user_action_time = e->time;
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_BLUR: {
      input_method_request(input_method(), NULL);

      mledit_update_status(widget);
      mledit_dispatch_event(widget, EVT_VALUE_CHANGED);
      break;
    }
    case EVT_FOCUS: {
      if (mledit->timer_id == TK_INVALID_ID) {
        mledit->timer_id = timer_add(mledit_update_caret, widget, 600);
      }

      if (widget->target == NULL) {
        mledit_request_input_method(widget);
        idle_add(mledit_focus_set_cursor, mledit);
      }
      break;
    }
    case EVT_WHEEL: {
      key_event_t kevt;
      wheel_event_t* evt = (wheel_event_t*)e;
      int32_t delta = evt->dy;
      text_edit_t* text_edit = mledit->model;
      widget_t* vscroll_bar = widget_lookup_by_type(widget, WIDGET_TYPE_SCROLL_BAR_DESKTOP, TRUE);

      if (vscroll_bar != NULL) {
        if (delta > 0) {
          scroll_bar_add_delta(vscroll_bar, -text_edit->c->font_size * mledit->scroll_line);
        } else if (delta < 0) {
          scroll_bar_add_delta(vscroll_bar, text_edit->c->font_size * mledit->scroll_line);
        }
      } else {
        if (delta > 0) {
          key_event_init(&kevt, EVT_KEY_DOWN, widget, TK_KEY_UP);
          text_edit_key_down(mledit->model, (key_event_t*)&kevt);
        } else if (delta < 0) {
          key_event_init(&kevt, EVT_KEY_DOWN, widget, TK_KEY_DOWN);
          text_edit_key_down(mledit->model, (key_event_t*)&kevt);
        }
      }
      ret = RET_STOP;
      mledit->last_user_action_time = e->time;
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      mledit_reset_text_edit_layout(mledit->model);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_VALUE_CHANGING: {
      mledit_update_status(widget);
      widget_invalidate(widget, NULL);
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t mledit_on_re_translate(widget_t* widget) {
  mledit_t* mledit = MLEDIT(widget);
  if (mledit->tr_tips != NULL) {
    const char* tr_tips = locale_info_tr(widget_get_locale_info(widget), mledit->tr_tips);
    mledit_set_tips(widget, tr_tips);
  }

  return RET_OK;
}

static ret_t mledit_sync_line_number(widget_t* widget, text_edit_state_t* state) {
  mledit_t* mledit = MLEDIT(widget);
  widget_t* line_number = widget_lookup_by_type(widget, WIDGET_TYPE_LINE_NUMBER, TRUE);

  if (line_number != NULL) {
    line_number_set_yoffset(line_number, state->oy);
    line_number_set_line_height(line_number, state->line_height);
    line_number_set_top_margin(line_number, mledit->top_margin);
    line_number_set_bottom_margin(line_number, mledit->bottom_margin);

    widget_invalidate_force(line_number, NULL);
  }

  return RET_BAD_PARAMS;
}

static ret_t mledit_sync_scrollbar(widget_t* widget, text_edit_state_t* state) {
  xy_t y = 0;
  mledit_t* mledit = MLEDIT(widget);
  wh_t virtual_h = (state->last_line_number + 1) * state->line_height + mledit->top_margin +
                   mledit->bottom_margin;
  widget_t* vscroll_bar = widget_lookup_by_type(widget, WIDGET_TYPE_SCROLL_BAR_DESKTOP, TRUE);

  if (vscroll_bar != NULL) {
    virtual_h = virtual_h >= vscroll_bar->h ? virtual_h : vscroll_bar->h;

    if (virtual_h > vscroll_bar->h) {
      y = state->oy * virtual_h / (virtual_h - vscroll_bar->h);
    } else {
      y = 0;
    }

    scroll_bar_set_params(vscroll_bar, virtual_h, state->line_height);
    scroll_bar_set_value_only(vscroll_bar, y);
    widget_invalidate_force(vscroll_bar, NULL);
  }

  return RET_OK;
}

static ret_t mledit_on_text_edit_state_changed(void* ctx, text_edit_state_t* state) {
  widget_t* widget = WIDGET(ctx);

  mledit_sync_line_number(widget, state);
  mledit_sync_scrollbar(widget, state);

  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

static ret_t mledit_on_scroll_bar_value_changed(void* ctx, event_t* e) {
  int32_t value = 0;
  mledit_t* mledit = MLEDIT(ctx);
  widget_t* vscroll_bar = WIDGET(e->target);
  scroll_bar_t* scroll_bar = SCROLL_BAR(vscroll_bar);

  return_value_if_fail(vscroll_bar != NULL && scroll_bar != NULL, RET_BAD_PARAMS);

  value = widget_get_value(vscroll_bar);
  value = (scroll_bar->virtual_size - vscroll_bar->h) * value / scroll_bar->virtual_size;

  text_edit_set_offset(mledit->model, 0, value);

  return RET_OK;
}

static ret_t mledit_on_add_child(widget_t* widget, widget_t* child) {
  mledit_t* mledit = MLEDIT(widget);
  const char* type = widget_get_type(child);
  return_value_if_fail(mledit != NULL && widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(type, WIDGET_TYPE_SCROLL_BAR_DESKTOP)) {
    widget_on(child, EVT_VALUE_CHANGED, mledit_on_scroll_bar_value_changed, widget);
  }

  text_edit_set_on_state_changed(mledit->model, mledit_on_text_edit_state_changed, widget);

  return RET_CONTINUE;
}

static ret_t mledit_init_idle_func(const idle_info_t* info) {
  text_edit_state_t state = {0};
  mledit_t* mledit = MLEDIT(info->ctx);

  mledit_set_cursor(WIDGET(mledit), 0);
  text_edit_get_state(mledit->model, &state);
  mledit_on_text_edit_state_changed(mledit, &state);

  return RET_REMOVE;
}

const char* s_mledit_properties[] = {WIDGET_PROP_READONLY,
                                     WIDGET_PROP_MARGIN,
                                     WIDGET_PROP_LEFT_MARGIN,
                                     WIDGET_PROP_RIGHT_MARGIN,
                                     WIDGET_PROP_TOP_MARGIN,
                                     WIDGET_PROP_BOTTOM_MARGIN,
                                     WIDGET_PROP_TIPS,
                                     WIDGET_PROP_TR_TIPS,
                                     WIDGET_PROP_KEYBOARD,
                                     MLEDIT_PROP_MAX_LINES,
                                     MLEDIT_PROP_WRAP_WORD,
                                     MLEDIT_PROP_SCROLL_LINE,
                                     NULL};

TK_DECL_VTABLE(mledit) = {.size = sizeof(mledit_t),
                          .type = WIDGET_TYPE_MLEDIT,
                          .focusable = TRUE,
                          .inputable = TRUE,
                          .pointer_cursor = WIDGET_CURSOR_EDIT,
                          .clone_properties = s_mledit_properties,
                          .persistent_properties = s_mledit_properties,
                          .parent = TK_PARENT_VTABLE(widget),
                          .create = mledit_create,
                          .on_paint_self = mledit_on_paint_self,
                          .on_re_translate = mledit_on_re_translate,
                          .set_prop = mledit_set_prop,
                          .get_prop = mledit_get_prop,
                          .on_event = mledit_on_event,
                          .on_add_child = mledit_on_add_child,
                          .on_destroy = mledit_on_destroy};

widget_t* mledit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(mledit), x, y, w, h);
  mledit_t* mledit = MLEDIT(widget);
  return_value_if_fail(mledit != NULL, NULL);

  mledit->model = text_edit_create(widget, FALSE);
  ENSURE(mledit->model != NULL);
  mledit->left_margin = 1;
  mledit->top_margin = 1;
  mledit->right_margin = 1;
  mledit->bottom_margin = 1;
  mledit->scroll_line = 1.0f;
  wstr_init(&(mledit->temp), 0);
  widget_set_text(widget, L"");

  mledit_update_status(widget);
  widget_add_idle(widget, mledit_init_idle_func);

  return widget;
}

widget_t* mledit_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, mledit), NULL);

  return widget;
}
