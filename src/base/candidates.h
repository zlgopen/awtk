/**
 * File:   candidates.h
 * Author: AWTK Develop Team
 * Brief:  input method text candidates
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANDIDATES_H
#define TK_CANDIDATES_H

#include "base/str.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class candidates_t
 * @parent widget_t
 * @scriptable no
 * 输入法候选字词控件。
 */
typedef struct _candidates_t {
  widget_t widget;
  /**
   * @property {bool_t} expanded
   * @readonly
   * 当前是否展开。
   */
  bool_t expanded;
  /**
   * @property {wh_t} normal_h
   * @readonly
   * 正常状态时的高度。
   */
  wh_t normal_h;
  /**
   * @property {uint32_t} event_id
   * @readonly
   * 用于注销事件。
   */
  uint32_t event_id;

  canvas_t* canvas;
} candidates_t;

/**
 * @method candidates_create
 * @constructor
 * 创建candidates对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* candidates_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define CANDIDATES(widget) ((candidates_t*)(widget))

END_C_DECLS

#endif /*TK_CANDIDATES_H*/
