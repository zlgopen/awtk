/**
 * File:   lcd_mem.h
 * Author: AWTK Develop Team
 * Brief:  memory implemented lcd interface
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LCD_MEM_H
#define TK_LCD_MEM_H

#include "base/lcd.h"
#include "base/graphic_buffer.h"

BEGIN_C_DECLS

typedef ret_t (*lcd_mem_wait_vbi_t)(void* ctx);

typedef struct _lcd_mem_t {
  lcd_t base;
  uint8_t* offline_fb;
  uint8_t* online_fb;
  uint8_t* next_fb;
  vgcanvas_t* vgcanvas;

  uint32_t online_line_length;
  uint32_t line_length;
  bitmap_format_t format;
  bool_t own_offline_fb;

  graphic_buffer_t* online_gb;
  graphic_buffer_t* offline_gb;

  /*VBI: vertical blank interrupt。用于2fb等待当前显示完成，以便把下一帧的数据从offline fb拷贝到online fb，从而避免因为同时访问online fb数据造成闪烁。*/
  lcd_mem_wait_vbi_t wait_vbi;
  void* wait_vbi_ctx;
} lcd_mem_t;

#define lcd_mem_set_line_length(lcd, value) lcd_set_line_length(lcd, value);

/**
 * @method lcd_resize
 * 设置等待VBI事件到来的回调函数。
 * > VBI: vertical blank interrupt。用于2fb等待当前显示完成，以便把下一帧的数据从offline fb拷贝到online fb，从而避免因为同时访问online fb数据造成闪烁。
 * @export none
 * @param {lcd_t*} lcd lcd对象。
 * @param {lcd_mem_wait_vbi_t} wait_vbi 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t lcd_mem_set_wait_vbi(lcd_t* lcd, lcd_mem_wait_vbi_t wait_vbi, void* ctx) {
  lcd_mem_t* mem = ((lcd_mem_t*)lcd);
  return_value_if_fail(mem != NULL, RET_BAD_PARAMS);

  mem->wait_vbi = wait_vbi;
  mem->wait_vbi_ctx = ctx;

  return RET_OK;
}

END_C_DECLS

#endif /*TK_LCD_MEM_H*/
