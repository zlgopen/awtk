#include "widgets/column.h"
#include "gtest/gtest.h"

TEST(Column, cast) {
  widget_t* w = column_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, column_cast(w));

  widget_destroy(w);
}
