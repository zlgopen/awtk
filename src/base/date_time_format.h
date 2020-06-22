/**
 * File:   date_time_format.h
 * Author: AWTK Develop Team
 * Brief:  date_time_format
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATE_TIME_FORMAT_H
#define TK_DATE_TIME_FORMAT_H

#include "tkc/str.h"
#include "tkc/wstr.h"
#include "tkc/types_def.h"
#include "tkc/date_time.h"

BEGIN_C_DECLS

ret_t wstr_format_time(wstr_t* str, const char* format, uint64_t time);
ret_t wstr_format_date_time(wstr_t* str, const char* format, const date_time_t* dt);

END_C_DECLS

#endif /*TK_DATE_TIME_FORMAT_H*/
