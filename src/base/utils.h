/**
 * File:   utils.h
 * Author: Li XianJing <xianjimli@hotmail.com>
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

int ftk_atoi(const char* str);
float_t ftk_atof(const char* str);
const char* ftk_itoa(char* str, int len, int n);
const char* ftk_ftoa(char* str, int len, float_t f);
long ftk_strtol(const char* str, const char** end, int base);

#define str_fast_equal(s1, s2) (*(s1) == *(s2) && strcmp((s1), (s2)) == 0)

END_C_DECLS

#endif /*TK_UTILS_H*/
