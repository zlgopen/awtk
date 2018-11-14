/**
 * File:   edit.h
 * Author: AWTK Develop Team
 * Brief:  edit
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utf8.h"
#include "base/edit.h"
#include "base/keys.h"
#include "base/idle.h"
#include "base/timer.h"
#include "base/utils.h"
#include "base/enums.h"
#include "base/events.h"

static ret_t edit_update_status(widget_t* widget);

static ret_t edit_dispatch_event(widget_t* widget, event_type_t type) {
  event_t evt = event_init(type, widget);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t edit_update_caret(const timer_info_t* timer) {
  rect_t r;
  edit_t* edit = EDIT(timer->ctx);
  widget_t* widget = WIDGET(timer->ctx);

  r = rect_init(edit->caret_x, 0, 1, widget->h);
  edit->caret_visible = !edit->caret_visible;
  widget_invalidate(widget, &r);

  if (widget->focused) {
    return RET_REPEAT;
  } else {
    edit->timer_id = 0;
    return RET_REMOVE;
  }
}

#define INVISIBLE_CHAR '*'
#define TEMP_STR_LEN 31

static ret_t edit_get_display_text(widget_t* widget, canvas_t* c, wstr_t* text, wchar_t* temp_str) {
  int32_t i = 0;
  float_t cw = 0;
  edit_t* edit = EDIT(widget);
  wstr_t* str = &(widget->text);
  wh_t w = widget->w - edit->left_margin - edit->right_margin;
  bool_t invisible = str->size && (edit->limit.type == INPUT_PASSWORD && !(edit->password_visible));

  if (!str->size && !widget->focused) {
    if (edit->tips != NULL) {
      utf8_to_utf16(edit->tips, temp_str, TEMP_STR_LEN);
    }
  }

  if (str->size > 0) {
    i = str->size - 1;
    while (w > 0 && i >= 0) {
      wchar_t chr = invisible ? INVISIBLE_CHAR : str->str[i];

      cw = canvas_measure_text(c, &chr, 1);
      w -= cw;

      if (w > cw && i > 0) {
        i--;
      } else {
        break;
      }
    }

    text->str = str->str + i;
    text->size = wcslen(text->str);
    text->capacity = text->size;
  } else {
    text->str = temp_str;
    text->size = wcslen(temp_str);
  }

  if (invisible) {
    uint32_t size = tk_min(text->size, TEMP_STR_LEN);
    for (i = 0; i < size; i++) {
      temp_str[i] = INVISIBLE_CHAR;
    }
    temp_str[size] = '\0';
    text->str = temp_str;
    text->size = size;
  }

  return RET_OK;
}

ret_t edit_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  wstr_t text;
  uint32_t text_w = 0;
  edit_t* edit = EDIT(widget);
  style_t* style = widget->astyle;
  wchar_t temp_str[TEMP_STR_LEN + 1];
  color_t trans = color_init(0, 0, 0, 0);
  uint8_t left_margin = edit->left_margin;
  uint8_t right_margin = edit->right_margin;
  uint8_t top_margin = edit->top_margin;
  uint8_t bottom_margin = edit->bottom_margin;
  wh_t w = widget->w - left_margin - right_margin;
  wh_t h = widget->h - top_margin - bottom_margin;
  align_h_t align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  /*
    color_t selected_bg = style_get_color(style, STYLE_ID_SELECTED_FG_COLOR, trans);
    color_t selected_fg = style_get_color(style, STYLE_ID_SELECTED_BG_COLOR, trans);
    color_t selected_tc = style_get_color(style, STYLE_ID_SELECTED_TEXT_COLOR, trans);

    log_debug("selected_bg=0x%08X selected_fg=%08X selected_tc=%08X\n",
        selected_bg.color, selected_fg.color, selected_tc.color);
  */
  memset(temp_str, 0x00, sizeof(temp_str));
  return_value_if_fail(widget_prepare_text_style(widget, c) == RET_OK, RET_FAIL);
  return_value_if_fail(edit_get_display_text(widget, c, &text, temp_str) == RET_OK, RET_FAIL);

  r = rect_init(left_margin, top_margin, w, h);
  if (align_h == ALIGN_H_RIGHT) {
    r.w -= 3;
  }
  text_w = canvas_measure_text(c, text.str, text.size);
  if (text.size > 0) {
    canvas_draw_text_in_rect(c, text.str, text.size, &r);
  }

  switch (align_h) {
    case ALIGN_H_RIGHT: {
      edit->caret_x = r.x + r.w + 1;
      break;
    }
    case ALIGN_H_CENTER: {
      edit->caret_x = r.x + (r.w + text_w) / 2 + 1;
      break;
    }
    default: {
      edit->caret_x = r.x + text_w + 1;
      break;
    }
  }

  if (widget->focused && !edit->readonly && edit->caret_visible) {
    color_t black = color_init(0, 0, 0, 0xff);
    color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
    canvas_set_stroke_color(c, fg);

    canvas_draw_vline(c, edit->caret_x, top_margin, h);
  }

  return RET_OK;
}

