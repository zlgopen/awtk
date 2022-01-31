/**
 * File:   fscript_math.c
 * Author: AWTK Develop Team
 * Brief:  math functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"

static ret_t func_pow(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, pow(value_double(args->args), value_double(args->args + 1)));

  return RET_OK;
}

/*adapted from https://gsamaras.wordpress.com/code/isprime-c */
static bool_t is_prime(int number) {
  int i, r, prime = 1;
  if (number <= 1) {
    return 0;
  }
  if (number != 2 && number % 2 == 0) {
    prime = 0;
  } else {
    for (i = 3, r = sqrt(number); i <= r && prime; i += 2)
      if (number % i == 0) prime = 0;
  }
  return prime;
}

static ret_t func_is_prime(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t v = 0;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  v = value_int(args->args);

  value_set_bool(result, is_prime(v));

  return RET_OK;
}

static ret_t func_sqrt(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, sqrt(value_double(args->args)));

  return RET_OK;
}

static ret_t func_log(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, log(value_double(args->args)));

  return RET_OK;
}

static ret_t func_log10(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, log(value_double(args->args)) / log(10));

  return RET_OK;
}

static ret_t func_pow10(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, pow(10, value_double(args->args)));

  return RET_OK;
}

static ret_t func_exp(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, exp(value_double(args->args)));

  return RET_OK;
}

static ret_t func_sin(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, sin(value_double(args->args)));

  return RET_OK;
}

static ret_t func_cos(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, cos(value_double(args->args)));

  return RET_OK;
}

static ret_t func_tan(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, tan(value_double(args->args)));

  return RET_OK;
}

static ret_t func_asin(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, asin(value_double(args->args)));

  return RET_OK;
}

static ret_t func_acos(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, acos(value_double(args->args)));

  return RET_OK;
}

static ret_t func_atan(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, atan(value_double(args->args)));

  return RET_OK;
}

static ret_t func_atan2(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, atan2(value_double(args->args), value_double(args->args + 1)));

  return RET_OK;
}

static ret_t func_d2r(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, TK_D2R(value_double(args->args)));

  return RET_OK;
}

static ret_t func_r2d(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, TK_R2D(value_double(args->args)));

  return RET_OK;
}

static ret_t func_sin_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, sin(TK_D2R(value_double(args->args))));

  return RET_OK;
}

static ret_t func_cos_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, cos(TK_D2R(value_double(args->args))));

  return RET_OK;
}

static ret_t func_tan_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, tan(TK_D2R(value_double(args->args))));

  return RET_OK;
}

static ret_t func_asin_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, TK_R2D(asin(value_double(args->args))));

  return RET_OK;
}

static ret_t func_acos_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, TK_R2D(acos(value_double(args->args))));

  return RET_OK;
}

static ret_t func_atan_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, TK_R2D(atan(value_double(args->args))));

  return RET_OK;
}

static ret_t func_atan2_deg(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, TK_R2D(atan2(value_double(args->args), value_double(args->args + 1))));

  return RET_OK;
}

static ret_t func_is_int(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  bool_t is_int = FALSE;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  is_int = (double)value_int(args->args) == value_double(args->args);
  value_set_bool(result, is_int);

  return RET_OK;
}

static ret_t func_is_odd(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  bool_t is_int = FALSE;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  is_int = (double)value_int(args->args) == value_double(args->args);
  value_set_bool(result, is_int && (value_int(args->args) % 2 == 1));

  return RET_OK;
}

static ret_t func_is_even(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  bool_t is_int = FALSE;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  is_int = (double)value_int(args->args) == value_double(args->args);
  value_set_bool(result, is_int && (value_int(args->args) % 2 == 0));

  return RET_OK;
}

static ret_t func_is_negative(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  value_set_bool(result, value_double(args->args) < 0);

  return RET_OK;
}

static ret_t func_is_positive(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  value_set_bool(result, value_double(args->args) > 0);

  return RET_OK;
}

static ret_t func_random0to1(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v = (double)(random() % 100000) / 100000.0f;

  value_set_double(result, v);

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_math)
FACTORY_TABLE_ENTRY("d2r", func_d2r)
FACTORY_TABLE_ENTRY("r2d", func_r2d)

FACTORY_TABLE_ENTRY("acos", func_acos)
FACTORY_TABLE_ENTRY("asin", func_asin)
FACTORY_TABLE_ENTRY("atan", func_atan)
FACTORY_TABLE_ENTRY("atan2", func_atan2)
FACTORY_TABLE_ENTRY("cos", func_cos)
FACTORY_TABLE_ENTRY("sin", func_sin)
FACTORY_TABLE_ENTRY("tan", func_tan)

/*主要给AWBLOCK使用*/
FACTORY_TABLE_ENTRY("acos_deg", func_acos_deg)
FACTORY_TABLE_ENTRY("asin_deg", func_asin_deg)
FACTORY_TABLE_ENTRY("atan_deg", func_atan_deg)
FACTORY_TABLE_ENTRY("atan2_deg", func_atan2_deg)
FACTORY_TABLE_ENTRY("cos_deg", func_cos_deg)
FACTORY_TABLE_ENTRY("sin_deg", func_sin_deg)
FACTORY_TABLE_ENTRY("tan_deg", func_tan_deg)
FACTORY_TABLE_ENTRY("is_int", func_is_int)
FACTORY_TABLE_ENTRY("is_odd", func_is_odd)
FACTORY_TABLE_ENTRY("is_even", func_is_even)
FACTORY_TABLE_ENTRY("is_negative", func_is_negative)
FACTORY_TABLE_ENTRY("is_positive", func_is_positive)
FACTORY_TABLE_ENTRY("random0to1", func_random0to1)

FACTORY_TABLE_ENTRY("exp", func_exp)
FACTORY_TABLE_ENTRY("logf", func_log)
FACTORY_TABLE_ENTRY("sqrt", func_sqrt)
FACTORY_TABLE_ENTRY("pow", func_pow)
FACTORY_TABLE_ENTRY("is_prime", func_is_prime)
FACTORY_TABLE_ENTRY("log10", func_log10)
FACTORY_TABLE_ENTRY("pow10", func_pow10)

FACTORY_TABLE_END()

ret_t fscript_math_register(void) {
  return fscript_register_funcs(s_ext_math);
}
