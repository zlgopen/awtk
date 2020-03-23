/**
 * File:   xml_str_gen.h
 * Author: AWTK Develop Team
 * Brief:  xml str to bin
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "str_gen.h"

#ifndef XML_STR_GEN_H
#define XML_STR_GEN_H

bool xml_to_str_gen(const char* input_file, StrGen* sg);
bool xml_buff_to_str_gen(const char* buff, StrGen* sg);
bool xml_to_str_gen(const char* input_file, const char* output_dir, const char* theme, bool bin);

#endif /*XML_STR_GEN_H*/
