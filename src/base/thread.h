/**
 * File:   thread.h
 * Author: AWTK Develop Team
 * Brief:  thread
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

#ifndef TK_THREAD_H
#define TK_THREAD_H

#include "base/types_def.h"

/**
 * @class thread_t
 * 线程对象。
 */
struct _thread_t;
typedef struct _thread_t thread_t;

typedef void* (*thread_entry_t)(void* args);

BEGIN_C_DECLS

/**
 * @method thread_create
 * 创建thread对象。
 *
 * @return {thread_t*} thread对象。
 */
thread_t* thread_create(thread_entry_t entry, void* args);

/**
 * @method thread_start
 * 启动线程。
 * @param {thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t thread_start(thread_t* thread);

/**
 * @method thread_join
 * 等待线程退出。
 * @param {thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t thread_join(thread_t* thread);

/**
 * @method thread_destroy
 * 销毁thread对象。
 * @param {thread_t*}    thread thread对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t thread_destroy(thread_t* thread);

END_C_DECLS

#endif /*TK_THREAD_H*/
