/**
 * File:   xml_gen.c
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
#include "base/theme_xml.h"
#include "tkc/asset_info.h"
#include "common/utils.h"

bool xml_gen(const char* input_file, const char* output_file, const char* theme,
             bool_t output_bin) {
  return_value_if_fail(input_file != NULL && output_file != NULL, false);
  char* xml = (char*)file_read(input_file, NULL);
  return_value_if_fail(xml != NULL, false);
  uint32_t size = 0;
  uint8_t* data = theme_xml_gen(xml, &size);
  return_value_if_fail(data != NULL, false);

  if (output_bin) {
    write_file(output_file, data, size);
  } else {
    output_res_c_source(output_file, theme, ASSET_TYPE_STYLE, 0, data, size);
  }

  return true;
}
