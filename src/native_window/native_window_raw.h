/**
 * File:   native_window_raw.h
 * Author: AWTK Develop Team
 * Brief:  native window raw
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NATIVE_WINDOW_RAW_H
#define TK_NATIVE_WINDOW_RAW_H

#include "base/lcd.h"
#include "base/native_window.h"

BEGIN_C_DECLS

ret_t native_window_raw_deinit(void);
ret_t native_window_raw_init(lcd_t* lcd);

END_C_DECLS

#endif /*TK_NATIVE_WINDOW_RAW_H*/
