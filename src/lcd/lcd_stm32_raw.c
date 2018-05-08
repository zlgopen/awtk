/**
 * File:   lcd_stm32_raw.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stm32_raw implemented lcd interface/
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "gui.h"
#include "lcd_driver.h"
#include "lcd/lcd_reg.h"

#define set_window_func TFT_SetWindow
#define write_data_func TFT_WriteData

#include "base/color.h"
#include "base/mem.h"
#include "lcd/rgb565.h"
#include "lcd/lcd_reg.inc"
