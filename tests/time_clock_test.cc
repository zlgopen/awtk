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

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_HOUR_ANCHOR_X, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_HOUR_ANCHOR_X, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->hour_anchor_x));

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_HOUR_ANCHOR_Y, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_HOUR_ANCHOR_Y, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->hour_anchor_y));

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_MINUTE_ANCHOR_X, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_MINUTE_ANCHOR_X, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->minute_anchor_x));

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_MINUTE_ANCHOR_Y, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_MINUTE_ANCHOR_Y, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->minute_anchor_y));

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_SECOND_ANCHOR_X, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_SECOND_ANCHOR_X, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->second_anchor_x));

  value_set_str(&v1, "0.5");
  ASSERT_EQ(widget_set_prop(w, TIME_CLOCK_PROP_SECOND_ANCHOR_Y, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, TIME_CLOCK_PROP_SECOND_ANCHOR_Y, &v2), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));
  ASSERT_EQ(string(value_str(&v1)), string(t->second_anchor_y));

  widget_destroy(w);
}

TEST(TimeClock, cast) {
  widget_t* w = time_clock_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, time_clock_cast(w));

  widget_destroy(w);
}

TEST(TimeClock, set_anchor_for_str) {
  float_t image_anchor = 0.0f;

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "0.3f", &image_anchor), RET_OK);
  ASSERT_EQ(image_anchor, 100.0f * 0.3f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "50px", &image_anchor), RET_OK);
  ASSERT_EQ(image_anchor, 50.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "50PX", &image_anchor), RET_OK);
  ASSERT_EQ(image_anchor, 50.0f);

  image_anchor = 0.0f;
  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "150PX", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "150px", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "-150PX", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "-150px", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "-10", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);

  ASSERT_EQ(time_clock_set_anchor_for_str(100.0f, "20", &image_anchor), RET_BAD_PARAMS);
  ASSERT_EQ(image_anchor, 0.0f);
}
