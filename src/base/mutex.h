/**
 * File:   mutex.h
 * Author: AWTK Develop Team
 * Brief:  mutex
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MUTEX_H
#define TK_MUTEX_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class mutex_t
 * 互斥锁。
 */
struct _mutex_t;
typedef struct _mutex_t mutex_t;

/**
 * @method mutex_create
 * 创建mutex。
 *
 * @return {mutex_t*} mutex对象。
 */
mutex_t* mutex_create(void);

/**
 * @method mutex_lock
 * 加锁。
 * @param {mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutex_lock(mutex_t* mutex);

/**
 * @method mutex_unlock
 * 解锁。
 * @param {mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutex_unlock(mutex_t* mutex);

/**
 * @method mutex_destroy
 * 销毁mutex对象。
 * @param {mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mutex_destroy(mutex_t* mutex);

END_C_DECLS

#endif /*TK_MUTEX_H*/
