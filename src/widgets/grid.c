/**
 * File:   grid.h
 * Author: AWTK Develop Team
 * Brief:  grid
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/grid.h"

TK_DECL_VTABLE(grid) = {.size = sizeof(grid_t),
                        .type = WIDGET_TYPE_ROW,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = grid_create};

widget_t* grid_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(grid), x, y, w, h);
}

widget_t* grid_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, grid), NULL);

  return widget;
}
