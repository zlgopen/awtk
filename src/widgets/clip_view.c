/**
 * File:   clip_view.c
 * Author: AWTK Develop Team
 * Brief:  clip view
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-12-12 Lou ZhiMing <louzhiming@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "widgets/clip_view.h"
#include "base/widget_vtable.h"

TK_DECL_VTABLE(clip_view) = {.size = sizeof(clip_view_t),
                             .type = WIDGET_TYPE_CLIP_VIEW,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = clip_view_create,
                             .on_paint_children = widget_on_paint_children_clip};

widget_t* clip_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(clip_view), x, y, w, h);
}

widget_t* clip_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, clip_view), NULL);

  return widget;
}