static ret_t edit_delete_prev_char(widget_t* widget) {
  if (widget->text.size > 0) {
    wstr_pop(&(widget->text));
  }

  return RET_OK;
}

static ret_t edit_delete_next_char(widget_t* widget) {
  /*FIXME*/
  if (widget->text.size > 0) {
    wstr_pop(&(widget->text));
  }

  return RET_OK;
}

static ret_t edit_input_char(widget_t* widget, wchar_t c) {
  edit_t* edit = EDIT(widget);
  wstr_t* text = &(widget->text);
  input_type_t input_type = edit->limit.type;

  switch (input_type) {
    case INPUT_INT:
    case INPUT_UINT: {
      if (text->size >= TK_NUM_MAX_LEN) {
        return RET_FAIL;
      }

      if (c >= '0' && c <= '9') {
        wstr_push(text, c);
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_INT)) {
        if (text->size == 0) {
          wstr_push(text, c);
        }
        break;
      }
      break;
    }
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      if (text->size >= TK_NUM_MAX_LEN) {
        return RET_FAIL;
      }
      if (c >= '0' && c <= '9') {
        wstr_push(text, c);
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_FLOAT)) {
        if (text->size == 0) {
          wstr_push(text, c);
        }
        break;
      } else if (c == '.' || c == 'e') {
        if (text->size > 0 && wcs_chr(text->str, c) == NULL) {
          wstr_push(text, c);
        }
      }
      break;
    }
    case INPUT_EMAIL: {
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' ||
          c == '.' || c == '_') {
        wstr_push(text, c);
      } else if (c == '@') {
        if (text->size > 0 && wcs_chr(text->str, c) == NULL) {
          wstr_push(text, c);
        }
      }
      break;
    }
    case INPUT_HEX: {
      if (text->size > 10) {
        break;
      }
      if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
        wstr_push(text, c);
      } else if (c == 'X' || c == 'x') {
        if (text->size == 1 && text->str[0] == '0') {
          wstr_push(text, c);
        }
      }
      break;
    }
    case INPUT_PHONE: {
      if (c >= '0' && c <= '9') {
        wstr_push(text, c);
        break;
      } else if (c == '-') {
        if (text->size > 0 && wcs_chr(text->str, c) == NULL) {
          wstr_push(text, c);
        }
      }
      break;
    }
    default: {
      if (widget->text.size < edit->limit.u.t.max) {
        wstr_push(&(widget->text), c);
      }
    }
  }

  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

  return RET_OK;
}

static ret_t edit_commit_str(widget_t* widget, const char* str) {
  uint32_t i = 0;
  wchar_t wstr[32];
  utf8_to_utf16(str, wstr, ARRAY_SIZE(wstr));

  while (wstr[i]) {
    edit_input_char(widget, wstr[i]);
    i++;
  }

  return RET_OK;
}

