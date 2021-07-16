/**
 * File:   fscript_math.c
 * Author: AWTK Develop Team
 * Brief:  math functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

ret_t fscript_math_register(void) {
  ENSURE(fscript_register_func("acos", func_acos) == RET_OK);
  ENSURE(fscript_register_func("asin", func_asin) == RET_OK);
  ENSURE(fscript_register_func("atan", func_atan) == RET_OK);
  ENSURE(fscript_register_func("atan2", func_atan2) == RET_OK);
  ENSURE(fscript_register_func("cos", func_cos) == RET_OK);
  ENSURE(fscript_register_func("sin", func_sin) == RET_OK);
  ENSURE(fscript_register_func("exp", func_exp) == RET_OK);
  ENSURE(fscript_register_func("log", func_log) == RET_OK);
  ENSURE(fscript_register_func("sqrt", func_sqrt) == RET_OK);
  ENSURE(fscript_register_func("tan", func_tan) == RET_OK);
  ENSURE(fscript_register_func("pow", func_pow) == RET_OK);
  ENSURE(fscript_register_func("is_prime", func_is_prime) == RET_OK);

  return RET_OK;
}
