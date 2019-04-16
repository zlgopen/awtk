/**
 * File:   edit.h
 * Author: AWTK Develop Team
 * Brief:  edit
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "widgets/edit.h"
#include "base/keys.h"
#include "base/idle.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/events.h"
#include "base/window_manager.h"

static ret_t edit_update_status(widget_t* widget);

static ret_t edit_dispatch_event(widget_t* widget, event_type_t type) {
  event_t evt = event_init(type, widget);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t edit_update_caret(const timer_info_t* timer) {
  rect_t r;
  edit_t* edit = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(timer != NULL, RET_REMOVE);

  edit = EDIT(timer->ctx);
  widget = WIDGET(timer->ctx);
  return_value_if_fail(edit != NULL && widget != NULL, RET_REMOVE);

  r = rect_init(edit->caret_x, 0, 1, widget->h);
  edit->caret_visible = !edit->caret_visible;
  widget_invalidate_force(widget, &r);

  if (widget->focused) {
    return RET_REPEAT;
  } else {
    edit->timer_id = TK_INVALID_ID;
    return RET_REMOVE;
  }
}

#define INVISIBLE_CHAR '*'
#define TEMP_STR_LEN 31

static ret_t edit_get_display_text(widget_t* widget, canvas_t* c, wstr_t* text, wchar_t* temp_str) {
  int32_t i = 0;
  edit_t* edit = EDIT(widget);
  wstr_t* str = &(widget->text);
  bool_t invisible = str->size && (edit->limit.type == INPUT_PASSWORD && !(edit->password_visible));

  if (!str->size && !widget->focused) {
    if (edit->tips != NULL) {
      utf8_to_utf16(edit->tips, temp_str, TEMP_STR_LEN);
    }
  }

  if (str->size > 0) {
    text->str = str->str;
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

static ret_t edit_draw_text(widget_t* widget, canvas_t* c, wstr_t* text, rect_t* r) {
  xy_t y = 0;
  int32_t min_p = 0;
  int32_t max_p = 0;
  int32_t sel_w = 0;
  int32_t text_w = 0;
  int32_t pos_w = 0;
  int32_t baseline = 0;
  int32_t font_size = 0;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  y = 0;
  text_w = 0;
  pos_w = 0;
  baseline = 0;
  font_size = 0;
  min_p = edit->cursor_pos < edit->cursor_pre ? edit->cursor_pos : edit->cursor_pre;
  max_p = edit->cursor_pos > edit->cursor_pre ? edit->cursor_pos : edit->cursor_pre;
  sel_w = 0;

  font_size = c->font_size;
  baseline = font_get_baseline(c->font, font_size);
  pos_w = canvas_measure_text(c, text->str, edit->cursor_pos);
  text_w =
      pos_w + canvas_measure_text(c, text->str + edit->cursor_pos, text->size - edit->cursor_pos);
  sel_w = canvas_measure_text(c, text->str + min_p, max_p - min_p);

  switch (c->text_align_v) {
    case ALIGN_V_TOP:
      y = r->y;
      break;
    case ALIGN_V_BOTTOM:
      y = r->y + (r->h - baseline);
      break;
    default:
      y = r->y + ((r->h - baseline) >> 1);
      break;
  }

  if (text_w <= r->w) {
    switch (c->text_align_h) {
      case ALIGN_H_LEFT:
        edit->offset_x = r->x;
        break;
      case ALIGN_H_RIGHT:
        edit->offset_x = r->x + (r->w - text_w);
        break;
      default:
        edit->offset_x = r->x + ((r->w - text_w) >> 1);
        break;
    }

    edit->caret_x = edit->offset_x + pos_w;
  } else {
    edit->caret_x = edit->offset_x + pos_w;
    if (edit->caret_x < r->x) {
      edit->offset_x += r->x - edit->caret_x;
      edit->caret_x = r->x;
    } else if (edit->caret_x > r->x + r->w - 1) {
      edit->offset_x -= edit->caret_x - r->x - r->w + 1;
      edit->caret_x = r->x + r->w - 1;
    }
  }

  {
    rect_t tmp;
    canvas_get_clip_rect(c, &tmp);
    canvas_set_clip_rect_ex(c, r, TRUE);
    canvas_draw_text(c, text->str, text->size, edit->offset_x, y);
    if (sel_w > 0 && widget->focused && !edit->readonly) {
      xy_t sel_x = edit->cursor_pos < edit->cursor_pre ? edit->caret_x : edit->caret_x - sel_w++;
      canvas_set_fill_color(c, color_init(0, 0, 0, 0xff));
      canvas_set_text_color(c, color_init(255, 255, 255, 0xff));
      canvas_fill_rect(c, sel_x, r->y, sel_w, r->h);
      canvas_draw_text(c, text->str + min_p, max_p - min_p, sel_x, y);
    }
    canvas_set_clip_rect(c, &tmp);
  }
  return RET_OK;
}

ret_t edit_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  wh_t w = 0;
  wh_t h = 0;
  wstr_t text;
  style_t* style = NULL;
  uint8_t left_margin = 0;
  uint8_t right_margin = 0;
  uint8_t top_margin = 0;
  uint8_t bottom_margin = 0;
  edit_t* edit = EDIT(widget);
  align_h_t align_h = (align_h_t)0;
  wchar_t temp_str[TEMP_STR_LEN + 1];
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  left_margin = edit->left_margin;
  right_margin = edit->right_margin;
  top_margin = edit->top_margin;
  bottom_margin = edit->bottom_margin;
  w = widget->w - left_margin - right_margin;
  h = widget->h - top_margin - bottom_margin;
  align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);

  memset(temp_str, 0x00, sizeof(temp_str));
  return_value_if_fail(widget_prepare_text_style(widget, c) == RET_OK, RET_FAIL);
  return_value_if_fail(edit_get_display_text(widget, c, &text, temp_str) == RET_OK, RET_FAIL);

  r = rect_init(left_margin, top_margin, w, h);
  if (align_h == ALIGN_H_RIGHT) {
    r.w -= 3;
  }

  edit_draw_text(widget, c, &text, &r);

  if (widget->focused && !edit->readonly && edit->caret_visible) {
    color_t black = edit->cursor_pos == edit->cursor_pre ? color_init(0, 0, 0, 0xff)
                                                         : color_init(255, 255, 255, 0xff);
    color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);
    canvas_set_stroke_color(c, fg);

    canvas_draw_vline(c, edit->caret_x, top_margin, h);
  }

  return RET_OK;
}

static ret_t edit_set_cursor_pos(widget_t* widget, int32_t pre, int32_t pos) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (pre <= 0) {
    edit->cursor_pre = 0;
  } else {
    edit->cursor_pre = pre < widget->text.size ? pre : widget->text.size;
  }

  if (pos <= 0) {
    edit->cursor_pos = 0;
  } else {
    edit->cursor_pos = pos < widget->text.size ? pos : widget->text.size;
  }
  widget_invalidate(widget, NULL);
  return RET_OK;
}

static int32_t edit_calcu_pos(widget_t* widget, xy_t posx) {
  xy_t x = 0;
  int32_t pos = 0;
  canvas_t* c = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, 0);
  return_value_if_fail(widget_prepare_text_style(widget, c) == RET_OK, 0);

  x = edit->offset_x;
  c = WINDOW_MANAGER(window_manager())->canvas;

  for (pos = 0; pos < widget->text.size; ++pos) {
    int32_t w = canvas_measure_text(c, widget->text.str + pos, 1);
    if (posx < x + w) {
      if (posx > x + w / 2) {
        ++pos;
        x += w;
      }
      break;
    }
    x += w;
  }

  if (posx < 0 && x > 0) {
    --pos;
  } else if (posx > widget->w && x < widget->w) {
    ++pos;
  }

  return pos;
}

static ret_t edit_remove_select_str(widget_t* widget) {
  int32_t min_p = 0;
  int32_t max_p = 0;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit->cursor_pos == edit->cursor_pre) {
    return RET_FAIL;
  }

  if (edit->cursor_pos < edit->cursor_pre) {
    min_p = edit->cursor_pos;
    max_p = edit->cursor_pre;
  } else {
    min_p = edit->cursor_pre;
    max_p = edit->cursor_pos;
  }
  wstr_remove(&(widget->text), min_p, max_p - min_p);
  edit_set_cursor_pos(widget, min_p, min_p);
  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

  return RET_OK;
}

static ret_t edit_delete_prev_char(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit_remove_select_str(widget) == RET_OK) {
    return RET_OK;
  }

  if (edit->cursor_pos > 0) {
    wstr_remove(&(widget->text), edit->cursor_pos - 1, 1);
    edit_set_cursor_pos(widget, edit->cursor_pos - 1, edit->cursor_pos - 1);
    edit_dispatch_event(widget, EVT_VALUE_CHANGING);
  }

  return RET_OK;
}

static ret_t edit_delete_next_char(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit_remove_select_str(widget) == RET_OK) {
    return RET_OK;
  }

  if (edit->cursor_pos < widget->text.size) {
    wstr_remove(&(widget->text), edit->cursor_pos, 1);
    edit_dispatch_event(widget, EVT_VALUE_CHANGING);
  }

  return RET_OK;
}

static ret_t edit_input_char(widget_t* widget, wchar_t c) {
  wstr_t* text = NULL;
  ret_t ret = RET_FAIL;
  edit_t* edit = EDIT(widget);
  input_type_t input_type = (input_type_t)0;

  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->limit.type;

  edit_remove_select_str(widget);

  switch (input_type) {
    case INPUT_INT:
    case INPUT_UINT: {
      if (text->size >= TK_NUM_MAX_LEN) {
        return RET_FAIL;
      }

      if (c >= '0' && c <= '9') {
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_INT)) {
        if (edit->cursor_pos == 0) {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
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
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        break;
      } else if (c == '+' || (c == '-' && input_type == INPUT_FLOAT)) {
        if (edit->cursor_pos == 0) {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        }
        break;
      } else if (c == '.' || c == 'e') {
        if (edit->cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        }
      }
      break;
    }
    case INPUT_EMAIL: {
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' ||
          c == '.' || c == '_') {
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
      } else if (c == '@') {
        if (edit->cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        }
      }
      break;
    }
    case INPUT_HEX: {
      if (text->size > 10) {
        break;
      }
      if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
      } else if (c == 'X' || c == 'x') {
        if (edit->cursor_pos == 1 && text->str[0] == '0') {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        }
      }
      break;
    }
    case INPUT_PHONE: {
      if (c >= '0' && c <= '9') {
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        break;
      } else if (c == '-') {
        if (edit->cursor_pos > 0 && wcs_chr(text->str, c) == NULL) {
          ret = wstr_insert(text, edit->cursor_pos, &c, 1);
        }
      }
      break;
    }
    default: {
      if (widget->text.size < edit->limit.u.t.max) {
        ret = wstr_insert(text, edit->cursor_pos, &c, 1);
      }
    }
  }

  edit_dispatch_event(widget, EVT_VALUE_CHANGING);

  if (ret == RET_OK) {
    edit_set_cursor_pos(widget, edit->cursor_pos + 1, edit->cursor_pos + 1);
  }
  return ret;
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

static bool_t edit_is_number(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  input_type = edit->limit.type;

  return input_type == INPUT_UINT || input_type == INPUT_INT || input_type == INPUT_FLOAT ||
         input_type == INPUT_UFLOAT || input_type == INPUT_HEX;
}

static ret_t edit_on_key_down(widget_t* widget, key_event_t* e) {
  uint32_t key = 0;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL && e != NULL, RET_BAD_PARAMS);

  key = e->key;
  if (key == TK_KEY_BACKSPACE) {
    return edit_delete_prev_char(widget);
  } else if (key == TK_KEY_DELETE) {
    return edit_delete_next_char(widget);
  } else if (key == TK_KEY_LEFT || key == TK_KEY_RIGHT) {
    if (key == TK_KEY_LEFT) {
      return edit_set_cursor_pos(widget, edit->cursor_pos - 1, edit->cursor_pos - 1);
    } else {
      return edit_set_cursor_pos(widget, edit->cursor_pos + 1, edit->cursor_pos + 1);
    }
  } else if (key == TK_KEY_TAB) {
    widget_focus_next(widget);
    return RET_OK;
  } else if (key == TK_KEY_DOWN) {
    if (!edit_is_number(widget)) {
      widget_focus_next(widget);
    } else {
      edit_dec(edit);
    }
    return RET_OK;
  } else if (key == TK_KEY_UP) {
    if (!edit_is_number(widget)) {
      widget_focus_prev(widget);
    } else {
      edit_inc(edit);
    }
    return RET_OK;
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
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, FALSE);

  text = &(widget->text);

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

static ret_t edit_auto_fix(widget_t* widget) {
  bool_t fix = FALSE;
  wstr_t* text = NULL;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);

  switch (edit->limit.type) {
    case INPUT_TEXT: {
      uint32_t size = text->size;
      uint32_t max = edit->limit.u.t.max;

      if (size > max) {
        text->size = max;
        fix = TRUE;
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
        fix = TRUE;
      }

      if (v > max) {
        v = max;
        fix = TRUE;
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
        fix = TRUE;
      }

      if (v > max) {
        v = max;
        fix = TRUE;
      }
      wstr_from_float(text, v);
      wstr_trim_float_zero(text);
      break;
    }
    default:
      break;
  }

  if (fix) {
    edit_set_cursor_pos(widget, 0, 0x0fffffff);
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

static ret_t edit_request_input_method_on_window_open(void* ctx, event_t* e) {
  input_method_request(input_method(), WIDGET(ctx));

  return RET_REMOVE;
}

static ret_t edit_request_input_method(widget_t* widget) {
  if (widget_is_window_opened(widget)) {
    input_method_request(input_method(), widget);
  } else {
    widget_t* win = widget_get_window(widget);
    if (win != NULL) {
      widget_on(win, EVT_WINDOW_OPEN, edit_request_input_method_on_window_open, widget);
    }
  }

  return RET_OK;
}

ret_t edit_on_event(widget_t* widget, event_t* e) {
  uint32_t type = e->type;
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  if (edit->readonly) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget_find_target(widget, evt.x, evt.y) == NULL) {
        point_t p = {evt.x, evt.y};
        uint32_t pos = 0;
        widget_to_local(widget, &p);
        pos = edit_calcu_pos(widget, p.x);
        edit_set_cursor_pos(widget, pos, pos);
        widget_grab(widget->parent, widget);
      }

      if (widget->target == NULL) {
        input_method_request(input_method(), widget);
      }
      edit_update_status(widget);

      break;
    }
    case EVT_POINTER_MOVE: {
      if (widget->parent && widget->parent->grab_widget == widget) {
        pointer_event_t evt = *(pointer_event_t*)e;
        point_t p = {evt.x, evt.y};
        uint32_t pos = 0;
        widget_to_local(widget, &p);
        pos = edit_calcu_pos(widget, p.x);
        edit_set_cursor_pos(widget, edit->cursor_pre, pos);
      }
      break;
    }
    case EVT_POINTER_UP: {
      widget_ungrab(widget->parent, widget);
      break;
    }
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
      if (edit->timer_id == TK_INVALID_ID) {
        edit->timer_id = timer_add(edit_update_caret, widget, 600);
      }

      if (widget->target == NULL) {
        edit_request_input_method(widget);
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
      break;
    }
    case EVT_PROP_CHANGED: {
      prop_change_event_t* evt = (prop_change_event_t*)e;
      if (tk_str_eq(evt->name, WIDGET_PROP_TEXT) || tk_str_eq(evt->name, WIDGET_PROP_VALUE)) {
        edit_update_status(widget);
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
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->limit.u.t.min = min;
  edit->limit.u.t.max = max;
  edit->limit.type = INPUT_TEXT;

  return RET_OK;
}

ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max, uint32_t step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->limit.u.i.min = min;
  edit->limit.u.i.max = max;
  edit->limit.u.i.step = step;
  edit->limit.type = INPUT_INT;

  return RET_OK;
}

ret_t edit_set_float_limit(widget_t* widget, double min, double max, double step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->limit.u.f.min = min;
  edit->limit.u.f.max = max;
  edit->limit.u.f.step = step;
  edit->limit.type = INPUT_FLOAT;

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

ret_t edit_set_input_type(widget_t* widget, input_type_t type) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

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
  return_value_if_fail(edit != NULL && tips != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(edit->tips);
  edit->tips = tk_strdup(tips);

  return RET_OK;
}

ret_t edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->limit.type;
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
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUS)) {
    value_set_bool(v, edit->focus);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    value_set_str(v, edit->tips);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    value_set_bool(v, !(edit->readonly));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_wstr(v, widget->text.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_t* edit = EDIT(widget);
  input_type_t input_type = INPUT_TEXT;
  return_value_if_fail(edit != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  input_type = edit->limit.type;
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
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUS)) {
    edit_set_focus(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TIPS)) {
    edit_set_input_tips(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    edit->offset_x = 0;
    edit_set_cursor_pos(widget, 0x0fffffff, 0x0fffffff);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    edit->offset_x = 0;
    wstr_from_value(&(widget->text), v);
    edit_set_cursor_pos(widget, 0x0fffffff, 0x0fffffff);
    return RET_OK;
  }

  edit_update_status(widget);

  return RET_NOT_FOUND;
}

ret_t edit_set_password_visible(widget_t* widget, bool_t password_visible) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->password_visible = password_visible;

  return RET_OK;
}

ret_t edit_set_focus(widget_t* widget, bool_t focus) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_BAD_PARAMS);

  edit->focus = focus;
  widget->focused = focus;

  if (focus) {
    event_t e = event_init(EVT_FOCUS, widget);
    widget_set_as_key_target(widget);
    widget_dispatch(widget, &e);
  } else {
    event_t e = event_init(EVT_BLUR, widget);
    widget_dispatch(widget, &e);
  }
  edit_update_status(widget);

  return RET_OK;
}

static ret_t edit_add_float(edit_t* edit, double delta) {
  double v = 0;
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
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
  edit_set_cursor_pos(widget, 0, 0x0fffffff);

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
  edit_set_cursor_pos(widget, 0, 0x0fffffff);

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
  edit_set_cursor_pos(widget, 0x0fffffff, 0x0fffffff);
  return RET_OK;
}

ret_t edit_set_double(widget_t* widget, double value) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wstr_from_float(&(widget->text), value);
  edit_set_cursor_pos(widget, 0x0fffffff, 0x0fffffff);
  return RET_OK;
}

ret_t edit_inc(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->limit.type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->limit.u.f.step ? edit->limit.u.f.step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->limit.u.f.min);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->limit.u.i.step ? edit->limit.u.i.step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->limit.u.i.min);
      }
      edit_add_int(edit, step);
      break;
    }
    default:
      break;
  }

  return widget_invalidate_force(widget, NULL);
}

ret_t edit_dec(edit_t* edit) {
  wstr_t* text = NULL;
  widget_t* widget = WIDGET(edit);
  input_type_t input_type = (input_type_t)0;
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  text = &(widget->text);
  input_type = edit->limit.type;

  switch (input_type) {
    case INPUT_FLOAT:
    case INPUT_UFLOAT: {
      float_t step = edit->limit.u.f.step ? edit->limit.u.f.step : 0.1;
      if (text->size == 0) {
        wstr_from_float(text, edit->limit.u.f.max);
        wstr_trim_float_zero(text);
      }
      edit_add_float(edit, -step);
      break;
    }
    case INPUT_INT:
    case INPUT_UINT: {
      int32_t step = edit->limit.u.i.step ? edit->limit.u.i.step : 1;
      if (text->size == 0) {
        wstr_from_int(text, edit->limit.u.i.max);
      }
      edit_add_int(edit, -step);
      break;
    }
    default:
      break;
  }

  return widget_invalidate_force(widget, NULL);
}

ret_t edit_clear(edit_t* edit) {
  widget_t* widget = WIDGET(edit);
  return_value_if_fail(widget != NULL && edit != NULL, RET_BAD_PARAMS);

  widget->text.size = 0;
  edit->selected_start = 0;
  edit->selected_end = 0;
  edit->visible_start = 0;
  edit->visible_end = 0;
  edit_set_cursor_pos(widget, 0, 0);

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
  widget_t* widget = WIDGET(e->target);
  return_value_if_fail(edit != NULL && widget != NULL, RET_BAD_PARAMS);

  edit->password_visible = widget_get_prop_bool(widget, WIDGET_PROP_VALUE, edit->password_visible);

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

static ret_t edit_on_destroy(widget_t* widget) {
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
                                   WIDGET_PROP_FOCUS,
                                   WIDGET_PROP_PASSWORD_VISIBLE,
                                   NULL};
TK_DECL_VTABLE(edit) = {.size = sizeof(edit_t),
                        .type = WIDGET_TYPE_EDIT,
                        .focusable = TRUE,
                        .inputable = TRUE,
                        .clone_properties = s_edit_properties,
                        .persistent_properties = s_edit_properties,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = edit_create,
                        .on_paint_self = edit_on_paint_self,
                        .set_prop = edit_set_prop,
                        .get_prop = edit_get_prop,
                        .on_destroy = edit_on_destroy,
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
  edit_set_text_limit(widget, 0, 1024);
  edit_update_status(widget);
  edit->timer_id = TK_INVALID_ID;
  edit->idle_id = idle_add(edit_hook_children_button, edit);

  return widget;
}

widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return edit_create_ex(parent, TK_REF_VTABLE(edit), x, y, w, h);
}

widget_t* edit_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, edit), NULL);

  return widget;
}
