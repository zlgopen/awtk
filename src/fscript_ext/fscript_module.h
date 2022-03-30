/**
 * File:   fscript_module.h
 * Author: AWTK Develop Team
 * Brief:  fscript module
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-03-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FSCRIPT_MODULE_H
#define TK_FSCRIPT_MODULE_H

#include "tkc/object.h"
#include "tkc/fscript.h"

BEGIN_C_DECLS

/**
 * @class fscript_module_t
 * @parent tk_object_t
 *
 * 用require加载外部模块。
 *
 */
typedef struct _fscript_module_t {
  tk_object_t object;

  /*private*/
  fscript_t* fscript;
} fscript_module_t;

/**
 * @method fscript_module_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 *
 * @param {const char*} url URL(用于读取数据)
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* fscript_module_create(const char* url);

/**
 * @method fscript_module_cast
 * 转换为fscript_module对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj fscript_module对象。
 *
 * @return {fscript_module_t*} fscript_module对象。
 */
fscript_module_t* fscript_module_cast(tk_object_t* obj);
#define FSCRIPT_MODULE(obj) fscript_module_cast(obj)

/**
 * @method fscript_module_register
 * 注册module函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_module_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_MODULE_H*/

