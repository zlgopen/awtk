#include "gtest/gtest.h"
#include "tkc/date_time.h"

TEST(DateTime, leap) {
  ASSERT_EQ(date_time_is_leap(1904), TRUE);
  ASSERT_EQ(date_time_is_leap(2000), TRUE);
  ASSERT_EQ(date_time_is_leap(2004), TRUE);
  ASSERT_EQ(date_time_is_leap(2008), TRUE);
  ASSERT_EQ(date_time_is_leap(1900), FALSE);
  ASSERT_EQ(date_time_is_leap(1901), FALSE);
  ASSERT_EQ(date_time_is_leap(1902), FALSE);
  ASSERT_EQ(date_time_is_leap(2001), FALSE);
  ASSERT_EQ(date_time_is_leap(2002), FALSE);
  ASSERT_EQ(date_time_is_leap(2003), FALSE);
}

TEST(DateTime, days) {
  ASSERT_EQ(date_time_get_days(2020, 1), 31);
  ASSERT_EQ(date_time_get_days(2020, 2), 29);
  ASSERT_EQ(date_time_get_days(2020, 3), 31);
  ASSERT_EQ(date_time_get_days(2020, 4), 30);
  ASSERT_EQ(date_time_get_days(2020, 5), 31);
  ASSERT_EQ(date_time_get_days(2020, 6), 30);
  ASSERT_EQ(date_time_get_days(2020, 7), 31);
  ASSERT_EQ(date_time_get_days(2020, 8), 31);
  ASSERT_EQ(date_time_get_days(2020, 9), 30);
  ASSERT_EQ(date_time_get_days(2020, 10), 31);
  ASSERT_EQ(date_time_get_days(2020, 11), 30);
  ASSERT_EQ(date_time_get_days(2020, 12), 31);

  ASSERT_EQ(date_time_get_days(2021, 1), 31);
  ASSERT_EQ(date_time_get_days(2021, 2), 28);
  ASSERT_EQ(date_time_get_days(2021, 3), 31);
  ASSERT_EQ(date_time_get_days(2021, 4), 30);
  ASSERT_EQ(date_time_get_days(2021, 5), 31);
  ASSERT_EQ(date_time_get_days(2021, 6), 30);
  ASSERT_EQ(date_time_get_days(2021, 7), 31);
  ASSERT_EQ(date_time_get_days(2021, 8), 31);
  ASSERT_EQ(date_time_get_days(2021, 9), 30);
  ASSERT_EQ(date_time_get_days(2021, 10), 31);
  ASSERT_EQ(date_time_get_days(2021, 11), 30);
  ASSERT_EQ(date_time_get_days(2021, 12), 31);
}

TEST(DateTime, wday) {
  ASSERT_EQ(date_time_get_wday(2020, 7, 8), 2);
  ASSERT_EQ(date_time_get_wday(2020, 7, 9), 3);
  ASSERT_EQ(date_time_get_wday(2020, 7, 10), 4);
  ASSERT_EQ(date_time_get_wday(2020, 7, 11), 5);
  ASSERT_EQ(date_time_get_wday(2020, 7, 12), 6);
  ASSERT_EQ(date_time_get_wday(2020, 7, 13), 0);
  ASSERT_EQ(date_time_get_wday(2020, 7, 14), 1);

  ASSERT_EQ(date_time_get_wday(2014, 7, 8), 1);
  ASSERT_EQ(date_time_get_wday(2014, 7, 9), 2);
  ASSERT_EQ(date_time_get_wday(2014, 7, 10), 3);
  ASSERT_EQ(date_time_get_wday(2014, 7, 11), 4);
  ASSERT_EQ(date_time_get_wday(2014, 7, 12), 5);
  ASSERT_EQ(date_time_get_wday(2014, 7, 13), 6);
  ASSERT_EQ(date_time_get_wday(2014, 7, 14), 0);

  ASSERT_EQ(date_time_get_wday(1000, 7, 8), 1);
  ASSERT_EQ(date_time_get_wday(1000, 7, 9), 2);
  ASSERT_EQ(date_time_get_wday(1000, 7, 10), 3);
  ASSERT_EQ(date_time_get_wday(1000, 7, 11), 4);
  ASSERT_EQ(date_time_get_wday(1000, 7, 12), 5);
  ASSERT_EQ(date_time_get_wday(1000, 7, 13), 6);
  ASSERT_EQ(date_time_get_wday(1000, 7, 14), 0);
}
