/**
 * File:   velocity.h
 * Author: AWTK Develop Team
 * Brief:  velocity tracker
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VELOCITY_H
#define TK_VELOCITY_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class velocity_t
 * velocity。
 */
typedef struct _velocity_t {
  uint32_t time;
  uint32_t degree;

  float x;
  float xv;
  float xa;
  float y;
  float yv;
  float ya;
} velocity_t;

velocity_t* velocity_reset(velocity_t* v);

ret_t velocity_update(velocity_t* v, uint32_t time, xy_t x, xy_t y);

END_C_DECLS

#endif /*TK_VELOCITY_H*/
