/**
 * File:   fscript_endian.c
 * Author: AWTK Develop Team
 * Brief:  endian functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

FACTORY_TABLE_BEGIN(s_ext_endian)
FACTORY_TABLE_ENTRY("is_little", func_is_little_endian)

FACTORY_TABLE_ENTRY("htonl", func_htonl)
FACTORY_TABLE_ENTRY("ntohl", func_ntohl)

FACTORY_TABLE_ENTRY("htons", func_htons)
FACTORY_TABLE_ENTRY("ntohs", func_ntohs)

FACTORY_TABLE_ENTRY("htonf", func_htonf)
FACTORY_TABLE_ENTRY("ntohf", func_ntohf)

FACTORY_TABLE_ENTRY("htonll", func_htonll)
FACTORY_TABLE_ENTRY("ntohll", func_ntohll)
FACTORY_TABLE_END()

ret_t fscript_endian_register(void) {
  return fscript_register_funcs(s_ext_endian);
}
