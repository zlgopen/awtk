#include "tkc/fps.h"
#include "tkc/platform.h"
#include "gtest/gtest.h"

#define FPS_TEST_COUNT 180
#define FPS_TEST_TIME_MS 3000

TEST(FPS, basic) {
  fps_t fps;
  uint32_t i = 0;
  uint64_t cost = 0;
  uint64_t start = time_now_ms();

  ASSERT_EQ(fps_init(&fps), RET_OK);
  for (i = 0; i < FPS_TEST_COUNT; i++) {
    ASSERT_EQ(fps_inc(&fps), RET_OK);
  }

  cost = time_now_ms() - start;
  /* cost 超过 FPS_TEST_TIME_MS 时相减会下溢，sleep_ms 截断成 uint32 后要睡几十天，直接判失败。 */
  ASSERT_LT(cost, FPS_TEST_TIME_MS);

  sleep_ms(FPS_TEST_TIME_MS - cost);
  cost = time_now_ms() - start;

  /* 必须放在取完 cost 之后：log_debug 会 printf + fflush，拖慢下一次取时钟。 */
  log_debug("cost=%" PRIu64 "\n", cost);

  /* fps_get() 用 elapse = now - fps->time，与 cost 不是同一次读数，差 1ms 取整后就会差 1。 */
  ASSERT_NEAR((double)fps_get(&fps), FPS_TEST_COUNT * 1000.0 / cost, 1.0);
}
