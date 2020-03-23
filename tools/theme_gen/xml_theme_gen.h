/**
 * File:   xml_gen.h
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

#include "tkc/types_def.h"

#ifndef XML_THEME_GEN_H
#define XML_THEME_GEN_H

bool xml_gen(const char* input_file, const char* output_file, const char* theme, bool_t output_bin);
uint32_t xml_gen_buff(const char* xml, uint8_t* output, uint32_t max_size);

#endif /*XML_THEME_GEN_H*/
