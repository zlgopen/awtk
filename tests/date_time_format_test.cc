#include "gtest/gtest.h"
#include "base/date_time_format.h"

TEST(DateTimeFormat, basic) {
  wstr_t str;
  wstr_init(&str, 0);
  wstr_format_time(&str, "YY-MM-DD", 0);
  ASSERT_EQ(wcscmp(str.str, L"70-01-01") == 0 || wcscmp(str.str, L"69-12-31") == 0, true);
  wstr_reset(&str);
}
