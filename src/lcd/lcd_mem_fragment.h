/**
 * File:   lcd_mem_fragment.h
 * Author: AWTK Develop Team
 * Brief:  mem fragment based implemented lcd interface
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2019-11-14 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_MEM_FRAGMENT_H
#define LCD_MEM_FRAGMENT_H

#include "base/lcd.h"

BEGIN_C_DECLS

/**
 * @class lcd_mem_fragment_t
 * @parent lcd_t
 * 片段的LCD。
 *
 * 在一些低端平台没有足够的内存提供一个完整的framebuffer，此时我们用一小块内存模拟framebuffer，
 * 每次只画屏幕上一小块。这样可以有些避免屏幕闪烁的问题。
 *
 */

/**
 * @method lcd_mem_fragment_create
 * @export none
 * 创建lcd对象。
 *
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {lcd_t*} 返回lcd对象。
 */
lcd_t* lcd_mem_fragment_create(wh_t w, wh_t h);

/**
 * @method lcd_mem_fragment_get_buff
 * @export none
 *
 * 获取framebuffer的地址。
 *
 * @param {lcd_t*} lcd lcd对象。
 *
 * @return {uint8_t*} 返回framebuffer的地址。
 */
uint8_t* lcd_mem_fragment_get_buff(lcd_t* lcd);

END_C_DECLS

#endif /*LCD_MEM_FRAGMENT_H*/
