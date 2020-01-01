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
 * 2019-11-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "cmsis_os.h"
#include "tkc/types_def.h"

#ifndef TK_RTOS_H
#define TK_RTOS_H

BEGIN_C_DECLS

ret_t rtos_init(void);

ret_t rtos_start(void);

END_C_DECLS

#endif /*TK_RTOS_H*/
