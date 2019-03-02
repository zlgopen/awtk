#include "widgets/tab_control.h"
#include "gtest/gtest.h"

TEST(TabControl, cast) {
  widget_t* w = tab_control_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, tab_control_cast(w));

  widget_destroy(w);
}
