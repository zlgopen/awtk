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
#include "widgets/button.h"
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

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);

  if (tk_str_eq(name, WIDGET_PROP_HIGHLIGHT)) {
    dialog->highlight = tk_str_copy(dialog->highlight, value_str(v));

    return RET_OK;
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);

  if (tk_str_eq(name, WIDGET_PROP_HIGHLIGHT)) {
    value_set_str(v, dialog->highlight);

    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

static ret_t dialog_on_destroy(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  TKMEM_FREE(dialog->highlight);

  return window_base_on_destroy(widget);
}

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
                          .set_prop = dialog_set_prop,
                          .get_prop = dialog_get_prop,
                          .on_destroy = dialog_on_destroy};

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
  return_value_if_fail(dialog != NULL && title != NULL, RET_BAD_PARAMS);

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

  dialog->quited = FALSE;
  dialog->is_model = TRUE;
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
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  dialog->quited = TRUE;
  dialog->quit_code = code;
  main_loop_quit(main_loop());

  return RET_OK;
}

bool_t dialog_is_quited(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, FALSE);

  return dialog->quited;
}

bool_t dialog_is_modal(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, FALSE);

  return dialog->is_model;
}

widget_t* dialog_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dialog), NULL);

  return widget;
}

widget_t* dialog_get_title(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);

  return dialog->title;
}

widget_t* dialog_get_client(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);

  return dialog->client;
}

#include "base/window_manager.h"

#define OK_STYLE_NAME "default"
#define CANCEL_STYLE_NAME "default"

static ret_t on_ok_to_quit(void* ctx, event_t* e) {
  widget_t* dlg = WIDGET(ctx);
  dialog_quit(dlg, RET_OK);

  return RET_REMOVE;
}

static ret_t on_cancel_to_quit(void* ctx, event_t* e) {
  widget_t* dlg = WIDGET(ctx);
  dialog_quit(dlg, RET_FAIL);

  return RET_REMOVE;
}

static ret_t on_timer_to_quit(const timer_info_t* info) {
  widget_t* dlg = WIDGET(info->ctx);

  dialog_quit(dlg, 0);

  return RET_REMOVE;
}

/*simple dialogs*/

static ret_t dialog_get_size_limits(rect_t* r) {
  uint32_t min_w = 128;
  uint32_t max_w = 0;
  uint32_t min_h = 30;
  uint32_t max_h = 0;
  widget_t* wm = window_manager();

  max_w = wm->w * 0.8;
  max_h = wm->h * 0.8;

  r->x = min_w;
  r->w = max_w;
  r->y = min_h;
  r->h = max_h;

  return RET_OK;
}

static widget_t* dialog_create_label(const char* text) {
  rect_t r;
  widget_t* label = NULL;

  dialog_get_size_limits(&r);

  label = label_create(NULL, 0, 0, 0, 0);
  return_value_if_fail(label != NULL, NULL);

  widget_set_text_utf8(label, text);
  label_resize_to_content(label, r.x, r.w, r.y, r.h);

  return label;
}

ret_t dialog_toast(const char* text, uint32_t duration) {
  widget_t* label = NULL;
  widget_t* dialog = NULL;
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  label = dialog_create_label(text);
  return_value_if_fail(label != NULL, RET_OOM);

  dialog = dialog_create(NULL, 0, 0, label->w, label->h);
  goto_error_if_fail(dialog != NULL);

  widget_set_prop_str(dialog, WIDGET_PROP_ANIM_HINT, "fade(duration=500)");
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, "dialog_toast");
  widget_add_child(dialog, label);

  widget_on(dialog, EVT_POINTER_UP, on_ok_to_quit, dialog);
  widget_add_timer(dialog, on_timer_to_quit, duration);

  return (ret_t)dialog_modal(dialog);
error:
  widget_destroy(label);

  return RET_FAIL;
}

ret_t dialog_info_ex(const char* text, const char* title_text, const char* theme) {
  uint32_t w = 0;
  uint32_t h = 0;
  char params[128];
  widget_t* ok = NULL;
  widget_t* label = NULL;
  widget_t* dialog = NULL;
  widget_t* title = NULL;
  widget_t* client = NULL;
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  label = dialog_create_label(text);
  return_value_if_fail(label != NULL, RET_OOM);

  tk_snprintf(params, sizeof(params) - 1, "default(x=10, y=10, w=%d, h=%d)", label->w, label->h);
  widget_set_self_layout(label, params);

  h = label->h + 90;
  w = tk_max(label->w, 128) + 20;
  dialog = dialog_create_simple(NULL, 0, 0, w, h);
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, theme);

  client = dialog_get_client(dialog);
  title = dialog_get_title(dialog);

  widget_set_tr_text(title, title_text);
  widget_add_child(client, label);

  ok = button_create(client, 0, 0, 0, 0);
  widget_set_tr_text(ok, "OK");
  widget_set_self_layout(ok, "default(x=c, y=bottom:10, w=50%, h=30)");
  widget_on(ok, EVT_CLICK, on_ok_to_quit, dialog);

  if (dialog != NULL && ok != NULL) {
    widget_layout(dialog);

    return (ret_t)dialog_modal(dialog);
  } else {
    widget_destroy(dialog);

    return RET_FAIL;
  }
}

ret_t dialog_info(const char* text) {
  return dialog_info_ex(text, "Infomation", "dialog_info");
}

ret_t dialog_warn(const char* text) {
  return dialog_info_ex(text, "Warnning", "dialog_warn");
}

ret_t dialog_confirm(const char* text) {
  uint32_t w = 0;
  uint32_t h = 0;
  char params[128];
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_t* label = NULL;
  widget_t* dialog = NULL;
  widget_t* title = NULL;
  widget_t* client = NULL;
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  label = dialog_create_label(text);
  return_value_if_fail(label != NULL, RET_OOM);

  tk_snprintf(params, sizeof(params) - 1, "default(x=10, y=10, w=%d, h=%d)", label->w, label->h);
  widget_set_self_layout(label, params);

  h = label->h + 90;
  w = tk_max(label->w, 128) + 20;
  dialog = dialog_create_simple(NULL, 0, 0, w, h);
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, "dialog_confirm");

  client = dialog_get_client(dialog);
  title = dialog_get_title(dialog);

  widget_set_tr_text(title, "Confirm");
  widget_add_child(client, label);

  ok = button_create(client, 0, 0, 0, 0);
  widget_set_tr_text(ok, "OK");
  widget_use_style(ok, OK_STYLE_NAME);
  widget_set_self_layout(ok, "default(x=10%, y=bottom:10, w=30%, h=30)");
  widget_on(ok, EVT_CLICK, on_ok_to_quit, dialog);

  cancel = button_create(client, 0, 0, 0, 0);
  widget_set_tr_text(cancel, "Cancel");
  widget_use_style(cancel, CANCEL_STYLE_NAME);
  widget_set_self_layout(cancel, "default(x=r:10%, y=bottom:10, w=30%, h=30)");
  widget_on(cancel, EVT_CLICK, on_cancel_to_quit, dialog);

  if (dialog != NULL && ok != NULL) {
    widget_layout(dialog);

    return (ret_t)dialog_modal(dialog);
  } else {
    widget_destroy(dialog);

    return RET_FAIL;
  }
}
