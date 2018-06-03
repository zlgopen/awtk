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
#include "base/image.h"
#include "base/label.h"
#include "base/value.h"
#include "base/window.h"
#include "base/button.h"
#include "base/dialog.h"
#include "base/slider.h"
#include "base/edit.h"
#include "base/group_box.h"
#include "base/check_button.h"
#include "base/progress_bar.h"
#include "base/resource_manager.h"
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
  uint16_t type = desc->type;
  widget_t* parent = b->widget;

  if (parent != NULL) {
    widget_layout_calc(layout, &r, parent->w, parent->h);
    x = r.x;
    y = r.y;
    w = r.w;
    h = r.h;
  }

  if (parent && parent->type == WIDGET_DIALOG) {
    dialog_t* dlg = DIALOG(parent);
    parent = dlg->client;
  }

  switch (type) {
    case WIDGET_DIALOG:
      widget = dialog_create(parent, x, y, w, h);
      break;
    case WIDGET_NORMAL_WINDOW:
      widget = window_create(parent, x, y, w, h);
      break;
    case WIDGET_IMAGE:
      widget = image_create(parent, x, y, w, h);
      break;
    case WIDGET_BUTTON:
      widget = button_create(parent, x, y, w, h);
      break;
    case WIDGET_LABEL:
      widget = label_create(parent, x, y, w, h);
      break;
    case WIDGET_EDIT:
      widget = edit_create(parent, x, y, w, h);
      break;
    case WIDGET_PROGRESS_BAR:
      widget = progress_bar_create(parent, x, y, w, h);
      break;
    case WIDGET_SLIDER:
      widget = slider_create(parent, x, y, w, h);
      break;
    case WIDGET_GROUP_BOX:
      widget = group_box_create(parent, x, y, w, h);
      break;
    case WIDGET_VIEW:
      widget = view_create(parent, x, y, w, h);
      break;
    case WIDGET_CHECK_BUTTON:
      widget = check_button_create(parent, x, y, w, h);
      break;
    case WIDGET_RADIO_BUTTON:
      widget = check_button_create_radio(parent, x, y, w, h);
      break;
    default:
      log_debug("%s: not supported type %d\n", __func__, type);
      break;
  }

  if (layout->x_attr != X_ATTR_DEFAULT || layout->y_attr != Y_ATTR_DEFAULT ||
      layout->w_attr != W_ATTR_PIXEL || layout->h_attr != H_ATTR_PIXEL) {
    widget_set_parsed_self_layout_params(widget, layout);
  }

  b->widget = widget;
  if (b->root == NULL) {
    b->root = widget;
  }

  log_debug("%d %d %d %d %d\n", (int)(type), (int)(x), (int)(y), (int)(w), (int)(h));

  return RET_OK;
}

static ret_t ui_builder_default_on_widget_prop(ui_builder_t* b, const char* name,
                                               const char* value) {
  value_t v;
  if (strcmp(name, "layout") == 0) {
    /*2 2 5 10 10*/
    children_layout_t cl;
    children_layout_parser(&cl, value);
    widget_set_children_layout_params(b->widget, cl.rows, cl.cols, cl.margin, cl.cell_spacing);
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
    widget_layout(b->root);
  }

  return RET_OK;
}

static ui_builder_t s_ui_builder;

ui_builder_t* ui_builder_default() {
  memset(&s_ui_builder, 0x00, sizeof(ui_builder_t));

  s_ui_builder.on_widget_start = ui_builder_default_on_widget_start;
  s_ui_builder.on_widget_prop = ui_builder_default_on_widget_prop;
  s_ui_builder.on_widget_prop_end = ui_builder_default_on_widget_prop_end;
  s_ui_builder.on_widget_end = ui_builder_default_on_widget_end;
  s_ui_builder.on_end = ui_builder_default_on_end;

  return &s_ui_builder;
}

widget_t* window_open(const char* name) {
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  const resource_info_t* ui = resource_manager_ref(resource_manager(), RESOURCE_TYPE_UI, name);
  return_value_if_fail(ui != NULL, NULL);

  ui_loader_load(loader, ui->data, ui->size, builder);
  resource_manager_unref(resource_manager(), ui);

  return builder->root;
}

widget_t* dialog_open(const char* name) {
  return window_open(name);
}
