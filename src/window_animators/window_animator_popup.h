﻿/**
 * File:   window_animator_popup.h
 * Author: AWTK Develop Team
 * Brief:  popup window animator
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_WINDOW_ANIMATOR_POPUP_H
#define TK_WINDOW_ANIMATOR_POPUP_H

BEGIN_C_DECLS

window_animator_t* window_animator_popup_create(bool_t open, tk_object_t* args);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_POPUP_H*/
