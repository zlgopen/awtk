#include "tkc/utf8.h"
#include "gtest/gtest.h"

TEST(Utf8, ascii) {
  const char* str = "hello";
  const wchar_t* wstr = L"hello";
  char res_str[128];
  wchar_t res_wstr[128];

  ASSERT_EQ(wcscmp(tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr)), wstr), 0);
  ASSERT_EQ(strcmp(tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str)), str), 0);
}

TEST(Utf8, chinese) {
  const char* str = "中文字符串测试";
  const wchar_t* wstr = L"中文字符串测试";
  char res_str[128];
  wchar_t res_wstr[128];

  ASSERT_EQ(wcscmp(tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr)), wstr), 0);
  ASSERT_EQ(strcmp(tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str)), str), 0);
}
