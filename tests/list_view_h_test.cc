#include "scroll_view/list_view_h.h"
#include "gtest/gtest.h"

TEST(ListViewH, cast) {
  widget_t* w = list_view_h_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, list_view_h_cast(w));

  widget_destroy(w);
}
