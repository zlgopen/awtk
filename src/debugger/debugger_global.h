/**
 * File:   debugger_global.h
 * Author: AWTK Develop Team
 * Brief:  debugger global
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_GLOBAL_H
#define TK_DEBUGGER_GLOBAL_H

#include "debugger/debugger_fscript.h"

BEGIN_C_DECLS

/**
 * @class debugger_global_t
 * @annotaion ["fake"]
 * 调试器全局函数。
 */

/**
 * @method debugger_global_init
 * 启用调试器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_global_init(void);

/**
 * @method debugger_global_deinit
 * 禁用调试器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_global_deinit(void);

END_C_DECLS

#endif /*TK_DEBUGGER_GLOBAL_H*/
