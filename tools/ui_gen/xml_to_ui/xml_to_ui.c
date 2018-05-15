/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "common/utils.h"
#include "base/resource_manager.h"
#include "ui_loader/ui_builder_writer.h"
#include "ui_loader/ui_loader_xml.h"

int main(int argc, char** argv) {
  uint32_t size = 0;
  wbuffer_t wbuffer;
  char* content = NULL;
  uint8_t data[10 * 1024];
  bool_t output_bin = argc > 3;
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder = ui_builder_writer(wbuffer_init(&wbuffer, data, sizeof(data)));

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 3) {
    printf("Usage: %s in_filename out_filename [bin]\n", argv[0]);

    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  content = read_file(in_filename, &size);
  return_value_if_fail(content != NULL, 0);

  ui_loader_load(loader, (const uint8_t*)content, size, builder);

  if (output_bin) {
    write_file(out_filename, wbuffer.data, wbuffer.cursor);
  } else {
    output_res_c_source(out_filename, RESOURCE_TYPE_UI, RESOURCE_TYPE_UI_BIN, wbuffer.data,
                        wbuffer.cursor);
  }

  TKMEM_FREE(content);

  return 0;
}
