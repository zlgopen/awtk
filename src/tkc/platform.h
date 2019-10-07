/**
 * File:   platform.h
 * Author: AWTK Develop Team
 * Brief:  platform struct and utils functions.
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

uint64_t get_time_ms64(void);
void sleep_ms(uint32_t ms);

ret_t platform_prepare(void);

END_C_DECLS

#endif /*TK_PLATFORM_H*/
