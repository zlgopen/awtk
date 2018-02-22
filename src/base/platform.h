/**
 * File:   platform.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  platform struct and utils functions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_PLATFORM_H
#define LFTK_PLATFORM_H

#include "base/types_def.h"

BEGIN_C_DECLS

ret_t platform_init(void);
uint32_t get_time_ms();

END_C_DECLS

#endif/*LFTK_PLATFORM_H*/

