/**
 * File:   bsvg_dump.cc
 * Author: AWTK Develop Team
 * Brief:  bsvg_dump
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
 * 2018-11-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/fs.h"
#include "common/utils.h"
#include "base/assets_manager.h"
#include "svg/bsvg_to_svg.h"

static ret_t bsvg_dump(const char* input_file) {
  str_t str;
  uint32_t size = 0;
  uint32_t* bin = (uint32_t*)file_read(input_file, &size);
  bsvg_header_t* header = (bsvg_header_t*)bin;
  return_value_if_fail(bin != NULL, RET_FAIL);

  str_init(&str, 1000);

  bsvg_to_svg(bin, size, &str);
  log_debug("<svg viewPoint=\"%d %d %d %d\" width=\"%d\" height=\"%d\">\n", header->viewport.x,
            header->viewport.y, header->viewport.w, header->viewport.h, header->w, header->h);
  log_debug("%s", str.str);
  log_debug("</svg>\n");

  TKMEM_FREE(bin);
  str_reset(&str);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  platform_prepare();

  if (argc < 2) {
    printf("Usage: %s bsvg_filename\n", argv[0]);
    return 0;
  }

  bsvg_dump(argv[1]);

  return 0;
}
