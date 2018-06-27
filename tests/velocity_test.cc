#include "base/velocity.h"
#include "gtest/gtest.h"
#include <math.h>

TEST(Velocity, basic) {
  velocity_t velocity;
  velocity_t* v = velocity_reset(&velocity);

  velocity_update(v, 100, 0, 0);
  velocity_update(v, 110, 10, 20);
  velocity_update(v, 120, 20, 40);

  ASSERT_EQ(round(v->xv), 1000);
  ASSERT_EQ(round(v->yv), 2000);
}
