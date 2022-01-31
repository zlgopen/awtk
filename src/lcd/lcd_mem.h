/**
 * File:   lcd_mem.h
 * Author: AWTK Develop Team
 * Brief:  memory implemented lcd interface
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
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

  lcd_fb_dirty_rects_t fb_dirty_rects_list;

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

/**
 * @method lcd_mem_deinit
 * 清除 lcd_mem 对象中的数据
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t lcd_mem_deinit(lcd_mem_t* mem) {
  return_value_if_fail(mem != NULL && mem->base.begin_frame != NULL, RET_BAD_PARAMS);

  lcd_fb_dirty_rects_deinit(&(mem->fb_dirty_rects_list));
  if (mem->vgcanvas != NULL) {
    vgcanvas_destroy(mem->vgcanvas);
    mem->vgcanvas = NULL;
  }

  if (mem->own_offline_fb) {
    TKMEM_FREE(mem->offline_fb);
  }

  graphic_buffer_destroy(mem->online_gb);
  graphic_buffer_destroy(mem->offline_gb);

  return RET_OK;
}

/**
 * @method lcd_mem_set_offline_fb
 * 设置 lcd_mem 对象中的 offline_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * @param {uint8_t*} offline_fb offline_fb 地址。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t lcd_mem_set_offline_fb(lcd_mem_t* lcd, uint8_t* offline_fb) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);
  lcd->offline_fb = offline_fb;
  return lcd_fb_dirty_rects_add_fb_info(&(lcd->fb_dirty_rects_list), offline_fb);
}

/**
 * @method lcd_mem_get_offline_fb
 * 获取 lcd_mem 对象中的 offline_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * 
 * @return {uint8_t*} 返回 offline_fb 地址。
 */
static inline uint8_t* lcd_mem_get_offline_fb(lcd_mem_t* lcd) {
  return_value_if_fail(lcd != NULL, NULL);
  return lcd->offline_fb;
}

/**
 * @method lcd_mem_set_online_fb
 * 设置 lcd_mem 对象中的 online_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * @param {uint8_t*} online_fb online_fb 地址。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t lcd_mem_set_online_fb(lcd_mem_t* lcd, uint8_t* online_fb) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);
  lcd->online_fb = online_fb;
  return lcd_fb_dirty_rects_add_fb_info(&(lcd->fb_dirty_rects_list), online_fb);
}

/**
 * @method lcd_mem_get_online_fb
 * 获取 lcd_mem 对象中的 online_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * 
 * @return {uint8_t*} 返回 online_fb 地址。
 */
static inline uint8_t* lcd_mem_get_online_fb(lcd_mem_t* lcd) {
  return_value_if_fail(lcd != NULL, NULL);
  return lcd->online_fb;
}

/**
 * @method lcd_mem_set_next_fb
 * 设置 lcd_mem 对象中的 next_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * @param {uint8_t*} next_fb next_fb 地址。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t lcd_mem_set_next_fb(lcd_mem_t* lcd, uint8_t* next_fb) {
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);
  lcd->next_fb = next_fb;
  return lcd_fb_dirty_rects_add_fb_info(&(lcd->fb_dirty_rects_list), next_fb);
}

/**
 * @method lcd_mem_get_next_fb
 * 获取 lcd_mem 对象中的 next_fb
 * @export none
 * @param {lcd_mem_t*} mem lcd_mem 对象。
 * 
 * @return {uint8_t*} 返回 next_fb 地址。
 */
static inline uint8_t* lcd_mem_get_next_fb(lcd_mem_t* lcd) {
  return_value_if_fail(lcd != NULL, NULL);
  return lcd->next_fb;
}

END_C_DECLS

#endif /*TK_LCD_MEM_H*/
