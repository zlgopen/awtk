/**
 * File:   system_info.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  system info
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"

static system_info_t info;

system_info_t* system_info() { return &info; }
