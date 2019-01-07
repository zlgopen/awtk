/**
 * File:   grid_item.h
 * Author: AWTK Develop Team
 * Brief:  grid_item
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
 * 2018-11-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/grid_item.h"

static const widget_vtable_t s_grid_item_vtable = {
    .size = sizeof(grid_item_t), .type = WIDGET_TYPE_ROW, .create = grid_item_create};

widget_t* grid_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  grid_item_t* grid_item = TKMEM_ZALLOC(grid_item_t);
  widget_t* widget = WIDGET(grid_item);
  return_value_if_fail(grid_item != NULL, NULL);

  return widget_init(widget, parent, &s_grid_item_vtable, x, y, w, h);
}

widget_t* grid_item_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_grid_item_vtable, NULL);

  return widget;
}
