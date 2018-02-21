/**
 * File:   widget.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic class of all widget
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"
#include "base/mem.h"
#include "base/widget_vtable.h"

ret_t widget_move(widget_t* widget, xy_t x, xy_t y) {
  event_t e = {EVT_MOVE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->x = x;
  widget->y = y;

  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_resize(widget_t* widget, wh_t w, wh_t h) {
  event_t e = {EVT_RESIZE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->w = w;
  widget->h = h;

  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h) {
  event_t e = {EVT_MOVE_RESIZE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->x = x;
  widget->y = y;
  widget->w = w;
  widget->h = h;

  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_set_value(widget_t* widget, uint32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, "value", value_set_uint32(&v, value));
}

ret_t widget_set_text(widget_t* widget, const wchar_t* text) {
  value_t v;
  return_value_if_fail(widget != NULL && text != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, "text", value_set_wstr(&v, text));
}

uint32_t widget_get_value(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, 0);

  return widget_get_prop(widget, "value", &v) == RET_OK ? value_int(v) : 0;
}

const wchar_t* widget_get_text(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, NULL);

  return widget_get_prop(widget, "value", &v) == RET_OK ? value_wstr(v) : NULL;
}

ret_t widget_set_name(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);
  strncpy(widget->name, name, NAME_LEN);

  return RET_OK;
}

ret_t widget_set_enable(widget_t* widget, bool_t enable) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->enable != enable) {
    widget->enable = enable;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_selected(widget_t* widget, bool_t selected) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->selected != selected) {
    widget->selected = selected;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_state(widget_t* widget, widget_state_t state) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->state != state) {
    widget->state = state;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_remove_children(widget_t* widget) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_destroy(iter);
    }
    widget->children->size = 0;
  }

  return RET_OK;
}

ret_t widget_add_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  child->parent = widget;
  if (widget->children == NULL) {
    widget->children = array_create(4);
  }

  return array_push(widget->children, child) ? RET_OK : RET_FAIL;
}

ret_t widget_remove_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (widget->target == child) {
    widget->target = NULL;
  }

  return array_remove(widget->children, NULL, child) ? RET_OK : RET_NOT_FOUND;
}

widget_t* widget_lookup_child(widget_t* widget, const char* name) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (strcmp(iter->name, name) == 0) {
        return iter;
      }
    }
  }

  return NULL;
}

widget_t* widget_lookup(widget_t* widget, const char* name) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (strcmp(iter->name, name) == 0) {
        return iter;
      } else {
        iter = widget_lookup(iter, name);
        if (iter != NULL) {
          return iter;
        }
      }
    }
  }

  return NULL;
}

ret_t widget_set_visible(widget_t* widget, bool_t visible) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->visible != visible) {
    widget->visible = visible;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_visible_recursive(widget_t* widget, bool_t visible) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->visible = visible;
  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_set_visible_recursive(iter, visible);
    }
  }

  return RET_OK;
}

widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y) {
  widget_t* ret = NULL;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->vt && widget->vt->find_target) {
    ret = widget->vt->find_target(widget, x, y);
  } else {
    ret = widget_find_target_default(widget, x, y);
  }

  return ret;
}

ret_t widget_dispatch(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->vt && widget->vt->on_event) {
    ret = widget->vt->on_event(widget, e);
  } else {
    ret = widget_on_event_default(widget, e);
  }

  if (widget->emitter != NULL) {
    ret = emitter_dispatch(widget->emitter, e);
  }

  return ret;
}

ret_t widget_on(widget_t* widget, event_type_t type, event_handler on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  if (widget->emitter == NULL) {
    widget->emitter = emitter_create();
  }

  return emitter_on(widget->emitter, type, on_event, ctx);
}

ret_t widget_off(widget_t* widget, event_type_t type, event_handler on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->emitter != NULL, RET_BAD_PARAMS);

  return emitter_off(widget->emitter, type, on_event, ctx);
}

ret_t widget_paint(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_translate(c, widget->x, widget->y);
  widget_on_paint_self(widget, c);
  widget_on_paint_children(widget, c);
  canvas_untranslate(c, widget->x, widget->y);

  return RET_OK;
}

ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  event_t e = {EVT_PROP_CHANGED, widget};
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (name[1] == '\0') {
    switch (name[0]) {
      case 'x':
        widget->x = (wh_t)value_int(*v);
        break;
      case 'y':
        widget->y = (wh_t)value_int(*v);
        break;
      case 'w':
        widget->w = (wh_t)value_int(*v);
        break;
      case 'h':
        widget->h = (wh_t)value_int(*v);
        break;
      case 'v': /*visible*/
        widget->visible = !!value_int(*v);
        break;
      case 'e': /*enable*/
        widget->enable = !!value_int(*v);
        break;
      default: {
        if (widget->vt->set_prop) {
          ret = widget->vt->set_prop(widget, name, v);
        } else {
          ret = RET_NOT_FOUND;
        }
      }
    }
  } else if (strcmp(name, "visible") == 0) {
    widget->visible = !!value_int(*v);
  } else if (strcmp(name, "enable") == 0) {
    widget->enable = !!value_int(*v);
  } else if (strcmp(name, "name") == 0) {
    const char* str = value_str(*v);
    if (str != NULL) {
      strncpy(widget->name, str, NAME_LEN);
    }
  } else {
    if (widget->vt->set_prop) {
      ret = widget->vt->set_prop(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  if (ret != RET_NOT_FOUND) {
    widget_dispatch(widget, &e);
  }

  return ret;
}

ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (name[1] == '\0') {
    switch (name[0]) {
      case 'x':
        value_set_int32(v, widget->x);
        break;
      case 'y':
        value_set_int32(v, widget->y);
        break;
      case 'w':
        value_set_int32(v, widget->w);
        break;
      case 'h':
        value_set_int32(v, widget->h);
        break;
      case 'v': /*visible*/
        value_set_bool(v, widget->visible);
        break;
      case 'e': /*enable*/
        value_set_bool(v, widget->enable);
        break;
      default: {
        if (widget->vt->get_prop) {
          ret = widget->vt->get_prop(widget, name, v);
        } else {
          ret = RET_NOT_FOUND;
        }
      }
    }
  } else if (strcmp(name, "visible") == 0) {
    value_set_bool(v, widget->visible);
  } else if (strcmp(name, "enable") == 0) {
    value_set_bool(v, widget->enable);
  } else if (strcmp(name, "name") == 0) {
    value_set_str(v, widget->name);
  } else {
    if (widget->vt->get_prop) {
      ret = widget->vt->get_prop(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

ret_t widget_on_paint_self(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_self) {
    ret = widget->vt->on_paint_self(widget, c);
  }

  return ret;
}

ret_t widget_on_paint_children(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_children) {
    ret = widget->vt->on_paint_children(widget, c);
  } else {
    ret = widget_on_paint_children_default(widget, c);
  }

  return ret;
}

ret_t widget_on_keydown(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_keydown) {
    ret = widget->vt->on_keydown(widget, e);
  } else if (widget->target != NULL) {
    widget_on_keydown(widget->target, e);
  }

  return ret;
}

ret_t widget_on_keyup(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_keyup) {
    ret = widget->vt->on_keyup(widget, e);
  } else if (widget->target != NULL) {
    widget_on_keyup(widget->target, e);
  }

  return ret;
}

ret_t widget_on_click(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_click) {
    ret = widget->vt->on_click(widget, e);
  } else if (widget->target != NULL) {
    widget_on_click(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_down) {
    ret = widget->vt->on_pointer_down(widget, e);
  }

  target = widget_find_target(widget, e->x, e->y);
  if (target != NULL && target->enable) {
    widget->target = target;
  }

  if (widget->target != NULL) {
    ret = widget_on_pointer_down(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_move) {
    ret = widget->vt->on_pointer_move(widget, e);
  }

  target = widget_find_target(widget, e->x, e->y);
  if (target != widget->target) {
    if (widget->target != NULL) {
      event_t leave = {EVT_POINTER_LEAVE, widget->target};
      widget_dispatch(widget->target, &leave);
    }

    if (target != NULL) {
      event_t enter = {EVT_POINTER_ENTER, widget->target};
      widget_dispatch(target, &enter);
    }
    widget->target = target;
  } else if (widget->target != NULL) {
    widget_on_pointer_move(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_up) {
    ret = widget->vt->on_pointer_up(widget, e);
  }

  if (widget->target != NULL) {
    widget_on_pointer_up(widget->target, e);
  }

  return ret;
}

ret_t widget_grab(widget_t* widget, widget_t* child) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && child != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  if (widget->vt->grab) {
    ret = widget->vt->grab(widget, child);
  } else {
    ret = widget_grab_default(widget, child);
  }

  return ret;
}

ret_t widget_ungrab(widget_t* widget, widget_t* child) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  if (widget->vt->ungrab) {
    ret = widget->vt->ungrab(widget, child);
  } else {
    ret = widget_ungrab_default(widget, child);
  }

  return ret;
}

ret_t widget_destroy(widget_t* widget) {
  event_t e = {EVT_DESTROY, widget};
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    widget_dispatch(widget, &e);
    emitter_destroy(widget->emitter);
  }

  if (widget->children != NULL) {
    widget_remove_children(widget);
    array_destroy(widget->children);
  }

  if (widget->vt->destroy) {
    widget->vt->destroy(widget);
  }

  memset(widget, 0x00, sizeof(widget_t));
  MEM_FREE(widget);

  return RET_OK;
}

