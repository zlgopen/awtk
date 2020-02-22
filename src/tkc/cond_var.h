/**
 * File:   cond_var.h
 * Author: AWTK Develop Team
 * Brief:  cond_var
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

#ifndef TK_COND_VAR_SIMPLE_H
#define TK_COND_VAR_SIMPLE_H

#include "tkc/cond.h"

BEGIN_C_DECLS

/**
 * @class tk_cond_var_t
 * 简化版的条件变量。
 */
struct _tk_cond_var_t;
typedef struct _tk_cond_var_t tk_cond_var_t;

/**
 * @method tk_cond_var_create
 * 创建cond_var。
 *
 * @return {tk_cond_var_t*} cond_var对象。
 */
tk_cond_var_t* tk_cond_var_create(void);

/**
 * @method tk_cond_var_wait
 * 等待。
 * @param {tk_cond_var_t*}    cond_var cond_var对象。
 * @param {uint32_t*}  timeout_ms 最长等待时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms);

/**
 * @method tk_cond_var_awake
 * 唤醒。
 * @param {tk_cond_var_t*}    cond_var cond_var对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_var_awake(tk_cond_var_t* cond_var);

/**
 * @method tk_cond_var_destroy
 * 销毁cond_var对象。
 * @param {tk_cond_var_t*}    cond_var cond_var对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var);

END_C_DECLS

#endif /*TK_COND_VAR_SIMPLE_H*/
