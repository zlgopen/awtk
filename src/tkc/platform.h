/**
 * File:   platform.h
 * Author: AWTK Develop Team
 * Brief:  platform struct and utils functions.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PLATFORM_H
#define TK_PLATFORM_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class platform_t
 * @annotation ["fake"]
 * 平台接口，包括：获取时间、休眠等函数。
 */

/**
 * @method get_time_ms64
 * 获取当前时间。
 *
 * @return {uint64_t} 成功返回当前时间。
 */
uint64_t get_time_ms64(void);

/**
 * @method sleep_ms
 *
 * 睡眠指定时间。
 *
 * @param {uint32_t} ms 睡眠时间(毫秒)。
 *
 * @return {void} 无。
 */
void sleep_ms(uint32_t ms);

/**
 * @method platform_prepare
 *
 * 平台准备函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t platform_prepare(void);

END_C_DECLS

#endif /*TK_PLATFORM_H*/
