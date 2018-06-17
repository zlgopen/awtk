/**
 * File:   keyboard.h
 * Author: AWTK Develop Team
 * Brief:  keyboard
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
#include "base/utils.h"
#include "base/enums.h"
#include "base/pages.h"
#include "base/keyboard.h"
#include "base/prop_names.h"
#include "base/window_manager.h"

static ret_t keyboard_on_load(void* ctx, event_t* e);

static ret_t keyboard_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_keyboard_vtable = {.on_paint_self = keyboard_on_paint_self};

widget_t* keyboard_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  keyboard_t* win = TKMEM_ZALLOC(keyboard_t);
  return_value_if_fail(win != NULL, NULL);

  widget = WIDGET(win);
  widget_init(widget, NULL, WIDGET_KEYBOARD);
  widget->vt = &s_keyboard_vtable;

  if (parent == NULL) {
    parent = window_manager();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  widget_update_style(widget);
  widget_on(widget, EVT_WINDOW_LOAD, keyboard_on_load, widget);

  return widget;
}

static ret_t keyboard_set_active_page(widget_t* button, const char* name) {
  widget_t* parent = button;
  while (parent != NULL && parent->type != WIDGET_PAGES) parent = parent->parent;

  return_value_if_fail(parent != NULL, RET_FAIL);

  return pages_set_active_by_name(parent, name);
}

static ret_t keyboard_on_button_click(void* ctx, event_t* e) {
  widget_t* button = WIDGET(e->target);
  const char* name = button->name.str;
  const char* page_name = strstr(name, "page:");

  if (page_name != NULL) {
    keyboard_set_active_page(button, page_name + 5);
  }

  log_debug("%s clicked\n", button->name.str);

  return RET_OK;
}

static ret_t keyboard_hook_buttons(void* ctx, void* iter) {
  widget_t* win = WIDGET(ctx);
  widget_t* widget = WIDGET(iter);
  const char* name = widget->name.str;

  if (widget->type == WIDGET_BUTTON && name != NULL) {
    widget_on(widget, EVT_CLICK, keyboard_on_button_click, win);
  }

  return RET_OK;
}

static ret_t keyboard_on_load(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);

  (void)e;
  widget_foreach(widget, keyboard_hook_buttons, widget);

  return RET_OK;
}

ret_t keyboard_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_remove_child(widget->parent, widget);
}
