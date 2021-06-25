#include "tkc/fps.h"
#include "tkc/platform.h"
#include "gtest/gtest.h"

TEST(FPS, basic) {
  fps_t fps;
  uint32_t i = 0;
  uint64_t start = time_now_ms();

  ASSERT_EQ(fps_init(&fps), RET_OK);
  for (i = 0; i < 60; i++) {
    ASSERT_EQ(fps_inc(&fps), RET_OK);
  }
  uint32_t cost = time_now_ms() - start;
  sleep_ms(1000 - cost);

  ASSERT_EQ(fps_get(&fps), 60);
}
