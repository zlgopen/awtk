/**
 * File:   idle.h
 * Author: AWTK Develop Team
 * Brief:  idle manager
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

#ifndef TK_IDLE_MANAGER_H
#define TK_IDLE_MANAGER_H

#include "tkc/slist.h"
#include "tkc/idle_info.h"

BEGIN_C_DECLS

/**
 * @class idle_manager_t
 * @annotation ["scriptable"]
 * idle_manager_t管理器。
 */
struct _idle_manager_t {
  slist_t idles;
  uint32_t next_idle_id;
  uint32_t dispatch_times;
};

/**
 * @method idle_manager
 * 获取缺省的idle_manager_t管理器。
 * @annotation ["constructor"]
 * @return {idle_manager_t*} 返回idle_manager_t管理器对象。
 */
idle_manager_t* idle_manager(void);

/**
 * @method idle_manager_set
 * 设置缺省的idle_manager_t管理器。
 * @param {idle_manager_t*} idle_manager_t idle_manager_t管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_set(idle_manager_t* idle_manager);

/**
 * @method idle_manager_create
 * 创建idle_manager_t管理器。
 * @annotation ["constructor"]
 *
 * @return {idle_manager_t*} 返回idle_manager_t管理器对象。
 */
idle_manager_t* idle_manager_create(void);

/**
 * @method idle_manager_init
 * 初始化idle_manager_t管理器。
 * @annotation ["constructor"]
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 *
 * @return {idle_manager_t*} 返回idle_manager_t管理器对象。
 */
idle_manager_t* idle_manager_init(idle_manager_t* idle_manager);

/**
 * @method idle_manager_deinit
 * 析构idle_manager_t管理器。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_deinit(idle_manager_t* idle_manager);

/**
 * @method idle_manager_destroy
 * 析构并释放idle_manager_t管理器。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_destroy(idle_manager_t* idle_manager);

/**
 * @method idle_manager_count
 * 返回idle的个数。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 *
 * @return {uint32_t} 返回timer的个数。
 */
uint32_t idle_manager_count(idle_manager_t* idle_manager);

/**
 * @method idle_manager_dispatch
 * 检查全部idle的函数，如果时间到期，调用相应的idle函数。
 * @annotation ["private"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_dispatch(idle_manager_t* idle_manager);

/**
 * @method idle_manager_remove_all
 * 删除全部idle。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_remove_all(idle_manager_t* idle_manager);

/**
 * @method idle_manager_remove
 * 根据idle_id删除idle。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 * @param {uint32_t} idle_id idle_id。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id);

/**
 * @method idle_manager_append
 * 追加idle。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 * @param {idle_info_t*} idle idle对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t idle_manager_append(idle_manager_t* idle_manager, idle_info_t* idle);

/**
 * @method idle_manager_find
 * 查找指定ID的idle。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 * @param {uint32_t} idle_id idle_id。
 *
 * @return {idle_info_t*} 返回idle的信息。
 */
const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id);

/**
 * @method idle_manager_add
 * 添加idle。
 * @param {idle_manager_t*} idle_manager idle_manager_t管理器对象。
 * @param {idle_func_t*} on_idle idle回调函数。
 * @param {void*} ctx idle回调函数的上下文。
 *
 * @return {uint32_t} 返回idle的ID，TK_INVALID_ID表示失败。
 */
uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);

bool_t idle_manager_exist(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);

END_C_DECLS

#endif /*TK_IDLE_MANAGER_H*/
