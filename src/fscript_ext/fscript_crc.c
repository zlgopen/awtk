/**
 * File:   fscript_crc.c
 * Author: AWTK Develop Team
 * Brief:  crc functions for fscript
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

#include "tkc/crc.h"
#include "tkc/fscript.h"

static ret_t get_data_size(fscript_args_t* args, const uint8_t** ret_data, uint32_t* ret_size) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  if (args->size == 1) {
    data = (const uint8_t*)value_str(args->args);
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
    size = strlen((const char*)data);
  } else {
    data = (const uint8_t*)value_pointer(args->args);
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
    size = value_int(args->args + 1);
  }

  *ret_data = data;
  *ret_size = size;

  return RET_OK;
}

static ret_t func_crc16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  return_value_if_fail(get_data_size(args, &data, &size) == RET_OK, RET_BAD_PARAMS);

  value_set_uint16(result, tk_crc16(PPPINITFCS16, data, size));

  return RET_OK;
}

static ret_t func_crc32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  return_value_if_fail(get_data_size(args, &data, &size) == RET_OK, RET_BAD_PARAMS);

  value_set_uint32(result, tk_crc32(PPPINITFCS32, data, size));

  return RET_OK;
}

static ret_t func_chsum(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  return_value_if_fail(get_data_size(args, &data, &size) == RET_OK, RET_BAD_PARAMS);

  value_set_uint16(result, tk_chksum(0, data, size));

  return RET_OK;
}

ret_t fscript_crc_register(void) {
  ENSURE(fscript_register_func("crc16", func_crc16) == RET_OK);
  ENSURE(fscript_register_func("crc32", func_crc32) == RET_OK);
  ENSURE(fscript_register_func("cksum", func_chsum) == RET_OK);

  return RET_OK;
}
