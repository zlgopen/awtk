/**
 * File:   fargs_utils.c
 * Author: AWTK Develop Team
 * Brief:  utils functions
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-13 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/utils.h"
#include "tkc/fscript.h"

ret_t fargs_get_data_and_size(fscript_args_t* args, const uint8_t** ret_data, uint32_t* ret_size) {
  int32_t size = 0;
  const uint8_t* data = NULL;
  value_t* v = args->args;

  if (args->size > 1) {
    size = value_int(args->args + 1);
  }

  if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    return_value_if_fail(bin != NULL, RET_BAD_PARAMS);
    data = bin->data;
    if (size < 0 || size > bin->size || args->size == 1) {
      size = bin->size;
    }
  } else if (v->type == VALUE_TYPE_STRING) {
    uint32_t len = 0;
    data = (const uint8_t*)value_str(args->args);
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
    len = strlen((const char*)data);
    if (size < 0 || size > len || args->size == 1) {
      size = len;
    }
  } else if (v->type == VALUE_TYPE_POINTER) {
    data = (const uint8_t*)value_pointer(args->args);
    if (size < 0) {
      size = 0;
    }
  }

  *ret_data = data;
  *ret_size = size;

  return RET_OK;
}
