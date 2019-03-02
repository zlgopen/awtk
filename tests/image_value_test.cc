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
