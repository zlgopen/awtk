/**
 * File:   edit.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  edit
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
#include "base/edit.h"
#include "base/keys.h"
#include "base/enums.h"
#include "base/events.h"

static ret_t edit_on_paint_self(widget_t* widget, canvas_t* c) {
  edit_t* edit = EDIT(widget);
  wstr_t* str = &(widget->text);
  if (str->size == 0) {
    str = &(edit->tips);
    widget_set_state(widget, WIDGET_STATE_EMPTY);
  }

  return widget_paint_helper(widget, c, NULL, str);
}

static ret_t edit_on_key_down(widget_t* widget, key_event_t* e) {
  uint32_t key = e->key;
  edit_t* edit = EDIT(widget);
  widget_state_t state = WIDGET_STATE_FOCUSED;
  input_type_t input_type = edit->limit.type;

  if (isprint(key)) {
    switch (input_type) {
      case INPUT_INT:
      case INPUT_FLOAT: {
        if (key >= FKEY_0 && key <= FKEY_9) {
          wstr_push(&(widget->text), key);
        } else if (widget->text.size == 0 && (key == '-' || key == '+')) {
          wstr_push(&(widget->text), key);
        } else if (input_type == INPUT_FLOAT && key == '.') {
          if (wcschr(widget->text.str, '.') == NULL) {
            wstr_push(&(widget->text), key);
          }
        }

        break;
      }
      default: {
        if (widget->text.size < edit->limit.t.max) {
          wstr_push(&(widget->text), key);
        }
      }
    }
  } else if (key == FKEY_BACKSPACE) {
    if (widget->text.size > 0) {
      wstr_pop(&(widget->text));
    }
  }

  if (widget->text.size == 0) {
    state = WIDGET_STATE_EMPTY;
  } else if (input_type == INPUT_TEXT && edit->limit.t.min > widget->text.size) {
    state = WIDGET_STATE_ERROR;
  }

  widget_invalidate(widget, NULL);
  widget_set_state(widget, state);

  return RET_OK;
}

static ret_t edit_on_key_up(widget_t* widget, key_event_t* e) { return RET_OK; }

static ret_t edit_on_blur(edit_t* edit) {
  widget_set_state(WIDGETP(edit), WIDGET_STATE_NORMAL);
  return RET_OK;
}

static ret_t edit_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_set_state(widget, WIDGET_STATE_FOCUSED);
      break;
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      edit_on_key_down(widget, evt);
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      edit_on_key_up(widget, evt);
      break;
    }
    case EVT_BLUR: {
      edit_on_blur(EDIT(widget));
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

  edit->limit.t.min = min;
  edit->limit.t.max = max;
  edit->limit.type = INPUT_TEXT;

  return RET_OK;
}

ret_t edit_set_int_limit(widget_t* widget, int32_t min, int32_t max) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.i.min = min;
  edit->limit.i.max = max;
  edit->limit.type = INPUT_INT;

  return RET_OK;
}

ret_t edit_set_float_limit(widget_t* widget, float min, float max, float step) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.f.min = min;
  edit->limit.f.max = max;
  edit->limit.f.step = step;
  edit->limit.type = INPUT_FLOAT;

  return RET_OK;
}

ret_t edit_set_readonly(widget_t* widget, bool_t readonly) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->readonly = readonly;

  return RET_OK;
}

ret_t edit_set_input_type(widget_t* widget, input_type_t type) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  edit->limit.type = type;

  return RET_OK;
}

ret_t edit_set_input_tips(widget_t* widget, const wchar_t* tips) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && tips != NULL, RET_BAD_PARAMS);

  return wstr_set(&(edit->tips), tips);
}

static ret_t edit_get_prop(widget_t* widget, const char* name, value_t* v) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "min") == 0) {
    if (edit->limit.type == INPUT_INT) {
      value_set_int(v, edit->limit.i.min);
    } else if (edit->limit.type == INPUT_TEXT) {
      value_set_uint32(v, edit->limit.t.min);
    } else if (edit->limit.type == INPUT_FLOAT) {
      value_set_float(v, edit->limit.f.min);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (strcmp(name, "max") == 0) {
    if (edit->limit.type == INPUT_INT) {
      value_set_int(v, edit->limit.i.max);
    } else if (edit->limit.type == INPUT_TEXT) {
      value_set_uint32(v, edit->limit.t.max);
    } else if (edit->limit.type == INPUT_FLOAT) {
      value_set_float(v, edit->limit.f.max);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (strcmp(name, "step") == 0) {
    if (edit->limit.type == INPUT_FLOAT) {
      value_set_float(v, edit->limit.f.step);
      return RET_OK;
    } else {
      return RET_NOT_FOUND;
    }
  } else if (strcmp(name, "input_type") == 0) {
    value_set_uint32(v, edit->limit.type);
    return RET_OK;
  } else if (strcmp(name, "readonly") == 0) {
    value_set_bool(v, edit->readonly);
    return RET_OK;
  } else if (strcmp(name, "tips") == 0) {
    value_set_wstr(v, edit->tips.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t edit_set_prop(widget_t* widget, const char* name, const value_t* v) {
  edit_t* edit = EDIT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "min") == 0) {
    if (edit->limit.type == INPUT_INT) {
      edit->limit.i.min = value_int(v);
    } else if (edit->limit.type == INPUT_TEXT) {
      edit->limit.t.min = value_uint32(v);
    } else if (edit->limit.type == INPUT_FLOAT) {
      edit->limit.f.min = value_float(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (strcmp(name, "max") == 0) {
    if (edit->limit.type == INPUT_INT) {
      edit->limit.i.max = value_int(v);
    } else if (edit->limit.type == INPUT_TEXT) {
      edit->limit.t.max = value_uint32(v);
    } else if (edit->limit.type == INPUT_FLOAT) {
      edit->limit.f.max = value_float(v);
    } else {
      return RET_NOT_FOUND;
    }
    return RET_OK;
  } else if (strcmp(name, "step") == 0) {
    if (edit->limit.type == INPUT_FLOAT) {
      edit->limit.f.step = value_float(v);
      return RET_OK;
    } else {
      return RET_NOT_FOUND;
    }
  } else if (strcmp(name, "input_type") == 0) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = input_type_find(value_str(v));
      if (kv != NULL) {
        edit->limit.type = kv->value;
      }
    } else {
      edit->limit.type = value_int(v);
    }
    return RET_OK;
  } else if (strcmp(name, "readonly") == 0) {
    edit->readonly = value_bool(v);
    return RET_OK;
  } else if (strcmp(name, "tips") == 0) {
    if (v->type == VALUE_TYPE_STRING) {
      wstr_set_utf8(&(edit->tips), value_str(v));
      return RET_OK;
    } else if (v->type == VALUE_TYPE_WSTRING) {
      wstr_set(&(edit->tips), value_wstr(v));
      return RET_OK;
    }
    return RET_BAD_PARAMS;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_edit_vtable = {.on_paint_self = edit_on_paint_self,
                                              .set_prop = edit_set_prop,
                                              .get_prop = edit_get_prop,
                                              .on_event = edit_on_event};

widget_t* edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  edit_t* edit = MEM_ZALLOC(edit_t);
  return_value_if_fail(edit != NULL, NULL);

  widget = WIDGETP(edit);
  widget_init(widget, parent, WIDGET_EDIT);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_edit_vtable;
  edit_set_text_limit(widget, 0, 1204);

  return widget;
}