ret_t widget_invalidate(widget_t* widget, rect_t* r) {
  rect_t rself;
  if (r == NULL) {
    rect_init(rself, 0, 0, widget->w, widget->h);
    r = &rself;
  }

  return_value_if_fail(widget != NULL && r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(r->x >= 0 && r->y >= 0, RET_BAD_PARAMS);
  return_value_if_fail((r->x + r->w) <= widget->w && (r->y + r->h) <= widget->h, RET_BAD_PARAMS);

  if (widget->vt && widget->vt->invalidate) {
    return widget->vt->invalidate(widget, r);
  } else {
    return widget_invalidate_default(widget, r);
  }
}

ret_t widget_update_style(widget_t* widget) {
  uint8_t state = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  state = widget->state;
  if (!widget->enable) {
    state = WIDGET_STATE_DISABLE;
  } else if (widget->selected) {
    state = WIDGET_STATE_SELECTED;
  }

  widget->style.data = theme_find_style(default_theme(), widget->type, widget->subtype, state);

  return RET_OK;
}

widget_t* widget_init(widget_t* widget, widget_t* parent, uint8_t type) {
  return_value_if_fail(widget != NULL, NULL);

  widget->type = type;
  widget->subtype = 0;
  widget->enable = TRUE;
  widget->visible = TRUE;
  widget->emitter = NULL;
  widget->children = NULL;
  widget->state = WIDGET_STATE_NORMAL;

  if (parent) {
    widget_add_child(parent, widget);
  }

  if (!widget->vt) {
    widget->vt = widget_vtable_default();
  }
  widget_update_style(widget);

  return widget;
}

ret_t widget_to_local(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x -= iter->x;
    p->y -= iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

ret_t widget_to_global(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x += iter->x;
    p->y += iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

uint32_t widget_count_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, 0);

  return widget->children != NULL ? widget->children->size : 0;
}

widget_t* widget_get_child(widget_t* widget, uint32_t index) {
  return_value_if_fail(widget != NULL && widget->children != NULL, NULL);
  return_value_if_fail(index < widget->children->size, NULL);

  return WIDGETP(widget->children->elms[index]);
}
