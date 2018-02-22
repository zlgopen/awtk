/**
 * File:   platform_default.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  default platform
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/platform.h"

ret_t platform_prepare() {
  timer_init(get_time_ms);

  return RET_OK;
}

uint32_t get_time_ms() {
  /**/
  return 0;
}
