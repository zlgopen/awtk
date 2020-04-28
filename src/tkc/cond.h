/**
 * File:   cond.h
 * Author: AWTK Develop Team
 * Brief:  cond
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
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COND_VAR_H
#define TK_COND_VAR_H

#include "tkc/mutex.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class tk_cond_t
 * 条件变量。
 */
struct _tk_cond_t;
typedef struct _tk_cond_t tk_cond_t;

/**
 * @method tk_cond_create
 * 创建cond。
 *
 * @return {tk_cond_t*} cond对象。
 */
tk_cond_t* tk_cond_create(void);

/**
 * @method tk_cond_wait
 * 等待。
 * @param {tk_cond_t*}    cond cond对象。
 * @param {tk_mutex_t*}    mutex mutex对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_wait(tk_cond_t* cond, tk_mutex_t* mutex);

/**
 * @method tk_cond_wait_timeout
 * 等待指定时间。
 * @param {tk_cond_t*}    cond cond对象。
 * @param {tk_mutex_t*}    mutex mutex对象。
 * @param {uint32_t*}  timeout_ms 最长等待时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_wait_timeout(tk_cond_t* cond, tk_mutex_t* mutex, uint32_t ms);

/**
 * @method tk_cond_signal
 * 唤醒。
 * @param {tk_cond_t*}    cond cond对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_signal(tk_cond_t* cond);

/**
 * @method tk_cond_destroy
 * 销毁cond对象。
 * @param {tk_cond_t*}    cond cond对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_destroy(tk_cond_t* cond);

END_C_DECLS

#endif /*TK_COND_VAR_H*/
