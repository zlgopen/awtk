/**
 * File:   line_break.c
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

#include "base/line_break.h"

#ifdef WITH_UNICODE_BREAK
#include "linebreak.h"
line_break_t line_break_check(wchar_t c1, wchar_t c2) {
  int ret = is_line_breakable(c1, c2, "");

  switch (ret) {
    case LINEBREAK_MUSTBREAK: {
      return LINE_BREAK_MUST;
    }
    case LINEBREAK_ALLOWBREAK: {
      return LINE_BREAK_ALLOW;
    }
    case LINEBREAK_NOBREAK: {
      return LINE_BREAK_NO;
    }
    default: { return LINE_BREAK_ALLOW; }
  }
}
#else
static const wchar_t* no_start_symbols = L",.?!)>:;，。？！》）：；";
line_break_t line_break_check(wchar_t c1, wchar_t c2) {
  if (wcschr(no_start_symbols, c2) != NULL) {
    return LINE_BREAK_NO;
  } else if (c1 == '\r' || c1 == '\n') {
    return LINE_BREAK_MUST;
  } else {
    return LINE_BREAK_ALLOW;
  }
}
#endif /*WITH_UNICODE_BREAK*/
