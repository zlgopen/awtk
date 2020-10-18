#include "widgets/group_box.h"
#include "gtest/gtest.h"

TEST(GroupBox, cast) {
  widget_t* w = group_box_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, group_box_cast(w));

  widget_destroy(w);
  idle_dispatch();
}
