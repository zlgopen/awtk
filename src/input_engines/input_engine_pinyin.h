/**
 * File:   input_engine_pinyin.h
 * Author: AWTK Develop Team
 * Brief:  input_engine_pinyin
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

#ifndef TK_INPUT_ENGINE_PINYIN_H
#define TK_INPUT_ENGINE_PINYIN_H

#include "base/input_engine.h"

BEGIN_C_DECLS

/**
 * @class input_engine_pinyin_t
 * @parent input_engine_t
 *
 * 拼音输入法引擎实现。
 *
 * 基于谷歌拼音输入法实现。
 *
 * @annotation["fake"]
 *
 */

/**
 * @method input_engine_create
 * @export none
 * @annotation ["constructor"]
 *
 * 创建拼音输入法引擎对象。
 *
 * @return {input_engine_t*} 返回输入法引擎对象。
 *
 */
input_engine_t* input_engine_create(void);

END_C_DECLS

#endif /*TK_INPUT_ENGINE_PINYIN_H*/
