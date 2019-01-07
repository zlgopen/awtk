/**
 * File:   style_mutable.c
 * Author: AWTK Develop Team
 * Brief:  mutable style
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
 * 2018-10-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/value.h"
#include "tkc/utils.h"
#include "base/widget.h"
#include "base/style_const.h"
#include "base/style_factory.h"
#include "designer_support/style_mutable.h"

typedef struct _style_item_t {
  const char* name;
  value_t value;

  struct _style_item_t* next;
} style_item_t;

static ret_t value_depp_copy(value_t* d, const value_t* s) {
  if (s->type == VALUE_TYPE_STRING) {
    if (d->type != VALUE_TYPE_STRING) {
      d->type = VALUE_TYPE_STRING;
      d->value.str = NULL;
    }
    d->value.str = tk_str_copy((char*)(d->value.str), s->value.str);

    return RET_OK;
  }

  if (d->type == VALUE_TYPE_STRING) {
    TKMEM_FREE(d->value.str);
    d->value.str = NULL;
  }
  *d = *s;

  return RET_OK;
}

static style_item_t* style_item_add(style_item_t* first, const char* name, const value_t* value) {
  style_item_t* iter = first;
  style_item_t* item = TKMEM_ZALLOC(style_item_t);
  return_value_if_fail(item != NULL, NULL);

  item->name = name;
  value_depp_copy(&(item->value), value);

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
      if (iter->name == name) {
        value_depp_copy(&(iter->value), value);
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
      if (iter->name == name) {
        *value = iter->value;
        return RET_OK;
      }
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

struct _widget_state_style_t {
  const char* state;
  style_item_t* items;

  struct _widget_state_style_t* next;
};

widget_state_style_t* widget_state_style_add(widget_state_style_t* first, const char* state) {
  widget_state_style_t* iter = first;
  widget_state_style_t* item = TKMEM_ZALLOC(widget_state_style_t);
  return_value_if_fail(item != NULL, NULL);

  item->state = state;
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
    if (iter->state == state) {
      return iter;
    }
    iter = iter->next;
  }

  return NULL;
}

const char* style_mutable_get_name(style_t* s) {
  style_mutable_t* style = (style_mutable_t*)s;

  return style->name;
}

ret_t style_mutable_set_name(style_t* s, const char* name) {
  style_mutable_t* style = (style_mutable_t*)s;
  return_value_if_fail(s != NULL && name != NULL, RET_BAD_PARAMS);

  style->name = tk_str_copy(style->name, name);

  return RET_OK;
}

static const char* style_mutable_get_widget_state(style_t* s) {
  style_mutable_t* style = (style_mutable_t*)s;

  return (const char*)widget_get_prop_str(style->widget, WIDGET_PROP_STATE_FOR_STYLE,
                                          WIDGET_STATE_NORMAL);
}

static ret_t style_mutable_notify_widget_state_changed(style_t* s, widget_t* widget) {
  style_mutable_t* style = (style_mutable_t*)s;

  style->widget = widget;
  return style_notify_widget_state_changed(style->default_style, widget);
}

bool_t style_mutable_is_valid(style_t* s) {
  style_mutable_t* style = (style_mutable_t*)s;
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  return current != NULL || style_is_valid(style->default_style);
}

static ret_t style_mutable_get_value(widget_state_style_t* witer, const char* name, value_t* v) {
  if (witer == NULL || witer->items == NULL) {
    return RET_NOT_FOUND;
  }

  return style_item_get(witer->items, name, v);
}

static int32_t style_mutable_get_int(style_t* s, const char* name, int32_t defval) {
  value_t v;
  style_mutable_t* style = (style_mutable_t*)s;
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && style_mutable_get_value(current, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return style_get_int(style->default_style, name, defval);
  }
}

static color_t style_mutable_get_color(style_t* s, const char* name, color_t defval) {
  value_t v;
  style_mutable_t* style = (style_mutable_t*)s;
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && style_mutable_get_value(current, name, &v) == RET_OK) {
    color_t c;
    c.color = value_uint32(&v);
    return c;
  } else {
    return style_get_color(style->default_style, name, defval);
  }
}

const char* style_mutable_get_str(style_t* s, const char* name, const char* defval) {
  value_t v;
  style_mutable_t* style = (style_mutable_t*)s;
  const char* state = style_mutable_get_widget_state(s);
  widget_state_style_t* current = widget_state_style_find(style->styles, state);

  if (current != NULL && style_mutable_get_value(current, name, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return style_get_str(style->default_style, name, defval);
  }
}

ret_t style_mutable_set_value(style_t* s, const char* state, const char* name, const value_t* v) {
  widget_state_style_t* witer = NULL;
  style_mutable_t* style = (style_mutable_t*)s;
  return_value_if_fail(s != NULL, RET_BAD_PARAMS);

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
  style_mutable_t* style = (style_mutable_t*)s;
  return_value_if_fail(s != NULL && on_style_item != NULL, RET_BAD_PARAMS);

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

static ret_t style_mutable_destroy(style_t* s) {
  style_mutable_t* style = (style_mutable_t*)s;
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(style->name);
  style_destroy(style->default_style);
  TKMEM_FREE(s);

  return RET_OK;
}

static const style_vtable_t style_mutable_vt = {
    .is_valid = style_mutable_is_valid,
    .notify_widget_state_changed = style_mutable_notify_widget_state_changed,
    .get_int = style_mutable_get_int,
    .get_str = style_mutable_get_str,
    .get_color = style_mutable_get_color,
    .destroy = style_mutable_destroy};

style_t* style_mutable_create(widget_t* widget) {
  style_mutable_t* style = TKMEM_ZALLOC(style_mutable_t);
  return_value_if_fail(style != NULL, NULL);

  style->style.vt = &style_mutable_vt;
  style->default_style = style_const_create(widget);

  return (style_t*)style;
}

static style_t* style_factory_create_style_mutable(style_factory_t* factory, widget_t* widget) {
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(factory != NULL && widget != NULL, NULL);

  if (win && win->vt->is_designing_window) {
    return style_mutable_create(widget);
  } else {
    return style_const_create(widget);
  }
}

static const style_factory_t s_style_factory = {.create_style = style_factory_create_style_mutable};

ret_t style_mutable_register(void) {
  style_factory_set((style_factory_t*)&s_style_factory);
  return RET_OK;
}
