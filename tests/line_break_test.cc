#include "base/canvas.h"
#include "widgets/label.h"
#include "base/widget.h"
#include "base/line_break.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(line_break, line_break_count) {
  ASSERT_EQ(line_break_count(L"a"), 1);
  ASSERT_EQ(line_break_count(L"ab"), 1);
  ASSERT_EQ(line_break_count(L"a\nb"), 2);
  ASSERT_EQ(line_break_count(L"\na\nb"), 3);
  ASSERT_EQ(line_break_count(L"\na\nb\n"), 4);
  ASSERT_EQ(line_break_count(L"\na\nb\r"), 4);
  ASSERT_EQ(line_break_count(L"\na\nb\r\n"), 4);
  ASSERT_EQ(line_break_count(L"\na\rb\r"), 4);
  ASSERT_EQ(line_break_count(L"\na\rb\r\r"), 5);
  ASSERT_EQ(line_break_count(L"\na\r\nb\r\r"), 5);
  ASSERT_EQ(line_break_count(L"\na\r\rb\r\r"), 6);
}

static void test_str(const wchar_t* s1, const wchar_t* s2, uint32_t size) {
  if (size == 0) {
    ASSERT_EQ(*s1, 0);
  } else {
    ASSERT_EQ(memcmp(s1, s2, size * sizeof(wchar_t)), 0);
  }
}

static ret_t test_on_line(void* ctx, uint32_t index, const wchar_t* str, uint32_t size) {
  const wchar_t** strs = (const wchar_t**)ctx;
  test_str(strs[index], str, size);

  return RET_OK;
}

TEST(line_break, line_break1) {
  const wchar_t* strs[] = {L"a", NULL};
  ASSERT_EQ(line_break(L"a", test_on_line, (void*)strs), RET_OK);
}

TEST(line_break, line_break2) {
  const wchar_t* strs[] = {L"a", L"b", NULL};
  ASSERT_EQ(line_break(L"a\rb", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_break(L"a\nb", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_break(L"a\r\nb", test_on_line, (void*)strs), RET_OK);
}

TEST(line_break, line_break3) {
  const wchar_t* strs[] = {L"a", L"b", L"", NULL};
  ASSERT_EQ(line_break(L"a\rb\r", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_break(L"a\nb\n", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_break(L"a\r\nb\r\n", test_on_line, (void*)strs), RET_OK);
}
