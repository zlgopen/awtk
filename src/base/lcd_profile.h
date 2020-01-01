/**
 * File:   lcd_profile.h
 * Author: AWTK Develop Team
 * Brief:  wrap lcd for performance profile.
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
 * 2018-03-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LCD_PROFILE_H
#define LCD_PROFILE_H

#include "base/lcd.h"

BEGIN_C_DECLS

/**
 * @class lcd_profile_t
 * @parent lcd_t
 * 对现有LCD对象进行包装，实现对各个函数的时间统计，用于辅助定位性能瓶颈。
 *
 */
typedef struct _lcd_profile_t {
  lcd_t lcd;

  /*private*/
  lcd_t* impl;

  uint32_t begin_frame_time;

  uint32_t total_cost;
  uint32_t swap_cost;
  uint32_t flush_cost;

  uint32_t draw_image_cost;
  uint32_t draw_image_times;
  uint32_t draw_image_pixels;

  uint32_t draw_text_cost;
  uint32_t draw_text_times;
  uint32_t draw_text_chars;

  uint32_t fill_cost;
  uint32_t fill_times;
  uint32_t fill_pixels;

  uint32_t stroke_cost;
  uint32_t stroke_times;
  uint32_t stroke_pixels;
} lcd_profile_t;

lcd_t* lcd_profile_create(lcd_t* impl);

#define LCD_PROFILE(lcd) ((lcd_profile_t*)(lcd))

END_C_DECLS

#endif /*LCD_PROFILE_H*/
