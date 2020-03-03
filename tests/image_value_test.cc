#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include "image_value/image_value.h"

#include <stdlib.h>
#include <string>
using std::string;

TEST(ImageValue, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, "image");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  value_set_str(&v1, "%2f");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_FORMAT, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  widget_destroy(w);
}

TEST(ImageValue, cast) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, image_value_cast(w));

  widget_destroy(w);
}

TEST(ImageValue, min_max) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  widget_destroy(w);
}

TEST(ImageValue, add_delta_p) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 0), 1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 4);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 5);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 1);

  widget_destroy(w);
}

TEST(ImageValue, add_delta_n) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, -1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 0), -1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 2);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 5);

  widget_destroy(w);
}
