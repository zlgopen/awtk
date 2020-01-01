/**
 * File:   event_source.h
 * Author: AWTK Develop Team
 * Brief:  event source interface.
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
 * 2019-09-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EVENT_SOURCE_H
#define TK_EVENT_SOURCE_H

#include "tkc/object.h"

BEGIN_C_DECLS

typedef int32_t (*event_source_get_fd_t)(event_source_t* source);
typedef uint32_t (*event_source_get_wakeup_time_t)(event_source_t* source);
typedef ret_t (*event_source_check_t)(event_source_t* source);
typedef ret_t (*event_source_dispatch_t)(event_source_t* source);
typedef ret_t (*event_source_on_event_t)(event_source_t* source);

/**
 * @class event_source_t
 * @parent object
 *
 * 表示一个事件源。
 *
 * 事件源有下列两种方式：
 *
 * * 对于有文件描述符的事件源(如socket)，get_fd返回一个有效的文件描述符。
 * * 对于定时器，则get_wakeup_time返回下次唤醒的时间。
 *
 */
struct _event_source_t {
  object_t object;

  event_source_check_t check;
  event_source_get_fd_t get_fd;
  event_source_dispatch_t dispatch;
  event_source_get_wakeup_time_t get_wakeup_time;

  void* tag;
  event_source_manager_t* manager;
};

/**
 * @method event_source_get_fd
 *
 * 获取文件描述符。
 *
 * @param {event_source_t*} source event_source对象。
 *
 * @return {int32_t} 返回文件描述符。
 *
 */
int32_t event_source_get_fd(event_source_t* source);

/**
 * @method event_source_dispatch
 *
 * 分发事件。
 *
 * @param {event_source_t*} source event_source对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_dispatch(event_source_t* source);

/**
 * @method event_source_set_tag
 *
 * 设置tag，方便通过tag一次移除多个事件源。
 *
 * @param {event_source_t*} source event_source对象。
 * @param {void*} tag tag。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_set_tag(event_source_t* source, void* tag);

/**
 * @method event_source_check
 *
 * 对于没有文件描述符的事件源，需要自己检查是否准备就绪。
 *
 * @param {event_source_t*} source event_source对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_check(event_source_t* source);

/**
 * @method event_source_get_wakeup_time
 *
 * 获取唤醒时间(ms)。
 *
 * @param {event_source_t*} source event_source对象。
 *
 * @return {uint32_t} 返回唤醒时间(ms)。
 *
 */
uint32_t event_source_get_wakeup_time(event_source_t* source);

#define EVENT_SOURCE(obj) ((event_source_t*)(obj))

END_C_DECLS

#endif /*TK_EVENT_SOURCE_H*/
