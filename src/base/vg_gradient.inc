
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

vg_gradient_t* vg_gradient_init_radial(vg_gradient_t* gradient, float x0, float y0, float r0, float x1, float y1, float r1) {
  vg_gradient_radial_info_t* info = NULL;
  return_value_if_fail(gradient != NULL, NULL);

  memset(gradient, 0x00, sizeof(vg_gradient_t));
  info = &(gradient->info.radial);
  info->x0 = x0;
  info->y0 = y0;
  info->r0 = r0;
  info->x1 = x1;
  info->y1 = y1;
  info->r1 = r1;
  gradient->gradient.type = GRADIENT_RADIAL;

  return gradient;
}

vg_gradient_t* vg_gradient_create_radial(float x0, float y0, float r0, float x1, float y1, float r1) {
  vg_gradient_t* gradient = TKMEM_ZALLOC(vg_gradient_t);
  return_value_if_fail(gradient != NULL, NULL);

  return vg_gradient_init_radial(gradient, x0, y0, r0, x1, y1, r1);
}

vg_gradient_t* vg_gradient_init_with_gradient(vg_gradient_t* gradient, const rect_t* rect, const gradient_t* g) {
  vg_gradient_linear_info_t* info = NULL;
  return_value_if_fail(gradient != NULL, NULL);

  memset(gradient, 0x00, sizeof(vg_gradient_t));
  gradient->gradient.type = g->type;
  info = &(gradient->info.linear);
  if (g->type == GRADIENT_LINEAR) {
    if (g->degree == 180) {
      info->sx = rect->x;
      info->sy = rect->y;
      info->ex = rect->x;
      info->ey = rect->y + rect->h; 
    }
  }
  gradient->gradient = *g;
  return gradient;
}

vg_gradient_t* vg_gradient_init_linear(vg_gradient_t* gradient, float sx, float sy, float ex, float ey) {
  vg_gradient_linear_info_t* info = NULL;
  return_value_if_fail(gradient != NULL, NULL);

  memset(gradient, 0x00, sizeof(vg_gradient_t));
  info = &(gradient->info.linear);
  info->sx = sx;
  info->sy = sy;
  info->ex = ex;
  info->ey = ey;
  gradient->gradient.type = GRADIENT_LINEAR;

  return gradient;
}

vg_gradient_t* vg_gradient_create_linear(float sx, float sy, float ex, float ey) {
  vg_gradient_t* gradient = TKMEM_ZALLOC(vg_gradient_t);
  return_value_if_fail(gradient != NULL, NULL);

  return vg_gradient_init_linear(gradient, sx, sy, ex, ey);
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

