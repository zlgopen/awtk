#include "widgets/grid.h"
#include "gtest/gtest.h"

TEST(Grid, cast) {
  widget_t* w = grid_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, grid_cast(w));

  widget_destroy(w);
  idle_dispatch();
}
