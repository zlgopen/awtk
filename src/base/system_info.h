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

/**
 * @class system_info_t
 * 当前系统的信息。
 */
typedef struct _system_info_t {
  /**
   * @property {uint32_t} lcd_w
   * @readonly
   * 显示屏的宽度。
   */
  uint32_t lcd_w;

  /**
   * @property {uint32_t} lcd_h
   * @readonly
   * 显示屏的高度。
   */
  uint32_t lcd_h;

  /**
   * @property {lcd_type_t} lcd_type
   * @readonly
   * 显示屏的类型。
   */
  lcd_type_t lcd_type;

  /**
   * @property {float_t} device_pixel_ratio
   * @readonly
   * 显示屏的密度。
   */
  float_t device_pixel_ratio;
} system_info_t;

/**
 * @method system_info
 * 获取system_info对象。
 * @constructor
 * @return {system_info_t*} 返回system_info对象。
 */
system_info_t* system_info(void);

END_C_DECLS

#endif /*TK_SYSTEM_INFO_H*/
