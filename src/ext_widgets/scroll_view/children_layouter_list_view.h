/**
 * File:   children_layouter_list_view.h
 * Author: AWTK Develop Team
 * Brief:  children layouter list view
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
 * 2019-05-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHILDREN_LAYOUTER_LIST_VIEW_H
#define TK_CHILDREN_LAYOUTER_LIST_VIEW_H

#include "base/children_layouter.h"

BEGIN_C_DECLS

/**
 * @class children_layouter_list_view_t
 * @parent children_layouter_t
 *
 * listview的子控件布局算法实现。
 *
 *> 用法请参考：[listview子控件布局算法](
 *https://github.com/zlgopen/awtk/blob/master/docs/children_layouter_list_view.md)
 *
 */
typedef struct _children_layouter_list_view_t {
  children_layouter_t layouter;
  uint8_t x_margin;
  uint8_t y_margin;
  uint16_t spacing;
  uint16_t cols;
  uint16_t item_height;
  uint16_t default_item_height;
  uint8_t keep_invisible : 1;
  uint8_t keep_disable : 1;
} children_layouter_list_view_t;

/**
 * @method children_layouter_list_view_create
 * @annotation ["constructor"]
 *
 * 创建子控件布局对象。
 *
 * @return {children_layouter_t*} 返回创建子控件布局对象。
 *
 */
children_layouter_t* children_layouter_list_view_create(void);

#define CHILDREN_LAYOUTER_LIST_VIEW "list_view"

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_LIST_VIEW_H*/
