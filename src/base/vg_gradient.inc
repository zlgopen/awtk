
/**
 * File: vg_gradient.c
 * Author: AWTK Develop Team
 * Brief:  vector graphic gradient
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "vg_gradient.h"

vg_gradient_t* vg_gradient_init(vg_gradient_t* gradient, vg_gradient_type_t type, float sx,
                                float sy, float ex, float ey) {
  return_value_if_fail(gradient != NULL, NULL);
  memset(gradient, 0x00, sizeof(vg_gradient_t));
  gradient->sx = sx;
  gradient->sy = sy;
  gradient->ex = ex;
  gradient->ey = ey;
  gradient->type = type;

  return gradient;
}

vg_gradient_t* vg_gradient_create_radial(float sx, float sy, float ex, float ey) {
  vg_gradient_t* gradient = TKMEM_ZALLOC(vg_gradient_t);
  return_value_if_fail(gradient != NULL, NULL);

  return vg_gradient_init(gradient, VG_GRADIENT_RADIAL, sx, sy, ex, ey);
}

vg_gradient_t* vg_gradient_create_linear(float sx, float sy, float ex, float ey) {
  vg_gradient_t* gradient = TKMEM_ZALLOC(vg_gradient_t);
  return_value_if_fail(gradient != NULL, NULL);

  return vg_gradient_init(gradient, VG_GRADIENT_LINEAR, sx, sy, ex, ey);
}

ret_t vg_gradient_add_stop(vg_gradient_t* gradient, color_t color, float offset) {
  return_value_if_fail(gradient != NULL && gradient->nr < TK_GRADIENT_MAX_STOP_NR, RET_BAD_PARAMS);

  gradient->stops[gradient->nr].color = color;
  gradient->stops[gradient->nr++].offset = offset;

  return RET_OK;
}

ret_t vg_gradient_deinit(vg_gradient_t* gradient) {
  return_value_if_fail(gradient != NULL, RET_BAD_PARAMS);
  memset(gradient, 0x00, sizeof(vg_gradient_t));

  return RET_OK;
}

ret_t vg_gradient_destroy(vg_gradient_t* gradient) {
  return_value_if_fail(gradient != NULL, RET_BAD_PARAMS);

  vg_gradient_deinit(gradient);
  TKMEM_FREE(gradient);

  return RET_OK;
}

vg_gradient_stop_t* vg_gradient_get_stop(vg_gradient_t* gradient, uint32_t index) {
  return_value_if_fail(gradient != NULL, NULL);
  return_value_if_fail(gradient->nr > index, NULL);

  return gradient->stops + index;
}

color_t vg_gradient_get_first_color(vg_gradient_t* gradient) {
  return_value_if_fail(gradient != NULL && gradient->nr > 0, color_init(0xff, 0xff, 0xff, 0xff));

  return gradient->stops[0].color;
}

color_t vg_gradient_get_last_color(vg_gradient_t* gradient) {
  return_value_if_fail(gradient != NULL && gradient->nr > 0, color_init(0xff, 0xff, 0xff, 0xff));

  return gradient->stops[gradient->nr - 1].color;
}