static ret_t edit_on_key_down(widget_t* widget, key_event_t* e) {
  uint32_t key = e->key;
  if (key == FKEY_BACKSPACE) {
    return edit_delete_prev_char(widget);
  } else if (key == FKEY_DELETE) {
    return edit_delete_next_char(widget);
  } else {
    if (system_info()->app_type != APP_DESKTOP && key < 128 && isprint(key)) {
      return edit_input_char(widget, (wchar_t)key);
    } else {
      return RET_OK;
    }
  }
}

static ret_t edit_on_key_up(widget_t* widget, key_event_t* e) {
  return RET_OK;
}

bool_t edit_is_valid_value(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  wstr_t* text = &(widget->text);

  switch (edit->limit.type) {
    case INPUT_TEXT: {
      uint32_t size = text->size;
      uint32_t min = edit->limit.u.t.min;
      uint32_t max = edit->limit.u.t.max;

      if (min == max && min == 0) {
        return TRUE;
      }

      return min <= size && size <= max;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t v = 0;
      int32_t min = edit->limit.u.i.min;
      int32_t max = edit->limit.u.i.max;

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
      double min = edit->limit.u.f.min;
      double max = edit->limit.u.f.max;

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
      const wchar_t* p = wcschr(text->str, '@');
      /*FIXME*/
      return text->size > 0 && p != NULL && p != text->str && wcschr(p + 1, '@') == NULL;
    }
    default:
      break;
  }

  return TRUE;
}

static ret_t edit_auto_fix(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  wstr_t* text = &(widget->text);

  switch (edit->limit.type) {
    case INPUT_TEXT: {
      uint32_t size = text->size;
      uint32_t max = edit->limit.u.t.max;

      if (size > max) {
        text->size = max;
      }

      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t v = 0;
      int32_t min = edit->limit.u.i.min;
      int32_t max = edit->limit.u.i.max;

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
      double min = edit->limit.u.f.min;
      double max = edit->limit.u.f.max;

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

  return RET_OK;
}

static ret_t edit_update_status(widget_t* widget) {
  if (widget->focused) {
    widget_set_state(widget, WIDGET_STATE_FOCUSED);
  } else if (widget->text.size == 0) {
    widget_set_state(widget, WIDGET_STATE_EMPTY);
  } else {
    widget_set_state(widget, WIDGET_STATE_NORMAL);
  }

  return RET_OK;
}

ret_t edit_on_event(widget_t* widget, event_t* e) {
  uint32_t type = e->type;
  edit_t* edit = EDIT(widget);

  if (edit->readonly) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
      if (edit->timer_id == 0) {
        edit->timer_id = timer_add(edit_update_caret, widget, 600);
      }

      if (widget->target == NULL) {
        input_method_request(input_method(), widget);
      }
      edit_update_status(widget);

      break;
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      edit_on_key_down(widget, evt);
      widget_invalidate(widget, NULL);

      break;
    }
    case EVT_IM_COMMIT: {
      im_commit_event_t* evt = (im_commit_event_t*)e;
      edit_commit_str(widget, evt->text);
      widget_invalidate(widget, NULL);

      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      edit_on_key_up(widget, evt);
      widget_invalidate(widget, NULL);

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
      edit_dispatch_event(widget, EVT_VALUE_CHANGED);

      break;
    }
    case EVT_FOCUS: {
      if (widget->target == NULL) {
        input_method_request(input_method(), widget);
      }
      break;
    }
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;
      if (evt->dy > 0) {
        edit_dec(edit);
      } else if (evt->dy < 0) {
        edit_inc(edit);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t edit_set_text_limit(widget_t* widget, uint32_t min, uint32_t max) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.u.t.min = min;
  edit->limit.u.t.max = max;
  edit->limit.type = INPUT_TEXT;

  return RET_OK;
}

ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max, uint32_t step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.u.i.min = min;
  edit->limit.u.i.max = max;
  edit->limit.u.i.step = step;
  edit->limit.type = INPUT_INT;

  return RET_OK;
}

ret_t edit_set_float_limit(widget_t* widget, double min, double max, double step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.u.f.min = min;
  edit->limit.u.f.max = max;
  edit->limit.u.f.step = step;
  edit->limit.type = INPUT_FLOAT;

  return RET_OK;
}

ret_t edit_set_readonly(widget_t* widget, bool_t readonly) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->readonly = readonly;

  return RET_OK;
}

