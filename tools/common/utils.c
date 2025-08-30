/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils function
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/wstr.h"
#include "tkc/path.h"
#include "tkc/str.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_file.h"
#include "base/enums.h"
#include "base/assets_manager.h"
#include "conf_io/conf_json.h"
#include "utils.h"

static bool_t exit_if_need_not_update_for_include_subfile(const char* in, fs_stat_info_t* fst_out) {
  fs_stat_info_t fst_in;
  uint32_t i = 0, size = 0;
  char** subfilenames = NULL;
  bool_t is_need_not_update = TRUE;
  return_value_if_fail(in != NULL && fst_out != NULL, TRUE);

  if (!path_extname_is(in, ".xml")) {
    return TRUE;
  }

  if (xml_file_expand_subfilenames_get(in, &subfilenames, &size) != RET_OK) {
    return TRUE;
  }

  for (i = 0; i < size; i++) {
    const char* iter = subfilenames[i];
    if (is_need_not_update) {
      if (fs_stat(os_fs(), iter, &fst_in) != RET_OK) {
        TKMEM_FREE(iter);
        is_need_not_update = FALSE;
        log_debug("get \"%s\" filetime failed\n", iter);
        continue;
      }
      if (fst_in.mtime > fst_out->mtime) {
        is_need_not_update = FALSE;
      }
    }
    TKMEM_FREE(iter);
  }
  TKMEM_FREE(subfilenames);

  return is_need_not_update;
}

bool_t exit_if_need_not_update(const char* in, const char* out) {
  if (in == NULL || out == NULL) {
    log_debug("invalid params: %s %s\n", in, out);
    return TRUE;
  }

  if (!fs_file_exist(os_fs(), in)) {
    log_debug("%s not exist\n", in);
    return TRUE;
  }

  if (file_get_size(in) == 0) {
    log_debug("%s is empty\n", in);
    return TRUE;
  }

  if (!fs_file_exist(os_fs(), out)) {
    return FALSE;
  }

  ret_t rt = RET_OK;
  fs_stat_info_t fst_in;
  fs_stat_info_t fst_out;

  rt = fs_stat(os_fs(), in, &fst_in);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", in);
    return TRUE;
  }
  rt = fs_stat(os_fs(), out, &fst_out);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", out);
    return TRUE;
  }
  if (fst_in.mtime < fst_out.mtime && exit_if_need_not_update_for_include_subfile(in, &fst_out)) {
    log_debug("Skip because: %s is newer than %s\n", out, in);
    return TRUE;
  }
  return FALSE;
}

void exit_if_need_not_update_for_infiles(const char* out, int infiles_number, ...) {
  int i = 0;
  va_list va;
  ret_t rt = RET_OK;
  fs_stat_info_t st_in;
  fs_stat_info_t st_out;
  bool_t is_not_need_update = TRUE;

  if (out == NULL) {
    log_debug("invalid params\n");
    exit(-1);
  }

  if (!fs_file_exist(os_fs(), out)) {
    return;
  }

  rt = fs_stat(os_fs(), out, &st_out);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", out);
    exit(-1);
  }

  va_start(va, infiles_number);

  for (i = 0; i < infiles_number; i++) {
    char* in = va_arg(va, char*);

    if (in == NULL) {
      continue;
    }

    if (!fs_file_exist(os_fs(), in)) {
      log_debug("%s not exist\n", in);
      is_not_need_update = TRUE;
      break;
    }

    rt = fs_stat(os_fs(), in, &st_in);
    if (rt != RET_OK) {
      log_debug("get \"%s\" filetime failed\n", in);
      exit(-1);
    }

    if (st_in.mtime > st_out.mtime) {
      is_not_need_update = FALSE;
      break;
    }
  }

  va_end(va);

  if (is_not_need_update) {
    log_debug("Skip because: %s is new \n", out);
    exit(0);
  }
}

char* read_file(const char* file_name, uint32_t* length) {
  return (char*)file_read(file_name, length);
}

ret_t write_file(const char* file_name, const void* buff, uint32_t length) {
  return file_write(file_name, buff, length);
}

