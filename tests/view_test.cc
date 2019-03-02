#include "widgets/view.h"
#include "gtest/gtest.h"

TEST(View, cast) {
  widget_t* w = view_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, view_cast(w));

  widget_destroy(w);
}
