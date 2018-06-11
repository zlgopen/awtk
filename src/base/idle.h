/**
 * File:   idle.h
 * Author: AWTK Develop Team
 * Brief:  idle manager
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IDLE_H
#define TK_IDLE_H

#include "base/array.h"

BEGIN_C_DECLS

struct _idle_info_t;
typedef struct _idle_info_t idle_info_t;

typedef ret_t (*idle_func_t)(const idle_info_t* idle);

struct _idle_info_t {
  idle_func_t on_idle;
  void* ctx;
  uint32_t id;
};

typedef struct _idle_manager_t {
  array_t idles[2];
  uint32_t active;
  uint32_t next_idle_id;
} idle_manager_t;

idle_manager_t* idle_manager(void);
ret_t idle_manager_set(idle_manager_t* idle_manager);

idle_manager_t* idle_manager_create(void);
idle_manager_t* idle_manager_init(idle_manager_t* idle_manager);
ret_t idle_manager_deinit(idle_manager_t* idle_manager);
ret_t idle_manager_destroy(idle_manager_t* idle_manager);

uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);
ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id);
const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id);
ret_t idle_manager_dispatch(idle_manager_t* idle_manager);

/**
 * @class idle_t
 * @scriptable
 * @fake
 * idle函数在paint之后执行。
 */

/**
 * @method idle_add
 * 增加一个idle。
 * @static
 * @scriptable custom
 * @param {idle_func_t} on_idle idle回调函数。
 * @param {void*} ctx idle回调函数的上下文。
 *
 * @return {uint32_t} 返回idle的ID，0表示失败。
 */
uint32_t idle_add(idle_func_t on_idle, void* ctx);

/**
 * @method idle_queue
 * 用于非GUI线程增加一个idle，本函数向主循环的事件队列中发送一个增加idle的请求。
 * @static
 * @scriptable no
 * @param {idle_func_t} on_idle idle回调函数。
 * @param {void*} ctx idle回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_queue(idle_func_t on_idle, void* ctx);

/**
 * @method idle_remove
 * 删除指定的idle。
 * @static
 * @scriptable custom
 * @param {uint32_t} idle_id idleID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_remove(uint32_t idle_id);

/**
 * @method idle_find
 * 查找指定ID的idle。
 * @private
 *
 * @return {idle_info_t*} 返回idle的信息。
 */
const idle_info_t* idle_find(uint32_t idle_id);

/**
 * @method idle_dispatch
 * 调用全部idle的函数。
 * @private
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_dispatch(void);

/**
 * @method idle_count
 * 返回idle的个数。
 * @static
 *
 * @return {uint32_t} 返回idle的个数。
 */
uint32_t idle_count(void);

END_C_DECLS

#endif /*TK_IDLE_H*/
