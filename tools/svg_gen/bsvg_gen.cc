/**
 * File:   bsvg_gen.cc
 * Author: AWTK Develop Team
 * Brief:  bsvg_gen
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
 * 2018-11-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/fs.h"
#include "common/utils.h"
#include "base/assets_manager.h"
#include "svg/svg_to_bsvg.h"

static ret_t bsvg_gen(const char* input_file, const char* output_file, bool_t bin) {
  uint32_t* out = NULL;
  uint32_t size = 0;
  uint32_t out_size = 0;
  char* xml = (char*)file_read(input_file, &size);

  if (svg_to_bsvg(xml, size, &out, &out_size) == RET_OK) {
    if (bin) {
      write_file(output_file, out, out_size);
    } else {
      output_res_c_source(output_file, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BSVG, (uint8_t*)out,
                          out_size);
    }
  }

  TKMEM_FREE(xml);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  bool_t output_bin = argc > 3;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024)

  if (argc < 3) {
    printf("Usage: %s svg_filename bsvg_filename [bin]\n", argv[0]);
    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  exit_if_need_not_update(in_filename, out_filename);
  bsvg_gen(in_filename, out_filename, output_bin);

  return 0;
}
