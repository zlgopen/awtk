/**
 * File:   timer.h
 * Author: AWTK Develop Team
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIMER_MANAGER_H
#define TK_TIMER_MANAGER_H

#include "tkc/slist.h"
#include "tkc/timer_info.h"

BEGIN_C_DECLS

/**
 * @class timer_manager_t
 * @annotation ["scriptable"]
 *
 * 定时器管理器。
 */
struct _timer_manager_t {
  uint32_t next_timer_id;
  uint64_t last_dispatch_time;
  timer_get_time_t get_time;

  slist_t timers;
};

/**
 * @method timer_manager
 * 获取缺省的定时器管理器。
 * @annotation ["constructor"]
 * @return {timer_manager_t*} 返回定时器管理器对象。
 */
timer_manager_t* timer_manager(void);

/**
 * @method timer_manager_set
 * 设置缺省的定时器管理器。
 * @param {timer_manager_t*} timer_manager_t 定时器管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_set(timer_manager_t* timer_manager);

/**
 * @method timer_manager_create
 * 创建定时器管理器。
 * @annotation ["constructor"]
 * @param {timer_get_time_t} get_time 获取当前时间的函数。
 *
 * @return {timer_manager_t*} 返回定时器管理器对象。
 */
timer_manager_t* timer_manager_create(timer_get_time_t get_time);

/**
 * @method timer_manager_init
 * 初始化定时器管理器。
 * @annotation ["constructor"]
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 * @param {timer_get_time_t} get_time 获取当前时间的函数。
 *
 * @return {timer_manager_t*} 返回定时器管理器对象。
 */
timer_manager_t* timer_manager_init(timer_manager_t* timer_manager, timer_get_time_t get_time);

/**
 * @method timer_manager_deinit
 * 析构定时器管理器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_deinit(timer_manager_t* timer_manager);

/**
 * @method timer_manager_destroy
 * 析构并释放定时器管理器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_destroy(timer_manager_t* timer_manager);

/**
 * @method timer_manager_add
 * 添加定时器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 * @param {timer_func_t*} on_timer timer回调函数。
 * @param {void*} ctx timer回调函数的上下文。
 * @param {uint32_t} duration 时间。
 *
 * @return {uint32_t} 返回timer的ID，TK_INVALID_ID表示失败。
 */
uint32_t timer_manager_add(timer_manager_t* timer_manager, timer_func_t on_timer, void* ctx,
                           uint32_t duration);

/**
 * @method timer_manager_remove
 * 根据id删除定时器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 * @param {uint32_t} timer_id timer_id。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id);

/**
 * @method timer_manager_reset
 * 重置定时器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 * @param {uint32_t*} timer_id timer_id。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_reset(timer_manager_t* timer_manager, uint32_t timer_id);

/**
 * @method timer_manager_find
 * 查找指定ID的定时器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 * @param {uint32_t} timer_id timer_id
 *
 * @return {timer_info_t*} 返回timer的信息。
 */
const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id);

/**
 * @method timer_manager_dispatch
 * 检查全部定时器的函数，如果时间到期，调用相应的timer函数。
 * @annotation ["private"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_dispatch(timer_manager_t* timer_manager);

/**
 * @method timer_manager_count
 * 返回定时器的个数。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 *
 * @return {uint32_t} 返回timer的个数。
 */
uint32_t timer_manager_count(timer_manager_t* timer_manager);

/**
 * @method timer_manager_next_time
 * 返回最近的定时器到期时间。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。
 *
 * @return {uint64_t} 返回最近的timer到期时间。
 */
uint64_t timer_manager_next_time(timer_manager_t* timer_manager);

/**
 * @method timer_manager_append
 * 追加定时器。
 * @param {timer_manager_t*} timer_manager 定时器管理器对象。。
 * @param {timer_info_t*} timer timer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t timer_manager_append(timer_manager_t* timer_manager, timer_info_t* timer);

END_C_DECLS

#endif /*TK_TIMER_MANAGER_H*/
