/**
 * File:   vpage.c
 * Author: AWTK Develop Team
 * Brief:  虚拟页面(根据情况自动加载/卸载页面)。
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/ui_loader.h"
#include "base/window_manager.h"
#include "ui_loader/ui_serializer.h"
#include "widget_animators/widget_animator_prop.h"

#include "vpage.h"

ret_t vpage_set_ui_asset(widget_t* widget, const char* ui_asset) {
  vpage_t* vpage = VPAGE(widget);
  return_value_if_fail(vpage != NULL, RET_BAD_PARAMS);

  vpage->ui_asset = tk_str_copy(vpage->ui_asset, ui_asset);

  return RET_OK;
}

ret_t vpage_set_anim_hint(widget_t* widget, const char* anim_hint) {
  vpage_t* vpage = VPAGE(widget);
  return_value_if_fail(vpage != NULL, RET_BAD_PARAMS);

  vpage->anim_hint = tk_str_copy(vpage->anim_hint, anim_hint);

  return RET_OK;
}

static ret_t vpage_get_prop(widget_t* widget, const char* name, value_t* v) {
  vpage_t* vpage = VPAGE(widget);
  return_value_if_fail(vpage != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VPAGE_PROP_UI_ASSET, name)) {
    value_set_str(v, vpage->ui_asset);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_ANIM_HINT, name)) {
    value_set_str(v, vpage->anim_hint);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t vpage_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VPAGE_PROP_UI_ASSET, name)) {
    vpage_set_ui_asset(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_ANIM_HINT, name)) {
    vpage_set_anim_hint(widget, value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t vpage_on_destroy(widget_t* widget) {
  vpage_t* vpage = VPAGE(widget);
  return_value_if_fail(widget != NULL && vpage != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(vpage->ui_asset);
  TKMEM_FREE(vpage->anim_hint);

  return RET_OK;
}

static ret_t vpage_dispatch_event(widget_t* widget, int32_t etype) {
  event_t e = event_init(etype, widget);

  return widget_dispatch_async(widget, &e);
}

static ret_t vpage_on_enter_done(widget_t* widget) {
  vpage_dispatch_event(widget, EVT_VPAGE_OPEN);
  return RET_OK;
}

static ret_t vpage_on_enter_animation_done(void* ctx, event_t* e) {
  return vpage_on_enter_done(WIDGET(ctx));
}

static ret_t vpage_on_enter(widget_t* widget, uint32_t index, uint32_t old_index) {
  vpage_t* vpage = VPAGE(widget);
  widget_animator_t* am = NULL;
  uint32_t nr = widget_count_children(widget->parent);
  bool_t can_animate = old_index < nr && widget_is_window_opened(widget);

  if (vpage->ui_asset != NULL) {
    widget_t* children = ui_loader_load_widget(vpage->ui_asset);
    event_t will_open = event_init(EVT_WINDOW_WILL_OPEN, children);
    return_value_if_fail(children != NULL, RET_BAD_PARAMS);

    widget_add_child(widget, children);
    /*some widget depends on will open to initialize*/
    widget_dispatch_recursive(children, &will_open);
  }

  widget_set_visible(widget, TRUE);
  vpage_dispatch_event(widget, EVT_VPAGE_WILL_OPEN);
  window_manager_set_ignore_input_events(window_manager(), FALSE);

  if (vpage->anim_hint != NULL && can_animate) {
    if (tk_str_eq(vpage->anim_hint, "htranslate")) {
      am = widget_animator_prop_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT, "x");
      if (old_index < index) {
        widget_animator_prop_set_params(am, widget->w, 0);
      } else {
        widget_animator_prop_set_params(am, -widget->w, 0);
      }
    } else {
      am = widget_animator_prop_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT, "y");
      if (old_index < index) {
        widget_animator_prop_set_params(am, widget->h, 0);
      } else {
        widget_animator_prop_set_params(am, -widget->h, 0);
      }
    }
    if (am != NULL) {
      widget_animator_start(am);
      widget_animator_on(am, EVT_ANIM_END, vpage_on_enter_animation_done, widget);
    } else {
      vpage_on_enter_done(widget);
    }
  } else {
    vpage_on_enter_done(widget);
  }

  return RET_OK;
}

