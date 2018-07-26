/**
 * File:   popup.h
 * Author: AWTK Develop Team
 * Brief:  popup
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_POPUP_H
#define TK_POPUP_H

#include "base/wstr.h"
#include "base/widget.h"
#include "base/window_animator.h"

BEGIN_C_DECLS

/**
 * @class popup_t
 * @parent widget_t
 * @scriptable
 * popup窗口。
 */
typedef struct _popup_t {
  widget_t widget;
  char* anim_hint;
  char* theme;
  char* script;
  bool_t close_when_click;
  bool_t close_when_click_outside;
} popup_t;

/**
 * @method popup_create
 * @constructor
 * 创建popup对象。
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} popup对象。
 */
widget_t* popup_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define POPUP(widget) ((popup_t*)(widget))

END_C_DECLS

#endif /*TK_POPUP_H*/
