#include "base/wstr.h"
#include "gtest/gtest.h"

TEST(WStr, basic) {
  wstr_t str;
  const wchar_t* cstr = L"Hello";
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcscmp(str.str, cstr), 0);
}
