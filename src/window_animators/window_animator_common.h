/**
 * File:   window_animator_common.h
 * Author: AWTK Develop Team
 * Brief:  window animator common used functions.
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

#ifndef TK_WINDOW_ANIMATOR_COMMON_H
#define TK_WINDOW_ANIMATOR_COMMON_H

BEGIN_C_DECLS

ret_t window_animator_to_bottom_draw_curr(window_animator_t* wa);
ret_t window_animator_to_top_draw_curr(window_animator_t* wa);
ret_t window_animator_to_left_draw_curr(window_animator_t* wa);
ret_t window_animator_to_right_draw_curr(window_animator_t* wa);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_COMMON_H*/
