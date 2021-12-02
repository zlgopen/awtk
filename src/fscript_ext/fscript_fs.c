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
#include "tkc/object_array.h"
#include "tkc/object_default.h"

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

static ret_t func_file_write_append(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v = NULL;
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  fs_file_t* fp = NULL;
  const char* content = NULL;
  const char* filename = NULL;

  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  fp = fs_open_file(os_fs(), filename, "ab+");
  FSCRIPT_FUNC_CHECK(fp != NULL, RET_BAD_PARAMS);

  v = args->args + 1;
  if (args->size == 2) {
    if (v->type == VALUE_TYPE_STRING) {
      content = value_str(v);
      size = tk_strlen(content);
    } else if (v->type == VALUE_TYPE_BINARY) {
      binary_data_t* bin = value_binary_data(v);
      FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
      content = (const char*)bin->data;
      size = bin->size;
    }
  } else {
    content = (const char*)value_pointer(v);
    size = args->size > 2 ? value_uint32(args->args + 2) : 0;
  }

  if (size > 0 && content != NULL) {
    ret = fs_file_write(fp, content, size) == size ? RET_OK : RET_FAIL;
  }
  fs_file_close(fp);
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

static ret_t func_file_rename(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  const char* new_name = NULL;

  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  new_name = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(new_name != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fs_file_rename(os_fs(), filename, new_name) == RET_OK);

  return RET_OK;
}

static ret_t func_file_copy(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  const char* dst_name = NULL;

  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  dst_name = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(dst_name != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fs_copy_file(os_fs(), filename, dst_name) == RET_OK);

  return RET_OK;
}

static ret_t func_file_stat(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  fs_stat_info_t fst = {0};
  tk_object_t* obj = NULL;
  const char* filename = NULL;

  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);

  if (fs_stat(os_fs(), filename, &fst) == RET_OK) {
    obj = object_default_create();
    FSCRIPT_FUNC_CHECK(obj != NULL, RET_OOM);
    tk_object_set_prop_uint32(obj, "dev", fst.dev);
    tk_object_set_prop_uint16(obj, "ino", fst.ino);
    tk_object_set_prop_uint16(obj, "mode", fst.mode);
    tk_object_set_prop_int16(obj, "nlink", fst.nlink);
    tk_object_set_prop_int16(obj, "uid", fst.uid);
    tk_object_set_prop_int16(obj, "gid", fst.gid);
    tk_object_set_prop_uint32(obj, "rdev", fst.rdev);
    tk_object_set_prop_uint64(obj, "size", fst.size);
    tk_object_set_prop_uint64(obj, "atime", fst.atime);
    tk_object_set_prop_uint64(obj, "mtime", fst.mtime);
    tk_object_set_prop_uint64(obj, "ctime", fst.ctime);
    tk_object_set_prop_bool(obj, "is_dir", fst.is_dir);
    tk_object_set_prop_bool(obj, "is_link", fst.is_link);
    tk_object_set_prop_bool(obj, "is_reg_file", fst.is_reg_file);
    value_set_object(result, obj);
    result->free_handle = TRUE;
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_path_rename(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* filename = NULL;
  const char* new_name = NULL;

  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  new_name = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(new_name != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fs_dir_rename(os_fs(), filename, new_name) == RET_OK);

  return RET_OK;
}

static ret_t func_path_list(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t v = {0};
  fs_item_t info = {0};
  fs_dir_t* dir = NULL;
  tk_object_t* array = NULL;
  const char* filename = NULL;

  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  FSCRIPT_FUNC_CHECK(filename != NULL, RET_BAD_PARAMS);
  dir = fs_open_dir(os_fs(), filename);
  FSCRIPT_FUNC_CHECK(dir != NULL, RET_BAD_PARAMS);
  array = object_array_create();
  FSCRIPT_FUNC_CHECK(array != NULL, RET_OOM);

  while (fs_dir_read(dir, &info) == RET_OK) {
    if (tk_str_eq(info.name, ".") || tk_str_eq(info.name, "..")) {
      continue;
    }

    tk_object_t* item = object_default_create();
    if (item != NULL) {
      tk_object_set_prop_bool(item, "is_dir", info.is_dir);
      tk_object_set_prop_bool(item, "is_link", info.is_link);
      tk_object_set_prop_bool(item, "is_reg_file", info.is_reg_file);
      tk_object_set_prop_str(item, "name", info.name);
      value_set_object(&v, item);
      object_array_push(array, &v);
      TK_OBJECT_UNREF(item);
    }
  }
  fs_dir_close(dir);
  value_set_object(result, array);
  result->free_handle = TRUE;

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

static ret_t func_path_get_exe(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char path[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(path != NULL, RET_BAD_PARAMS);

  if (fs_get_exe(os_fs(), path) == RET_OK) {
    value_dup_str(result, path);
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_path_get_cwd(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char path[MAX_PATH + 1];
  FSCRIPT_FUNC_CHECK(path != NULL, RET_BAD_PARAMS);

  if (fs_get_cwd(os_fs(), path) == RET_OK) {
    value_dup_str(result, path);
    return RET_OK;
  }

  return RET_FAIL;
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

static ret_t func_fs_get_disk_info(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t free_kb = 0;
  int32_t total_kb = 0;
  tk_object_t* obj = NULL;
  const char* volume = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  volume = value_str(args->args);
  FSCRIPT_FUNC_CHECK(volume != NULL, RET_BAD_PARAMS);

  if (fs_get_disk_info(os_fs(), volume, &free_kb, &total_kb) == RET_OK) {
    obj = object_default_create();
    tk_object_set_prop_int32(obj, "free_kb", free_kb);
    tk_object_set_prop_int32(obj, "total_kb", total_kb);
    value_set_object(result, obj);
    result->free_handle = TRUE;
    return RET_OK;
  }

  return RET_FAIL;
}

FACTORY_TABLE_BEGIN(s_ext_fs)
FACTORY_TABLE_ENTRY("file_read_text", func_file_read_text)
FACTORY_TABLE_ENTRY("file_read_binary", func_file_read_binary)
FACTORY_TABLE_ENTRY("file_write", func_file_write)
FACTORY_TABLE_ENTRY("file_write_append", func_file_write_append)
FACTORY_TABLE_ENTRY("file_exist", func_file_exist)
FACTORY_TABLE_ENTRY("file_get_size", func_file_get_size)
FACTORY_TABLE_ENTRY("file_remove", func_file_remove)
FACTORY_TABLE_ENTRY("file_rename", func_file_rename)
FACTORY_TABLE_ENTRY("file_copy", func_file_copy)
FACTORY_TABLE_ENTRY("file_stat", func_file_stat)
FACTORY_TABLE_ENTRY("path_create", func_path_create)
FACTORY_TABLE_ENTRY("path_remove", func_path_remove)
FACTORY_TABLE_ENTRY("path_exist", func_path_exist)
FACTORY_TABLE_ENTRY("path_rename", func_path_rename)
FACTORY_TABLE_ENTRY("path_list", func_path_list)
FACTORY_TABLE_ENTRY("path_get_exe", func_path_get_exe)
FACTORY_TABLE_ENTRY("path_get_cwd", func_path_get_cwd)
FACTORY_TABLE_ENTRY("path_get_temp", func_path_get_temp)
FACTORY_TABLE_ENTRY("path_get_app_root", func_path_get_app_root)
FACTORY_TABLE_ENTRY("path_get_user_storage_root", func_path_get_user_storage_root)
FACTORY_TABLE_ENTRY("fs_get_disk_info", func_fs_get_disk_info)
FACTORY_TABLE_END()

ret_t fscript_fs_register(void) {
  return fscript_register_funcs(s_ext_fs);
}
