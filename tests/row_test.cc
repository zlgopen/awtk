#include "widgets/row.h"
#include "gtest/gtest.h"

TEST(Row, cast) {
  widget_t* w = row_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, row_cast(w));

  widget_destroy(w);
}
