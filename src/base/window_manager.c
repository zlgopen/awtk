/**
 * File:   window_manager.c
 * Author: AWTK Develop Team
 * Brief:  window manager
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"
#include "base/canvas.h"
#include "base/dialog.h"
#include "base/dialog_highlighter.h"
#include "base/input_device_status.h"
#include "base/window_manager.h"
#include "base/window_animator_factory.h"

static widget_t* s_window_manager = NULL;

widget_t* window_manager(void) {
  return s_window_manager;
}

ret_t window_manager_set(widget_t* widget) {
  s_window_manager = widget;

  return RET_OK;
}

static ret_t window_manager_default_impl_back(widget_t* widget) {
  event_t e;
  widget_t* top_window = window_manager_get_top_window(widget);
  return_value_if_fail(top_window != NULL, RET_NOT_FOUND);

  if (widget_is_normal_window(top_window)) {
    e = event_init(EVT_REQUEST_CLOSE_WINDOW, top_window);
    return widget_dispatch(top_window, &e);
  } else {
    log_warn("not support call window_manager_back on non-normal window\n");
    return RET_FAIL;
  }
}

static ret_t window_manager_back_to_home_sync(widget_t* widget) {
  uint32_t k = 0;
  darray_t wins;
  widget_t* top = NULL;
  widget_t* home = NULL;
  int32_t children_nr = widget_count_children(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (children_nr < 2) {
    return RET_OK;
  }

  darray_init(&wins, 10, NULL, NULL);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (home == NULL) {
    if (widget_is_normal_window(iter)) {
      home = iter;
    }
  } else if ((i + 1) < children_nr) {
    if (!widget_is_system_bar(iter)) {
      darray_push(&wins, iter);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  for (k = 0; k < wins.size; k++) {
    widget_t* iter = WIDGET(wins.elms[k]);
    assert(!widget_is_dialog(iter));
    window_manager_close_window_force(widget, iter);
  }
  darray_deinit(&wins);

  children_nr = widget_count_children(widget);
  top = widget_get_child(widget, children_nr - 1);
  return_value_if_fail(top != home, RET_OK);

  return window_manager_close_window(widget, top);
}

static ret_t window_manager_back_to_home_async(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  window_manager_back_to_home_sync(widget);

  return RET_REMOVE;
}

static ret_t window_manager_back_to_home_on_dialog_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);

  window_manager_back_to_home_sync(widget);

  return RET_REMOVE;
}

static ret_t window_manager_default_impl_back_to_home(widget_t* widget) {
  widget_t* top = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  top = window_manager_get_top_window(widget);
  return_value_if_fail(top != NULL, RET_BAD_PARAMS);

  if (!widget_is_dialog(top) || !dialog_is_modal(top)) {
    idle_add(window_manager_back_to_home_async, widget);

    return RET_OK;
  } else {
    if (dialog_is_quited(top)) {
      widget_on(top, EVT_DESTROY, window_manager_back_to_home_on_dialog_destroy, widget);
    } else {
      log_warn("not support call window_manager_back_to_home on dialog\n");
    }

    return RET_FAIL;
  }
}

static widget_t* window_manager_default_impl_get_top_main_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (widget_is_normal_window(iter) && iter->visible) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

static widget_t* window_manager_default_impl_get_top_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->visible) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* window_manager_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  return widget;
}

widget_t* window_manager_get_top_main_window(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, NULL);
  if (wm->vt->get_top_main_window != NULL) {
    return wm->vt->get_top_main_window(widget);
  } else {
    return window_manager_default_impl_get_top_main_window(widget);
  }
}

widget_t* window_manager_get_top_window(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, NULL);

  if (wm->vt->get_top_window != NULL) {
    return wm->vt->get_top_window(widget);
  } else {
    return window_manager_default_impl_get_top_window(widget);
  }
}

widget_t* window_manager_get_prev_window(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, NULL);
  return_value_if_fail(wm->vt->get_prev_window != NULL, NULL);

  return wm->vt->get_prev_window(widget);
}

ret_t window_manager_post_init(widget_t* widget, wh_t w, wh_t h) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->post_init != NULL, RET_BAD_PARAMS);

  return wm->vt->post_init(widget, w, h);
}

ret_t window_manager_open_window(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(window != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->open_window != NULL, RET_BAD_PARAMS);

  return wm->vt->open_window(widget, window);
}

ret_t window_manager_close_window(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(window != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->close_window != NULL, RET_BAD_PARAMS);

  return wm->vt->close_window(widget, window);
}

ret_t window_manager_close_window_force(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(window != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->close_window_force != NULL, RET_BAD_PARAMS);

  return wm->vt->close_window_force(widget, window);
}

ret_t window_manager_paint(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->paint != NULL, RET_BAD_PARAMS);

  return wm->vt->paint(widget);
}

ret_t window_manager_dispatch_input_event(widget_t* widget, event_t* e) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->dispatch_input_event != NULL, RET_BAD_PARAMS);

  return wm->vt->dispatch_input_event(widget, e);
}

ret_t window_manager_set_show_fps(widget_t* widget, bool_t show_fps) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->set_show_fps != NULL, RET_BAD_PARAMS);

  wm->show_fps = show_fps;
  return wm->vt->set_show_fps(widget, show_fps);
}

ret_t window_manager_set_screen_saver_time(widget_t* widget, uint32_t time) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->set_screen_saver_time != NULL, RET_BAD_PARAMS);

  return wm->vt->set_screen_saver_time(widget, time);
}

ret_t window_manager_set_cursor(widget_t* widget, const char* cursor) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm->vt->set_cursor != NULL, RET_BAD_PARAMS);

  return wm->vt->set_cursor(widget, cursor);
}

ret_t window_manager_back(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  if (wm->vt->back != NULL) {
    return wm->vt->back(widget);
  } else {
    return window_manager_default_impl_back(widget);
  }
}
ret_t window_manager_back_to_home(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);

  if (wm->vt->back_to_home != NULL) {
    return wm->vt->back_to_home(widget);
  } else {
    return window_manager_default_impl_back_to_home(widget);
  }
}

xy_t window_manager_get_pointer_x(widget_t* widget) {
  xy_t x = 0;
  xy_t y = 0;
  bool_t pressed = TRUE;

  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, 0);
  return_value_if_fail(wm->vt->get_pointer != NULL, 0);

  wm->vt->get_pointer(widget, &x, &y, &pressed);

  return x;
}

xy_t window_manager_get_pointer_y(widget_t* widget) {
  xy_t x = 0;
  xy_t y = 0;
  bool_t pressed = TRUE;

  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, 0);
  return_value_if_fail(wm->vt->get_pointer != NULL, 0);

  wm->vt->get_pointer(widget, &x, &y, &pressed);

  return y;
}

bool_t window_manager_get_pointer_pressed(widget_t* widget) {
  xy_t x = 0;
  xy_t y = 0;
  bool_t pressed = TRUE;

  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, 0);
  return_value_if_fail(wm->vt->get_pointer != NULL, 0);

  wm->vt->get_pointer(widget, &x, &y, &pressed);

  return pressed;
}

static ret_t wm_on_locale_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_re_translate_text(iter);
  widget_dispatch(iter, e);
  WIDGET_FOR_EACH_CHILD_END();
  widget_invalidate(widget, NULL);

  return RET_OK;
}

widget_t* window_manager_init(window_manager_t* wm, const widget_vtable_t* wvt,
                              const window_manager_vtable_t* vt) {
  widget_t* widget = WIDGET(wm);
  return_value_if_fail(wm != NULL, NULL);

  widget_init(widget, NULL, wvt, 0, 0, 0, 0);
  locale_info_on(locale_info(), EVT_LOCALE_CHANGED, wm_on_locale_changed, wm);
  wm->vt = vt;

  return widget;
}

ret_t window_manager_dispatch_native_window_event(widget_t* widget, event_t* e, void* handle) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && e != NULL && handle != NULL, RET_BAD_PARAMS);

  if (wm->vt->dispatch_native_window_event != NULL) {
    wm->vt->dispatch_native_window_event(widget, e, handle);
  }

  return RET_OK;
}

widget_t* window_manager_find_target_by_win(widget_t* widget, void* win) {
  native_window_t* nw = NULL;
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  nw = (native_window_t*)widget_get_prop_pointer(iter, WIDGET_PROP_NATIVE_WINDOW);
  if (nw != NULL && nw->handle == win) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

widget_t* window_manager_find_target(widget_t* widget, void* win, xy_t x, xy_t y) {
  point_t p = {x, y};
  native_window_t* nw = NULL;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->grab_widget != NULL) {
    return widget->grab_widget;
  }

  widget_to_local(widget, &p);
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  xy_t r = iter->x + iter->w;
  xy_t b = iter->y + iter->h;

  if (win != NULL) {
    nw = (native_window_t*)widget_get_prop_pointer(iter, WIDGET_PROP_NATIVE_WINDOW);
    if (nw == NULL || nw->handle != win) {
      continue;
    }
  }

  if (iter->visible && iter->sensitive && iter->enable && p.x >= iter->x && p.y >= iter->y &&
      p.x <= r && p.y <= b) {
    return iter;
  }

  if (widget_is_dialog(iter) || widget_is_popup(iter)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

ret_t window_manager_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(curr_win != NULL && img != NULL && fbo != NULL, RET_BAD_PARAMS);

  if (wm->vt->snap_curr_window != NULL) {
    return wm->vt->snap_curr_window(widget, curr_win, img, fbo, auto_rotate);
  }

  return RET_NOT_IMPL;
}

ret_t window_manager_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(prev_win != NULL && img != NULL && fbo != NULL, RET_BAD_PARAMS);

  if (wm->vt->snap_prev_window != NULL) {
    return wm->vt->snap_prev_window(widget, prev_win, img, fbo, auto_rotate);
  }

  return RET_NOT_IMPL;
}

dialog_highlighter_t* window_manager_get_dialog_highlighter(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && wm->vt != NULL, NULL);

  if (wm->vt->get_dialog_highlighter != NULL) {
    return wm->vt->get_dialog_highlighter(widget);
  }

  return NULL;
}
