/**
 * File:   event_source_manager_default.h
 * Author: AWTK Develop Team
 * Brief:  event manager manager implement with default
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

#ifndef TK_EVENT_SOURCE_MANAGER_DEFAULT_H
#define TK_EVENT_SOURCE_MANAGER_DEFAULT_H

#include "tkc/event_source_manager.h"

BEGIN_C_DECLS

/**
 * @class event_source_manager_default_t
 * @parent event_source_manager_t
 * @annotation ["fake"]
 *
 * 缺省的事件源管理器。
 *
 */

/**
 * @method event_source_manager_default_create
 *
 * 创建事件源管理器。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
event_source_manager_t* event_source_manager_default_create(void);

END_C_DECLS

#endif /*TK_EVENT_SOURCE_MANAGER_DEFAULT_H*/
