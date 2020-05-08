/**
 * File:   mutex.h
 * Author: AWTK Develop Team
 * Brief:  mutex
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
 * 2018-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MUTEX_H
#define TK_MUTEX_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class tk_mutex_t
 * 互斥锁。
 */
struct _tk_mutex_t;
typedef struct _tk_mutex_t tk_mutex_t;

/**
 * @method tk_mutex_create
 * 创建mutex。
 *
 * @return {tk_mutex_t*} mutex对象。
 */
tk_mutex_t* tk_mutex_create(void);

/**
 * @method tk_mutex_lock
 * 加锁。
 * @param {tk_mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_lock(tk_mutex_t* mutex);

/**
 * @method tk_mutex_try_lock
 * 尝试加锁。
 * @param {tk_mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_try_lock(tk_mutex_t* mutex);

/**
 * @method tk_mutex_unlock
 * 解锁。
 * @param {tk_mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_unlock(tk_mutex_t* mutex);

/**
 * @method tk_mutex_destroy
 * 销毁mutex对象。
 * @param {tk_mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_destroy(tk_mutex_t* mutex);

END_C_DECLS

#endif /*TK_MUTEX_H*/
