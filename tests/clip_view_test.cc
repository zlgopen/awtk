#include "widgets/clip_view.h"
#include "gtest/gtest.h"

TEST(ClipView, cast) {
  widget_t* w = clip_view_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, clip_view_cast(w));

  widget_destroy(w);
}
