/**
 * File:   window_animator_bottom_to_top.c
 * Author: AWTK Develop Team
 * Brief:  bottom_to_top window animator
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
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window_animators/window_animator_common.h"
#include "window_animators/window_animator_bottom_to_top.h"

static const window_animator_vtable_t s_window_animator_bottom_to_top_vt = {
    .overlap = TRUE,
    .type = "bottom_to_top",
    .desc = "bottom_to_top",
    .size = sizeof(window_animator_t),
    .draw_prev_window = window_animator_overlap_default_draw_prev,
    .draw_curr_window = window_animator_to_top_draw_curr};

window_animator_t* window_animator_bottom_to_top_create(bool_t open, object_t* args) {
  return window_animator_create(open, &s_window_animator_bottom_to_top_vt);
}
