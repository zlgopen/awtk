/**
 * File:   fargs_utils.c
 * Author: AWTK Develop Team
 * Brief:  utils functions
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  uint32_t size = 0;
  const uint8_t* data = NULL;
  if (args->size == 1) {
    value_t* v = args->args;
    if (v->type == VALUE_TYPE_BINARY) {
      binary_data_t* bin = value_binary_data(v);
      return_value_if_fail(bin != NULL, RET_BAD_PARAMS);
      data = bin->data;
      size = bin->size;
    } else if (v->type == VALUE_TYPE_STRING) {
      data = (const uint8_t*)value_str(args->args);
      return_value_if_fail(data != NULL, RET_BAD_PARAMS);
      size = strlen((const char*)data);
    }
  } else {
    data = (const uint8_t*)value_pointer(args->args);
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
    size = value_int(args->args + 1);
  }

  *ret_data = data;
  *ret_size = size;

  return RET_OK;
}
