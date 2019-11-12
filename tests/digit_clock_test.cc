#include "widgets/digit_clock.h"
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
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_FORMAT, &v2), RET_OK);
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

  ASSERT_EQ(digit_clock_format_time(w, "YY", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-"));

  dt.month = 9;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-"));

  dt.day = 3;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D ", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 "));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD ", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03 "));

  dt.hour = 1;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 1"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD hh", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03 01"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 1:"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD hh:", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03 01:"));

  dt.minute = 2;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 1:2"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD hh:mm", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03 01:02"));

  dt.second = 3;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m:s", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 1:2:3"));

  ASSERT_EQ(digit_clock_format_time(w, "YY-MM-DD hh:mm:ss", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("18-09-03 01:02:03"));

  dt.second = 32;
  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D h:m:s", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 1:2:32"));

  ASSERT_EQ(digit_clock_format_time(w, "Y-M-D hh:mm:ss", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("2018-9-3 01:02:32"));

  widget_destroy(w);
}

TEST(DigitClock, cast) {
  widget_t* w = digit_clock_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, digit_clock_cast(w));

  widget_destroy(w);
}

TEST(DigitClock, wday) {
  date_time_t dt;
  char str[128];
  widget_t* w = digit_clock_create(NULL, 10, 20, 30, 40);

  dt.wday = 0;
  ASSERT_EQ(digit_clock_format_time(w, "W", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("Sun"));

  dt.wday = 5;
  ASSERT_EQ(digit_clock_format_time(w, "W", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("Fri"));

  widget_destroy(w);
}

TEST(DigitClock, month) {
  date_time_t dt;
  char str[128];
  widget_t* w = digit_clock_create(NULL, 10, 20, 30, 40);

  dt.month = 1;
  ASSERT_EQ(digit_clock_format_time(w, "MMM", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("Jan"));

  dt.month = 6;
  ASSERT_EQ(digit_clock_format_time(w, "MMM", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("Jun"));

  dt.month = 12;
  ASSERT_EQ(digit_clock_format_time(w, "MMM", &dt), RET_OK);
  ASSERT_EQ(wstr_get_utf8(&(w->text), str, sizeof(str)), RET_OK);
  ASSERT_EQ(string(str), string("Dec"));

  widget_destroy(w);
}
