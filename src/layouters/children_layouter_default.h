/**
 * File:   children_layouter_default.h
 * Author: AWTK Develop Team
 * Brief:  children layouter default
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHILDREN_LAYOUTER_DEFAULT_H
#define TK_CHILDREN_LAYOUTER_DEFAULT_H

#include "base/children_layouter.h"

BEGIN_C_DECLS

typedef struct _children_layouter_default_t {
  children_layouter_t layouter;
  uint16_t rows;
  uint16_t cols;
  uint8_t x_margin;
  uint8_t y_margin;
  uint8_t spacing;
  uint8_t cols_is_width : 1;
  uint8_t rows_is_height : 1;
} children_layouter_default_t;

children_layouter_t* children_layouter_default_create(void);

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_DEFAULT_H*/
