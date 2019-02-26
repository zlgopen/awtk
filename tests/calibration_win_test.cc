#include "widgets/calibration_win.h"
#include "gtest/gtest.h"

TEST(Calibration_win, cast) {
  widget_t* w = calibration_win_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, calibration_win_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  widget_destroy(w);
}
