/**
 * File:   rect.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rect struct and utils functions.
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
#include "base/rect.h"

ret_t rect_merge(rect_t* dr, rect_t* r) {
  return_value_if_fail(r != NULL && dr != NULL, RET_BAD_PARAMS);

  if (r->w > 0 && r->h > 0) {
    if (dr->w > 0 && dr->h > 0) {
      xy_t x = ftk_min(dr->x, r->x);
      xy_t y = ftk_min(dr->y, r->y);
      wh_t right = ftk_max((r->x + r->w), (dr->x + dr->w));
      wh_t bottom = ftk_max((r->y + r->h), (dr->y + dr->h));

      dr->x = x;
      dr->y = y;
      dr->w = right - x;
      dr->h = bottom - y;
    } else {
      *dr = *r;
    }
  }

  return RET_OK;
}

bool_t rect_contains(rect_t* r, xy_t x, xy_t y) {
  return_value_if_fail(r != NULL, FALSE);

  return (x >= r->x && x < (r->x + r->w)) && (y >= r->y && y < (r->y + r->h));
}
