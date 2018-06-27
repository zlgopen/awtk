/**
 * File:   rect.c
 * Author: AWTK Develop Team
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
      xy_t x = tk_min(dr->x, r->x);
      xy_t y = tk_min(dr->y, r->y);
      wh_t right = tk_max((r->x + r->w), (dr->x + dr->w));
      wh_t bottom = tk_max((r->y + r->h), (dr->y + dr->h));

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

rect_t rect_init(xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;

  return r;
}

rect_t rect_fix(rect_t* r, wh_t max_w, wh_t max_h) {
  if(r->x < 0) {
    r->x = 0;
  }

  if(r->x > max_w) {
    r->x = max_w;
    r->w = 0;
  }

  if(r->y < 0) {
    r->y = 0;
  }

  if(r->y > max_h) {
    r->y = max_h;
    r->h = 0;
  }

  if(r->w < 0) {
    r->w = 0;
  }

  if(r->h < 0) {
    r->h = 0;
  }

  if((r->x + r->w) > max_w) {
    r->w = max_w - r->x;
  }

  if((r->y + r->h) > max_h) {
    r->h = max_h - r->y;
  }

  if(r->w < 0) {
    r->w = 0;
  }

  if(r->h < 0) {
    r->h = 0;
  }

  return *r;
}
