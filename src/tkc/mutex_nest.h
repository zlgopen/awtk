/**
 * File:   mutex_nest.h
 * Author: AWTK Develop Team
 * Brief:  mutex recursive
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MUTEX_NEST_H
#define TK_MUTEX_NEST_H

#include "tkc/mutex.h"

BEGIN_C_DECLS

/**
 * @class tk_mutex_nest_t
 * 嵌套互斥锁。允许同一个线程多次加锁。
 */
typedef struct _tk_mutex_nest_t {
  /*private*/
  tk_mutex_t* mutex;
  uint64_t owner;
  int32_t ref;
} tk_mutex_nest_t;

/**
 * @method tk_mutex_nest_create
 * 创建嵌套互斥锁。
 *
 * @return {tk_mutex_nest_t*} mutex_nest对象。
 */
tk_mutex_nest_t* tk_mutex_nest_create(void);

/**
 * @method tk_mutex_nest_lock
 * 加锁。
 * @param {tk_mutex_nest_t*}    mutex_nest mutex_nest对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_nest_lock(tk_mutex_nest_t* mutex_nest);

/**
 * @method tk_mutex_nest_try_lock
 * 尝试加锁。
 * @param {tk_mutex_nest_t*}    mutex_nest mutex_nest对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_nest_try_lock(tk_mutex_nest_t* mutex_nest);

/**
 * @method tk_mutex_nest_unlock
 * 解锁。只允许解锁当前线程所加的锁。
 * @param {tk_mutex_nest_t*}    mutex_nest mutex_nest对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_nest_unlock(tk_mutex_nest_t* mutex_nest);

/**
 * @method tk_mutex_nest_destroy
 * 销毁mutex_nest对象。
 * @param {tk_mutex_nest_t*}    mutex_nest mutex_nest对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mutex_nest_destroy(tk_mutex_nest_t* mutex_nest);

END_C_DECLS

#endif /*TK_MUTEX_NEST_H*/
