/**
 * File:   velocity.h
 * Author: AWTK Develop Team
 * Brief:  velocity tracker
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> adapted from android
 *
 */

#include "base/velocity.h"

velocity_t* velocity_reset(velocity_t* v) {
  return_value_if_fail(v != NULL, NULL);
  memset(v, 0x00, sizeof(velocity_t));

  return v;
}

static ret_t velocity_init(velocity_t* v, uint32_t time, xy_t x, xy_t y) {
  memset(v, 0x00, sizeof(velocity_t));

  v->time = time;
  v->x = x;
  v->y = y;

  return RET_OK;
}

static ret_t velocity_do_update(velocity_t* v, uint32_t time, xy_t x, xy_t y) {
  const float FILTER_TIME_CONSTANT = 0.010f;
  float dt = time > v->time ? (time - v->time) * 0.001f : 0.001f;
  float xv = (x - v->x) / dt;
  float yv = (y - v->y) / dt;

  v->time = time;
  if (v->degree == 0) {
    v->xv = xv;
    v->yv = yv;
    v->degree = 1;
  } else {
    float alpha = dt / (FILTER_TIME_CONSTANT + dt);
    if (v->degree == 1) {
      v->xv += (xv - v->xv) * alpha;
      v->yv += (yv - v->yv) * alpha;
    } else {
      float xa = (xv - v->xv) / dt;
      float ya = (yv - v->yv) / dt;
      if (v->degree == 1) {
        v->xa = xa;
        v->ya = ya;
        v->degree = 2;
      } else {
        v->xa += (xa - v->xa) * alpha;
        v->ya += (ya - v->ya) * alpha;
      }
      v->xv += (v->xa * dt) * alpha;
      v->yv += (v->ya * dt) * alpha;
    }
  }
  v->x = x;
  v->y = y;

  return RET_OK;
}

ret_t velocity_update(velocity_t* v, uint32_t time, xy_t x, xy_t y) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  if (v->time) {
    return velocity_do_update(v, time, x, y);
  } else {
    return velocity_init(v, time, x, y);
  }
}
