/**
 * File:   rtos.h
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
 * 2019-11-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/types_def.h"

#ifndef TK_RTOS_H
#define TK_RTOS_H

BEGIN_C_DECLS

ret_t rtos_init(void);
ret_t rtos_start(void);
void rtos_tick(void);
void rtos_delay(uint32_t ms);
bool_t rtos_is_running(void);

END_C_DECLS

#endif /*TK_RTOS_H*/
