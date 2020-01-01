/**
 * File:   time.h
 * Author: AWTK Develop Team
 * Brief:  time
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
 * 2018-04-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TIME_NOW_H
#define TK_TIME_NOW_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class time_now_t
 * @annotation ["scriptable", "fake"]
 * 获取当前时间的函数。
 * 这里的当前时间是相对的，在嵌入式系统一般相对于开机时间。
 * 它本身并没有任何意义，一般用来计算时间间隔，如实现定时器和动画等等。
 */

/**
 * @method time_now_s
 * 获取当前时间(秒)。
 * @annotation ["scriptable", "static"]
 *
 * @return {uint64_t} 返回当前时间(秒)。
 */
uint64_t time_now_s(void);

/**
 * @method time_now_ms
 * 获取当前时间(毫秒)。
 * @annotation ["scriptable", "static"]
 *
 * @return {uint64_t} 返回当前时间(毫秒)。
 */
uint64_t time_now_ms(void);

END_C_DECLS

#endif /*TK_TIME_NOW_H*/
