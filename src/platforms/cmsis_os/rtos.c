/**
 * File:   rtos.c
 * Author: AWTK Develop Team
 * Brief:  rtos
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
 * 2019-11-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "cmsis_os.h"
#include "rtos.h"

#ifdef _TOS_CONFIG_H_

ret_t rtos_init(void) {
  tos_knl_init();
  tos_robin_config(TOS_ROBIN_STATE_ENABLED, (k_timeslice_t)500u);

  return RET_OK;
}

ret_t rtos_start(void) {
  tos_knl_start();

  return RET_OK;
}

#endif /*_TOS_CONFIG_H_*/
