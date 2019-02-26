#include "widgets/tab_button_group.h"
#include "gtest/gtest.h"

TEST(TabButtonGroup, cast) {
  widget_t* w = tab_button_group_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, tab_button_group_cast(w));

  widget_destroy(w);
}
