/**
 * File:   lcd_mem_special.h
 * Author: AWTK Develop Team
 * Brief:  lcd_mem_special
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
 * 2019-06-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_MEM_SPECIAL_H
#define LCD_MEM_SPECIAL_H

#include "base/lcd.h"
#include "lcd/lcd_mem.h"

BEGIN_C_DECLS

/**
 * @class lcd_mem_special_t
 * @parent lcd_t
 * 特殊的LCD。
 *
 * 通过重载flush函数来提交GUI绘制的结果到硬件。适用于：
 *
 * * lcd 硬件不支持framebuffer，如SPI接口的屏。
 * * lcd 硬件支持framebuffer，但是其格式特殊，AWTK目前不支持。可以用flush进行格式转换。
 *
 *
 */
typedef struct _lcd_mem_special_t {
  lcd_t base;
  lcd_mem_t* lcd_mem;
  bitmap_format_t format;

  /*custom info*/
  void* ctx;
  lcd_flush_t on_flush;
  lcd_resize_t on_resize;
  lcd_destroy_t on_destroy;
} lcd_mem_special_t;

/**
 * @method lcd_mem_special_create
 *
 * 创建lcd对象。
 *
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 * @param {bitmap_format_t} format 离线lcd的格式。一般用 BITMAP_FMT_BGR565 或 BITMAP_FMT_RGBA8888。
 * @param {lcd_flush_t} flush 回调函数，用于刷新GUI数据到实际的LCD。
 * @param {lcd_resize_t} on_resize 用于调整LCD的大小。一般用NULL即可。
 * @param {lcd_destroy_t} on_destroy lcd销毁时的回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {lcd_t*} 返回lcd对象。
 */
lcd_t* lcd_mem_special_create(wh_t w, wh_t h, bitmap_format_t fmt, lcd_flush_t on_flush,
                              lcd_resize_t on_resize, lcd_destroy_t on_destroy, void* ctx);

END_C_DECLS

#endif /*LCD_MEM_SPECIAL_H*/
