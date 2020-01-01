/**
 * File:   event_source_manager.h
 * Author: AWTK Develop Team
 * Brief:  event manager manager
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

#ifndef TK_EVENT_SOURCE_MANAGER_H
#define TK_EVENT_SOURCE_MANAGER_H

#include "tkc/darray.h"
#include "tkc/event_source.h"

BEGIN_C_DECLS

typedef ret_t (*event_source_manager_dispatch_t)(event_source_manager_t* manager);
typedef ret_t (*event_source_manager_destroy_t)(event_source_manager_t* manager);

/**
 * @class event_source_manager_t
 *
 * 事件源管理器。
 *
 */
struct _event_source_manager_t {
  darray_t sources;
  uint32_t last_dispatch_time;
  darray_t dispatching_sources;

  event_source_manager_dispatch_t dispatch;
  event_source_manager_destroy_t destroy;
};

/**
 * @method event_source_manager_init
 *
 * 初始化。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_init(event_source_manager_t* manager);

/**
 * @method event_source_manager_deinit
 *
 * ~初始化。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_deinit(event_source_manager_t* manager);

/**
 * @method event_source_manager_get_wakeup_time
 *
 * 获取wakeup时间(ms)
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {uint32_t} 返回wakeup时间(ms)。
 *
 */
uint32_t event_source_manager_get_wakeup_time(event_source_manager_t* manager);

/**
 * @method event_source_manager_dispatch
 *
 * 分发事件。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_dispatch(event_source_manager_t* manager);

/**
 * @method event_source_manager_add
 *
 * 增加事件源对象。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 * @param {event_source_t*} source event_source对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_add(event_source_manager_t* manager, event_source_t* source);

/**
 * @method event_source_manager_remove
 *
 * 移除事件源对象。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 * @param {event_source_t*} source event_source对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_remove(event_source_manager_t* manager, event_source_t* source);

/**
 * @method event_source_manager_remove_by_tag
 *
 * 移除所有tag相同的事件源对象。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 * @param {void*} tag tag。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_remove_by_tag(event_source_manager_t* manager, void* tag);

/**
 * @method event_source_manager_destroy
 *
 * 销毁事件源管理器。
 *
 * @param {event_source_manager_t*} manager event_source_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t event_source_manager_destroy(event_source_manager_t* manager);

#define TK_EVENT_SOURCE_MANAGER(obj) ((event_source_manager_t*)(obj))

/*public for test*/
bool_t event_source_manager_exist(event_source_manager_t* manager, event_source_t* source);

END_C_DECLS

#endif /*TK_EVENT_SOURCE_MANAGER_H*/