ret_t edit_set_auto_fix(widget_t* widget, bool_t auto_fix) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->auto_fix = auto_fix;

  return RET_OK;
}

ret_t edit_set_input_type(widget_t* widget, input_type_t type) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.type = type;
  if (type == INPUT_INT || type == INPUT_UINT) {
    edit->limit.u.i.step = 1;
  } else if (type == INPUT_FLOAT || type == INPUT_UFLOAT) {
    edit->limit.u.f.step = 1.0f;
  }

  return RET_OK;
}

ret_t edit_set_input_tips(widget_t* widget, const char* tips) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && tips != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(edit->tips);
  edit->tips = tk_strdup(tips);

  return RET_OK;
}

ret_t edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = edit->limit.type;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_int(v, edit->limit.u.i.min);
    } else if (input_type == INPUT_TEXT) {
      value_set_uint32(v, edit->limit.u.t.min);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_float(v, edit->limit.u.f.min);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_int(v, edit->limit.u.i.max);
    } else if (input_type == INPUT_TEXT) {
      value_set_uint32(v, edit->limit.u.t.max);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_float(v, edit->limit.u.f.max);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      value_set_float(v, edit->limit.u.f.step);
      return RET_OK;
    } else if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      value_set_float(v, edit->limit.u.i.step);
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
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    value_set_str(v, edit->tips);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = edit->limit.type;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->limit.u.i.min = value_int(v);
    } else if (input_type == INPUT_TEXT) {
      edit->limit.u.t.min = value_int(v);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->limit.u.f.min = value_float(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->limit.u.i.max = value_int(v);
    } else if (input_type == INPUT_TEXT) {
      edit->limit.u.t.max = value_int(v);
    } else if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->limit.u.f.max = value_float(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    if (input_type == INPUT_FLOAT || input_type == INPUT_UFLOAT) {
      edit->limit.u.f.step = value_float(v);
      return RET_OK;
    } else if (input_type == INPUT_INT || input_type == INPUT_UINT) {
      edit->limit.u.i.step = value_int(v);
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
    edit->limit.type = input_type;

    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_READONLY)) {
    edit->readonly = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_FIX)) {
    edit->auto_fix = value_bool(v);
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
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    edit_set_input_tips(widget, value_str(v));
    return RET_OK;
  }

  edit_update_status(widget);

  return RET_NOT_FOUND;
}

ret_t edit_set_password_visible(widget_t* widget, bool_t password_visible) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->password_visible = password_visible;

  return RET_OK;
}

