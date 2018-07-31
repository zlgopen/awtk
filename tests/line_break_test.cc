#include "gtest/gtest.h"
#include <stdlib.h>
#include "wordbreak.h"
#include "linebreak.h"
#include "base/line_break.h"

void set_wordbreaks_wchar(const wchar_t* s, size_t len, const char* lang, char* brks) {
  if (sizeof(wchar_t) == 2) {
    set_wordbreaks_utf16((const utf16_t*)s, len, lang, brks);
  } else {
    set_wordbreaks_utf32((const utf32_t*)s, len, lang, brks);
  }
}

void set_linebreaks_wchar(const wchar_t* s, size_t len, const char* lang, char* brks) {
  if (sizeof(wchar_t) == 2) {
    set_linebreaks_utf16((const utf16_t*)s, len, lang, brks);
  } else {
    set_linebreaks_utf32((const utf32_t*)s, len, lang, brks);
  }
}

static void dump_word_breaks(const wchar_t* s, char* brks, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    printf("break:%c: %d\n", (char)s[i], (int)brks[i]);
  }
}

TEST(LineBreak, word) {
  init_wordbreak();
  char brks[128] = {0};
  const wchar_t* str = L"It is ok\nhello world";

  set_wordbreaks_wchar(str, wcslen(str), "", brks);
  dump_word_breaks(str, brks, wcslen(str));
}

TEST(LineBreak, line) {
  init_linebreak();
  char brks[128] = {0};
  const wchar_t* str = L"It is ok\nhello world";

  set_linebreaks_wchar(str, wcslen(str), "", brks);
  dump_word_breaks(str, brks, wcslen(str));
}

TEST(LineBreak, chars) {
  init_linebreak();
  const wchar_t* str = L"中文，英\n文,";

  ASSERT_EQ(is_line_breakable(str[0], str[1], ""), LINEBREAK_ALLOWBREAK);
  ASSERT_EQ(is_line_breakable(str[1], str[2], ""), LINEBREAK_NOBREAK);
  ASSERT_EQ(is_line_breakable(str[2], str[3], ""), LINEBREAK_ALLOWBREAK);
  ASSERT_EQ(is_line_breakable(str[4], str[5], ""), LINEBREAK_MUSTBREAK);
}

TEST(LineBreak, chars_word) {
  ASSERT_EQ(word_break_check(' ', 'h'), LINE_BREAK_ALLOW);
  ASSERT_EQ(word_break_check('k', ' '), LINE_BREAK_ALLOW);
  ASSERT_EQ(word_break_check('k', 'o'), LINE_BREAK_NO);
}
