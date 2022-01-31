/**
 * File:   fscript_json.c
 * Author: AWTK Develop Team
 * Brief:  json functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-07 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "conf_io/conf_json.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_wbuffer.h"

typedef tk_object_t* (*json_load_t)(const char* url, bool_t create);
typedef ret_t (*json_save_t)(tk_object_t* obj, const char* url);

static ret_t func_json_load_ex(fscript_t* fscript, fscript_args_t* args, value_t* result,
                               json_load_t load) {
  value_t* v = NULL;
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  const void* data = NULL;
  char url[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    v = args->args;
    if (v->type == VALUE_TYPE_STRING) {
      data = value_str(v);
      FSCRIPT_FUNC_CHECK(data != NULL, RET_BAD_PARAMS);
      size = strlen(data);
    } else if (v->type == VALUE_TYPE_BINARY) {
      binary_data_t* bin = value_binary_data(v);
      FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
      data = bin->data;
      size = bin->size;
    }
  } else {
    data = value_pointer(v);
    size = value_uint32(args->args + 1);
  }

  if (data != NULL && size > 0) {
    data_reader_mem_build_url(data, size, url);
    obj = load(url, FALSE);
    value_set_object(result, obj);
    result->free_handle = TRUE;
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_json_save_ex(fscript_t* fscript, fscript_args_t* args, value_t* result,
                               json_save_t save, uint32_t result_type) {
  wbuffer_t wb;
  tk_object_t* obj = NULL;
  char url[MAX_PATH + 1];
  wbuffer_init_extendable(&wb);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(wbuffer_extend_capacity(&wb, 256) == RET_OK, RET_OOM);

  data_writer_wbuffer_build_url(&wb, url);
  if (save(obj, url) == RET_OK) {
    if (result_type == VALUE_TYPE_BINARY) {
      value_set_binary_data(result, wb.data, wb.cursor);
    } else if (result_type == VALUE_TYPE_STRING) {
      value_set_str(result, (const char*)wb.data);
    }
    result->free_handle = TRUE;
    return RET_OK;
  }
  wbuffer_deinit(&wb);

  return RET_FAIL;
}

static ret_t func_json_load(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_json_load_ex(fscript, args, result, conf_json_load);
}

static ret_t func_json_save(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_json_save_ex(fscript, args, result, conf_json_save_as, VALUE_TYPE_BINARY);
}

static ret_t func_json_save_to_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_json_save_ex(fscript, args, result, conf_json_save_as, VALUE_TYPE_STRING);
}

static ret_t func_ubjson_load(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_json_load_ex(fscript, args, result, conf_ubjson_load);
}

static ret_t func_ubjson_save(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_json_save_ex(fscript, args, result, conf_ubjson_save_as, VALUE_TYPE_BINARY);
}

FACTORY_TABLE_BEGIN(s_ext_json)
FACTORY_TABLE_ENTRY("json_load", func_json_load)
FACTORY_TABLE_ENTRY("json_save", func_json_save)
FACTORY_TABLE_ENTRY("json_save_to_string", func_json_save_to_string)
FACTORY_TABLE_ENTRY("ubjson_load", func_ubjson_load)
FACTORY_TABLE_ENTRY("ubjson_save", func_ubjson_save)
FACTORY_TABLE_END()

ret_t fscript_json_register(void) {
  return fscript_register_funcs(s_ext_json);
}
