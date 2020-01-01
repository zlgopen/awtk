/**
 * File:   window_animator_slide_up.h
 * Author: AWTK Develop Team
 * Brief:  slide window animator
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
 * 2019-03-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window_animator.h"

#ifndef TK_WINDOW_ANIMATOR_SLIDE_H
#define TK_WINDOW_ANIMATOR_SLIDE_H

BEGIN_C_DECLS

typedef struct _window_animator_slide_t {
  window_animator_t window_animator;
  uint8_t start_alpha;
  uint8_t end_alpha;
} window_animator_slide_t;

ret_t window_animator_slide_init(window_animator_t* wa);
ret_t window_animator_slide_draw_prev(window_animator_t* wa);
window_animator_t* window_animator_slide_create(bool_t open, object_t* args,
                                                const window_animator_vtable_t* vt);

#define WINDOW_ANIMATOR_SLIDE_ARG_START_ALPHA "start_alpha"
#define WINDOW_ANIMATOR_SLIDE_ARG_END_ALPHA "end_alpha"
#define WINDOW_ANIMATOR_SLIDE_ARG_ALPHA "alpha"

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_SLIDE_H*/
