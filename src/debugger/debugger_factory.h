/**
 * File:   debugger_factory.h
 * Author: AWTK Develop Team
 * Brief:  debugger factory
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
 * 2022-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_FACTORY_H
#define TK_DEBUGGER_FACTORY_H

#include "debugger/debugger.h"

BEGIN_C_DECLS

/**
 * @class debugger_factory_t
 * @annotaion ["fake"]
 * 调试器工厂
 */

/**
 * @method debugger_factory_init
 * 初始化工厂。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_factory_init(void);

/**
 * @method debugger_factory_reg
 * 注册创建函数。
 * @param {const char*} lang 语言类型。
 * @param {debugger_fscript_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_factory_reg(const char* lang, debugger_fscript_create_t create);

/**
 * @method debugger_factory_create_debugger
 * 创建调试器。
 * @param {const char*} lang 语言类型。
 * @param {const char*} code_id  代码的ID。
 *
 * @return {debugger_t*} 返回调试器对象。
 */
debugger_t* debugger_factory_create_debugger(const char* lang, const char* code_id);

/**
 * @method debugger_factory_deinit
 * 释放资源。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_factory_deinit(void);

END_C_DECLS

#endif /*TK_DEBUGGER_FACTORY_H*/
