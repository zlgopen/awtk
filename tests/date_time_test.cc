#include "gtest/gtest.h"
#include "tkc/date_time.h"
#include "tkc/object_date_time.h"

TEST(DateTime, convert) {
  uint64_t now = time(0);
  date_time_t* dt = date_time_create();

  ASSERT_EQ(date_time_from_time(dt, now), RET_OK);
  ASSERT_EQ(date_time_to_time(dt), now);

  date_time_destroy(dt);
}

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

TEST(DateTime, month_name) {
  ASSERT_STREQ(date_time_get_month_name(1), "Jan");
  ASSERT_STREQ(date_time_get_month_name(3), "Mar");
  ASSERT_STREQ(date_time_get_month_name(8), "Aug");
  ASSERT_STREQ(date_time_get_month_name(10), "Oct");
  ASSERT_STREQ(date_time_get_month_name(12), "Dec");
}

TEST(DateTime, wday_name) {
  ASSERT_STREQ(date_time_get_wday_name(0), "Sun");
  ASSERT_STREQ(date_time_get_wday_name(1), "Mon");
  ASSERT_STREQ(date_time_get_wday_name(6), "Sat");
}

TEST(DateTime, wday) {
  ASSERT_EQ(date_time_get_wday(2020, 1, 1), 3);
  ASSERT_EQ(date_time_get_wday(2020, 7, 8), 3);
  ASSERT_EQ(date_time_get_wday(2020, 7, 9), 4);
  ASSERT_EQ(date_time_get_wday(2020, 7, 10), 5);
  ASSERT_EQ(date_time_get_wday(2020, 7, 11), 6);
  ASSERT_EQ(date_time_get_wday(2020, 7, 12), 0);
  ASSERT_EQ(date_time_get_wday(2020, 7, 13), 1);
  ASSERT_EQ(date_time_get_wday(2020, 7, 14), 2);

  ASSERT_EQ(date_time_get_wday(2014, 7, 8), 2);
  ASSERT_EQ(date_time_get_wday(2014, 7, 9), 3);
  ASSERT_EQ(date_time_get_wday(2014, 7, 10), 4);
  ASSERT_EQ(date_time_get_wday(2014, 7, 11), 5);
  ASSERT_EQ(date_time_get_wday(2014, 7, 12), 6);
  ASSERT_EQ(date_time_get_wday(2014, 7, 13), 0);
  ASSERT_EQ(date_time_get_wday(2014, 7, 14), 1);

  ASSERT_EQ(date_time_get_wday(1000, 7, 8), 2);
  ASSERT_EQ(date_time_get_wday(1000, 7, 9), 3);
  ASSERT_EQ(date_time_get_wday(1000, 7, 10), 4);
  ASSERT_EQ(date_time_get_wday(1000, 7, 11), 5);
  ASSERT_EQ(date_time_get_wday(1000, 7, 12), 6);
  ASSERT_EQ(date_time_get_wday(1000, 7, 13), 0);
  ASSERT_EQ(date_time_get_wday(1000, 7, 14), 1);
}

TEST(DateTime, from_time) {
  date_time_t dt;
  date_time_t now;
  time_t t = time(0);
  date_time_init(&now);
  ASSERT_EQ(date_time_from_time(&dt, t), RET_OK);
  ASSERT_EQ(dt.year, now.year);
  ASSERT_EQ(dt.month, now.month);
  ASSERT_EQ(dt.day, now.day);
  ASSERT_EQ(dt.hour, now.hour);
}

TEST(DateTime, set_time) {
  date_time_t now;
  date_time_init(&now);
  date_time_set(&now);
}

TEST(DateTime, delta) {
  date_time_t dt;
  date_time_init(&dt);
  dt.day = 20;
  date_time_add_delta(&dt, 24 * 3600);
  ASSERT_EQ(dt.day, 21);
  date_time_add_delta(&dt, -24 * 3600);
  ASSERT_EQ(dt.day, 20);

  dt.year = 2020;
  dt.month = 11;
  dt.day = 20;
  date_time_add_delta(&dt, 24 * 3600);
  ASSERT_EQ(dt.day, 21);
  date_time_add_delta(&dt, -24 * 3600);
  ASSERT_EQ(dt.day, 20);

  dt.year = 2020;
  dt.month = 11;
  dt.day = 1;
  date_time_add_delta(&dt, -24 * 3600);
  ASSERT_EQ(dt.day, 31);

  dt.year = 2020;
  dt.month = 12;
  dt.day = 31;
  date_time_add_delta(&dt, 24 * 3600);
  ASSERT_EQ(dt.day, 1);
  ASSERT_EQ(dt.year, 2021);

  dt.year = 2020;
  dt.month = 12;
  dt.day = 31;
  dt.hour = 23;
  dt.minute = 50;
  date_time_add_delta(&dt, 3600);
  ASSERT_EQ(dt.day, 1);
  ASSERT_EQ(dt.year, 2021);

  dt.year = 2020;
  dt.month = 1;
  dt.day = 1;
  dt.hour = 1;
  date_time_add_delta(&dt, -2 * 3600);
  ASSERT_EQ(dt.day, 31);
  ASSERT_EQ(dt.year, 2019);
}

TEST(DateTime, object) {
  tk_object_t* obj = object_date_time_create();

  ASSERT_EQ(tk_object_set_prop_int(obj, "year", 2020), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "year", 0), 2020);

  ASSERT_EQ(tk_object_set_prop_int(obj, "month", 1), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "month", 0), 1);

  ASSERT_EQ(tk_object_set_prop_int(obj, "day", 1), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "day", 0), 1);

  ASSERT_EQ(tk_object_set_prop_int(obj, "hour", 2), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "hour", 0), 2);

  ASSERT_EQ(tk_object_set_prop_int(obj, "minute", 3), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "minute", 0), 3);

  ASSERT_EQ(tk_object_set_prop_int(obj, "second", 4), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "second", 0), 4);

  TK_OBJECT_UNREF(obj);
}

#include "tkc/fscript.h"
#include "tkc/object_default.h"

TEST(DateTime, fscript_object) {
  value_t v1;
  tk_object_t* obj = object_default_create();

  fscript_eval(
      obj,
      "a=date_time_create();date_time_set_prop(a,\"year\",2050);b=date_time_get_prop(a,\"year\");b",
      &v1);
  ASSERT_EQ(value_int(&v1), 2050);

  fscript_eval(
      obj,
      "a=date_time_create();date_time_set_prop(a,\"month\",10);b=date_time_get_prop(a,\"month\");b",
      &v1);
  ASSERT_EQ(value_int(&v1), 10);

  fscript_eval(
      obj,
      "a=date_time_create();date_time_set_prop(a,\"day\",11);b=date_time_get_prop(a,\"day\");b",
      &v1);
  ASSERT_EQ(value_int(&v1), 11);

  fscript_eval(
      obj,
      "a=date_time_create();date_time_set_prop(a,\"hour\",8);b=date_time_get_prop(a,\"hour\");b",
      &v1);
  ASSERT_EQ(value_int(&v1), 8);

  fscript_eval(obj,
               "a=date_time_create();date_time_set_prop(a,\"minute\",9);b=date_time_get_prop(a,"
               "\"minute\");b",
               &v1);
  ASSERT_EQ(value_int(&v1), 9);

  fscript_eval(obj,
               "a=date_time_create();date_time_set_prop(a,\"second\",5);b=date_time_get_prop(a,"
               "\"second\");b",
               &v1);
  ASSERT_EQ(value_int(&v1), 5);

  TK_OBJECT_UNREF(obj);
}
