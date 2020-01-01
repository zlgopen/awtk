/**
 * File:   children_layouter_default.h
 * Author: AWTK Develop Team
 * Brief:  children layouter default
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHILDREN_LAYOUTER_DEFAULT_H
#define TK_CHILDREN_LAYOUTER_DEFAULT_H

#include "base/children_layouter.h"

BEGIN_C_DECLS

/**
 * @class children_layouter_default_t
 * @parent children_layouter_t
 *
 * 缺省的子控件布局算法实现。
 *
 *> 用法请参考：[缺省子控件布局算法](
 *https://github.com/zlgopen/awtk/blob/master/docs/children_layouter_default.md)
 *
 */
typedef struct _children_layouter_default_t {
  children_layouter_t layouter;
  uint16_t rows;
  uint16_t cols;
  uint8_t x_margin;
  uint8_t y_margin;
  uint8_t spacing;
  uint8_t cols_is_width : 1;
  uint8_t rows_is_height : 1;
  uint8_t keep_invisible : 1;
  uint8_t keep_disable : 1;
  align_h_t align_h; /*for hbox*/
} children_layouter_default_t;

/**
 * @method children_layouter_default_create
 * @annotation ["constructor"]
 *
 * 创建子控件布局对象。
 *
 * @return {children_layouter_t*} 返回创建子控件布局对象。
 *
 */
children_layouter_t* children_layouter_default_create(void);

#define CHILDREN_LAYOUTER_DEFAULT "default"

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_DEFAULT_H*/