static ret_t vpage_on_leave_done(widget_t* widget) {
  vpage_t* vpage = VPAGE(widget);

  if (vpage->ui_asset != NULL) {
    vpage_dispatch_event(widget, EVT_VPAGE_CLOSE);
    widget_destroy_children(widget);
  }

  widget_set_visible(widget, FALSE);
  window_manager_set_ignore_input_events(window_manager(), FALSE);

  return RET_OK;
}

static ret_t vpage_on_leave_animation_done(void* ctx, event_t* e) {
  vpage_on_leave_done(WIDGET(ctx));
  return RET_OK;
}

static ret_t vpage_on_leave(widget_t* widget, uint32_t index, uint32_t new_index) {
  vpage_t* vpage = VPAGE(widget);

  if (vpage->anim_hint != NULL) {
    widget_animator_t* am = NULL;

    if (tk_str_eq(vpage->anim_hint, "htranslate")) {
      am = widget_animator_prop_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT, "x");
      if (new_index < index) {
        widget_animator_prop_set_params(am, 0, widget->w);
      } else {
        widget_animator_prop_set_params(am, 0, -widget->w);
      }
    } else {
      am = widget_animator_prop_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT, "y");
      if (new_index < index) {
        widget_animator_prop_set_params(am, 0, widget->h);
      } else {
        widget_animator_prop_set_params(am, 0, -widget->h);
      }
    }

    if (am != NULL) {
      widget_animator_start(am);
      widget_set_visible(widget, TRUE);
      window_manager_set_ignore_input_events(window_manager(), TRUE);
      widget_animator_on(am, EVT_ANIM_END, vpage_on_leave_animation_done, widget);
    } else {
      vpage_on_leave_done(widget);
    }
  } else {
    vpage_on_leave_done(widget);
  }

  return RET_OK;
}

static ret_t vpage_on_current_page_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  value_change_event_t* evt = value_change_event_cast(e);
  uint32_t old_index = value_uint32(&(evt->old_value));
  uint32_t new_index = value_uint32(&(evt->new_value));
  uint32_t index = widget_index_of(widget);

  log_debug("%u -> %u %u\n", old_index, new_index, index);
  if (old_index == index) {
    vpage_on_leave(widget, index, new_index);
  } else if (new_index == index) {
    vpage_on_enter(widget, index, old_index);
  }

  return RET_OK;
}

static ret_t vpage_on_attach_parent(widget_t* widget, widget_t* parent) {
  widget_on(parent, EVT_VALUE_CHANGED, vpage_on_current_page_changed, widget);
  
  return RET_OK;
}

static ret_t vpage_on_detach_parent(widget_t* widget, widget_t* parent) {
  return widget_off_by_ctx(parent, widget);
}

const char* s_vpage_properties[] = {VPAGE_PROP_UI_ASSET, WIDGET_PROP_ANIM_HINT, NULL};

TK_DECL_VTABLE(vpage) = {.size = sizeof(vpage_t),
                         .type = WIDGET_TYPE_VPAGE,
                         .clone_properties = s_vpage_properties,
                         .persistent_properties = s_vpage_properties,
                         .parent = TK_PARENT_VTABLE(widget),
                         .create = vpage_create,
                         .set_prop = vpage_set_prop,
                         .get_prop = vpage_get_prop,
                         .on_attach_parent = vpage_on_attach_parent,
                         .on_detach_parent = vpage_on_detach_parent,
                         .on_destroy = vpage_on_destroy};

widget_t* vpage_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(vpage), x, y, w, h);
  vpage_t* vpage = VPAGE(widget);
  return_value_if_fail(vpage != NULL, NULL);

  return widget;
}

widget_t* vpage_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, vpage), NULL);

  return widget;
}
