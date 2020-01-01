/**
 * File:   lcd_mono.h
 * Author: AWTK Develop Team
 * Brief:  mono lcd
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
 * 2019-08-10 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_MONO_H
#define LCD_MONO_H

#include "base/lcd.h"

BEGIN_C_DECLS

/**
 * @class lcd_mono_t
 * @parent lcd_t
 *
 * 单色LCD。
 *
 * lcd\_mono是一个特殊的Framebuffer，它用一个比特来表示一个pixel。
 *
 * lcd\_mono只是负责硬件无关的逻辑处理，调用者需要在创建时提供一个flush回调函数，
 * 在flush函数中把脏矩形中的数据刷新到硬件中。
 *
 */
typedef struct _lcd_mono_t {
  lcd_t lcd;

  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * Framebuffer首地址。
   */
  uint8_t* data;

  /**
   * @property {void*} ctx
   * @annotation ["private"]
   * flush/on_destroy回调函数的上下文信息。
   */
  void* ctx;

  /**
   * @property {lcd_destroy_t} on_destroy
   * @annotation ["private"]
   * lcd销毁时的回调函数。
   */
  lcd_destroy_t on_destroy;
} lcd_mono_t;

/**
 * @method lcd_mono_create
 * 创建单色LCD对象。
 *
 * @annotation ["constructor"]
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 * @param {lcd_flush_t} flush 用于刷新数据到硬件的回调函数。
 * @param {lcd_destroy_t} on_destroy 销毁lcd时的回调函数。
 * @param {void*} ctx flush/on_destroy回调函数的上下文。
 *
 * @return {lcd_t*} lcd对象。
 */
lcd_t* lcd_mono_create(wh_t w, wh_t h, lcd_flush_t flush, lcd_destroy_t on_destroy, void* ctx);

END_C_DECLS

#endif /*LCD_MONO_H*/
