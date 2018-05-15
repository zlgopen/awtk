/**
 * File:   widget_animator.h
 * Author: AWTK Develop Team
 * Brief:  widget animator interface
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
 * 2018-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_H
#define TK_WIDGET_ANIMATOR_H

#include "base/easing.h"
#include "base/widget.h"

BEGIN_C_DECLS

struct _widget_animator_t;
typedef struct _widget_animator_t widget_animator_t;

typedef ret_t (*widget_animator_update_t)(widget_animator_t* animator, float_t percent);
typedef ret_t (*widget_animator_destroy_t)(widget_animator_t* animator);

/**
 * @class widget_animator_t
 * 控件动画接口。
 */
typedef struct _widget_animator_t {
  widget_t* widget;
  bool_t yoyo;
  bool_t repeat;
  bool_t reversed;
  uint32_t now; 
  uint32_t start_time; 
  uint32_t duration; 
  uint32_t timer_id; 

  easing_func_t easing;
  widget_animator_update_t update;
  widget_animator_destroy_t destroy;
} widget_animator_t;

ret_t widget_animator_init(widget_animator_t* animator, widget_t* widget, uint32_t duration, easing_func_t easing);
ret_t widget_animator_update(widget_animator_t* animator, float_t percent);
ret_t widget_animator_start(widget_animator_t* animator);
ret_t widget_animator_stop(widget_animator_t* animator);

ret_t widget_animator_set_yoyo(widget_animator_t* animator, bool_t value);
ret_t widget_animator_set_repeat(widget_animator_t* animator, bool_t value);
ret_t widget_animator_set_reversed(widget_animator_t* animator, bool_t value);

ret_t widget_animator_destroy(widget_animator_t* animator);

END_C_DECLS

#endif/*TK_WIDGET_ANIMATOR_H*/

