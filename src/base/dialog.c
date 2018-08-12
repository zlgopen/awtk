/**
 * File:   dialog.c
 * Author: AWTK Develop Team
 * Brief:  dialog
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/layout.h"
#include "base/dialog.h"
#include "base/dialog_title.h"
#include "base/dialog_client.h"
#include "base/main_loop.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

static ret_t dialog_on_paint_self(widget_t* widget, canvas_t* c) {
  if (widget->style.data != NULL) {
    return widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, dialog->anim_hint.str);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    value_set_str(v, dialog->theme.str);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCRIPT)) {
    value_set_str(v, dialog->script.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    str_from_value(&(dialog->anim_hint), v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    str_from_value(&(dialog->theme), v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCRIPT)) {
    str_from_value(&(dialog->script), v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_destroy(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);

  str_reset(&(dialog->theme));
  str_reset(&(dialog->script));
  str_reset(&(dialog->anim_hint));

  return RET_OK;
}

static ret_t dialog_on_add_child(widget_t* widget, widget_t* child) {
  dialog_t* dialog = DIALOG(widget);

  if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_TITLE)) {
    dialog->title = child;
  } else if (tk_str_eq(child->vt->type, WIDGET_TYPE_DIALOG_CLIENT)) {
    dialog->client = child;
  }

  return RET_CONTINUE;
}

static const char* s_dialog_properties[] = {WIDGET_PROP_ANIM_HINT, WIDGET_PROP_THEME,
                                            WIDGET_PROP_SCRIPT, NULL};
static const widget_vtable_t s_dialog_vtable = {.size = sizeof(dialog_t),
                                                .type = WIDGET_TYPE_DIALOG,
                                                .clone_properties = s_dialog_properties,
                                                .persistent_properties = s_dialog_properties,
                                                .create = dialog_create,
                                                .get_prop = dialog_get_prop,
                                                .set_prop = dialog_set_prop,
                                                .on_add_child = dialog_on_add_child,
                                                .destroy = dialog_destroy,
                                                .on_paint_self = dialog_on_paint_self};

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  dialog_t* dialog = TKMEM_ZALLOC(dialog_t);
  widget_t* widget = WIDGET(dialog);
  return_value_if_fail(dialog != NULL, NULL);

  widget_init(widget, NULL, &s_dialog_vtable, x, y, w, h);

  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);

  widget_update_style(widget);
  str_init(&(dialog->anim_hint), 0);
  str_init(&(dialog->theme), 0);
  str_init(&(dialog->script), 0);

  return widget;
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
  dialog_t* dialog = DIALOG(widget);
  bool_t running = main_loop()->running;
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  log_debug("%s run\n", __FUNCTION__);

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
  return_value_if_fail(widget != NULL && widget->vt == &s_dialog_vtable, NULL);

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
