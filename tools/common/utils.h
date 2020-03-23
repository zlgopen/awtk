/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils function
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

#ifndef UTILS_H
#define UTILS_H

#include "tkc/str.h"
#include "tkc/utils.h"

BEGIN_C_DECLS

#define GEN_ERROR(filename) log_debug("gen fail, filename = %s!\n", filename)

bool_t exit_if_need_not_update(const char* in, const char* out);
void exit_if_need_not_update_for_infiles(const char* out, int infiles_number, ...);

int unique(wchar_t* str, int size);
char* read_file(const char* file_name, uint32_t* length);
ret_t write_file(const char* file_name, const void* buff, uint32_t length);
ret_t output_c_source(const char* filename, const char* theme, const char* prefix, const char* name,
                      uint8_t* buff, uint32_t size);

ret_t output_res_c_source(const char* filename, const char* theme, uint16_t type, uint16_t subtype,
                          uint8_t* buff, uint32_t size);
ret_t output_res_c_source_ex(const char* filename, const char* theme, uint16_t type,
                             uint16_t subtype, uint8_t* buff, uint32_t size, const char* name);

const char* to_lower(char* str);
const char* skip_to(const char* p, char c);
const char* skip_char(const char* p, char c);
const char* skip_to_next(const char* p, char c);
bool_t start_with(const char* p, const char* str);
bool_t end_with(const char* p, const char* str);
const char* get_next_token(const char* p, char* token, char c);

const char* filter_name(char* name);

wchar_t** argvw_create(int argc, char* argv[]);
ret_t argvw_destroy(wchar_t** argvw);

END_C_DECLS

#endif  // UTILS_H
