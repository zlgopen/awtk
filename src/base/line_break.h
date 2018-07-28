/**
 * File:   line_break.h
 * Author: AWTK Develop Team
 * Brief:  line_break struct and utils functions.
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LINE_BREAK_H
#define TK_LINE_BREAK_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef enum line_break_t { LINE_BREAK_MUST = 0, LINE_BREAK_ALLOW, LINE_BREAK_NO } line_break_t;

line_break_t line_break_check(wchar_t c1, wchar_t c2);

END_C_DECLS

#endif /*TK_LINE_BREAK_H*/