int unique(wchar_t* str, int size) {
  int i = 0;
  wchar_t* d = str + 1;

  for (i = 1; i < size; i++) {
    wchar_t c = str[i];
    if (c != d[-1]) {
      d[0] = c;
      d++;
    }
  }

  return d - str;
}

static str_t* format_folder_name(const char* name) {
  str_t* str = str_create(2 * TK_NAME_LEN + 1);

  tokenizer_t t;
  tokenizer_init(&t, name, strlen(name), "/\\");
  while (tokenizer_has_more(&t)) {
    const char* text = tokenizer_next(&t);
    if (str->size == 0) {
      str_set(str, text);
    } else {
      str_append(str, "_");  // 目录名称前再加一个 "_"，避免文件重名冲突。
      str_append(str, text);
    }
    str_append(str, "_");  // 将斜杠替换为 "_"
  }
  tokenizer_deinit(&t);

  str_pop(str);  // 去掉末尾多余的"_"
  return str;
}

static const char* to_var_name(char var_name[2 * TK_NAME_LEN + 1], const char* theme,
                               const char* prefix, const char* name) {
  char tmp[TK_NAME_LEN + 1] = {0};
  char tmp_var_name[2 * TK_NAME_LEN + 1] = {0};
  const char* p = name + tk_strlen(name);
  while (p != name) {
    if (*p == '\\' || *p == '/') {
      p++;
      break;
    }
    p--;
  }
  tk_strcpy(tmp, p);
  tk_snprintf(tmp_var_name, 2 * TK_NAME_LEN, "%s_%s_%s", prefix == NULL ? "" : prefix, tmp,
              theme == NULL ? "default" : theme);
  if (p != name) {
    tk_strncpy(tmp, name, p - name - 1);
    tk_snprintf(var_name, 2 * TK_NAME_LEN, "%s_%s", tmp_var_name, tmp);
  } else {
    tk_strcpy(var_name, tmp_var_name);
  }
  return filter_name(var_name);
}

ret_t output_c_source(const char* filename, const char* theme, const char* prefix, const char* name,
                      uint8_t* buff, uint32_t size) {
  uint32_t i = 0;
  fs_file_t* ft = NULL;
  char str[TK_NAME_LEN + 1];
  char var_name[2 * TK_NAME_LEN + 1];
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  if (name == NULL) {
    filename_to_name(filename, str, sizeof(str));
    name = str;
  }

  log_debug("filename=%s prefix=%s name=%s size=%u\n", filename, prefix, name, size);

  ft = fs_open_file(os_fs(), filename, "wb+");
  if (ft != NULL) {
    fs_file_printf(ft, "TK_CONST_DATA_ALIGN(const unsigned char %s[]) = {",
                   to_var_name(var_name, theme, prefix, name));
    for (i = 0; i < size; i++) {
      if ((i % 20) == 0) {
        fs_file_printf(ft, "\n");
      }
      fs_file_printf(ft, "0x%02x,", (int)(buff[i]));
    }

    fs_file_printf(ft, "};/*%u*/\n", size);
    fs_file_close(ft);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t output_res_c_source_ex(const char* filename, const char* theme, uint16_t type,
                             uint16_t subtype, uint8_t* buff, uint32_t size, const char* name) {
  asset_info_t* res = NULL;
  const char* array_name = NULL;
  const char* asset_name = NULL;
  uint32_t total_size = sizeof(asset_info_t) + size;
  const key_type_value_t* kv = asset_type_find_by_value(type);
  return_value_if_fail(kv != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  res = (asset_info_t*)malloc(total_size);
  return_value_if_fail(res != NULL, RET_OOM);

  memset(res, 0x00, total_size);
  res->size = size;
  res->type = type;
  res->refcount = 0;
  res->subtype = subtype;
  memcpy(res->data, buff, size);
  asset_info_set_is_in_rom(res, TRUE);
  if (name != NULL) {
    array_name = name;
    asset_name = asset_info_get_formatted_name(name);
  } else {
    char full_name[MAX_PATH] = {0};
    if (type == ASSET_TYPE_DATA) {
      filename_to_name_ex(filename, full_name, MAX_PATH, FALSE);
    } else {
      filename_to_name(filename, full_name, MAX_PATH);
    }
    array_name = (const char*)full_name;
    asset_name = asset_info_get_formatted_name(full_name);
  }
  tk_strncpy(res->name.small_name, asset_name, sizeof(res->name.small_name) - 1);

  output_c_source(filename, theme, kv->name, array_name, (uint8_t*)res, total_size);
  free(res);

  return RET_OK;
}

ret_t output_res_c_source(const char* filename, const char* theme, uint16_t type, uint16_t subtype,
                          uint8_t* buff, uint32_t size) {
  return output_res_c_source_ex(filename, theme, type, subtype, buff, size, NULL);
}

const char* skip_to(const char* p, char c) {
  while (*p && *p != c) p++;

  return p;
}

const char* skip_char(const char* p, char c) {
  while (*p && *p == c) p++;

  return p;
}

const char* skip_to_next(const char* p, char c) {
  return skip_char(skip_to(skip_char(p, c), c), c);
}

const char* get_next_token(const char* p, char* token, char c) {
  const char* start = skip_char(p, c);
  const char* end = skip_to(start, c);
  uint32_t size = end - start;
  strncpy(token, start, size);
  token[size] = '\0';

  return token;
}

bool_t start_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return strncmp(p, str, strlen(str)) == 0;
}

bool_t end_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return strncmp(p + strlen(p) - strlen(str), str, strlen(str)) == 0;
}

bool_t case_end_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return tk_str_ieq(p + strlen(p) - strlen(str), str);
}

