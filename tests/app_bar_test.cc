#include "widgets/app_bar.h"
#include "gtest/gtest.h"

TEST(AppBar, cast) {
  widget_t* w = app_bar_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, app_bar_cast(w));

  widget_destroy(w);
}
