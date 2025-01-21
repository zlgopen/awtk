/**
 * File:   main.cc
 * Author: AWTK Develop Team
 * Brief:  generate theme date from xml
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/tokenizer.h"
#include "common/utils.h"
#include "xml_theme_gen.h"

ret_t gen_one(const char* input_file, const char* output_file, const char* theme, const char* name,
              bool_t output_bin) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(input_file, output_file)) {
    if (!xml_gen(input_file, output_file, theme, name, output_bin)) {
      ret = RET_FAIL;
      GEN_ERROR(input_file);
    }
  }
  return ret;
}

static ret_t gen_sources(const char* src_filename, const char* in_foldername,
                         const char* out_foldername, const char* theme, const char* dir_name,
                         bool_t output_bin) {
  fs_item_t item;
  ret_t ret = RET_OK;
  const char* c_xml = ".xml";
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};

  if (!fs_file_exist(os_fs(), src_filename)) {
    log_debug("gen fail, sources file \"%s\" not exist!", src_filename);
    return RET_FAIL;
  }

  if (!fs_dir_exist(os_fs(), out_foldername)) {
    fs_create_dir(os_fs(), out_foldername);
  }

  darray_t sources;
  darray_init(&sources, 10, default_destroy, NULL);

  char type[63] = {0};
  path_basename(in_foldername, type, sizeof(type));
  get_res_names_from_sources_file(src_filename, &sources);

  for (size_t i = 0; i < sources.size; i++) {
    str_t str_name;
    str_t res_name;
    char ext_array[MAX_PATH] = {0};
    const char* name = (const char*)darray_get(&sources, i);

    path_build(in_name, MAX_PATH, in_foldername, name, NULL);
    if (!fs_file_exist(os_fs(), in_name)) {
      continue;
    }

    path_extname(name, ext_array, MAX_PATH);

    str_init(&res_name, 0);
    str_init(&str_name, 0);
    str_set(&str_name, name);
    str_replace(&str_name, ext_array, "");
    ensure_output_res_name(&str_name, output_bin, output_bin ? ".bin" : ".data");

    str_append(&res_name, dir_name);
    str_append(&res_name, name);
    str_replace(&res_name, ".xml", "");

    path_build(in_name, MAX_PATH, in_foldername, name, NULL);
    path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);

    char out_folder[MAX_PATH + 1] = {0};
    path_dirname(out_name, out_folder, MAX_PATH);
    makesure_folder_exist(out_folder);

    ret = gen_one(in_name, out_name, theme, res_name.str, output_bin);
    str_reset(&str_name);
    str_reset(&res_name);

    if (ret == RET_FAIL) {
      GEN_ERROR(in_name);
      break;
    }
  }

  darray_deinit(&sources);
  return ret;
}

static ret_t gen_folder(const char* in_foldername, const char* out_foldername, const char* theme,
                        const char* dir_name, bool_t output_bin) {
  fs_item_t item;
  ret_t ret = RET_OK;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), in_foldername);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && case_end_with(item.name, ".xml")) {
      str_t str_name;
      str_t res_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&res_name, 0);
      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      str_replace(&str_name, ext_array, "");
      if (output_bin) {
        str_append(&str_name, ".bin");
      } else {
        filter_name(str_name.str);
        str_append(&str_name, ".data");
      }

      str_append(&res_name, dir_name);
      str_append(&res_name, item.name);
      str_replace(&res_name, ".xml", "");

      path_build(in_name, MAX_PATH, in_foldername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);
      ret = gen_one(in_name, out_name, theme, res_name.str, output_bin);
      str_reset(&str_name);
      str_reset(&res_name);
      if (ret == RET_FAIL) {
        break;
      }
    } else if (item.is_dir && !tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
      str_t res_name;
      path_build(in_name, MAX_PATH, in_foldername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_foldername, item.name, NULL);

      if (!fs_dir_exist(os_fs(), out_name)) {
        fs_create_dir(os_fs(), out_name);
      }

      str_init(&res_name, 0);
      str_append(&res_name, item.name);
      str_append(&res_name, "/");
      ret = gen_folder(in_name, out_name, theme, res_name.str, output_bin);
      str_reset(&res_name);
      if (ret != RET_OK) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  bool_t output_bin = FALSE;
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  const char* src_filename = NULL;
  const char* output_type = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S input output [bin] [theme] [src_filename]\n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t src_file;
  str_t theme_name;
  str_t _output_type;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);
  in_filename = in_file.str;
  out_filename = out_file.str;

  str_init(&_output_type, 0);
  if(argc > 3) {
    str_from_wstr(&_output_type, argv[3]);
    str_trim(&_output_type, " ");
    output_type = _output_type.str;
    if(tk_str_eq(output_type, "bin")) {
      output_bin = TRUE;
    }
  }

  str_init(&theme_name, 0);
  if (argc > 4) {
    str_from_wstr(&theme_name, argv[4]);
  }

  str_init(&src_file, 0);
  if (argc > 5) {
    str_from_wstr(&src_file, argv[5]);
    str_trim(&src_file, " ");
    if (!str_eq(&src_file, "")) {
      src_filename = src_file.str;
    }
  }

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);
  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    if (src_filename != NULL) {
      gen_sources(src_filename, in_filename, out_filename, theme_name.str, "", output_bin);
    } else {
      gen_folder(in_filename, out_filename, theme_name.str, "", output_bin);
    }
  } else if (in_stat_info.is_reg_file == TRUE) {
    char name[MAX_PATH + 1] = {0};
    path_basename_ex(in_filename, TRUE, name, sizeof(name));
    gen_one(in_filename, out_filename, theme_name.str, name, output_bin);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&src_file);
  str_reset(&theme_name);
  str_reset(&_output_type);

  return 0;
}

#include "common/main.inc"
