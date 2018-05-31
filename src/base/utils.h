/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UTILS_H
#define TK_UTILS_H

#include "base/types_def.h"

BEGIN_C_DECLS

int tk_atoi(const char* str);
float_t tk_atof(const char* str);
const char* tk_itoa(char* str, int len, int n);
const char* tk_ftoa(char* str, int len, float_t f);
long tk_strtol(const char* str, const char** end, int base);

uint16_t* tk_memset16(uint16_t* buff, uint16_t val, uint32_t size);
uint32_t* tk_memset32(uint32_t* buff, uint32_t val, uint32_t size);
uint16_t* tk_memcpy16(uint16_t* dst, uint16_t* src, uint32_t size);
uint32_t* tk_memcpy32(uint32_t* dst, uint32_t* src, uint32_t size);
void* tk_pixel_copy(void* dst, const void* src, uint32_t size, uint8_t bpp);

#define str_fast_equal(s1, s2) (*(s1) == *(s2) && strcmp((s1), (s2)) == 0)
#define fequal(f1, f2) (fabs((f1) - (f2)) < 0.00001)

END_C_DECLS

#endif /*TK_UTILS_H*/
