/**
 * File:   style_mutable.c
 * Author: AWTK Develop Team
 * Brief:  mutable style
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
 * 2018-10-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/value.h"
#include "tkc/utils.h"
#include "base/widget.h"
#include "base/style_const.h"
#include "base/style_factory.h"
#include "base/style_mutable.h"

typedef struct _style_item_t {
  char name[TK_NAME_LEN + 1];
  value_t value;

  struct _style_item_t* next;
} style_item_t;

static ret_t style_item_set_value(style_item_t* item, const value_t* value) {
  const char* name = item->name;
  value_t* v = &(item->value);

  value_reset(v);
  if (value->type == VALUE_TYPE_STRING) {
    return style_normalize_value(name, value_str(value), v);
  } else {
    value_set_int(v, value_int(value));
    return RET_OK;
  }
}

static ret_t style_item_init(style_item_t* item, const char* name, const value_t* value) {
  tk_strncpy(item->name, name, TK_NAME_LEN);

  return style_item_set_value(item, value);
}

static style_item_t* style_item_add(style_item_t* first, const char* name, const value_t* value) {
  style_item_t* iter = first;
  style_item_t* item = TKMEM_ZALLOC(style_item_t);
  return_value_if_fail(item != NULL, NULL);

  style_item_init(item, name, value);

  if (first != NULL) {
    while (iter->next) {
      iter = iter->next;
    }
    iter->next = item;
  } else {
    first = item;
  }

  return first;
}

static ret_t style_item_set(style_item_t* first, const char* name, const value_t* value) {
  style_item_t* iter = first;

  if (first != NULL) {
    while (iter) {
      if (tk_str_eq(iter->name, name)) {
        style_item_set_value(iter, value);
        return RET_OK;
      }
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

static ret_t style_item_get(style_item_t* first, const char* name, value_t* value) {
  style_item_t* iter = first;
  return_value_if_fail(iter != NULL, RET_BAD_PARAMS);

  if (first != NULL) {
    while (iter) {
      if (tk_str_eq(iter->name, name)) {
        *value = iter->value;
        return RET_OK;
      }
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

struct _widget_state_style_t {
  char state[TK_NAME_LEN + 1];
  style_item_t* items;

  struct _widget_state_style_t* next;
};

widget_state_style_t* widget_state_style_add(widget_state_style_t* first, const char* state) {
  widget_state_style_t* iter = first;
  widget_state_style_t* item = TKMEM_ZALLOC(widget_state_style_t);
  return_value_if_fail(item != NULL, NULL);

  tk_strncpy(item->state, state, TK_NAME_LEN);
  if (first != NULL) {
    while (iter->next) {
      iter = iter->next;
    }

    iter->next = item;
  } else {
    first = item;
  }

  return first;
}

widget_state_style_t* widget_state_style_find(widget_state_style_t* first, const char* state) {
  widget_state_style_t* iter = first;

  while (iter != NULL) {
    if (tk_str_eq(iter->state, state)) {
      return iter;
    }
    iter = iter->next;
  }

  return NULL;
}

ret_t style_mutable_set_name(style_t* s, const char* name) {
  style_mutable_t* style = STYLE_MUTABLE(s);
  return_value_if_fail(style != NULL && name != NULL, RET_BAD_PARAMS);

  style->name = tk_str_copy(style->name, name);

  return RET_OK;
}

static const char* style_mutable_get_widget_state(style_t* s) {
  style_mutable_t* style = STYLE_MUTABLE(s);

  return (const char*)widget_get_prop_str(style->widget, WIDGET_PROP_STATE_FOR_STYLE,
                                          WIDGET_STATE_NORMAL);
}

static ret_t style_mutable_notify_widget_state_changed(style_t* s, widget_t* widget) {
  style_mutable_t* style = STYLE_MUTABLE(s);

  style->widget = widget;
  return style_notify_widget_state_changed(style->default_style, widget);
}

bool_t style_mutable_is_valid(style_t* s) {
  style_mutable_t* style = STYLE_MUTABLE(s);
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  return current != NULL || style_is_valid(style->default_style);
}

static ret_t widget_state_style_get_value(widget_state_style_t* witer, const char* name,
                                          value_t* v) {
  if (witer == NULL || witer->items == NULL) {
    return RET_NOT_FOUND;
  }

  return style_item_get(witer->items, name, v);
}

static int32_t style_mutable_get_int(style_t* s, const char* name, int32_t defval) {
  value_t v;
  style_mutable_t* style = STYLE_MUTABLE(s);
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && widget_state_style_get_value(current, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return style_get_int(style->default_style, name, defval);
  }
}

static color_t style_mutable_get_color(style_t* s, const char* name, color_t defval) {
  value_t v;
  style_mutable_t* style = STYLE_MUTABLE(s);
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && widget_state_style_get_value(current, name, &v) == RET_OK) {
    color_t c;
    c.color = value_uint32(&v);
    return c;
  } else {
    return style_get_color(style->default_style, name, defval);
  }
}

const char* style_mutable_get_str(style_t* s, const char* name, const char* defval) {
  value_t v;
  style_mutable_t* style = STYLE_MUTABLE(s);
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && widget_state_style_get_value(current, name, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return style_get_str(style->default_style, name, defval);
  }
}

ret_t style_mutable_get_value(style_t* s, const char* state, const char* name, value_t* v) {
  style_mutable_t* style = STYLE_MUTABLE(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  return widget_state_style_get_value(current, name, v);
}

ret_t style_mutable_set_value(style_t* s, const char* state, const char* name, const value_t* v) {
  widget_state_style_t* witer = NULL;
  style_mutable_t* style = STYLE_MUTABLE(s);
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);

  witer = widget_state_style_find(style->styles, state);
  if (witer == NULL) {
    style->styles = widget_state_style_add(style->styles, state);
  }
  witer = widget_state_style_find(style->styles, state);
  return_value_if_fail(witer != NULL, RET_OOM);

  if (style_item_set(witer->items, name, v) != RET_OK) {
    witer->items = style_item_add(witer->items, name, v);
  }

  return RET_OK;
}

ret_t style_mutable_set_int(style_t* s, const char* state, const char* name, uint32_t val) {
  value_t v;

  return style_mutable_set_value(s, state, name, value_set_int(&v, val));
}

ret_t style_mutable_set_color(style_t* s, const char* state, const char* name, color_t val) {
  value_t v;

  return style_mutable_set_value(s, state, name, value_set_uint32(&v, val.color));
}

ret_t style_mutable_set_str(style_t* s, const char* state, const char* name, const char* val) {
  value_t v;
  return_value_if_fail(s != NULL && val != NULL, RET_BAD_PARAMS);

  return style_mutable_set_value(s, state, name, value_set_str(&v, val));
}

ret_t style_mutable_foreach(style_t* s, tk_on_style_item_t on_style_item, void* ctx) {
  style_item_t* iter = NULL;
  widget_state_style_t* witer = NULL;
  style_mutable_t* style = STYLE_MUTABLE(s);
  return_value_if_fail(style != NULL && on_style_item != NULL, RET_BAD_PARAMS);

  witer = style->styles;
  while (witer != NULL) {
    iter = witer->items;
    while (iter != NULL) {
      on_style_item(ctx, witer->state, iter->name, &(iter->value));
      iter = iter->next;
    }
    witer = witer->next;
  }

  return RET_OK;
}

ret_t style_mutable_reset(style_t* s) {
  style_item_t* iter = NULL;
  widget_state_style_t* witer = NULL;
  style_mutable_t* style = STYLE_MUTABLE(s);
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);

  witer = style->styles;
  while (witer != NULL) {
    widget_state_style_t* wnext = witer->next;

    iter = witer->items;
    while (iter != NULL) {
      style_item_t* next = iter->next;

      value_reset(&(iter->value));
      TKMEM_FREE(iter);

      iter = next;
    }
    TKMEM_FREE(witer);

    witer = wnext;
  }
  style->styles = NULL;
  return RET_OK;
}

static ret_t style_mutable_destroy(style_t* s) {
  style_mutable_t* style = STYLE_MUTABLE(s);
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(style->name);
  style_mutable_reset(s);
  style_destroy(style->default_style);
  TKMEM_FREE(s);

  return RET_OK;
}

static const style_vtable_t style_mutable_vt = {
    .is_mutable = TRUE,
    .is_valid = style_mutable_is_valid,
    .notify_widget_state_changed = style_mutable_notify_widget_state_changed,
    .get_int = style_mutable_get_int,
    .get_str = style_mutable_get_str,
    .get_color = style_mutable_get_color,
    .set = style_mutable_set_value,
    .destroy = style_mutable_destroy};

style_t* style_mutable_create(widget_t* widget, style_t* default_style) {
  style_mutable_t* style = TKMEM_ZALLOC(style_mutable_t);
  return_value_if_fail(style != NULL, NULL);

  style->style.vt = &style_mutable_vt;
  if (default_style != NULL) {
    style->widget = widget;
    style->default_style = default_style;
  } else {
    style->default_style = style_const_create(widget);
  }

  return (style_t*)style;
}

static style_t* style_factory_create_style_mutable(style_factory_t* factory, widget_t* widget) {
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(factory != NULL && widget != NULL, NULL);

  if (win && win->vt->is_designing_window) {
    return style_mutable_create(widget, NULL);
  } else {
    return style_const_create(widget);
  }
}

static const style_factory_t s_style_factory = {.create_style = style_factory_create_style_mutable};

ret_t style_mutable_register(void) {
  style_factory_set((style_factory_t*)&s_style_factory);
  return RET_OK;
}

style_t* style_mutable_cast(style_t* s) {
  return_value_if_fail(s != NULL && s->vt == &style_mutable_vt, NULL);

  return s;
}

static ret_t visit_and_clone(void* ctx, const char* widget_state, const char* name,
                             const value_t* val) {
  style_set((style_t*)ctx, widget_state, name, val);

  return RET_OK;
}

ret_t style_mutable_copy(style_t* s, style_t* other) {
  return_value_if_fail(STYLE_MUTABLE(s) != NULL, RET_BAD_PARAMS);
  return_value_if_fail(STYLE_MUTABLE(other) != NULL, RET_BAD_PARAMS);

  style_mutable_reset(s);
  style_mutable_foreach(other, visit_and_clone, s);

  return RET_OK;
}
