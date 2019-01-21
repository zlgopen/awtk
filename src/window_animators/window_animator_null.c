/**
 * File:   window_animator_null.c
 * Author: AWTK Develop Team
 * Brief:  window_animator_null
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/window_animator.h"

window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  return NULL;
}

window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  return NULL;
}
