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

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_SHOW_DATE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_SHOW_DATE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), t->show_date);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_SHOW_DATE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_SHOW_DATE, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), t->show_date);

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_SHOW_TIME, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_SHOW_TIME, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), t->show_time);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_SHOW_TIME, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_SHOW_TIME, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), t->show_time);

  value_set_str(&v1, "%d/%d");
  ASSERT_EQ(widget_set_prop(w, DIGIT_CLOCK_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, DIGIT_CLOCK_PROP_FORMAT, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->format));

  widget_destroy(w);
}
