/**
 * File:   rect.c
 * Author: AWTK Develop Team
 * Brief:  rect struct and utils functions.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/mem.h"
#include "tkc/rect.h"

ret_t rect_merge(rect_t* dr, const rect_t* r) {
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

rect_t rect_intersect(const rect_t* r1, const rect_t* r2) {
  int32_t top = 0;
  int32_t left = 0;
  int32_t bottom = 0;
  int32_t right = 0;
  int32_t bottom1 = 0;
  int32_t right1 = 0;
  int32_t bottom2 = 0;
  int32_t right2 = 0;
  rect_t r = rect_init(0, 0, 0, 0);

  return_value_if_fail(r1 != NULL && r2 != NULL, r);

  bottom1 = r1->y + r1->h - 1;
  bottom2 = r2->y + r2->h - 1;
  right1 = r1->x + r1->w - 1;
  right2 = r2->x + r2->w - 1;

  top = tk_max(r1->y, r2->y);
  left = tk_max(r1->x, r2->x);
  right = tk_min(right1, right2);
  bottom = tk_min(bottom1, bottom2);

  r.x = left;
  r.y = top;
  r.w = right >= left ? (right - left + 1) : 0;
  r.h = bottom >= top ? (bottom - top + 1) : 0;

  return r;
}

rect_t rect_fix(rect_t* r, wh_t max_w, wh_t max_h) {
  if (r->x < 0) {
    r->x = 0;
  }

  if (r->x > max_w) {
    r->x = max_w;
    r->w = 0;
  }

  if (r->y < 0) {
    r->y = 0;
  }

  if (r->y > max_h) {
    r->y = max_h;
    r->h = 0;
  }

  if (r->w < 0) {
    r->w = 0;
  }

  if (r->h < 0) {
    r->h = 0;
  }

  if ((r->x + r->w) > max_w) {
    r->w = max_w - r->x;
  }

  if ((r->y + r->h) > max_h) {
    r->h = max_h - r->y;
  }

  if (r->w < 0) {
    r->w = 0;
  }

  if (r->h < 0) {
    r->h = 0;
  }

  return *r;
}

rect_t* rect_create(xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t* r = TKMEM_ZALLOC(rect_t);
  return_value_if_fail(r != NULL, NULL);

  *r = rect_init(x, y, w, h);

  return r;
}

rect_t* rect_set(rect_t* r, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(r != NULL, NULL);

  *r = rect_init(x, y, w, h);

  return r;
}

rect_t* rect_cast(rect_t* rect) {
  return_value_if_fail(rect != NULL, NULL);

  return rect;
}

ret_t rect_destroy(rect_t* r) {
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(r);

  return RET_OK;
}

rect_t* rect_scale(rect_t* r, float_t scale) {
  return_value_if_fail(r != NULL, r);

  if (scale != 1.0f) {
    r->x = tk_roundi(r->x * scale);
    r->y = tk_roundi(r->y * scale);
    r->w = tk_roundi(r->w * scale);
    r->h = tk_roundi(r->h * scale);
  }

  return r;
}
