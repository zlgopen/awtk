/**
 * File:   system_info.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  system info
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
 * 2018-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SYSTEM_INFO_H
#define TK_SYSTEM_INFO_H

#include "base/lcd.h"

BEGIN_C_DECLS

typedef struct _system_info_t {
  uint32_t lcd_w;
  uint32_t lcd_h;
  lcd_type_t lcd_type;
  float_t device_pixel_ratio;
} system_info_t;

system_info_t* system_info(void);

END_C_DECLS

#endif /*TK_SYSTEM_INFO_H*/
