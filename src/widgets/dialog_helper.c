/**
 * File:   dialog_helper.c
 * Author: AWTK Develop Team
 * Brief:  dialogi helper
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/utils.h"
#include "widgets/label.h"
#include "base/dialog.h"
#include "widgets/button.h"
#include "widgets/dialog_title.h"
#include "widgets/dialog_client.h"

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

  if (text != NULL) {
    widget_set_tr_text(label, text);
  }
  label_resize_to_content(label, r.x, r.w, r.y, r.h);

  return label;
}

ret_t dialog_toast(const char* text, uint32_t duration) {
  char params[128];
  int32_t margin = 10;
  widget_t* label = NULL;
  widget_t* dialog = NULL;
  return_value_if_fail(text != NULL && duration >= 1000, RET_BAD_PARAMS);

  label = dialog_create_label(text);
  return_value_if_fail(label != NULL, RET_OOM);

  dialog = dialog_create(NULL, 0, 0, label->w + 2 * margin, label->h + 2 * margin);
  goto_error_if_fail(dialog != NULL);

  widget_set_prop_str(dialog, WIDGET_PROP_ANIM_HINT, "fade(duration=500)");
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, "dialog_toast");
  widget_add_child(dialog, label);

  tk_snprintf(params, sizeof(params) - 1, "default(x=%d, y=%d, w=%d, h=%d)", margin, margin,
              label->w, label->h);
  widget_set_self_layout(label, params);

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
  w = tk_max(label->w, 128) + 40;
  dialog = dialog_create_simple(NULL, 0, 0, w, h);
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, theme);
  widget_set_prop_str(dialog, WIDGET_PROP_HIGHLIGHT, "default(alpha=40)");

  client = dialog_get_client(dialog);
  title = dialog_get_title(dialog);

  widget_set_tr_text(title, title_text);
  widget_add_child(client, label);

  ok = button_create(client, 0, 0, 0, 0);
  widget_set_tr_text(ok, "OK");
  widget_set_focused(ok, TRUE);
  widget_set_focusable(ok, TRUE);
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

ret_t dialog_info(const char* title, const char* text) {
  title = title != NULL ? title : "Infomation";
  return dialog_info_ex(text, title, "dialog_info");
}

ret_t dialog_warn(const char* title, const char* text) {
  title = title != NULL ? title : "Warning";
  return dialog_info_ex(text, title, "dialog_warn");
}

ret_t dialog_confirm(const char* stitle, const char* text) {
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

  stitle = stitle != NULL ? stitle : "Confirm";
  label = dialog_create_label(text);
  return_value_if_fail(label != NULL, RET_OOM);

  tk_snprintf(params, sizeof(params) - 1, "default(x=10, y=10, w=%d, h=%d)", label->w, label->h);
  widget_set_self_layout(label, params);

  h = label->h + 90;
  w = tk_max(label->w, 128) + 40;
  dialog = dialog_create_simple(NULL, 0, 0, w, h);
  widget_set_prop_str(dialog, WIDGET_PROP_THEME, "dialog_confirm");
  widget_set_prop_str(dialog, WIDGET_PROP_HIGHLIGHT, "default(alpha=40)");

  client = dialog_get_client(dialog);
  title = dialog_get_title(dialog);

  widget_set_tr_text(title, stitle);
  widget_add_child(client, label);

  ok = button_create(client, 0, 0, 0, 0);
  widget_set_tr_text(ok, "OK");
  widget_set_focused(ok, TRUE);
  widget_set_focusable(ok, TRUE);
  widget_use_style(ok, OK_STYLE_NAME);
  widget_set_self_layout(ok, "default(x=10%, y=bottom:10, w=30%, h=30)");
  widget_on(ok, EVT_CLICK, on_ok_to_quit, dialog);

  cancel = button_create(client, 0, 0, 0, 0);
  widget_set_focusable(cancel, TRUE);
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
