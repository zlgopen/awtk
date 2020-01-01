/**
 * File:   platform_time.c
 * Author: AWTK Develop Team
 * Brief:  default platform
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#define SOKOL_IMPL
#include "sokol/sokol_time.h"

uint64_t stm_now_ms() {
  uint64_t now = stm_now();

  return now / 1000000;
}

void stm_time_init(void) {
  stm_setup();
}
