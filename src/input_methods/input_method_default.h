/**
 * File:   input_method_default.h
 * Author: AWTK Develop Team
 * Brief:  input_method_default
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

#ifndef TK_INPUT_METHOD_DEFAULT_H
#define TK_INPUT_METHOD_DEFAULT_H

#include "base/input_method.h"

BEGIN_C_DECLS

/**
 * @class input_method_default_t
 * @parent input_method_t
 *
 * 缺省输入法实现。输入法会启用软键盘，但是开发者可以自定义输入法引擎。
 *
 * 目前可以选择不启用输入法引擎或启用拼音输入法引擎。
 *
 * 开发者也可以开发手写输入法或语音输入法，如果实现时遇到问题可以和我们交流。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method input_method_default_create
 * @annotation ["constructor"]
 *
 * 创建缺省输入法对象。
 *
 * @return {input_method_t*} 返回输入法对象。
 *
 */
input_method_t* input_method_default_create(void);

END_C_DECLS

#endif /*TK_INPUT_METHOD_DEFAULT_H*/
