/**
 * File:   semaphore.h
 * Author: AWTK Develop Team
 * Brief:  semaphore
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
 * 2019-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SEMAPHORE_H
#define TK_SEMAPHORE_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class tk_semaphore_t
 * 信号量。
 */
struct _tk_semaphore_t;
typedef struct _tk_semaphore_t tk_semaphore_t;

/**
 * @method tk_semaphore_create
 * 创建信号量对象。
 * @param {uint32_t} value 初始值。
 * @param {const char*} name 名称。
 *
 * @return {tk_semaphore_t*} semaphore对象。
 */
tk_semaphore_t* tk_semaphore_create(uint32_t value, const char* name);

/**
 * @method tk_semaphore_wait
 * 获取资源。
 * @param {tk_semaphore_t*} semaphore 信号量对象。
 * @param {uint32_t} timeout_ms 超时时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_semaphore_wait(tk_semaphore_t* semaphore, uint32_t timeout_ms);

/**
 * @method tk_semaphore_post
 * 释放资源。
 * @param {tk_semaphore_t*}    semaphore 信号量对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_semaphore_post(tk_semaphore_t* semaphore);

/**
 * @method tk_semaphore_destroy
 * 销毁信号量对象。
 * @param {tk_semaphore_t*}    semaphore 信号量对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_semaphore_destroy(tk_semaphore_t* semaphore);

END_C_DECLS

#endif /*TK_SEMAPHORE_H*/
