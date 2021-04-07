#include "base/window.h"
#include "gauge/gauge.h"
#include "gtest/gtest.h"

TEST(Guage, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* gauge = gauge_create(w, 0, 0, 100, 100);

  ASSERT_EQ(gauge, gauge_cast(gauge));

  widget_destroy(w);
  idle_dispatch();
}
