/**
 * File:   dialog.c
 * Author: AWTK Develop Team
 * Brief:  dialog
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

#include "tkc/mem.h"
#include "base/idle.h"
#include "widgets/label.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "widgets/dialog.h"
#include "widgets/dialog_title.h"
#include "widgets/dialog_client.h"
#include "base/main_loop.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

static ret_t dialog_on_add_child(widget_t* widget, widget_t* child) {
  dialog_t* dialog = DIALOG(widget);

  if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_TITLE)) {
    dialog->title = child;
  } else if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_CLIENT)) {
    dialog->client = child;
  }

  return RET_CONTINUE;
}

static const char* s_dialog_properties[] = {WIDGET_PROP_ANIM_HINT, WIDGET_PROP_OPEN_ANIM_HINT,
                                            WIDGET_PROP_CLOSE_ANIM_HINT, WIDGET_PROP_THEME, NULL};
TK_DECL_VTABLE(dialog) = {.size = sizeof(dialog_t),
                          .type = WIDGET_TYPE_DIALOG,
                          .is_window = TRUE,
                          .clone_properties = s_dialog_properties,
                          .persistent_properties = s_dialog_properties,
                          .parent = TK_PARENT_VTABLE(window_base),
                          .create = dialog_create,
                          .on_add_child = dialog_on_add_child,
                          .on_event = window_base_on_event,
                          .on_paint_self = window_base_on_paint_self,
                          .on_paint_begin = window_base_on_paint_begin,
                          .on_paint_end = window_base_on_paint_end,
                          .set_prop = window_base_set_prop,
                          .get_prop = window_base_get_prop,
                          .on_destroy = window_base_on_destroy};

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(dialog), x, y, w, h);
}

widget_t* dialog_create_simple(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* title = NULL;
  widget_t* client = NULL;
  widget_t* widget = dialog_create(parent, x, y, w, h);
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);

  title = dialog_title_create(widget, 0, 0, 0, 0);
  widget_set_self_layout_params(title, "0", "0", "100%", "30");

  client = dialog_client_create(widget, 0, 0, 0, 0);
  widget_set_self_layout_params(client, "0", "bottom", "100%", "-30");

  widget_layout(widget);

  return WIDGET(dialog);
}

ret_t dialog_set_title(widget_t* widget, const char* title) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && title != NULL, RET_BAD_PARAMS);

  return widget_set_text_utf8(dialog->title, title);
}

static ret_t dialog_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

static ret_t dialog_idle_close(const idle_info_t* info) {
  return dialog_close(WIDGET(info->ctx));
}

uint32_t dialog_modal(widget_t* widget) {
  bool_t running = FALSE;
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  log_debug("%s run\n", __FUNCTION__);

  running = main_loop()->running;
  widget_invalidate(widget, NULL);
  main_loop_run(main_loop());
  main_loop()->running = running;

  log_debug("%s quit\n", __FUNCTION__);
  idle_add(dialog_idle_close, widget);

  return dialog->quit_code;
}

ret_t dialog_quit(widget_t* widget, uint32_t code) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  dialog->quit_code = code;
  main_loop_quit(main_loop());

  return RET_OK;
}

widget_t* dialog_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dialog), NULL);

  return widget;
}

widget_t* dialog_get_title(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL, NULL);

  return dialog->title;
}

widget_t* dialog_get_client(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL, NULL);

  return dialog->client;
}

#include "base/window_manager.h"

#define TOAST_STYLE_NAME "default"

static ret_t on_event_to_quit(void* ctx, event_t* e) {
  widget_t* dlg = WIDGET(ctx);
  dialog_quit(dlg, 0);

  return RET_REMOVE;
}

static ret_t on_timer_to_quit(const timer_info_t* info) {
  widget_t* dlg = WIDGET(info->ctx);

  dialog_quit(dlg, 0);

  return RET_REMOVE;
}

/*simple dialogs*/

ret_t dialog_toast(const char* text, uint32_t duration) {
  xy_t x = 0;
  xy_t y = 0;
  uint32_t min_w = 128;
  uint32_t max_w = 0;
  uint32_t min_h = 30;
  uint32_t max_h = 0;
  widget_t* wm = NULL;
  widget_t* label = NULL;
  widget_t* dialog = NULL;
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  dialog = dialog_create(NULL, 0, 0, 200, 30);
  return_value_if_fail(dialog != NULL, RET_OOM);
  label = label_create(dialog, 0, 0, dialog->w, dialog->h);
  goto_error_if_fail(label != NULL);

  wm = dialog->parent;

  max_w = wm->w * 0.8;
  max_h = wm->h * 0.8;
  widget_set_text_utf8(label, text);
  widget_use_style(label, TOAST_STYLE_NAME);
  widget_use_style(dialog, TOAST_STYLE_NAME);
  label_resize_to_content(label, min_w, max_w, min_h, max_h);

  x = (wm->w - label->w) >> 1;
  y = (wm->h - label->h) >> 1;
  widget_resize(dialog, label->w, label->h);
  widget_move(dialog, x, y);

  widget_on(dialog, EVT_POINTER_UP, on_event_to_quit, dialog);
  widget_add_timer(dialog, on_timer_to_quit, duration);

  dialog_modal(dialog);

  return RET_OK;
error:
  widget_destroy(dialog);

  return RET_FAIL;
}
