#include "base/window.h"
#include "guage/guage.h"
#include "gtest/gtest.h"

TEST(Guage, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* guage = guage_create(w, 0, 0, 100, 100);

  ASSERT_EQ(guage, guage_cast(guage));

  widget_destroy(w);
}
