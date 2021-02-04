/**
 * File:   pages.c
 * Author: AWTK Develop Team
 * Brief:  pages
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/pages.h"
#include "tkc/tokenizer.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

#define PAGE_SAVE_TARGET_TAG_LENGT 16
#define PAGE_FIND_SAVE_TARGET_NUMBER 32
#define PAGE_FIND_SAVE_TARGET_SEPARTOR '/'

#define PAGE_SAVE_TARGET_STRING "save_target"

static widget_t* pages_find_child(widget_t* widget, uint32_t v) {
  int32_t i = 0;
  widget_t* child = NULL;
  return_value_if_fail(widget != NULL && v != 0, NULL);
  for (i = 0; i < widget->children->size; i++) {
    if (v == tk_pointer_to_int(darray_get(widget->children, i))) {
      child = WIDGET(darray_get(widget->children, i));
      break;
    }
  }
  return child;
}

static widget_t* pages_save_target_from_string(str_t* tag, widget_t* page) {
  tokenizer_t tokenizer;
  tokenizer_t* t = NULL;
  char separtor[2] = {0};
  widget_t* save_target = NULL;
  return_value_if_fail(tag != NULL && page != NULL, NULL);
  memset(&separtor, 0x0, sizeof(separtor));
  separtor[0] = PAGE_FIND_SAVE_TARGET_SEPARTOR;
  t = tokenizer_init(&tokenizer, tag->str, tag->size, (const char*)separtor);
  return_value_if_fail(t != NULL, NULL);

  save_target = page;
  while (tokenizer_has_more(t)) {
    const char* str_ptr = tokenizer_next(t);
    widget_t* tmp = pages_find_child(save_target, tk_atoi(str_ptr));
    if (tmp == NULL) {
      break;
    }
    save_target = tmp;
  }
  tokenizer_deinit(t);

  return save_target;
}

static str_t* pages_save_target_to_string(str_t* tag, widget_t* target, widget_t* page) {
  int32_t i = 0;
  darray_t ptrs;
  widget_t* parent = NULL;
  return_value_if_fail(tag != NULL && target != NULL && page != NULL, NULL);
  str_clear(tag);
  darray_init(&ptrs, PAGE_FIND_SAVE_TARGET_NUMBER, NULL, NULL);

  parent = target;
  while (parent != NULL && parent != page) {
    darray_push(&ptrs, parent);
    parent = parent->parent;
  }

  for (i = ptrs.size - 1; i >= 0; i--) {
    int32_t v = tk_pointer_to_int(darray_get(&ptrs, i));
    str_append_int(tag, v);
    str_append_char(tag, PAGE_FIND_SAVE_TARGET_SEPARTOR);
  }
  str_append_char(tag, '\0');
  darray_deinit(&ptrs);
  return tag;
}

static bool_t pages_target_is_page(widget_t* target) {
  return target->vt != NULL && tk_str_eq(target->vt->type, WIDGET_TYPE_PAGES);
}

static ret_t pages_save_target(widget_t* widget) {
  widget_t* target = NULL;
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL && widget->children->size > pages->active) {
    widget_t* active_view = widget_get_child(widget, pages->active);

    if (active_view != NULL) {
      target = active_view;
      while (target->key_target != NULL) {
        target = target->key_target;
        if (pages_target_is_page(target)) {
          break;
        }
      }

      if (target != NULL && active_view != target) {
        str_t str_target;
        str_init(&str_target, PAGE_SAVE_TARGET_TAG_LENGT);
        pages_save_target_to_string(&str_target, target, widget);
        widget_set_prop_str(active_view, PAGE_SAVE_TARGET_STRING, str_target.str);
        str_reset(&str_target);
      }
    }
  }

  return RET_OK;
}

static ret_t pages_on_idle_set_target_focused(const idle_info_t* idle) {
  pages_t* pages = NULL;
  widget_t* target = NULL;
  widget_t* save_target = NULL;
  return_value_if_fail(idle != NULL, RET_BAD_PARAMS);
  pages = PAGES(idle->ctx);
  save_target = target = pages_save_target_from_string(&(pages->str_target), WIDGET(pages));
  if (target != NULL) {
    while (target->parent != NULL) {
      if (target == target->parent->target) {
        break;
      }
      target->parent->target = target;
      target = target->parent;
    }

    widget_set_focused(save_target, TRUE);
  }
  pages->focused_idle_id = TK_INVALID_ID;
  pages->is_save = FALSE;

  return RET_OK;
}

static ret_t pages_restore_target(widget_t* widget) {
  widget_t* target = NULL;
  widget_t* active_view = NULL;
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);
  active_view = widget_get_child(widget, pages->active);

  if (active_view != NULL) {
    str_t str_tag;
    str_init(&str_tag, PAGE_SAVE_TARGET_TAG_LENGT);
    const char* str_target = widget_get_prop_str(active_view, PAGE_SAVE_TARGET_STRING, NULL);
    if (str_target != NULL) {
      str_set(&str_tag, str_target);
      target = pages_save_target_from_string(&str_tag, widget);
    }
    str_reset(&str_tag);

    if (target == NULL || target->parent == NULL) {
      const char* default_focused_child =
          widget_get_prop_str(active_view, WIDGET_PROP_DEFAULT_FOCUSED_CHILD, NULL);
      if (default_focused_child != NULL) {
        target = widget_lookup(active_view, default_focused_child, TRUE);
        if (target == NULL) {
          target = widget_lookup_by_type(active_view, default_focused_child, TRUE);
        }
      }
    }

    if (target == NULL || target->parent == NULL || target == widget) {
      target = active_view;
    }
    if (pages_target_is_page(target)) {
      pages_restore_target(target);
    } else {
      pages_save_target_to_string(&(pages->str_target), target, widget);
      if (pages->focused_idle_id == TK_INVALID_ID) {
        pages->focused_idle_id = idle_add(pages_on_idle_set_target_focused, widget);
      }
    }
  }

  return RET_OK;
}

ret_t pages_set_active(widget_t* widget, uint32_t index) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);

  if (pages->active != index && widget->children != NULL) {
    value_change_event_t evt;

    if (!pages->is_save) {
      pages_save_target(widget);
    }
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_uint32(&(evt.old_value), pages->active);
    value_set_uint32(&(evt.new_value), index);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      pages->active = index;
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
    pages_restore_target(widget);
  } else {
    pages->active = index;
  }

  return RET_OK;
}

ret_t pages_set_active_by_name(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(iter->name, name)) {
    return pages_set_active(widget, i);
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_NOT_FOUND;
}

static widget_t* pages_find_target(widget_t* widget, xy_t x, xy_t y) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, NULL);

  if (widget->children == NULL) {
    return NULL;
  }

  return widget_get_child(widget, pages->active);
}

static ret_t pages_on_paint_children(widget_t* widget, canvas_t* c) {
  widget_t* active = NULL;
  pages_t* pages = PAGES(widget);
  return_value_if_fail(widget != NULL && pages != NULL, RET_BAD_PARAMS);

  if (widget->children == NULL || widget->children->size == 0) {
    return RET_OK;
  }

  active = widget_get_child(widget, pages->active);
  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  return widget_paint(active, c);
}

static ret_t pages_get_prop(widget_t* widget, const char* name, value_t* v) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    value_set_uint32(v, pages->active);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t pages_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    return pages_set_active(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static ret_t pages_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  switch (type) {
    case EVT_BLUR: {
      pages_save_target(widget);
    }
  }

  return RET_OK;
}

static ret_t pages_on_idle_init_save_target(const idle_info_t* idle) {
  pages_t* pages = NULL;
  return_value_if_fail(idle != NULL, RET_BAD_PARAMS);
  pages = PAGES(idle->ctx);

  pages_restore_target(WIDGET(pages));
  pages->init_idle_id = TK_INVALID_ID;

  return RET_OK;
}

static ret_t pages_on_destroy(widget_t* widget) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);

  if (pages->init_idle_id != TK_INVALID_ID) {
    idle_remove(pages->init_idle_id);
  }
  if (pages->focused_idle_id != TK_INVALID_ID) {
    idle_remove(pages->focused_idle_id);
  }

  str_reset(&(pages->str_target));

  return RET_OK;
}

static const char* const s_pages_clone_properties[] = {WIDGET_PROP_VALUE, NULL};

TK_DECL_VTABLE(pages) = {.size = sizeof(pages_t),
                         .inputable = TRUE,
                         .type = WIDGET_TYPE_PAGES,
                         .only_active_child_visible = TRUE,
                         .clone_properties = s_pages_clone_properties,
                         .parent = TK_PARENT_VTABLE(widget),
                         .create = pages_create,
                         .on_paint_self = widget_on_paint_null,
                         .find_target = pages_find_target,
                         .on_paint_children = pages_on_paint_children,
                         .on_event = pages_on_event,
                         .get_prop = pages_get_prop,
                         .set_prop = pages_set_prop,
                         .on_destroy = pages_on_destroy};

widget_t* pages_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(pages), x, y, w, h);
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, NULL);
  str_init(&(pages->str_target), PAGE_SAVE_TARGET_TAG_LENGT);
  pages->init_idle_id = idle_add(pages_on_idle_init_save_target, widget);
  return widget;
}

widget_t* pages_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, pages), NULL);

  return widget;
}
