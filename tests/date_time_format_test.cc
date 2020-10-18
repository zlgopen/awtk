#include "gtest/gtest.h"
#include "base/date_time_format.h"

TEST(DateTimeFormat, basic) {
  str_t s;
  wstr_t str;
  str_init(&s, 0);
  wstr_init(&str, 0);
  wstr_format_time(&str, "YY-MM-DD", 0);

  str_from_wstr(&s, str.str);
  ASSERT_EQ(strcmp(s.str, "70-01-01") == 0 || strcmp(s.str, "69-12-31") == 0, true);

  str_reset(&s);
  wstr_reset(&str);
}
