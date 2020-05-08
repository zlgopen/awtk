#include "gtest/gtest.h"
#include "base/date_time_format.h"

TEST(DateTimeFormat, basic) {
  str_t s;
  wstr_t str;
  str_init(&s, 0);
  wstr_init(&str, 0);
  wstr_format_time(&str, "YY-MM-DD", 0);

  str_from_wstr(&s, str.str);
#ifdef WIN32
  ASSERT_STREQ(s.str, "00-00-00");
#elif defined(MACOS)
  ASSERT_STREQ(s.str, "70-01-01");
#else
  ASSERT_STREQ(s.str, "69-12-31");
#endif /*WIN32*/
  str_reset(&s);
  wstr_reset(&str);
}
