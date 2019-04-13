/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UTILS_H
#define TK_UTILS_H

#include "tkc/str.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

int tk_atoi(const char* str);
bool_t tk_atob(const char* str);
double tk_atof(const char* str);

int32_t tk_str_cmp(const char* a, const char* b);

int tk_watoi(const wchar_t* str);
bool_t tk_watob(const wchar_t* str);
double tk_watof(const wchar_t* str);

const char* tk_skip_to_num(const char* str);
const char* tk_itoa(char* str, int len, int n);
const char* tk_ftoa(char* str, int len, double f);
long tk_strtol(const char* str, const char** end, int base);

char* tk_strncpy(char* dst, const char* src, size_t len);
char* tk_strdup(const char* str);
wchar_t* tk_wstrdup(const wchar_t* str);

char* tk_strndup(const char* str, uint32_t len);
ret_t tk_str_append(char* str, uint32_t max_len, const char* s);

uint16_t* tk_memset16(uint16_t* buff, uint16_t val, uint32_t size);
uint32_t* tk_memset24(uint32_t* buff, void* val, uint32_t size);
uint32_t* tk_memset32(uint32_t* buff, uint32_t val, uint32_t size);
uint16_t* tk_memcpy16(uint16_t* dst, uint16_t* src, uint32_t size);
uint32_t* tk_memcpy32(uint32_t* dst, uint32_t* src, uint32_t size);
void* tk_pixel_copy(void* dst, const void* src, uint32_t size, uint8_t bpp);

int tk_snprintf(char* str, size_t size, const char* format, ...);
int tk_sscanf(const char* str, const char* format, ...);
ret_t filename_to_name(const char* filename, char* str, uint32_t size);
ret_t filename_to_name_ex(const char* filename, char* str, uint32_t size, bool_t remove_extname);

/*expand include process instruction to file content: <?include filename="view_me.inc" ?>*/
ret_t xml_file_expand_read(const char* filename, str_t* s);

/*XXX:
 * 本函数仅供内部使用，可以提高速度，但使用不当极度危险。它要求dst为NULL或内存块的首地址，本函数调用之后，dst可能无效，请保留返回的地址*/
char* tk_str_copy(char* dst, const char* src);

ret_t dummy_destroy(void* data);
ret_t default_destroy(void* data);
int pointer_compare(const void* a, const void* b);

bool_t tk_is_valid_name(const char* name);
ret_t tk_replace_locale(const char* name, char out[TK_NAME_LEN + 1], const char* locale);

bool_t tk_str_start_with(const char* str, const char* prefix);
const char* tk_under_score_to_camel(const char* name, char* out, uint32_t max_out_size);

int32_t tk_pointer_to_int(void* p);
void* tk_pointer_from_int(int32_t v);

END_C_DECLS

#endif /*TK_UTILS_H*/
