#include "base/window.h"
#include "widgets/system_bar.h"
#include "gtest/gtest.h"

TEST(SystemBar, cast) {
  widget_t* w = system_bar_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, system_bar_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  window_close_force(w);
}
