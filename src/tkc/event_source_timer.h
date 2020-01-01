/**
 * File:   event_source_timer.h
 * Author: AWTK Develop Team
 * Brief:  event source timer
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EVENT_SOURCE_TIMER_H
#define TK_EVENT_SOURCE_TIMER_H

#include "tkc/event_source.h"
#include "tkc/timer_manager.h"

BEGIN_C_DECLS

struct _event_source_timer_t;
typedef struct _event_source_timer_t event_source_timer_t;

/**
 * @class event_source_timer_t
 * @parent object
 *
 * timer事件源。
 *
 */
struct _event_source_timer_t {
  event_source_t event_source;

  timer_manager_t* timer_manager;
};

/**
 * @method event_source_timer_create
 *
 * 创建timer事件源。
 *
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 *
 * @return {event_source_t*} 返回事件源对象。
 *
 */
event_source_t* event_source_timer_create(timer_manager_t* timer_manager);

#define EVENT_SOURCE_TIMER(obj) ((event_source_timer_t*)(obj))

END_C_DECLS

#endif /*TK_EVENT_SOURCE_TIMER_H*/
