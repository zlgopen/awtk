/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

int main(int argc, char** argv) {
  str_t s;
  uint32_t size = 0;
  wbuffer_t wbuffer;
  char* content = NULL;
  uint8_t data[100 * 1024];
  bool_t output_bin = argc > 3;
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  ui_binary_writer_t ui_binary_writer;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder =
      ui_binary_writer_init(&ui_binary_writer, wbuffer_init(&wbuffer, data, sizeof(data)));

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 3) {
    printf("Usage: %s in_filename out_filename [bin]\n", argv[0]);

    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  exit_if_need_not_update(in_filename, out_filename);

  str_init(&s, 0);

  return_value_if_fail(xml_file_expand_read(in_filename, &s) == RET_OK, 0);

  content = s.str;
  size = s.size;
  return_value_if_fail(content != NULL, 0);

  ui_loader_load(loader, (const uint8_t*)content, size, builder);

  if (output_bin) {
    write_file(out_filename, wbuffer.data, wbuffer.cursor);
  } else {
    output_res_c_source(out_filename, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, wbuffer.data,
                        wbuffer.cursor);
  }

  str_reset(&s);

  return 0;
}
