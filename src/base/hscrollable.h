/**
 * File:   hscrollable.h
 * Author: AWTK Develop Team
 * Brief:  input method text hscrollable
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
 * 2019-05-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HSCROLLABLE_H
#define TK_HSCROLLABLE_H

#include "tkc/str.h"
#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class hscrollable_t
 * 帮助实现控件左右滚动的效果。
 */
typedef struct _hscrollable_t {
  point_t down;
  bool_t dragged;
  int32_t xoffset;
  int32_t xoffset_end;
  int32_t xoffset_save;
  int32_t virtual_w;

  widget_t* widget;
  velocity_t velocity;
  widget_animator_t* wa;
  bool_t always_scrollable;
} hscrollable_t;

hscrollable_t* hscrollable_create(widget_t* widget);
ret_t hscrollable_set_xoffset(hscrollable_t* hscrollable, int32_t xoffset);
ret_t hscrollable_set_virtual_w(hscrollable_t* hscrollable, int32_t virtual_w);
ret_t hscrollable_set_always_scrollable(hscrollable_t* hscrollable, bool_t always_scrollable);

ret_t hscrollable_on_event(hscrollable_t* hscrollable, event_t* e);
ret_t hscrollable_get_prop(hscrollable_t* hscrollable, const char* name, value_t* v);
ret_t hscrollable_set_prop(hscrollable_t* hscrollable, const char* name, const value_t* v);
ret_t hscrollable_on_paint_children(hscrollable_t* hscrollable, canvas_t* c);

ret_t hscrollable_destroy(hscrollable_t* hscrollable);

END_C_DECLS

#endif /*TK_HSCROLLABLE_H*/
