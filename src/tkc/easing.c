/**
 * File:   easing.c
 * Author: AWTK Develop Team
 * Brief: easing functions.
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created, adapted from
 * https://github.com/tweenjs/tween.js.git
 *
 */

#include "tkc/easing.h"

#define PI 3.1415926
#include "tkc/easing.h"

static float_t easing_linear(float_t k) {
  return k;
}

static float_t easing_quadratic_in(float_t k) {
  return k * k;
}

static float_t easing_quadratic_out(float_t k) {
  return k * (2 - k);
}

static float_t easing_quadratic_inout(float_t k) {
  if ((k *= 2) < 1) {
    return 0.5f * k * k;
  }

  k -= 1;
  return -0.5f * (k * (k - 2) - 1);
}

static float_t easing_cubic_in(float_t k) {
  return k * k * k;
}

static float_t easing_cubic_out(float_t k) {
  k -= 1;

  return k * k * k + 1;
}

static float_t easing_sin_in(float_t k) {
  if ((k *= 2) < 1) {
    return 0.5f * k * k * k;
  }

  k -= 2;
  return 0.5f * ((k)*k * k + 2);
}

static float_t easing_sin_out(float_t k) {
  return 1 - (float_t)cos((float_t)(k * (float_t)PI / 2));
}

static float_t easing_sin_inout(float_t k) {
  return (float_t)sin((float_t)(k * (float_t)PI / 2));
}

static float_t easing_pow_in(float_t k) {
  return k == 0 ? 0 : pow(1024, k - 1);
}

static float_t easing_pow_out(float_t k) {
  return k == 1 ? 1 : 1 - pow(2, -10 * k);
}

static float_t easing_pow_inout(float_t k) {
  if (k == 0) {
    return 0;
  }

  if (k == 1) {
    return 1;
  }

  if ((k *= 2) < 1) {
    return 0.5 * pow(1024, k - 1);
  }

  return 0.5 * (-pow(2, -10 * (k - 1)) + 2);
}

static float_t easing_circular_in(float_t k) {
  return 1 - sqrt(1 - k * k);
}

static float_t easing_circular_out(float_t k) {
  k -= 1;
  return sqrt(1 - (k * k));
}

static float_t easing_circular_inout(float_t k) {
  if ((k *= 2) < 1) {
    return -0.5 * (sqrt(1 - k * k) - 1);
  }

  k -= 2;
  return 0.5 * (sqrt(1 - k * k) + 1);
}

static float_t easing_elastic_in(float_t k) {
  if (k == 0) {
    return 0;
  }

  if (k == 1) {
    return 1;
  }

  return -pow(2, 10 * (k - 1)) * (float_t)sin((float_t)((k - 1.1f) * 5 * (float_t)PI));
}

static float_t easing_elastic_out(float_t k) {
  if (k == 0) {
    return 0;
  }

  if (k == 1) {
    return 1;
  }

  return pow(2, -10 * k) * (float_t)sin((float_t)((k - 0.1f) * 5 * (float_t)PI)) + 1;
}

static float_t easing_elastic_inout(float_t k) {
  if (k == 0) {
    return 0;
  }

  if (k == 1) {
    return 1;
  }

  k *= 2;

  if (k < 1) {
    return -0.5f * pow(2, 10 * (k - 1)) * sin((float_t)((k - 1.1f) * 5 * (float_t)PI));
  }

  return 0.5f * pow(2, -10 * (k - 1)) * (float_t)sin((float_t)((k - 1.1f) * 5 * (float_t)PI)) + 1;
}

static float_t easing_back_in(float_t k) {
  float_t s = 1.70158f;

  return k * k * ((s + 1) * k - s);
}

static float_t easing_back_out(float_t k) {
  float_t s = 1.70158f;

  k -= 1;
  return k * k * ((s + 1) * k + s) + 1;
}

static float_t easing_back_inout(float_t k) {
  float_t s = 1.70158f * 1.525f;

  if ((k *= 2) < 1) {
    return 0.5f * (k * k * ((s + 1) * k - s));
  }

  k -= 2;
  return 0.5f * ((k)*k * ((s + 1) * k + s) + 2);
}

static float_t easing_bounce_out(float_t k);
static float_t easing_bounce_in(float_t k) {
  return 1 - easing_bounce_out(1 - k);
}

static float_t easing_bounce_out(float_t k) {
  if (k < (1 / 2.75f)) {
    return 7.5625f * k * k;
  } else if (k < (2 / 2.75f)) {
    k -= (1.5f / 2.75f);
    return 7.5625f * (k)*k + 0.75f;
  } else if (k < (2.5 / 2.75)) {
    k -= (2.25f / 2.75f);
    return 7.5625f * (k)*k + 0.9375f;
  } else {
    k -= (2.625f / 2.75f);
    return 7.5625f * (k)*k + 0.984375f;
  }
}

static float_t easing_bounce_inout(float_t k) {
  if (k < 0.5f) {
    return easing_bounce_in(k * 2) * 0.5f;
  }

  return easing_bounce_out(k * 2 - 1) * 0.5f + 0.5f;
}

static const easing_func_t s_easing_funcs[] = {
    easing_linear,      easing_quadratic_in,  easing_quadratic_out,  easing_quadratic_inout,
    easing_cubic_in,    easing_cubic_out,     easing_sin_in,         easing_sin_out,
    easing_sin_inout,   easing_pow_in,        easing_pow_out,        easing_pow_inout,
    easing_circular_in, easing_circular_out,  easing_circular_inout, easing_elastic_in,
    easing_elastic_out, easing_elastic_inout, easing_back_in,        easing_back_out,
    easing_back_inout,  easing_bounce_in,     easing_bounce_out,     easing_bounce_inout};

easing_func_t easing_get(easing_type_t type) {
  if ((int)type >= EASING_LINEAR && type < EASING_FUNC_NR) {
    return s_easing_funcs[type];
  } else {
    return s_easing_funcs[EASING_LINEAR];
  }
}
