/**
 * File:   ui_builder_default.c
 * Author: AWTK Develop Team
 * Brief:  ui_builder default
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/view.h"
#include "base/utf8.h"
#include "base/enums.h"
#include "base/dialog.h"
#include "base/widget_factory.h"
#include "ui_loader/ui_builder_default.h"
#include "ui_loader/ui_loader_default.h"

static ret_t ui_builder_default_on_widget_start(ui_builder_t* b, const widget_desc_t* desc) {
  rect_t r;
  const widget_layout_t* layout = &(desc->layout);

  xy_t x = layout->x;
  xy_t y = layout->y;
  wh_t w = layout->w;
  wh_t h = layout->h;
  widget_t* widget = NULL;
  widget_t* parent = b->widget;
  const char* type = desc->type;

  if (parent != NULL) {
    widget_layout_calc(layout, &r, parent->w, parent->h);
    x = r.x;
    y = r.y;
    w = r.w;
    h = r.h;
  }

  widget = widget_factory_create_widget(widget_factory(), type, parent, x, y, w, h);
  if (widget == NULL) {
    log_debug("%s: not supported type %s\n", __FUNCTION__, type);
    assert(!"not supported");
  }

  if (layout->x_attr != X_ATTR_DEFAULT || layout->y_attr != Y_ATTR_DEFAULT ||
      layout->w_attr != W_ATTR_PIXEL || layout->h_attr != H_ATTR_PIXEL || widget->w < 0 ||
      widget->h < 0) {
    widget_set_self_layout(widget, layout);
  }

  b->widget = widget;
  if (b->root == NULL) {
    b->root = widget;
  }

  // log_debug("%s %d %d %d %d\n", type, (int)(x), (int)(y), (int)(w), (int)(h));

  return RET_OK;
}

static ret_t ui_builder_default_on_widget_prop(ui_builder_t* b, const char* name,
                                               const char* value) {
  value_t v;
  if (strcmp(name, "layout") == 0) {
    widget_set_children_layout_params(b->widget, value);
  } else {
    value_set_str(&v, value);
    widget_set_prop(b->widget, name, &v);
  }

  return RET_OK;
}

static ret_t ui_builder_default_on_widget_prop_end(ui_builder_t* b) {
  (void)b;
  return RET_OK;
}

static ret_t ui_builder_default_on_widget_end(ui_builder_t* b) {
  b->widget = b->widget->parent;
  return RET_OK;
}

static ret_t ui_builder_default_on_end(ui_builder_t* b) {
  if (b->root != NULL) {
    widget_t* win = b->root;
    event_t e = event_init(EVT_WINDOW_LOAD, win);

    if (win && win->name == NULL) {
      widget_set_name(win, b->name);
    }

    widget_layout(win);
    widget_dispatch(win, &e);
    widget_invalidate_force(win);
  }

  return RET_OK;
}

static ui_builder_t s_ui_builder;

ui_builder_t* ui_builder_default(const char* name) {
  memset(&s_ui_builder, 0x00, sizeof(ui_builder_t));

  s_ui_builder.on_widget_start = ui_builder_default_on_widget_start;
  s_ui_builder.on_widget_prop = ui_builder_default_on_widget_prop;
  s_ui_builder.on_widget_prop_end = ui_builder_default_on_widget_prop_end;
  s_ui_builder.on_widget_end = ui_builder_default_on_widget_end;
  s_ui_builder.on_end = ui_builder_default_on_end;
  s_ui_builder.name = name;

  return &s_ui_builder;
}
