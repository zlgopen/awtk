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

#define LCD_FORMAT BITMAP_FMT_RGBA

#include "base/mem.h"
#include "lcd/lcd_mem.h"

typedef uint32_t pixel_t;
#include "blend/rgba8888.inc"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_mem.inc"
