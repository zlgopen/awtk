/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "common/utils.h"
#include "base/assets_manager.h"
#include "ui_loader/ui_binary_writer.h"
#include "ui_loader/ui_loader_xml.h"

static ret_t gen_one(const char* in_filename, const char* out_filename, bool_t output_bin,
                     const char* res_name, const char* theme_name) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(in_filename, out_filename)) {
    str_t s;
    uint32_t size = 0;
    wbuffer_t wbuffer;
    char* content = NULL;
    ui_binary_writer_t ui_binary_writer;
    ui_loader_t* loader = xml_ui_loader();
    ui_builder_t* builder =
        ui_binary_writer_init(&ui_binary_writer, wbuffer_init_extendable(&wbuffer));
    str_init(&s, 0);
    do {
      ret = RET_FAIL;
      if (xml_file_expand_read(in_filename, &s) != RET_OK) break;
      content = s.str;
      size = s.size;
      wbuffer_extend_capacity(&wbuffer, size);
      if (content == NULL) break;
      ui_loader_load(loader, (const uint8_t*)content, size, builder);
      if (output_bin) {
        write_file(out_filename, wbuffer.data, wbuffer.cursor);
      } else {
        output_res_c_source_ex(out_filename, theme_name, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN,
                               wbuffer.data, wbuffer.cursor, res_name);
      }
      ret = RET_OK;
    } while (FALSE);
    str_reset(&s);
    wbuffer_deinit(&wbuffer);
  }
  return ret;
}

static ret_t gen_floder(const char* in_flodername, const char* out_flodername, bool_t output_bin,
                        const char* res_name, const char* theme_name) {
  ret_t ret = RET_OK;
  fs_dir_t* dir = fs_open_dir(os_fs(), in_flodername);
  fs_item_t item;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  const char* c_xml = ".xml";
  if (!fs_dir_exist(os_fs(), out_flodername)) {
    fs_create_dir(os_fs(), out_flodername);
  }
  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && end_with(item.name, c_xml)) {
      str_t str_name;
      str_t sub_res_name;

      str_init(&sub_res_name, 0);
      str_set(&sub_res_name, res_name);

      str_init(&str_name, 0);
      str_set(&str_name, item.name);

      path_build(in_name, MAX_PATH, in_flodername, str_name.str, NULL);
      str_replace(&str_name, c_xml, "");

      str_append(&sub_res_name, str_name.str);

      if (output_bin) {
        str_append(&str_name, ".bin");
      } else {
        filter_name(str_name.str);
        str_append(&str_name, ".data");
      }
      path_build(out_name, MAX_PATH, out_flodername, str_name.str, NULL);

      ret = gen_one(in_name, out_name, output_bin, sub_res_name.str, theme_name);
      str_reset(&str_name);
      str_reset(&sub_res_name);

      if (ret == RET_FAIL) {
        GEN_ERROR(in_name);
        break;
      }

    } else if (item.is_dir && !tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
      char sub_in_floder[MAX_PATH] = {0};
      char sub_out_floder[MAX_PATH] = {0};

      str_t sub_res_name;
      str_init(&sub_res_name, 0);
      str_set(&sub_res_name, res_name);
      str_append(&sub_res_name, item.name);
      str_append(&sub_res_name, "/");

      path_build(sub_in_floder, MAX_PATH, in_flodername, item.name, NULL);
      path_build(sub_out_floder, MAX_PATH, out_flodername, item.name, NULL);

      ret = gen_floder(sub_in_floder, sub_out_floder, output_bin, sub_res_name.str, theme_name);
      str_reset(&sub_res_name);

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
  const char* res_name = NULL;
  const char* theme_name = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename [bin] [res_name] [theme]\n", argv[0]);

    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t _res_name;
  str_t str_theme;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);

  wchar_t* arg4 = argv[4];
  str_init(&_res_name, 0);
  if (argc > 4) {  //custom output res name
    str_from_wstr(&_res_name, arg4);
    str_trim(&_res_name, " ");
    res_name = _res_name.str;
  }

  str_init(&str_theme, 0);
  if (argc > 5) {  // theme
    str_from_wstr(&str_theme, argv[5]);
    theme_name = str_theme.str;
  }

  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_floder(in_filename, out_filename, output_bin, res_name, theme_name);
  } else if (in_stat_info.is_reg_file == TRUE) {
    if (gen_one(in_filename, out_filename, output_bin, res_name, theme_name) == RET_FAIL) {
      GEN_ERROR(in_filename);
    }
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&_res_name);
  str_reset(&str_theme);

  return 0;
}

#include "common/main.inc"
