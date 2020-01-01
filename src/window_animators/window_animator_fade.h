/**
 * File:   window_animator_fade.h
 * Author: AWTK Develop Team
 * Brief:  fade window animator
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
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window_animator.h"

#ifndef TK_WINDOW_ANIMATOR_FADE_H
#define TK_WINDOW_ANIMATOR_FADE_H

BEGIN_C_DECLS

window_animator_t* window_animator_fade_create(bool_t open, object_t* args);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_FADE_H*/
