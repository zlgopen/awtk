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

  /**
   * @property {app_type_t} app_type
   * @annotation ["readable"]
   * 应用程序的类型。
   */
  app_type_t app_type;

  /**
   * @property {const char*} app_name
   * @annotation ["readable"]
   * 应用程序的名称。
   */
  const char* app_name;

  /**
   * @property {const char*} app_root
   * @annotation ["readable"]
   * 应用程序的根目录，用于定位资源文件。
   */
  const char* app_root;
} system_info_t;

/**
 * @method system_info
 * 获取system_info对象。
 * @annotation ["constructor"]
 * @return {system_info_t*} 返回system_info对象。
 */
system_info_t* system_info(void);

/**
 * @method system_info_init
 * 初始化system_info对象(PC软件才需调用)。
 * @param {app_type_t} app_type 应用程序的类型。
 * @param {const char*} app_name 应用程序的名称。
 * @param {const char*} app_root 应用程序的根目录，用于定位资源文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t system_info_init(app_type_t app_type, const char* app_name, const char* app_root);

END_C_DECLS

#endif /*TK_SYSTEM_INFO_H*/
