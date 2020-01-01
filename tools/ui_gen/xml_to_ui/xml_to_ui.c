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

#include "tkc/mem.h"
#include "common/utils.h"
#include "base/assets_manager.h"
#include "ui_loader/ui_binary_writer.h"
#include "ui_loader/ui_loader_xml.h"

int wmain(int argc, wchar_t* argv[]) {
  str_t s;
  uint32_t size = 0;
  wbuffer_t wbuffer;
  char* content = NULL;
  uint8_t data[100 * 1024];
  bool_t output_bin = argc == 4;
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  const char* res_name = NULL;
  ui_binary_writer_t ui_binary_writer;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder =
      ui_binary_writer_init(&ui_binary_writer, wbuffer_init(&wbuffer, data, sizeof(data)));

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename [bin]\n", argv[0]);

    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t _res_name;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  exit_if_need_not_update(in_filename, out_filename);

  if (argc > 4) {
    str_init(&_res_name, 0);
    str_from_wstr(&_res_name, argv[4]);
    res_name = _res_name.str;
  }

  str_init(&s, 0);

  return_value_if_fail(xml_file_expand_read(in_filename, &s) == RET_OK, 0);

  content = s.str;
  size = s.size;
  return_value_if_fail(content != NULL, 0);

  ui_loader_load(loader, (const uint8_t*)content, size, builder);

  if (output_bin) {
    write_file(out_filename, wbuffer.data, wbuffer.cursor);
  } else {
    output_res_c_source_ex(out_filename, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, wbuffer.data,
                           wbuffer.cursor, res_name);
  }

  str_reset(&s);
  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&_res_name);

  return 0;
}

#include "common/main.inc"
