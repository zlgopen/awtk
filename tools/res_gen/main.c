/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  res to c source
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
 * 2018-04-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "common/utils.h"
#include "base/assets_manager.h"
static ret_t gen_one(const char* in_filename, const char* out_filename, const char* theme, str_t* res_name) {
  if (!exit_if_need_not_update(in_filename, out_filename)) {
    uint32_t size = 0;
    uint8_t* input_buff = NULL;
    input_buff = (uint8_t*)read_file(in_filename, &size);
    return_value_if_fail(input_buff != NULL, RET_FAIL);
    if (case_end_with(in_filename, ".ttf")) {
      str_replace(res_name, ".ttf", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".png")) {
      str_replace(res_name, ".png", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".bmp")) {
      str_replace(res_name, ".bmp", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BMP, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".jpg")) {
      str_replace(res_name, ".jpg", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".jpeg")) {
      str_replace(res_name, ".jpeg", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".gif")) {
      str_replace(res_name, ".gif", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_GIF, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".lz4")) {
      str_replace(res_name, ".lz4", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_LZ4, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".webp")) {
      str_replace(res_name, ".webp", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_WEBP, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".js")) {
      str_replace(res_name, ".js", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".lua")) {
      str_replace(res_name, ".lua", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_LUA, input_buff,
                          size, res_name->str);
    } else if (case_end_with(in_filename, ".py")) {
      str_replace(res_name, ".py", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_PYTHON,
                          input_buff, size, res_name->str);
    } else if (case_end_with(in_filename, ".xml")) {
      str_replace(res_name, ".xml", "");
      output_res_c_source_ex(out_filename, theme, ASSET_TYPE_XML, 0, input_buff, size, res_name->str);
    } else if (strstr(in_filename, "images") != NULL) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_OTHER, input_buff,
                          size);
    } else if (strstr(in_filename, "flows") != NULL) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_FLOW, 0, input_buff, size);
    } else {
      if (case_end_with(in_filename, ".txt")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_TEXT,
                               input_buff, size, res_name->str);
      } else if (case_end_with(in_filename, ".json")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_JSON,
                               input_buff, size, res_name->str);
      } else if (case_end_with(in_filename, ".bin")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_BIN,
                               input_buff, size, res_name->str);
      } else if (case_end_with(in_filename, ".dat")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_DAT,
                               input_buff, size, res_name->str);
      } else {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_NONE,
                               input_buff, size, res_name->str);
      }
    }
    TKMEM_FREE(input_buff);
  }
  return RET_OK;
}

static ret_t gen_folder(const char* in_foldername, const char* out_foldername, const char* theme,
                        const char* extname, const char* dir_name, bool_t data_folder) {
  fs_item_t item;
  ret_t ret = RET_OK;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), in_foldername);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file) {
      str_t str_name;
      str_t res_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);
      
      str_init(&res_name, 0);
      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      if (!data_folder) {
        str_replace(&str_name, ext_array, "");
      }
      filter_name(str_name.str);
      str_append(&str_name, extname);

      str_append(&res_name, dir_name);
      str_append(&res_name, item.name);

      path_build(in_name, MAX_PATH, in_foldername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);

      ret = gen_one(in_name, out_name, theme, &res_name);
      str_reset(&str_name);
      str_reset(&res_name);
      if (ret == RET_FAIL) {
        printf(
            "gen fail, filename = %s! desc = the resource file is empty, please confirm that the "
            "resource file has saved data.!\n",
            in_name);
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
      ret = gen_folder(in_name, out_name, theme, extname, res_name.str, data_folder);
      str_reset(&res_name);
      if (ret != RET_OK) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

bool_t is_data_folder(const char* folder_name) {
  char basename[MAX_PATH] = {0};
  path_basename(folder_name, basename, MAX_PATH);
  return tk_str_eq(basename, "data");
}

int wmain(int argc, wchar_t* argv[]) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename [theme] [out_extname]\n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t str_extname;
  str_t theme_name;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  str_init(&theme_name, 0);
  if (argc > 3) {
    str_from_wstr(&theme_name, argv[3]);
  }
  str_init(&str_extname, 0);
  if (argc > 4) {
    str_from_wstr(&str_extname, argv[4]);
  }

  in_filename = in_file.str;
  out_filename = out_file.str;

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);
  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_folder(in_filename, out_filename, theme_name.str, str_extname.str, "",
               is_data_folder(in_filename));
  } else if (in_stat_info.is_reg_file == TRUE) {
    str_t res_name;
    char name[MAX_PATH + 1] = {0};
    path_basename_ex(in_filename, TRUE, name, sizeof(name));
    str_init(&res_name, 0);
    str_append(&res_name, name);
    if (gen_one(in_filename, out_filename, theme_name.str, &res_name) == RET_FAIL) {
      printf(
          "gen fail, filename = %s! desc = the resource file is empty, please confirm that the "
          "resource file has saved data.!\n",
          in_filename);
    }
    str_reset(&res_name);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&theme_name);
  str_reset(&str_extname);

  printf("done\n");

  return 0;
}

#include "common/main.inc"
