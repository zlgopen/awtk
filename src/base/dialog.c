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
#include "base/idle.h"
#include "base/label.h"
#include "base/enums.h"
#include "base/utils.h"
#include "base/dialog.h"
#include "base/group_box.h"
#include "base/main_loop.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

enum { TITLE_H = 30 };
static ret_t dialog_on_relayout_children(widget_t* widget);

static ret_t dialog_on_paint_self(widget_t* widget, canvas_t* c) {
  if (widget->style.data != NULL) {
    return widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_equal(name, WIDGET_PROP_TEXT)) {
    return widget_get_prop(dialog->title, name, v);
  } else if (str_equal(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_int(v, dialog->anim_hint);
    return RET_OK;
  } else if (str_equal(name, WIDGET_PROP_MARGIN)) {
    value_set_int(v, dialog->margin);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_equal(name, WIDGET_PROP_TEXT)) {
    return widget_set_prop(dialog->title, name, v);
  } else if (str_equal(name, WIDGET_PROP_STYLE)) {
    widget_set_prop(dialog->title, name, v);
    widget_set_prop(dialog->client, name, v);
    return RET_OK;
  } else if (str_equal(name, WIDGET_PROP_ANIM_HINT)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = window_animator_type_find(value_str(v));
      dialog->anim_hint = kv != NULL ? kv->value : WINDOW_ANIMATOR_NONE;
    } else {
      dialog->anim_hint = value_int(v);
    }
    return RET_OK;
  } else if (str_equal(name, WIDGET_PROP_MARGIN)) {
    dialog->margin = value_int(v);
    dialog_on_relayout_children(widget);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_on_relayout_children(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  uint32_t margin = dialog->margin;
  wh_t w = widget->w - 2 * margin;
  wh_t h = widget->h - 2 * margin;

  widget_move_resize(dialog->title, margin, margin, w, TITLE_H);
  widget_move_resize(dialog->client, margin, margin + TITLE_H, w, h - TITLE_H);

  widget_layout_children(dialog->title);
  widget_layout_children(dialog->client);

  return RET_OK;
}

static const widget_vtable_t s_dialog_vtable = {.get_prop = dialog_get_prop,
                                                .set_prop = dialog_set_prop,
                                                .on_layout_children = dialog_on_relayout_children,
                                                .on_paint_self = dialog_on_paint_self};

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
    parent = window_manager();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  dialog->margin = 0;
  dialog->title = dialog_title_create(widget, 0, 0, 0, 0);
  dialog->client = dialog_client_create(widget, 0, 0, 0, 0);
  dialog_on_relayout_children(widget);

  return widget;
}

ret_t dialog_set_title(widget_t* widget, const wchar_t* title) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && title != NULL, RET_BAD_PARAMS);

  return widget_set_text(dialog->title, title);
}

static ret_t dialog_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_remove_child(widget->parent, widget);
}

static ret_t dialog_idle_close(const idle_info_t* info) { return dialog_close(WIDGETP(info->ctx)); }

uint32_t dialog_modal(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  bool_t running = main_loop()->running;
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  log_debug("%s run\n", __func__);

  widget_invalidate(widget, NULL);
  main_loop_run(main_loop());
  main_loop()->running = running;

  log_debug("%s quit\n", __func__);
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
