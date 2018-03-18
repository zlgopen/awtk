/**
 * File:   utils.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  utils struct and utils functions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_UTILS_H
#define LFTK_UTILS_H

#include "base/types_def.h"

BEGIN_C_DECLS

int ftk_atoi(const char* str);
double ftk_atof(const char* str);
const char* ftk_itoa(char* str, int len, int n);
const char* ftk_ftoa(char* str, int len, double f);
long ftk_strtol(const char* str, const char** end, int base);

END_C_DECLS

#endif /*LFTK_UTILS_H*/
