/**
 * File:   lcd_orientation_helper.h
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

#include "tkc/rect.h"
#include "base/types_def.h"

/* private */
ret_t lcd_orientation_point_rotate_by_clockwise(xy_t* x, xy_t* y, lcd_orientation_t o,
                                                wh_t src_limit_w, wh_t src_limit_h);
ret_t lcd_orientation_pointf_rotate_by_clockwise(float_t* x, float_t* y, lcd_orientation_t o,
                                                 wh_t src_limit_w, wh_t src_limit_h);

ret_t lcd_orientation_point_rotate_by_anticlockwise(xy_t* x, xy_t* y, lcd_orientation_t o,
                                                    wh_t src_limit_w, wh_t src_limit_h);
ret_t lcd_orientation_pointf_rotate_by_anticlockwise(float_t* x, float_t* y, lcd_orientation_t o,
                                                     wh_t src_limit_w, wh_t src_limit_h);

rectf_t lcd_orientation_rectf_init_by_point(const pointf_t* tl, const pointf_t* br);
rect_t lcd_orientation_rect_init_by_point(const point_t* tl, const point_t* br);

rect_t lcd_orientation_rect_rotate_by_anticlockwise(const rect_t* rect, lcd_orientation_t o,
                                                    wh_t src_limit_w, wh_t src_limit_h);
rectf_t lcd_orientation_rectf_rotate_by_anticlockwise(const rectf_t* rect, lcd_orientation_t o,
                                                      wh_t src_limit_w, wh_t src_limit_h);
