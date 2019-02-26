#include "time_clock/time_clock.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include <string>

using std::string;

TEST(TimeClock, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = time_clock_create(NULL, 10, 20, 30, 40);
  time_clock_t* t = TIME_CLOCK(w);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_HOUR, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_HOUR, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), t->hour);

  value_set_int(&v1, 12);
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_MINUTE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_MINUTE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), t->minute);

  value_set_int(&v1, 14);
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_SECOND, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_SECOND, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), t->second);

  value_set_str(&v1, "hour");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_HOUR_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_HOUR_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->hour_image));

  value_set_str(&v1, "minute");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_MINUTE_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_MINUTE_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->minute_image));

  value_set_str(&v1, "second");

  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_SECOND_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_SECOND_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->second_image));

  value_set_str(&v1, "bg");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_BG_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_BG_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->bg_image));

  value_set_str(&v1, "image");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->image));

  widget_destroy(w);
}

TEST(TimeClock, cast) {
  widget_t* w = time_clock_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, time_clock_cast(w));

  widget_destroy(w);
}
