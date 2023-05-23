/**
 * File:   dialog_helper.c
 * Author: AWTK Develop Team
 * Brief:  dialogi helper
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
#include "widgets/label.h"
#include "base/dialog.h"
#include "widgets/button.h"
#include "base/style_factory.h"
#include "widgets/dialog_title.h"
#include "widgets/dialog_client.h"
#include "base/window_manager.h"
#include "dialog_toast.inc"

#define DIALOG_OK_STYLE "ok"
#define DIALOG_CANCEL_STYLE "cancel"
#define DIALOG_INFO_THEME "dialog_info"
#define DIALOG_WARN_THEME "dialog_warn"
#define DIALOG_CONFIRM_THEME "dialog_confirm"

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

static widget_t* dialog_create_content_label(widget_t* parent, const char* text) {
  rect_t r;
  widget_t* label = label_create(parent, 0, 0, 0, 0);
  return_value_if_fail(label != NULL, NULL);

  if (text != NULL) {
    widget_set_tr_text(label, text);
    label_set_line_wrap(label, TRUE);
    label_set_word_wrap(label, TRUE);
  }

  dialog_get_size_limits(&r);
  label_resize_to_content(label, r.x, r.w, r.y, r.h);

  return label;
}

static ret_t dialog_create_ok(widget_t* client, bool_t has_cancel) {
  widget_t* ok = button_create(client, 0, 0, 0, 0);
  return_value_if_fail(ok != NULL, RET_OOM);

  widget_set_name(ok, DIALOG_CHILD_OK);
  widget_set_tr_text(ok, "OK");
  widget_set_focused(ok, TRUE);
  widget_set_focusable(ok, TRUE);
  widget_use_style(ok, DIALOG_OK_STYLE);
  if (has_cancel) {
    widget_set_self_layout(ok, "default(x=10%, y=bottom:10, w=35%, h=30)");
  } else {
    widget_set_self_layout(ok, "default(x=c, y=bottom:10, w=50%, h=30)");
  }
  widget_on(ok, EVT_CLICK, on_ok_to_quit, widget_get_window(client));

  return RET_OK;
}

static ret_t dialog_create_cancel(widget_t* client) {
  widget_t* cancel = button_create(client, 0, 0, 0, 0);
  return_value_if_fail(cancel != NULL, RET_OOM);

  widget_set_name(cancel, DIALOG_CHILD_CANCEL);
  widget_set_focusable(cancel, TRUE);
  widget_set_tr_text(cancel, "Cancel");
  widget_use_style(cancel, DIALOG_CANCEL_STYLE);
  widget_set_self_layout(cancel, "default(x=r:10%, y=bottom:10, w=35%, h=30)");

  widget_on(cancel, EVT_CLICK, on_cancel_to_quit, widget_get_window(client));

  return RET_OK;
}

static widget_t* dialog_create_simple_ex(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h,
                                         const char* stitle, const char* theme) {
  widget_t* title = NULL;
  widget_t* client = NULL;
  widget_t* widget = dialog_create(parent, x, y, w, h);
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(dialog != NULL, NULL);
  if (theme != NULL) {
    widget_set_prop_str(widget, WIDGET_PROP_THEME, theme);
  }
  widget_set_prop_str(widget, WIDGET_PROP_HIGHLIGHT, "default(alpha=40)");

  if (stitle != NULL) {
    widget_set_text_utf8(widget, stitle);
  }
  title = dialog_title_create(widget, 0, 0, 0, 0);
  goto_error_if_fail(title != NULL);

  widget_set_name(title, DIALOG_CHILD_TITLE);
  widget_set_tr_text(title, stitle);
  widget_set_self_layout_params(title, "0", "0", "100%", "30");

  client = dialog_client_create(widget, 0, 0, 0, 0);
  goto_error_if_fail(client != NULL);
  widget_set_name(client, DIALOG_CHILD_CLIENT);

  widget_set_self_layout_params(client, "0", "bottom", "100%", "-30");

  return widget;
error:
  widget_unref(widget);

  return NULL;
}

widget_t* dialog_create_simple(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return dialog_create_simple_ex(parent, x, y, w, h, NULL, NULL);
}

ret_t dialog_simple_show(const char* stitle, const char* scontent, const char* theme, bool_t has_ok,
                         bool_t has_cancel) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  widget_t* content = NULL;
  widget_t* widget = dialog_create_simple_ex(NULL, 0, 0, 0, 0, stitle, theme);
  widget_t* client = widget_lookup_by_type(widget, WIDGET_TYPE_DIALOG_CLIENT, TRUE);
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  content = dialog_create_content_label(client, scontent);
  goto_error_if_fail(content != NULL);

  if (has_ok) {
    dialog_create_ok(client, has_cancel);
  }

  if (has_cancel) {
    dialog_create_cancel(client);
  }

  h = content->h + 90;
  w = tk_max(content->w, 128) + 40;
  x = (widget->parent->w - w) / 2;
  y = (widget->parent->h - h) / 2;
  widget_move_resize(widget, x, y, w, h);
  widget_move(content, 10, 10);

  widget_layout(widget);

  return (ret_t)dialog_modal(widget);
error:
  widget_destroy(widget);

  return RET_FAIL;
}

/*simple dialogs*/

ret_t dialog_toast(const char* text, uint32_t duration) {
  dialog_toast_t* dialog_toast = NULL;
  return_value_if_fail(text != NULL && *text && duration != 0, RET_BAD_PARAMS);
  dialog_toast = dialog_toast_manager();
  return_value_if_fail(dialog_toast != NULL, RET_OOM);

  ENSURE(dialog_toast_push_message(dialog_toast, text, duration) != NULL);
  return RET_OK;
}

ret_t dialog_info(const char* title, const char* text) {
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  title = title != NULL ? title : "Information";
  return dialog_simple_show(title, text, DIALOG_INFO_THEME, TRUE, FALSE);
}

ret_t dialog_warn(const char* title, const char* text) {
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  title = title != NULL ? title : "Warning";
  return dialog_simple_show(title, text, DIALOG_WARN_THEME, TRUE, FALSE);
}

ret_t dialog_confirm(const char* title, const char* text) {
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);

  title = title != NULL ? title : "Confirm";
  return dialog_simple_show(title, text, DIALOG_CONFIRM_THEME, TRUE, TRUE);
}

widget_t* dialog_create_with_ok_cancel(const char* stitle, uint32_t w, uint32_t h) {
  widget_t* widget = dialog_create_simple_ex(NULL, 0, 0, w, h, stitle, NULL);
  widget_t* client = widget_lookup_by_type(widget, WIDGET_TYPE_DIALOG_CLIENT, TRUE);
  return_value_if_fail(widget != NULL, NULL);

  dialog_create_ok(client, TRUE);
  dialog_create_cancel(client);
  widget_move_to_center(widget);

  return widget;
}
