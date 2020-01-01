/**
 * File:   self_layouter_menu.h
 * Author: AWTK Develop Team
 * Brief:  self layouter menu
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
 * 2019-06-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SELF_LAYOUTER_MENU_H
#define TK_SELF_LAYOUTER_MENU_H

#include "base/self_layouter.h"

BEGIN_C_DECLS

/**
 * @class self_layouter_menu_t
 * @parent self_layouter_t
 *
 * menu的控件自身布局算法实现(一般用于popup或dialog窗口)。
 *
 *> 用法请参考：[缺省控件自身布局算法](
 *https://github.com/zlgopen/awtk/blob/master/docs/self_layouter_menu.md)
 *
 */
typedef struct _self_layouter_menu_t {
  self_layouter_t layouter;

  wh_t w;
  wh_t h;
  uint8_t w_attr;
  uint8_t h_attr;

  uint8_t x_attr;
  uint8_t y_attr;
  uint8_t position;
  point_t pressed;
  rect_t trigger_widget_rect;
} self_layouter_menu_t;

/**
 * @method self_layouter_menu_create
 * @annotation ["constructor"]
 *
 * 创建控件自身布局对象。
 *
 * @return {self_layouter_t*} 返回创建控件自身布局对象。
 *
 */
self_layouter_t* self_layouter_menu_create(void);

bool_t self_layouter_menu_is_valid(self_layouter_t* layouter);

#define SELF_LAYOUTER_MENU "menu"

/*public for test*/
ret_t widget_layout_self_menu_with_rect(self_layouter_t* layouter, widget_t* widget, rect_t* area);
ret_t widget_layout_self_set_trigger(self_layouter_t* layouter, point_t pressed,
                                     rect_t widget_rect);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_MENU_H*/
