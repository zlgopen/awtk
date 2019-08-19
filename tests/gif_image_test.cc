#include "base/window.h"
#include "gif_image/gif_image.h"
#include "gtest/gtest.h"

TEST(GifImage, basic) {
  value_t v;
  value_t v1;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = gif_image_create(w, 0, 0, 100, 100);

  value_set_str(&v, "earth");
  ASSERT_EQ(widget_set_prop(img, WIDGET_PROP_IMAGE, &v), RET_OK);
  ASSERT_EQ(widget_get_prop(img, WIDGET_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v), value_str(&v1)), 0);
  ASSERT_EQ(widget_count_children(w), 1);

  widget_destroy(w);
}

TEST(GifImage, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = gif_image_create(w, 0, 0, 100, 100);

  ASSERT_EQ(img, gif_image_cast(img));
  ASSERT_EQ(img, image_base_cast(img));

  widget_destroy(w);
}
