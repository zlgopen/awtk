/**
 * File:   lcd.c
 * Author: AWTK Develop Team
 * Brief:  lcd interface
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
 * 2018-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/lcd.h"
#include "base/vgcanvas.h"
#include "lcd/lcd_nanovg.h"
#include "lcd_vgcanvas.inc"

lcd_t* lcd_nanovg_init(native_window_t* window) {
  vgcanvas_t* vg = NULL;
  native_window_info_t info;
  return_value_if_fail(window != NULL, NULL);
  return_value_if_fail(native_window_get_info(window, &info) == RET_OK, NULL);

  vg = vgcanvas_create(info.w, info.h, 0, 0, window);
  return_value_if_fail(vg != NULL, NULL);

  return lcd_vgcanvas_init(info.w, info.h, vg);
}
