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
static ret_t gen_one(const char* in_filename, const char* out_filename, const char* theme) {
  if (!exit_if_need_not_update(in_filename, out_filename)) {
    uint32_t size = 0;
    uint8_t* input_buff = NULL;
    input_buff = (uint8_t*)read_file(in_filename, &size);
    return_value_if_fail(input_buff != NULL, RET_FAIL);
    if (end_with(in_filename, ".ttf")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, input_buff,
                          size);
    } else if (end_with(in_filename, ".png")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, input_buff,
                          size);
    } else if (end_with(in_filename, ".bmp")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BMP, input_buff,
                          size);
    } else if (end_with(in_filename, ".jpg")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff,
                          size);
    } else if (end_with(in_filename, ".jpeg")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff,
                          size);
    } else if (end_with(in_filename, ".gif")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_GIF, input_buff,
                          size);
    } else if (end_with(in_filename, ".lz4")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_LZ4, input_buff,
                          size);
    } else if (end_with(in_filename, ".webp")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_WEBP, input_buff,
                          size);
    } else if (end_with(in_filename, ".js")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, input_buff,
                          size);
    } else if (end_with(in_filename, ".lua")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_LUA, input_buff,
                          size);
    } else if (end_with(in_filename, ".py")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_PYTHON,
                          input_buff, size);
    } else if (end_with(in_filename, ".xml")) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_XML, 0, input_buff, size);
    } else if (strstr(in_filename, "images") != NULL) {
      output_res_c_source(out_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_OTHER, input_buff,
                          size);
    } else {
      const char* name = strrchr(in_filename, '/');
      if (name == NULL) {
        name = strrchr(in_filename, '\\');
      }
      if (name != NULL) {
        name++;
      }

      if (end_with(in_filename, ".txt")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_TEXT,
                               input_buff, size, name);
      } else if (end_with(in_filename, ".json")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_JSON,
                               input_buff, size, name);
      } else if (end_with(in_filename, ".bin")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_BIN,
                               input_buff, size, name);
      } else if (end_with(in_filename, ".dat")) {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_DAT,
                               input_buff, size, name);
      } else {
        output_res_c_source_ex(out_filename, theme, ASSET_TYPE_DATA, ASSET_TYPE_DATA_NONE,
                               input_buff, size, name);
      }
    }
    TKMEM_FREE(input_buff);
  }
  return RET_OK;
}

static ret_t gen_floder(const char* in_flodername, const char* out_flodername, const char* theme,
                        const char* extname, bool_t data_floder) {
  ret_t ret = RET_OK;
  fs_dir_t* dir = fs_open_dir(os_fs(), in_flodername);
  fs_item_t item;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file) {
      str_t str_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);
      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      if (!data_floder) {
        str_replace(&str_name, ext_array, "");
      }
      filter_name(str_name.str);
      str_append(&str_name, extname);

      path_build(in_name, MAX_PATH, in_flodername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_flodername, str_name.str, NULL);
      ret = gen_one(in_name, out_name, theme);
      str_reset(&str_name);
      if (ret == RET_FAIL) {
        GEN_ERROR(in_name);
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

bool_t is_data_floder(const char* floder_name) {
  char basename[MAX_PATH] = {0};
  path_basename(floder_name, basename, MAX_PATH);
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
    gen_floder(in_filename, out_filename, theme_name.str, str_extname.str,
               is_data_floder(in_filename));
  } else if (in_stat_info.is_reg_file == TRUE) {
    if (gen_one(in_filename, out_filename, theme_name.str) == RET_FAIL) {
      GEN_ERROR(in_filename);
    }
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
