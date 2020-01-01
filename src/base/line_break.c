/**
 * File:   line_break.c
 * Author: AWTK Develop Team
 * Brief:  line break and work break algorithm.
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/line_break.h"

#ifdef WITH_UNICODE_BREAK

#include "libunibreak/linebreak.h"
#include "libunibreak/wordbreak.h"

break_type_t line_break_check(wchar_t c1, wchar_t c2) {
  int ret = 0;
  static bool_t inited = FALSE;
  if (!inited) {
    inited = TRUE;
    init_linebreak();
  }

  if (c1 == ' ') {
    return LINE_BREAK_ALLOW;
  }

  ret = is_line_breakable(c1, c2, "");

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

break_type_t word_break_check(wchar_t c1, wchar_t c2) {
  utf32_t s[2];
  char brks[2];
  static bool_t inited = FALSE;

  if (!inited) {
    inited = TRUE;
    init_wordbreak();
  }

  s[0] = c1;
  s[1] = c2;
  set_wordbreaks_utf32(s, 2, "", brks);

  if (brks[0] == WORDBREAK_BREAK) {
    return LINE_BREAK_ALLOW;
  } else {
    return LINE_BREAK_NO;
  }
}
#else
/*FIXME:*/
#define tk_isspace(c) (c == ' ' || c == '\t')
static const wchar_t* no_start_symbols = L",.?!)>:;，。？！》）：；";
break_type_t line_break_check(wchar_t c1, wchar_t c2) {
  if (wcschr(no_start_symbols, c2) != NULL) {
    return LINE_BREAK_NO;
  } else if (c1 == '\r' || c1 == '\n') {
    return LINE_BREAK_MUST;
  } else {
    return word_break_check(c1, c2);
  }
}

break_type_t word_break_check(wchar_t c1, wchar_t c2) {
  if (tk_isspace(c1) || tk_isspace(c2)) {
    return LINE_BREAK_ALLOW;
  }

  return LINE_BREAK_NO;
}
#endif /*WITH_UNICODE_BREAK*/
