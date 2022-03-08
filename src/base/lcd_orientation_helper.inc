/**
 * File:   lcd_orientation_helper.inc
 * Author: AWTK Develop Team
 * Brief:  lcd orientation helper function.
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
 * 2022-02-22 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "lcd_orientation_helper.h"

#define LCD_ORIENTATION_POINT_ROTATE_BY_CW {                                                                           \
  switch (o) {                                                                                                         \
    case LCD_ORIENTATION_90: {                                                                                         \
      *y = tmp_x;                                                                                                      \
      *x = src_limit_h - tmp_y - 1;                                                                                    \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_180: {                                                                                        \
      *y = src_limit_h - tmp_y - 1;                                                                                    \
      *x = src_limit_w - tmp_x - 1;                                                                                    \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_270: {                                                                                        \
      *y = src_limit_w - tmp_x - 1;                                                                                    \
      *x = tmp_y;                                                                                                      \
      break;                                                                                                           \
    }                                                                                                                  \
    default:                                                                                                           \
      break;                                                                                                           \
  }                                                                                                                    \
}

#define LCD_ORIENTATION_POINT_ROTATE_BY_ACW {                                                                          \
  switch (o) {                                                                                                         \
    case LCD_ORIENTATION_90: {                                                                                         \
      *y = src_limit_w - tmp_x - 1;                                                                                    \
      *x = tmp_y;                                                                                                      \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_180: {                                                                                        \
      *y = src_limit_h - tmp_y - 1;                                                                                    \
      *x = src_limit_w - tmp_x - 1;                                                                                    \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_270: {                                                                                        \
      *y = tmp_x;                                                                                                      \
      *x = src_limit_h - tmp_y - 1;                                                                                    \
      break;                                                                                                           \
    }                                                                                                                  \
    default:                                                                                                           \
      break;                                                                                                           \
  }                                                                                                                    \
}

#define LCD_ORIENTATION_RECT_ROTATE_BY_ACW(type)  if (o != LCD_ORIENTATION_0)  {                                       \
  rect##type##fix(&rr, src_limit_w, src_limit_h);                                                                      \
  old_tl.x = rr.x;                                                                                                     \
  old_tl.y = rr.y;                                                                                                     \
  old_br.x = rr.x + rr.w - 1;                                                                                          \
  old_br.y = rr.y + rr.h - 1;                                                                                          \
  lcd_orientation_point##type##rotate_by_anticlockwise(&old_tl.x, &old_tl.y, o, src_limit_w, src_limit_h);             \
  lcd_orientation_point##type##rotate_by_anticlockwise(&old_br.x, &old_br.y, o, src_limit_w, src_limit_h);             \
  switch (o) {                                                                                                         \
    case LCD_ORIENTATION_90: {                                                                                         \
      new_tl = point##type##init(old_tl.x, old_br.y);                                                                  \
      new_br = point##type##init(old_br.x, old_tl.y);                                                                  \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_180: {                                                                                        \
      new_tl = old_br;                                                                                                 \
      new_br = old_tl;                                                                                                 \
      break;                                                                                                           \
    }                                                                                                                  \
    case LCD_ORIENTATION_270: {                                                                                        \
      new_tl = point##type##init(old_br.x, old_tl.y);                                                                  \
      new_br = point##type##init(old_tl.x, old_br.y);                                                                  \
      break;                                                                                                           \
    }                                                                                                                  \
    default:                                                                                                           \
      new_tl = old_tl;                                                                                                 \
      new_br = old_br;                                                                                                 \
      break;                                                                                                           \
  }                                                                                                                    \
  ret = lcd_orientation_rect##type##init_by_point(&new_tl, &new_br);                                                   \
}

ret_t lcd_orientation_point_rotate_by_clockwise(xy_t* x, xy_t* y, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  xy_t tmp_x = *x;
  xy_t tmp_y = *y;
  return_value_if_fail(x != NULL && y != NULL, RET_BAD_PARAMS);
  LCD_ORIENTATION_POINT_ROTATE_BY_CW;
  return RET_OK;
}

ret_t lcd_orientation_pointf_rotate_by_clockwise(float_t* x, float_t* y, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  float_t tmp_x = *x;
  float_t tmp_y = *y;
  return_value_if_fail(x != NULL && y != NULL, RET_BAD_PARAMS);
  LCD_ORIENTATION_POINT_ROTATE_BY_CW;
  return RET_OK;
}

ret_t lcd_orientation_point_rotate_by_anticlockwise(xy_t* x, xy_t* y, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  xy_t tmp_x = *x;
  xy_t tmp_y = *y;
  return_value_if_fail(x != NULL && y != NULL, RET_BAD_PARAMS);
  LCD_ORIENTATION_POINT_ROTATE_BY_ACW;        
  return RET_OK;
}

ret_t lcd_orientation_pointf_rotate_by_anticlockwise(float_t* x, float_t* y, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  float_t tmp_x = *x;
  float_t tmp_y = *y;
  return_value_if_fail(x != NULL && y != NULL, RET_BAD_PARAMS);
  LCD_ORIENTATION_POINT_ROTATE_BY_ACW;
  return RET_OK;
}

rectf_t lcd_orientation_rectf_init_by_point(const pointf_t* tl, const pointf_t* br) {
  float_t w = br->x - tl->x + 1.0f;
  float_t h = br->y - tl->y + 1.0f;
  rectf_t r = {tl->x, tl->y, w, h};
  assert(w >= 0);
  assert(h >= 0);
  return r;
}

rect_t lcd_orientation_rect_init_by_point(const point_t* tl, const point_t* br) {
  int32_t w = br->x - tl->x + 1;
  int32_t h = br->y - tl->y + 1;
  rect_t r = {tl->x, tl->y, w, h};
  assert(w >= 0);
  assert(h >= 0);
  return r;
}

rect_t lcd_orientation_rect_rotate_by_anticlockwise(const rect_t* rect, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  rect_t rr = *rect;
  rect_t ret = *rect;
  point_t new_tl, new_br, old_tl, old_br;
  return_value_if_fail(rect != NULL, rect_init(0, 0, 0, 0));
  LCD_ORIENTATION_RECT_ROTATE_BY_ACW(_);
  return ret;
}

rectf_t lcd_orientation_rectf_rotate_by_anticlockwise(const rectf_t* rect, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h) {
  rectf_t rr = *rect;
  rectf_t ret = *rect;
  pointf_t new_tl, new_br, old_tl, old_br;
  return_value_if_fail(rect != NULL, rectf_init(0, 0, 0, 0));
  LCD_ORIENTATION_RECT_ROTATE_BY_ACW(f_);
  return ret;
}
