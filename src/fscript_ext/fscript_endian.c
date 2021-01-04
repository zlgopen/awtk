/**
 * File:   fscript_endian.c
 * Author: AWTK Develop Team
 * Brief:  endian functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/endian.h"
#include "tkc/fscript.h"

static ret_t func_is_little_endian(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 0, RET_BAD_PARAMS);
  value_set_bool(result, is_little_endian());

  return RET_OK;
}

static ret_t func_htonl(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, htonl(value_uint32(args->args)));

  return RET_OK;
}

static ret_t func_ntohl(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, ntohl(value_uint32(args->args)));

  return RET_OK;
}

static ret_t func_htons(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint16(result, htons(value_uint16(args->args)));

  return RET_OK;
}

static ret_t func_ntohs(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint16(result, ntohs(value_uint16(args->args)));

  return RET_OK;
}

static ret_t func_htonf(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_float32(result, float_to_big_endian(value_float32(args->args)));

  return RET_OK;
}

static ret_t func_ntohf(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_float32(result, float_from_big_endian(value_float32(args->args)));

  return RET_OK;
}

static ret_t func_htonll(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int64(result, int64_to_big_endian(value_int64(args->args)));

  return RET_OK;
}

static ret_t func_ntohll(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int64(result, int64_from_big_endian(value_int64(args->args)));

  return RET_OK;
}

ret_t fscript_endian_register(void) {
  ENSURE(fscript_register_func("is_little", func_is_little_endian) == RET_OK);

  ENSURE(fscript_register_func("htonl", func_htonl) == RET_OK);
  ENSURE(fscript_register_func("ntohl", func_ntohl) == RET_OK);

  ENSURE(fscript_register_func("htons", func_htons) == RET_OK);
  ENSURE(fscript_register_func("ntohs", func_ntohs) == RET_OK);

  ENSURE(fscript_register_func("htonf", func_htonf) == RET_OK);
  ENSURE(fscript_register_func("ntohf", func_ntohf) == RET_OK);

  ENSURE(fscript_register_func("htonll", func_htonll) == RET_OK);
  ENSURE(fscript_register_func("ntohll", func_ntohll) == RET_OK);

  return RET_OK;
}
