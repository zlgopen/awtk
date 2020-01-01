/**
 * File:   input_method_sdl.h
 * Author: AWTK Develop Team
 * Brief:  input_method_sdl
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
 * 2019-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_INPUT_METHOD_SDL_H
#define TK_INPUT_METHOD_SDL_H

#include "base/input_method.h"

BEGIN_C_DECLS

/**
 * @class input_method_sdl_t
 * @parent input_method_t
 *
 * SDL输入法实现。
 *
 * SDL输入法使用平台自身的输入法引擎，一般在桌面Linux、MacOS、Windows、Android和iOS上使用。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method input_method_sdl_create
 * @annotation ["constructor"]
 *
 * 创建SDL输入法对象。
 *
 * @return {input_method_t*} 返回输入法对象。
 *
 */
input_method_t* input_method_sdl_create(void);

END_C_DECLS

#endif /*TK_INPUT_METHOD_SDL_H*/
