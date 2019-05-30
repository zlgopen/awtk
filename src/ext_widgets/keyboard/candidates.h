/**
 * File:   candidates.h
 * Author: AWTK Develop Team
 * Brief:  input method text candidates
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANDIDATES_H
#define TK_CANDIDATES_H

#include "tkc/str.h"
#include "base/widget.h"
#include "base/hscrollable.h"

BEGIN_C_DECLS

/**
 * @class candidates_t
 * @parent widget_t
 * @annotation ["widget"]
 * 输入法候选字词控件。
 */
typedef struct _candidates_t {
  widget_t widget;

  /*private*/
  bool_t expanded;
  wh_t normal_h;
  uint32_t event_id;
  canvas_t* canvas;
  hscrollable_t* hscrollable;
} candidates_t;

/**
 * @method candidates_create
 * 创建candidates对象
 * @annotation ["constructor"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* candidates_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method candidates_cast
 * 转换为candidates对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget candidates对象。
 *
 * @return {widget_t*} candidates对象。
 */
widget_t* candidates_cast(widget_t* widget);

#define CANDIDATES(widget) ((candidates_t*)(candidates_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(candidates);

END_C_DECLS

#endif /*TK_CANDIDATES_H*/
