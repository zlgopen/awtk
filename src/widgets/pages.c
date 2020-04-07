/**
 * File:   pages.c
 * Author: AWTK Develop Team
 * Brief:  pages
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/pages.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t pages_on_target_destroy(void* ctx, event_t* evt) {
  widget_t* view = WIDGET(ctx);

  widget_set_prop_pointer(view, "save_target", NULL);

  return RET_REMOVE;
}

static ret_t pages_save_target(widget_t* widget) {
  widget_t* target = NULL;
  pages_t* pages = PAGES(widget);
  widget_t* active_view = widget_get_child(widget, pages->active);

  if (active_view != NULL) {
    target = active_view;
    while (target->target != NULL) {
      target = target->target;
    }

    if (target != NULL) {
      widget_set_prop_pointer(active_view, "save_target", target);
      widget_on(target, EVT_DESTROY, pages_on_target_destroy, active_view);
    }
  }

  return RET_OK;
}

static ret_t pages_restore_target(widget_t* widget) {
  widget_t* target = NULL;
  pages_t* pages = PAGES(widget);
  widget_t* active_view = widget_get_child(widget, pages->active);

  if (active_view != NULL) {
    target = WIDGET(widget_get_prop_pointer(active_view, "save_target"));

    if (target == NULL || target->parent == NULL) {
      const char* default_focused_child =
          widget_get_prop_str(active_view, "default_focused_child", NULL);
      if (default_focused_child != NULL) {
        target = widget_lookup(active_view, default_focused_child, TRUE);
        if (target == NULL) {
          target = widget_lookup_by_type(active_view, default_focused_child, TRUE);
        }
      }
    }

    if (target == NULL || target->parent == NULL) {
      target = active_view;
    }
    widget_off_by_func(target, EVT_DESTROY, pages_on_target_destroy, active_view);

    log_debug("target=%s\n", target->vt->type);
    while (target->parent != NULL) {
      target->parent->target = target;
      target->parent->key_target = target;
      target = target->parent;
      if (target == widget) {
        break;
      }
    }
  }

  return RET_OK;
}

ret_t pages_set_active(widget_t* widget, uint32_t index) {
  pages_t* pages = PAGES(widget);
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);

  if (pages->active != index && widget->children != NULL) {
    event_t evt = event_init(EVT_VALUE_WILL_CHANGE, widget);

    pages_save_target(widget);
    widget_dispatch(widget, &evt);
    pages->active = index;
    evt = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &evt);
    pages_restore_target(widget);
    widget_invalidate(widget, NULL);
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

  if (widget->children == NULL) {
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
                         .get_prop = pages_get_prop,
                         .set_prop = pages_set_prop};

widget_t* pages_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(pages), x, y, w, h);
}

widget_t* pages_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, pages), NULL);

  return widget;
}
