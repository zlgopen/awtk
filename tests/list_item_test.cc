#include "scroll_view/list_item.h"
#include "gtest/gtest.h"

TEST(ListItem, cast) {
  widget_t* w = list_item_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, list_item_cast(w));

  widget_destroy(w);
}
