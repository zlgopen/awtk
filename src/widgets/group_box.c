/**
 * File:   group_box.h
 * Author: AWTK Develop Team
 * Brief:  group_box
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
 * 2018-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "widgets/group_box.h"
#include "tkc/mem.h"

static ret_t group_box_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

TK_DECL_VTABLE(group_box) = {.size = sizeof(group_box_t),
                             .type = WIDGET_TYPE_GROUP_BOX,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = group_box_create,
                             .on_paint_self = group_box_on_paint_self};

widget_t* group_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(group_box), x, y, w, h);
}

widget_t* group_box_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, group_box), NULL);

  return widget;
}
