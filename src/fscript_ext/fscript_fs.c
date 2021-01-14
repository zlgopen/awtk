/**
 * File:   fscript_fs.c
 * Author: AWTK Develop Team
 * Brief:  fs functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t func_file_read_text(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  char* content = NULL;
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  content = (char*)file_read(filename, &size);
  FSCRIPT_FUNC_CHECK(content != NULL, RET_BAD_PARAMS);

  value_set_str(result, content);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_file_read_binary(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  char* content = NULL;
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  content = (char*)file_read(filename, &size);
  FSCRIPT_FUNC_CHECK(content != NULL, RET_BAD_PARAMS);

  value_set_binary_data(result, content, size);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_file_write(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v = NULL;
  ret_t ret = RET_FAIL;
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  v = args->args + 1;
  if (args->size == 2) {
    if (v->type == VALUE_TYPE_STRING) {
      const char* content = value_str(v);
      ret = file_write(filename, content, tk_strlen(content));
    } else if (v->type == VALUE_TYPE_BINARY) {
      binary_data_t* bin = value_binary_data(v);
      FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
      ret = file_write(filename, bin->data, bin->size);
    }
  } else {
    const char* content = (const char*)value_pointer(v);
    uint32_t size = args->size > 2 ? value_uint32(args->args + 2) : 0;
    if (size > 0 && content != NULL) {
      ret = file_write(filename, content, size);
    }
  }

  value_set_bool(result, ret == RET_OK);
  return ret;
}

static ret_t func_file_exist(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_bool(result, file_exist(filename));

  return RET_OK;
}

static ret_t func_file_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_bool(result, file_remove(filename) == RET_OK);

  return RET_OK;
}

static ret_t func_file_get_size(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;

  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_int32(result, file_get_size(filename));

  return RET_OK;
}

static ret_t func_path_exist(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_bool(result, path_exist(filename));

  return RET_OK;
}

static ret_t func_path_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fs_remove_dir_r(os_fs(), filename) == RET_OK);

  return RET_OK;
}

static ret_t func_path_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fs_create_dir_r(os_fs(), filename) == RET_OK);

  return RET_OK;
}

static ret_t func_path_get_temp(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char path[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(path != NULL, RET_BAD_PARAMS);

  if (fs_get_temp_path(os_fs(), path) == RET_OK) {
    value_dup_str(result, path);
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_path_get_user_storage_root(fscript_t* fscript, fscript_args_t* args,
                                             value_t* result) {
  char path[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(path != NULL, RET_BAD_PARAMS);

  if (fs_get_user_storage_path(os_fs(), path) == RET_OK) {
    value_dup_str(result, path);
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_path_get_app_root(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char path[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(path != NULL, RET_BAD_PARAMS);

  if (path_app_root(path) == RET_OK) {
    value_dup_str(result, path);
    return RET_OK;
  }

  return RET_FAIL;
}

ret_t fscript_fs_register(void) {
  ENSURE(fscript_register_func("file_read_text", func_file_read_text) == RET_OK);
  ENSURE(fscript_register_func("file_read_binary", func_file_read_binary) == RET_OK);
  ENSURE(fscript_register_func("file_write", func_file_write) == RET_OK);
  ENSURE(fscript_register_func("file_exist", func_file_exist) == RET_OK);
  ENSURE(fscript_register_func("file_get_size", func_file_get_size) == RET_OK);
  ENSURE(fscript_register_func("file_remove", func_file_remove) == RET_OK);
  ENSURE(fscript_register_func("path_create", func_path_create) == RET_OK);
  ENSURE(fscript_register_func("path_remove", func_path_remove) == RET_OK);
  ENSURE(fscript_register_func("path_exist", func_path_exist) == RET_OK);
  ENSURE(fscript_register_func("path_get_temp", func_path_get_temp) == RET_OK);
  ENSURE(fscript_register_func("path_get_app_root", func_path_get_app_root) == RET_OK);
  ENSURE(fscript_register_func("path_get_user_storage_root", func_path_get_user_storage_root) ==
         RET_OK);

  return RET_OK;
}
