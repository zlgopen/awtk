/**
 * File:   system_info.h
 * Author: AWTK Develop Team
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
   * @annotation ["readable"]
   * 显示屏的宽度。
   */
  uint32_t lcd_w;

  /**
   * @property {uint32_t} lcd_h
   * @annotation ["readable"]
   * 显示屏的高度。
   */
  uint32_t lcd_h;

  /**
   * @property {lcd_type_t} lcd_type
   * @annotation ["readable"]
   * 显示屏的类型。
   */
  lcd_type_t lcd_type;

  /**
   * @property {float_t} device_pixel_ratio
   * @annotation ["readable"]
   * 显示屏的密度。
   */
  float_t device_pixel_ratio;

  /**
   * @property {lcd_orientation_t} lcd_orientation
   * @annotation ["readable"]
   * 显示屏的旋转角度。
   */
  lcd_orientation_t lcd_orientation;
} system_info_t;

/**
 * @method system_info
 * 获取system_info对象。
 * @annotation ["constructor"]
 * @return {system_info_t*} 返回system_info对象。
 */
system_info_t* system_info(void);

END_C_DECLS

#endif /*TK_SYSTEM_INFO_H*/
