#include "guage/guage_pointer.h"
#include "gtest/gtest.h"

TEST(GuagePointer, cast) {
  widget_t* w = guage_pointer_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, guage_pointer_cast(w));

  widget_destroy(w);
}
