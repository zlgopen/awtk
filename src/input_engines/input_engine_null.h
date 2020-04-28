/**
 * File:   input_engine_null.h
 * Author: AWTK Develop Team
 * Brief:  input_engine_null
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_INPUT_ENGINE_NULL_H
#define TK_INPUT_ENGINE_NULL_H

#include "base/input_engine.h"

BEGIN_C_DECLS

/**
 * @class input_engine_null_t
 * @parent input_engine_t
 *
 * 空输入法引擎实现。
 *
 * 缺省输入法可以选择启用软键盘，但不启用具体的输入法引擎，此时可以选择空输入法引擎实现。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method input_engine_create
 * @export none
 * @annotation ["constructor"]
 *
 * 创建空输入法引擎对象。
 *
 * @return {input_engine_t*} 返回输入法引擎对象。
 *
 */
input_engine_t* input_engine_create(void);

END_C_DECLS

#endif /*TK_INPUT_ENGINE_NULL_H*/
