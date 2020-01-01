/**
 * File:   grid_item.h
 * Author: AWTK Develop Team
 * Brief:  grid_item
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
 * 2018-11-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/grid_item.h"

TK_DECL_VTABLE(grid_item) = {.size = sizeof(grid_item_t),
                             .type = WIDGET_TYPE_ROW,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = grid_item_create};

widget_t* grid_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(grid_item), x, y, w, h);
}

widget_t* grid_item_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, grid_item), NULL);

  return widget;
}
