/**
 * File:   event_source_fd.h
 * Author: AWTK Develop Team
 * Brief:  event source fd
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

#ifndef TK_EVENT_SOURCE_FD_H
#define TK_EVENT_SOURCE_FD_H

#include "tkc/event_source.h"

BEGIN_C_DECLS

struct _event_source_fd_t;
typedef struct _event_source_fd_t event_source_fd_t;

/**
 * @class event_source_fd_t
 * @parent object
 *
 * fd事件源。用于文件、管道和套接字。
 *
 */
struct _event_source_fd_t {
  event_source_t event_source;

  int fd;
  void* ctx;
  void* ctx2;
  event_source_on_event_t on_event;
};

/**
 * @method event_source_fd_create
 *
 * 创建fd事件源。
 *
 * @param {int} fd 可以用select等待的文件描述符。
 * @param {event_source_on_event_t} on_event 用户的事件处理函数。
 * @param {void*} ctx on_event函数的上下文。
 *
 * @return {event_source_t*} 返回事件源对象。
 *
 */
event_source_t* event_source_fd_create(int fd, event_source_on_event_t on_event, void* ctx);

#define EVENT_SOURCE_FD(obj) ((event_source_fd_t*)(obj))

END_C_DECLS

#endif /*TK_EVENT_SOURCE_FD_H*/
