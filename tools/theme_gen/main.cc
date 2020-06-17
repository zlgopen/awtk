/**
 * File:   main.cc
 * Author: AWTK Develop Team
 * Brief:  generate theme date from xml
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "common/utils.h"
#include "xml_theme_gen.h"

ret_t gen_one(const char* input_file, const char* output_file, const char* theme,
              bool_t output_bin) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(input_file, output_file)) {
    if (!xml_gen(input_file, output_file, theme, output_bin)) {
      ret = RET_FAIL;
      GEN_ERROR(input_file);
    }
  }
  return ret;
}

static ret_t gen_floder(const char* in_flodername, const char* out_flodername, const char* theme,
                        bool_t output_bin) {
  ret_t ret = RET_OK;
  fs_dir_t* dir = fs_open_dir(os_fs(), in_flodername);
  fs_item_t item;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && end_with(item.name, ".xml")) {
      str_t str_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      str_replace(&str_name, ext_array, "");
      if (output_bin) {
        str_append(&str_name, ".bin");
      } else {
        filter_name(str_name.str);
        str_append(&str_name, ".data");
      }
      path_build(in_name, MAX_PATH, in_flodername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_flodername, str_name.str, NULL);
      ret = gen_one(in_name, out_name, theme, output_bin);
      str_reset(&str_name);
      if (ret == RET_FAIL) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  bool_t output_bin = argc == 4;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S input output [bin] [theme]\n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t theme_name;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);
  in_filename = in_file.str;
  out_filename = out_file.str;

  str_init(&theme_name, 0);
  if (argc > 4) {
    str_from_wstr(&theme_name, argv[4]);
  }

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);
  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_floder(in_filename, out_filename, theme_name.str, output_bin);
  } else if (in_stat_info.is_reg_file == TRUE) {
    gen_one(in_filename, out_filename, theme_name.str, output_bin);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&theme_name);

  return 0;
}

#include "common/main.inc"
