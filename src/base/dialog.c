/**
 * File:   dialog.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
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

#include "base/mem.h"
#include "base/label.h"
#include "base/dialog.h"
#include "base/group_box.h"
#include "base/main_loop.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

enum { TITLE_H = 30 };

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    return widget_get_prop(dialog->title, name, v);
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    return widget_set_prop(dialog->title, name, v);
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_dialog_vtable = {
                                                .get_prop = dialog_get_prop,
                                                .set_prop = dialog_set_prop};

widget_t* dialog_title_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = label_create(parent, x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  widget->type = WIDGET_DIALOG_TITLE;
  widget_update_style(widget);
  widget_set_name(widget, "title");

  return widget;
}

widget_t* dialog_client_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = group_box_create(parent, x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);
  widget->type = WIDGET_DIALOG_CLIENT;
  widget_update_style(widget);
  widget_set_name(widget, "client");

  return widget;
}

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  dialog_t* dialog = MEM_ZALLOC(dialog_t);
  return_value_if_fail(dialog != NULL, NULL);

  widget = WIDGETP(dialog);
  widget_init(widget, NULL, WIDGET_DIALOG);
  widget->vt = &s_dialog_vtable;

  if (parent == NULL) {
    parent = default_wm();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  dialog->title = dialog_title_create(widget, 0, 0, widget->w, TITLE_H);
  dialog->client = dialog_client_create(widget, 0, TITLE_H, widget->w, widget->h - TITLE_H);

  return widget;
}

ret_t dialog_set_title(widget_t* widget, const wchar_t* title) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && title != NULL, RET_BAD_PARAMS);

  return label_set_text(dialog->title, title);
}

uint32_t dialog_modal(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  bool_t running = main_loop_get_default()->running;
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  log_debug("%s run\n", __func__);

  widget_invalidate(widget, NULL);
  main_loop_run(main_loop_get_default());
  main_loop_get_default()->running = running;

  log_debug("%s quit\n", __func__);

  return dialog->quit_code;
}

ret_t dialog_quit(widget_t* widget, uint32_t code) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  dialog->quit_code = code;
  main_loop_quit(main_loop_get_default());

  return RET_OK;
}

