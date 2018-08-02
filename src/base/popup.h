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
 * @annotation ["scriptable"]
 * popup窗口。
 */
typedef struct _popup_t {
  widget_t widget;

  /**
   * @property {char*} theme
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 主题资源的名称。
   */
  char* theme;
  /**
   * @property {char*} anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 窗口动画名称。
   */
  char* anim_hint;
  /**
   * @property {bool_t} close_when_click
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击时是否关闭窗口。
   */
  bool_t close_when_click;
  /**
   * @property {bool_t} close_when_click_outside
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 点击到窗口外时是否关闭窗口。
   */
  bool_t close_when_click_outside;
} popup_t;

/**
 * @method popup_create
 * @annotation ["constructor", "scriptable"]
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
