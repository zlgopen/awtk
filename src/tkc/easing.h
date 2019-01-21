/**
 * File:   easing.h
 * Author: AWTK Develop Team
 * Brief: easing functions.
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EASING_H
#define TK_EASING_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef enum _easing_type_t {
  EASING_LINEAR = 0,
  EASING_QUADRATIC_IN,
  EASING_QUADRATIC_OUT,
  EASING_QUADRATIC_INOUT,
  EASING_CUBIC_IN,
  EASING_CUBIC_OUT,
  EASING_SIN_IN,
  EASING_SIN_OUT,
  EASING_SIN_INOUT,
  EASING_POW_IN,
  EASING_POW_OUT,
  EASING_POW_INOUT,
  EASING_CIRCULAR_IN,
  EASING_CIRCULAR_OUT,
  EASING_CIRCULAR_INOUT,
  EASING_ELASTIC_IN,
  EASING_ELASTIC_OUT,
  EASING_ELASTIC_INOUT,
  EASING_BACK_IN,
  EASING_BACK_OUT,
  EASING_BACK_INOUT,
  EASING_BOUNCE_IN,
  EASING_BOUNCE_OUT,
  EASING_BOUNCE_INOUT,
  EASING_FUNC_NR
} easing_type_t;

typedef float_t (*easing_func_t)(float_t k);

easing_func_t easing_get(easing_type_t type);

END_C_DECLS

#endif /*TK_EASING_H*/
