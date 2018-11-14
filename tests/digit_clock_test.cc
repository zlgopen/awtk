#include "time_clock/digit_clock.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include <string>

using std::string;

TEST(DigitClock, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = digit_clock_create(NULL, 10, 20, 30, 40);
  digit_clock_t* t = DIGIT_CLOCK(w);

  value_set_str(&v1, "M/D");
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_FORMAT, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->format));

  widget_destroy(w);
}

TEST(DigitClock, format) {
  date_time_t dt;
  char str[128];
  widget_t* w = digit_clock_create(NULL, 10, 20, 30, 40);
  dt.year = 2018;

  ASSERT_EQ(digit_clock_format_time(w, "Y", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-"));

  dt.month = 12;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-"));

  dt.day = 31;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D ", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 "));

  dt.hour = 1;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 1"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 1:"));

  dt.minute = 2;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 1:2"));

  dt.second = 3;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m:s", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 1:2:3"));

  dt.second = 32;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m:s", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-12-31 1:2:32"));

  widget_destroy(w);
}
