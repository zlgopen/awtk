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

#include "tkc/mem.h"
#include "common/utils.h"
#include "base/assets_manager.h"

int wmain(int argc, wchar_t* argv[]) {
  uint32_t size = 0;
  uint8_t* input_buff = NULL;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc != 3) {
    printf("Usage: %S in_filename out_filename\n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  exit_if_need_not_update(in_filename, out_filename);

  input_buff = (uint8_t*)read_file(in_filename, &size);
  return_value_if_fail(input_buff != NULL, 0);
  if (end_with(in_filename, ".ttf")) {
    output_res_c_source(out_filename, ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, input_buff, size);
  } else if (end_with(in_filename, ".png")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, input_buff, size);
  } else if (end_with(in_filename, ".bmp")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BMP, input_buff, size);
  } else if (end_with(in_filename, ".jpg")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff, size);
  } else if (end_with(in_filename, ".gif")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_GIF, input_buff, size);
  } else if (end_with(in_filename, ".lz4")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_LZ4, input_buff, size);
  } else if (end_with(in_filename, ".webp")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_WEBP, input_buff, size);
  } else if (end_with(in_filename, ".js")) {
    output_res_c_source(out_filename, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, input_buff, size);
  } else if (end_with(in_filename, ".lua")) {
    output_res_c_source(out_filename, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_LUA, input_buff, size);
  } else if (end_with(in_filename, ".py")) {
    output_res_c_source(out_filename, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_PYTHON, input_buff,
                        size);
  } else if (end_with(in_filename, ".xml")) {
    output_res_c_source(out_filename, ASSET_TYPE_XML, 0, input_buff, size);
  } else if (strstr(in_filename, "images") != NULL) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_OTHER, input_buff, size);
  } else {
    const char* name = strrchr(in_filename, '/');
    if (name == NULL) {
      name = strrchr(in_filename, '\\');
    }
    if (name != NULL) {
      name++;
    }

    if (end_with(in_filename, ".txt")) {
      output_res_c_source_ex(out_filename, ASSET_TYPE_DATA, ASSET_TYPE_DATA_TEXT, input_buff, size,
                             name);
    } else if (end_with(in_filename, ".json")) {
      output_res_c_source_ex(out_filename, ASSET_TYPE_DATA, ASSET_TYPE_DATA_JSON, input_buff, size,
                             name);
    } else if (end_with(in_filename, ".bin")) {
      output_res_c_source_ex(out_filename, ASSET_TYPE_DATA, ASSET_TYPE_DATA_BIN, input_buff, size,
                             name);
    } else if (end_with(in_filename, ".dat")) {
      output_res_c_source_ex(out_filename, ASSET_TYPE_DATA, ASSET_TYPE_DATA_DAT, input_buff, size,
                             name);
    } else {
      output_res_c_source_ex(out_filename, ASSET_TYPE_DATA, ASSET_TYPE_DATA_NONE, input_buff, size,
                             name);
    }
  }

  TKMEM_FREE(input_buff);

  str_reset(&in_file);
  str_reset(&out_file);

  printf("done\n");

  return 0;
}

#include "common/main.inc"
