#include "widgets/grid_item.h"
#include "gtest/gtest.h"

TEST(GridItem, cast) {
  widget_t* w = grid_item_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, grid_item_cast(w));

  widget_destroy(w);
  idle_dispatch();
}