const char* to_lower(char* str) {
  char* p = str;

  while (*p) {
    *p = tolower(*p);
    p++;
  }

  return str;
}

const char* filter_name(char* name) {
  char* p = name;
  while (*p) {
    if (!(tk_isdigit(*p) || tk_isalpha(*p) || *p == '_')) {
      *p = '_';
    }
    p++;
  }

  return name;
}

ret_t ensure_output_res_name(str_t* str_name, bool_t is_bin, const char* ext) {
  return_value_if_fail(str_name != NULL, RET_BAD_PARAMS);

  if (!is_bin) {
    char basename[MAX_PATH] = {0};
    char dirname[MAX_PATH] = {0};

    path_basename(str_name->str, basename, MAX_PATH);
    if (!tk_str_eq(str_name->str, basename)) {
      path_dirname(str_name->str, dirname, MAX_PATH);
      str_set(str_name, dirname);
      str_append(str_name, "/");
      str_append(str_name, filter_name(basename));
    } else {
      str_set(str_name, filter_name(basename));
    }
  }

  str_append(str_name, ext);

  return RET_OK;
}

ret_t makesure_folder_exist(const char* folder) {
  char tmp[MAX_PATH + 1] = {0};
  if (end_with(folder, "\\") || end_with(folder, "/")) {
    tk_strncpy(tmp, folder, tk_strlen(folder) - 1);
    folder = tmp;
  }

  char p_folder[MAX_PATH + 1] = {0};
  path_dirname(folder, p_folder, MAX_PATH);
  if (!fs_dir_exist(os_fs(), p_folder)) {
    makesure_folder_exist(p_folder);
  }

  if (!fs_dir_exist(os_fs(), folder)) {
    fs_create_dir(os_fs(), folder);
  }

  return RET_OK;
}

darray_t* get_res_names_from_sources_file(const char* src_filename, darray_t* sources) {
  data_reader_factory_t* factory = data_reader_factory();
  if (factory == NULL) {
    data_reader_factory_set(data_reader_factory_create());
    data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  }

  tk_object_t* json = conf_json_load(src_filename, FALSE);
  if (json != NULL) {
    char propname[MAX_PATH] = {0};
    int32_t size = tk_object_get_prop_int(json, "sources.#size", -1);
    if (size > 0) {
      for (int32_t i = 0; i < size; i++) {
        memset(propname, 0x00, sizeof(propname));
        tk_snprintf(propname, sizeof(propname), "sources.[%d]", i);

        const char* item = tk_object_get_prop_str(json, propname);
        char* name = tk_str_copy(NULL, item);
        darray_push(sources, name);
      }
    }
  }

  return sources;
}

