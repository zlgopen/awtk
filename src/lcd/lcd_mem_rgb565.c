/**
 * File:   lcd_mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  mem implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define LCD_FORMAT BITMAP_FMT_RGB565

#include "base/mem.h"
#include "lcd/lcd_mem.h"

typedef uint16_t pixel_t;
#include "blend/rgb565.inc"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_mem.inc"