static ret_t edit_add_float(edit_t* edit, double delta) {
  double v = 0;
  widget_t* widget = WIDGET(edit);
  wstr_t* text = &(widget->text);

  return_value_if_fail(wstr_to_float(text, &v) == RET_OK, RET_FAIL);

  v += delta;
  if (edit->limit.u.f.min < edit->limit.u.f.max) {
    if (v < edit->limit.u.f.min) {
      wstr_from_float(text, edit->limit.u.f.min);
    } else if (v > edit->limit.u.f.max) {
      wstr_from_float(text, edit->limit.u.f.max);
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
  widget_t* widget = WIDGET(edit);
  wstr_t* text = &(widget->text);

  return_value_if_fail(wstr_to_int(text, &v) == RET_OK, RET_FAIL);

  v += delta;
  if (edit->auto_fix && (edit->limit.u.i.min < edit->limit.u.i.max)) {
    if (v < edit->limit.u.i.min) {
      v = edit->limit.u.i.min;
    }

    if (v > edit->limit.u.i.max) {
      v = edit->limit.u.i.max;
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
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wstr_from_int(&(widget->text), value);

  return RET_OK;
}

ret_t edit_set_double(widget_t* widget, double value) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wstr_from_float(&(widget->text), value);

  return RET_OK;
}

ret_t edit_inc(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  wstr_t* text = &(widget->text);
  input_type_t input_type = edit->limit.type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      if (text->size == 0) {
        wstr_from_float(text, edit->limit.u.f.min);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, edit->limit.u.f.step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      if (text->size == 0) {
        wstr_from_int(text, edit->limit.u.i.min);
      }
      edit_add_int(edit, edit->limit.u.i.step);
      break;
    }
    default:
      break;
  }

  return widget_invalidate_force(widget);
}

ret_t edit_dec(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  wstr_t* text = &(widget->text);
  input_type_t input_type = edit->limit.type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      if (text->size == 0) {
        wstr_from_float(text, edit->limit.u.f.max);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, -edit->limit.u.f.step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      if (text->size == 0) {
        wstr_from_int(text, edit->limit.u.i.max);
      }
      edit_add_int(edit, -edit->limit.u.i.step);
      break;
    }
    default:
      break;
  }

  return widget_invalidate_force(widget);
}

ret_t edit_clear(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  widget->text.size = 0;

  return widget_invalidate_force(widget);
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

static ret_t edit_hook_button(void* ctx, void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* edit = WIDGET(ctx);

  if (widget->name && widget != edit) {
    const char* name = widget->name;
    if (tk_str_eq(name, "inc")) {
      widget_on(widget, EVT_CLICK, edit_on_inc, edit);
    } else if (tk_str_eq(name, "dec")) {
      widget_on(widget, EVT_CLICK, edit_on_dec, edit);
    } else if (tk_str_eq(name, "clear")) {
      widget_on(widget, EVT_CLICK, edit_on_clear, edit);
    }
  }

  return RET_OK;
}

static ret_t edit_destroy(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  if (edit->timer_id != TK_INVALID_ID) {
    timer_remove(edit->timer_id);
    edit->timer_id = TK_INVALID_ID;
  }

  if (edit->idle_id != TK_INVALID_ID) {
    idle_remove(edit->idle_id);
    edit->idle_id = TK_INVALID_ID;
  }
  TKMEM_FREE(edit->tips);

  return RET_OK;
}

static ret_t edit_hook_children_button(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  edit_t* edit = EDIT(widget);

  widget_foreach(widget, edit_hook_button, widget);
  edit->idle_id = TK_INVALID_ID;

  return RET_REMOVE;
}

const char* s_edit_properties[] = {WIDGET_PROP_MIN,
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
                                   WIDGET_PROP_PASSWORD_VISIBLE,
                                   NULL};
static const widget_vtable_t s_edit_vtable = {.size = sizeof(edit_t),
                                              .type = WIDGET_TYPE_EDIT,
                                              .clone_properties = s_edit_properties,
                                              .persistent_properties = s_edit_properties,
                                              .create = edit_create,
                                              .on_paint_self = edit_on_paint_self,
                                              .set_prop = edit_set_prop,
                                              .get_prop = edit_get_prop,
                                              .destroy = edit_destroy,
                                              .on_event = edit_on_event};

widget_t* edit_init(widget_t* parent, edit_t* edit, xy_t x, xy_t y, wh_t w, wh_t h,
                    const widget_vtable_t* vt) {
  widget_t* widget = WIDGET(edit);

  return_value_if_fail(edit != NULL, NULL);

  widget_init(widget, parent, vt, x, y, w, h);

  edit->left_margin = 2;
  edit->right_margin = 2;
  edit->top_margin = 2;
  edit->bottom_margin = 2;
  edit_set_text_limit(widget, 0, 1024);
  edit_update_status(widget);
  edit->timer_id = TK_INVALID_ID;
  edit->idle_id = idle_add(edit_hook_children_button, edit);

  return widget;
}

widget_t* edit_create_ex(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h,
                         const widget_vtable_t* vt) {
  edit_t* edit = TKMEM_ZALLOC(edit_t);
  return_value_if_fail(edit != NULL, NULL);

  return edit_init(parent, edit, x, y, w, h, vt);
}

widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return edit_create_ex(parent, x, y, w, h, &s_edit_vtable);
}

widget_t* edit_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_edit_vtable, NULL);

  return widget;
}