darray_t* get_image_names_from_sources_file(const char* src_filename, darray_t* sources,
                                            const char* dpr) {
  data_reader_factory_t* factory = data_reader_factory();
  if (factory == NULL) {
    data_reader_factory_set(data_reader_factory_create());
    data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  }

  tk_object_t* json = conf_json_load(src_filename, FALSE);

  char propname[MAX_PATH] = {0};
  int32_t size = tk_object_get_prop_int(json, "sources.#size", -1);
  if (size > 0) {
    for (int32_t i = 0; i < size; i++) {
      memset(propname, 0x00, sizeof(propname));
      tk_snprintf(propname, sizeof(propname), "sources.[%d]", i);

      const char* item = tk_object_get_prop_str(json, propname);
      if (tk_str_start_with(item, dpr)) {
        char* file = tk_str_copy(NULL, item + strlen(dpr) + 1);
        darray_push(sources, file);
      }
    }
  }

  return sources;
}

darray_t* get_res_names_from_sources_file_bak(const char* src_filename, darray_t* sources,
                                              const char* theme, const char* type) {
  data_reader_factory_t* factory = data_reader_factory();
  if (factory == NULL) {
    data_reader_factory_set(data_reader_factory_create());
    data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  }

  tk_object_t* json = conf_json_load(src_filename, FALSE);

  char type_prop[128] = {0};
  tk_snprintf(type_prop, sizeof(type_prop), "sources.%s.%s", theme, type);
  char propname[MAX_PATH] = {0};
  tk_snprintf(propname, sizeof(propname), "%s.#size", type_prop);

  int32_t size = tk_object_get_prop_int(json, propname, -1);
  if (size > 0) {
    for (int32_t i = 0; i < size; i++) {
      memset(propname, 0x00, sizeof(propname));
      tk_snprintf(propname, sizeof(propname), "%s.[%d]", type_prop, i);

      const char* item = tk_object_get_prop_str(json, propname);
      char* file = tk_str_copy(NULL, item);
      darray_push(sources, file);
    }
  }

  return sources;
}

darray_t* get_image_names_from_sources_file_bak(const char* src_filename, darray_t* sources,
                                                const char* theme, const char* dpr) {
  data_reader_factory_t* factory = data_reader_factory();
  if (factory == NULL) {
    data_reader_factory_set(data_reader_factory_create());
    data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  }

  tk_object_t* json = conf_json_load(src_filename, FALSE);

  char type_prop[128] = {0};
  tk_snprintf(type_prop, sizeof(type_prop), "sources.%s.%s", theme, "images");
  char propname[MAX_PATH] = {0};
  tk_snprintf(propname, sizeof(propname), "%s.#size", type_prop);

  int32_t size = tk_object_get_prop_int(json, propname, -1);
  if (size > 0) {
    for (int32_t i = 0; i < size; i++) {
      memset(propname, 0x00, sizeof(propname));
      tk_snprintf(propname, sizeof(propname), "%s.[%d]", type_prop, i);

      const char* item = tk_object_get_prop_str(json, propname);
      if (tk_str_start_with(item, dpr)) {
        char* file = tk_str_copy(NULL, item + strlen(dpr) + 1);
        darray_push(sources, file);
      }
    }
  }

  return sources;
}

char* get_image_dpr(const char* folder) {
  char dpr_names[5][4] = {"xx", "x1", "x2", "x3", "x4"};
  char basename[MAX_PATH] = {0};
  path_basename(folder, basename, MAX_PATH);

  for (int32_t i = 0; i < 5; i++) {
    if (tk_str_eq(basename, dpr_names[i])) {
      char* dpr = TKMEM_ZALLOCN(char, 4);
      return tk_str_copy(dpr, basename);
    }
  }

  return NULL;
}